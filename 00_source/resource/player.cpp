//============================================================
//
//	プレイヤー処理 [player.cpp]
//	Author：藤田勇一
//
//============================================================
//************************************************************
//	インクルードファイル
//************************************************************
#include "player.h"
#include "main.h"
#include "manager.h"
#include "renderer.h"
#include "sound.h"
#include "camera.h"
#include "texture.h"
#include "fade.h"
#include "stage.h"
#include "sceneGame.h"
#include "gameManager.h"
#include "block.h"
#include "bullet.h"
#include "comboArea.h"
#include "collision.h"

//************************************************************
//	マクロ定義
//************************************************************
// ジャンプ中移動ON/OFF
#if 1
#define JUMP_MOVE
#endif

//************************************************************
//	定数宣言
//************************************************************
namespace
{
	const char*	TEXTURE_RIGHT	= "data\\TEXTURE\\player_right000.png";	// プレイヤー右方向のテクスチャパス
	const char*	TEXTURE_LEFT	= "data\\TEXTURE\\player_left000.png";	// プレイヤー左方向のテクスチャパス
	const int	PRIORITY	= 3;		// プレイヤーの優先順位
	const float	MOVE		= 0.9f;		// 移動量
	const float	JUMP		= 4.7f;		// ジャンプ力
	const float	GRAVITY		= 0.39f;	// 重力
	const float	RADIUS		= 20.0f;	// 半径
	const float WIDTH		= 70.0f;	// 横幅
	const float HEIGHT		= 70.0f;	// 縦幅
	const float	REV_ROTA	= 0.25f;	// 向き変更の補正係数
	const float	JUMP_REV	= 0.955f;	// 通常状態時の空中の移動量の減衰係数
	const float	LAND_REV	= 0.18f;	// 通常状態時の地上の移動量の減衰係数
	const float	HOVER_PRESS_TIME	= 0.65f;	// ホバー時間
	const float HOVER_MIN_GRAVITY	= -2.45f;	// ホバー時の補正重力
	const float	JUMP_PRESS_TIME		= 0.46f;	// ジャンプ加算時間
	const float	MAX_ADD_JUMP		= 1.38f;	// ジャンプ加算最大値
	const float	SHOT_INTERVAL		= 0.4f;		// 攻撃インターバル
	const float	BULLET_OFFSET_X		= 8.0f;		// 弾発生座標のXオフセット

	namespace camera
	{
		const CCamera::SSwing HIT_SWING = CCamera::SSwing(8.0f, 1.8f, 0.14f);	// ヒット時のカメラ揺れ
	}
}

//************************************************************
//	静的メンバ変数宣言
//************************************************************
CListManager<CPlayer>* CPlayer::m_pList = nullptr;	// オブジェクトリスト
CPlayer::AFuncState CPlayer::m_aFuncState[] =		// 状態更新関数リスト
{
	&CPlayer::UpdateNone,	// 何もしない状態の更新
	&CPlayer::UpdateNormal,	// 通常状態の更新
	&CPlayer::UpdateDeath,	// 死亡状態の更新
};

//************************************************************
//	子クラス [CPlayer] のメンバ関数
//************************************************************
//============================================================
//	コンストラクタ
//============================================================
CPlayer::CPlayer() : CObject3D(CObject::LABEL_PLAYER, CObject::DIM_3D, PRIORITY),
	m_oldPos		(VEC3_ZERO),	// 過去位置
	m_move			(VEC3_ZERO),	// 移動量
	m_state			(STATE_NONE),	// 状態
	m_bRight		(false),		// 左右フラグ
	m_bJump			(false),		// 現在ジャンプ状況
	m_bOldJump		(false),		// 過去ジャンプ状況
	m_bReleaseJump	(false),		// ジャンプ操作解除フラグ
	m_bJumpPress	(false),		// ジャンプ操作フラグ
	m_fJumpTimer	(0.0f),			// ジャンプ操作時間
	m_fShotTimer	(0.0f),			// 攻撃インターバル
	m_fHoverTimer	(0.0f),			// ホバー時間
	m_fMaxMulti		(0.0f)			// ジャンプ中の最高倍率
{
	// スタティックアサート
	static_assert(NUM_ARRAY(m_aFuncState) == CPlayer::STATE_MAX, "ERROR : State Count Mismatch");
}

//============================================================
//	デストラクタ
//============================================================
CPlayer::~CPlayer()
{

}

//============================================================
//	初期化処理
//============================================================
HRESULT CPlayer::Init()
{
	// メンバ変数を初期化
	m_oldPos		= VEC3_ZERO;	// 過去位置
	m_move			= VEC3_ZERO;	// 移動量
	m_state			= STATE_NORMAL;	// 状態
	m_bRight		= false;		// 左右フラグ
	m_bJump			= true;			// ジャンプ状況
	m_bOldJump		= true;			// 過去ジャンプ状況
	m_bReleaseJump	= false;		// ジャンプ操作解除フラグ
	m_bJumpPress	= false;		// ジャンプ操作フラグ
	m_fJumpTimer	= 0.0f;			// ジャンプ操作時間
	m_fShotTimer	= 0.0f;			// 攻撃インターバル
	m_fHoverTimer	= 0.0f;			// ホバー時間
	m_fMaxMulti		= 0.0f;			// ジャンプ中の最高倍率

	// オブジェクトキャラクターの初期化
	if (FAILED(CObject3D::Init()))
	{ // 初期化に失敗した場合

		// 失敗を返す
		assert(false);
		return E_FAIL;
	}

	// 方向の更新
	UpdateDirection();

	// 大きさの設定
	SetVec3Size(VECTOR3(WIDTH, HEIGHT, 0.0f));

	// 原点の設定
	SetOrigin(CObject3D::ORIGIN_DOWN);

	if (m_pList == nullptr)
	{ // リストマネージャーが存在しない場合

		// リストマネージャーの生成
		m_pList = CListManager<CPlayer>::Create();
		if (m_pList == nullptr)
		{ // 生成に失敗した場合

			// 失敗を返す
			assert(false);
			return E_FAIL;
		}
	}

	// リストに自身のオブジェクトを追加・イテレーターを取得
	m_iterator = m_pList->AddList(this);

	// 成功を返す
	return S_OK;
}

//============================================================
//	終了処理
//============================================================
void CPlayer::Uninit()
{
	// リストから自身のオブジェクトを削除
	m_pList->DelList(m_iterator);

	if (m_pList->GetNumAll() == 0)
	{ // オブジェクトが一つもない場合

		// リストマネージャーの破棄
		m_pList->Release(m_pList);
	}

	// オブジェクトキャラクターの終了
	CObject3D::Uninit();
}

//============================================================
//	更新処理
//============================================================
void CPlayer::Update(const float fDeltaTime)
{
	// 過去位置の更新
	UpdateOldPosition();

	// 過去ジャンプ状況の更新
	m_bOldJump = m_bJump;

	// 状態の更新
	(this->*(m_aFuncState[m_state]))(fDeltaTime);

	// オブジェクトキャラクターの更新
	CObject3D::Update(fDeltaTime);
}

//============================================================
//	描画処理
//============================================================
void CPlayer::Draw(CShader* pShader)
{
	// オブジェクトキャラクターの描画
	CObject3D::Draw(pShader);
}

//============================================================
//	更新状況の設定処理
//============================================================
void CPlayer::SetEnableUpdate(const bool bUpdate)
{
	// 引数の更新状況を設定
	CObject::SetEnableUpdate(bUpdate);	// 自身
}

//============================================================
//	描画状況の設定処理
//============================================================
void CPlayer::SetEnableDraw(const bool bDraw)
{
	// 引数の描画状況を設定
	CObject::SetEnableDraw(bDraw);	// 自身
}

//============================================================
//	生成処理
//============================================================
CPlayer* CPlayer::Create
(
	const VECTOR3& rPos,	// 位置
	const VECTOR3& rRot		// 向き
)
{
	// プレイヤーの生成
	CPlayer* pPlayer = new CPlayer;
	if (pPlayer == nullptr)
	{ // 生成に失敗した場合

		return nullptr;
	}
	else
	{ // 生成に成功した場合

		// プレイヤーの初期化
		if (FAILED(pPlayer->Init()))
		{ // 初期化に失敗した場合

			// プレイヤーの破棄
			SAFE_DELETE(pPlayer);
			return nullptr;
		}

		// 位置を設定
		pPlayer->SetVec3Position(rPos);
		pPlayer->m_oldPos = rPos;	// 過去位置も同一の位置にする

		// 向きを設定
		pPlayer->SetVec3Rotation(rRot);

		// 確保したアドレスを返す
		return pPlayer;
	}
}

//============================================================
//	リスト取得処理
//============================================================
CListManager<CPlayer>* CPlayer::GetList()
{
	// オブジェクトリストを返す
	return m_pList;
}

//============================================================
//	ヒット処理
//============================================================
bool CPlayer::Hit()
{
	// 死んでる場合抜ける
	if (IsDeath()) { return false; }

	// カメラ揺れを設定
	GET_MANAGER->GetCamera()->SetSwing(camera::HIT_SWING);

	// 死亡状態にする
	SetState(STATE_DEATH);

	CGameManager* pGameManager = CSceneGame::GetGameManager();	// ゲームマネージャー
	if (pGameManager != nullptr)
	{ // ゲームマネージャーがある場合

		// リザルト画面に遷移する
		pGameManager->TransResult();
	}

	return true;
}

//============================================================
//	状態の設定処理
//============================================================
void CPlayer::SetState(const EState state)
{
	if (state > NONE_IDX && state < STATE_MAX)
	{ // 範囲内の場合

		// 引数の状態を設定
		m_state = state;
	}
	else { assert(false); }	// 範囲外
}

//============================================================
//	状態取得処理
//============================================================
CPlayer::EState CPlayer::GetState() const
{
	// 状態を返す
	return m_state;
}

//============================================================
//	ブロックとの当たり判定
//============================================================
bool CPlayer::CollisionBlock(const VECTOR3& rPos)
{
	// ブロックがない場合抜ける
	CListManager<CBlock>* pList = CBlock::GetList();
	if (pList == nullptr) { return false; }

	// 内部リストを取得
	std::list<CBlock*> listBlock = pList->GetList();

	for (const auto& rBlock : listBlock)
	{ // 要素数分繰り返す

		// XY平面の当たり判定
		bool bHit = collision::BoxXY
		( // 引数
			rPos,
			rBlock->GetVec3Position() - VECTOR3(0.0f, HEIGHT * 0.5f, 0.0f),
			GetVec3Size() * 0.5f,
			GetVec3Size() * 0.5f,
			rBlock->GetVec3Size() * 0.5f,
			rBlock->GetVec3Size() * 0.5f
		);
		if (bHit)
		{ // 当たった場合

			// ヒット処理
			Hit();
			return true;
		}
	}

	return false;
}

//============================================================
//	倍率エリアとの当たり判定
//============================================================
void CPlayer::CollisionMulti(const VECTOR3& rPos)
{
	// ジャンプ中ではない場合抜ける
	if (!m_bJump) { return; }

	// 倍率エリアがない場合抜ける
	CListManager<CComboArea>* pList = CComboArea::GetList();
	if (pList == nullptr) { return; }

	// 内部リストを取得
	std::list<CComboArea*> listArea = pList->GetList();

	float fMaxMulti = 0.0f;	// 最高倍率
	for (const auto& rArea : listArea)
	{ // 要素数分繰り返す

		// XY平面の当たり判定
		bool bHit = collision::BoxXY
		( // 引数
			rPos,
			rArea->GetVec3Position() - VECTOR3(0.0f, HEIGHT * 0.5f, 0.0f),
			GetVec3Size() * 0.5f,
			GetVec3Size() * 0.5f,
			rArea->GetVec3Size() * 0.5f,
			rArea->GetVec3Size() * 0.5f
		);

		// 当たっていない場合次へ
		if (!bHit) { continue; }

		const float fCurMulti = rArea->GetMulti();	// エリア倍率
		if (fMaxMulti < fCurMulti)
		{ // より大きい倍率があった場合

			// 倍率を更新
			fMaxMulti = fCurMulti;
		}
	}

	if (fMaxMulti > m_fMaxMulti)
	{ // 倍率が現在の最大倍率を上回った場合

		// 倍率を更新
		m_fMaxMulti = fMaxMulti;
	}
}

//============================================================
//	何もしない状態時の更新処理
//============================================================
void CPlayer::UpdateNone(const float fDeltaTime)
{
	VECTOR3 posPlayer = GetVec3Position();	// プレイヤー位置
	VECTOR3 rotPlayer = GetVec3Rotation();	// プレイヤー向き

	// 重力の更新
	UpdateGravity(fDeltaTime);

	// 位置更新
	UpdatePosition(&posPlayer, fDeltaTime);

	// 着地判定
	UpdateLanding(&posPlayer, fDeltaTime);

	// 位置補正
	CStage* pStage = GET_MANAGER->GetStage();	// ステージ情報
	pStage->LimitPosition(posPlayer, RADIUS);

	// 方向を反映
	UpdateDirection();

	// 位置を反映
	SetVec3Position(posPlayer);

	// 向きを反映
	SetVec3Rotation(rotPlayer);
}

//============================================================
//	通常状態時の更新処理
//============================================================
void CPlayer::UpdateNormal(const float fDeltaTime)
{
	// 操作不可能な場合抜ける
	CGameManager* pGameManager = CSceneGame::GetGameManager();	// ゲームマネージャー
	if (pGameManager != nullptr)
	{ // ゲームマネージャーがある場合

		// 操作不可能な場合抜ける
		if (!pGameManager->IsControlOK()) { return; }
	}

	VECTOR3 posPlayer = GetVec3Position();	// プレイヤー位置
	VECTOR3 rotPlayer = GetVec3Rotation();	// プレイヤー向き

	// 移動操作
	UpdateMove(fDeltaTime);

	// 重力の更新
	UpdateGravity(fDeltaTime);

	// ジャンプの更新
	UpdateJump(fDeltaTime);

	// 位置更新
	UpdatePosition(&posPlayer, fDeltaTime);

	// 倍率エリアとの当たり判定
	CollisionMulti(posPlayer);

	// コンボ倍率の設定
	CSceneGame::GetGameManager()->SetComboValue(m_fMaxMulti);

	// 着地判定
	UpdateLanding(&posPlayer, fDeltaTime);

	// 位置補正
	CStage* pStage = GET_MANAGER->GetStage();	// ステージ情報
	pStage->LimitPosition(posPlayer, RADIUS);

	// ブロックとの当たり判定
	CollisionBlock(posPlayer);

	// 攻撃の更新
	UpdateShot(posPlayer, fDeltaTime);

	// 方向を反映
	UpdateDirection();

	// 位置を反映
	SetVec3Position(posPlayer);

	// 向きを反映
	SetVec3Rotation(rotPlayer);

	GET_MANAGER->GetDebugProc()->Print(CDebugProc::POINT_RIGHT, "スコア倍率：[%f]\n", m_fMaxMulti);
}

//============================================================
//	死亡状態時の更新処理
//============================================================
void CPlayer::UpdateDeath(const float fDeltaTime)
{
	VECTOR3 posPlayer = GetVec3Position();	// プレイヤー位置
	VECTOR3 rotPlayer = GetVec3Rotation();	// プレイヤー向き

	// 重力の更新
	UpdateGravity(fDeltaTime);

	// 位置更新
	UpdatePosition(&posPlayer, fDeltaTime);

	// 着地判定
	UpdateLanding(&posPlayer, fDeltaTime);

	// 位置補正
	CStage* pStage = GET_MANAGER->GetStage();	// ステージ情報
	pStage->LimitPosition(posPlayer, RADIUS);

	// 方向を反映
	UpdateDirection();

	// 位置を反映
	SetVec3Position(posPlayer);

	// 向きを反映
	SetVec3Rotation(rotPlayer);
}

//============================================================
//	過去位置の更新処理
//============================================================
void CPlayer::UpdateOldPosition()
{
	// 過去位置を更新
	m_oldPos = GetVec3Position();
}

//============================================================
//	方向の更新処理
//============================================================
void CPlayer::UpdateDirection()
{
	if (m_bRight)
	{ // 右方向の場合

		BindTexture(TEXTURE_RIGHT);
	}
	else
	{ // 左方向の場合

		BindTexture(TEXTURE_LEFT);
	}
}

//============================================================
//	移動量/目標向きの更新処理
//============================================================
void CPlayer::UpdateMove(const float fDeltaTime)
{
	float fDeltaRate = fDeltaTime / (1.0f / (float)main::FPS);	// 経過時間の割合
	CInputKeyboard* pKey = GET_INPUTKEY;	// キーボード情報
	CInputPad* pPad = GET_INPUTPAD;			// パッド情報
	if (pKey->IsPress(DIK_A) || pPad->IsPress(CInputPad::KEY_LEFT) || pPad->GetPressLStickX() < 0)
	{ // 左移動の操作が行われた場合

#ifdef JUMP_MOVE
		// 移動量を更新
		m_move.x -= MOVE * fDeltaRate;
#else
		// 移動量を更新
		if (!m_bJump) { m_move.x -= MOVE * fDeltaRate; }
#endif

		// 左方向にする
		m_bRight = false;
	}
	else if (pKey->IsPress(DIK_D) || pPad->IsPress(CInputPad::KEY_RIGHT) || pPad->GetPressLStickX() > 0)
	{ // 右移動の操作が行われた場合

#ifdef JUMP_MOVE
		// 移動量を更新
		m_move.x += MOVE * fDeltaRate;
#else
		// 移動量を更新
		if (!m_bJump) { m_move.x += MOVE * fDeltaRate; }
#endif

		// 右方向にする
		m_bRight = true;
	}
}

//============================================================
//	重力の更新処理
//============================================================
void CPlayer::UpdateGravity(const float fDeltaTime)
{
	// 重力を加算
	float fDeltaRate = fDeltaTime / (1.0f / (float)main::FPS);	// 経過時間の割合
	m_move.y -= GRAVITY * fDeltaRate;
}

//============================================================
//	ジャンプの更新処理
//============================================================
void CPlayer::UpdateJump(const float fDeltaTime)
{
	CInputKeyboard* pKey = GET_INPUTKEY;	// キーボード情報
	CInputPad* pPad = GET_INPUTPAD;			// パッド情報
	if (pKey->IsTrigger(DIK_SPACE) || pPad->IsTrigger(CInputPad::KEY_A))
	{
		if (!m_bJump)
		{ // ジャンプしていない場合

			// ジャンプ量を与える
			m_move.y = JUMP;

			// ジャンプ開始する
			m_bJumpPress = m_bJump = true;

			// ジャンプ操作解除フラグを初期化
			m_bReleaseJump = false;

			// ジャンプ操作時間を初期化
			m_fJumpTimer = 0.0f;

#ifndef JUMP_MOVE
			// 移動量を初期化
			m_move.x = 0.0f;
			m_move.z = 0.0f;
#endif
		}
	}

	if (pKey->IsRelease(DIK_SPACE) || pPad->IsRelease(CInputPad::KEY_A))
	{ // まだプレス中の場合

		// ジャンプ操作解除済みにする
		m_bReleaseJump = true;
	}

	if (m_bReleaseJump)
	{ // ジャンプ操作を解除済みの場合

		if (pKey->IsPress(DIK_SPACE) || pPad->IsPress(CInputPad::KEY_A))
		{ // まだプレス中の場合

			// ホバー時間を経過させる
			m_fHoverTimer += fDeltaTime;

			if (m_bJump								// ジャンプ中
			&&  m_move.y <= HOVER_MIN_GRAVITY		// 重力に負け始めている
			&&  m_fHoverTimer < HOVER_PRESS_TIME)	// ホバー時間が残っている
			{ // 上記の場合

				// 移動量を補正
				m_move.y = -2.45f;
			}
		}
	}

	// ジャンプ時間を経過させる
	m_fJumpTimer += fDeltaTime;

	if (m_bJumpPress && m_fJumpTimer < JUMP_PRESS_TIME)
	{ // ジャンプ操作し続けている且つ、ジャンプ操作の受付時間内の場合

		if (pKey->IsPress(DIK_SPACE) || pPad->IsPress(CInputPad::KEY_A))
		{ // まだプレス中の場合

			// ジャンプ加算量の割合計算
			float fJumpRate = useful::ValueToRate(m_fJumpTimer, JUMP_PRESS_TIME, 0.0f);

			// 上移動量を加算
			float fDeltaRate = fDeltaTime / (1.0f / (float)main::FPS);
			m_move.y += (MAX_ADD_JUMP * fJumpRate) * fDeltaRate;
		}
		else
		{ // プレス中ではない場合

			// ジャンプ操作OFFにする
			m_bJumpPress = false;
		}
	}
	else
	{ // ジャンプ操作できない場合

		// ジャンプ操作OFFにする
		m_bJumpPress = false;
	}
}

//============================================================
//	攻撃の更新処理
//============================================================
void CPlayer::UpdateShot(const VECTOR3& rPos, const float fDeltaTime)
{
	// ジャンプしていない場合抜ける
	if (!m_bJump) { return; }

	// インターバルから経過時間分減算
	m_fShotTimer -= fDeltaTime;
	useful::LimitMinNum(m_fShotTimer, 0.0f);

	CInputKeyboard* pKey = GET_INPUTKEY;	// キーボード情報
	CInputPad* pPad = GET_INPUTPAD;			// パッド情報
	if (pKey->IsPress(DIK_RETURN) || pPad->IsPress(CInputPad::KEY_B))
	{
		if (m_fShotTimer <= 0.0f)
		{ // 攻撃可能な場合

			// 弾生成位置を計算
			VECTOR3 posShot = rPos;		// プレイヤー位置原点
			posShot.y += HEIGHT * 0.5f;	// プレイヤー中心Y座標にする
			posShot.x += (m_bRight) ? BULLET_OFFSET_X : -BULLET_OFFSET_X;	// 発生座標Xにオフセットを与える

			// 弾の生成
			CBullet::Create(posShot, m_bRight);

			// インターバルを設定
			m_fShotTimer = SHOT_INTERVAL;
		}
	}
}

//============================================================
//	着地状況の更新処理
//============================================================
bool CPlayer::UpdateLanding(VECTOR3* pPos, const float fDeltaTime)
{
	CStage* pStage = GET_MANAGER->GetStage();	// ステージ情報
	bool bLand = false;	// 着地フラグ

	// 地面・制限位置着地判定
	if (pStage->LandFieldPosition(*pPos, m_oldPos, m_move)
	||  pStage->LandLimitPosition(*pPos, m_move, 0.0f))
	{ // プレイヤーが着地していた場合

		CGameManager* pGameManager = CSceneGame::GetGameManager();	// ゲームマネージャー
		if (pGameManager != nullptr)
		{ // ゲームマネージャーがある場合

			// スコアを加算
			pGameManager->AddScore((int)((float)pGameManager->GetBaseScore() * m_fMaxMulti));
			if (m_bOldJump)
			{ // 前フレームで着地していない場合

				// 基礎スコアを初期化
				pGameManager->InitBaseScore();
			}
		}

		// 着地している状態にする
		bLand = true;

		// ジャンプしていない状態にする
		m_bJumpPress = m_bJump = false;

		// ジャンプ操作時間を初期化
		m_fJumpTimer = 0.0f;

		// 攻撃インターバルを初期化
		m_fShotTimer = 0.0f;

		// ホバー時間を初期化
		m_fHoverTimer = 0.0f;

		// 倍率を初期化
		m_fMaxMulti = 0.0f;
	}

	// 着地フラグを返す
	return bLand;
}

//============================================================
//	位置の更新処理
//============================================================
void CPlayer::UpdatePosition(VECTOR3* pPos, const float fDeltaTime)
{
	float fDeltaRate = fDeltaTime / (1.0f / (float)main::FPS);	// 経過時間の割合

	// 移動量を加算
	*pPos += m_move * fDeltaRate;

	// 移動量を減衰
	if (m_bJump)
	{ // 空中の場合

#ifdef JUMP_MOVE
		m_move.x += (0.0f - m_move.x) * JUMP_REV * fDeltaRate;
		m_move.z += (0.0f - m_move.z) * JUMP_REV * fDeltaRate;
#else
		m_move.x = 0.0f;
		m_move.z = 0.0f;
#endif
	}
	else
	{ // 地上の場合

		m_move.x += (0.0f - m_move.x) * LAND_REV * fDeltaRate;
		m_move.z += (0.0f - m_move.z) * LAND_REV * fDeltaRate;
	}
}
