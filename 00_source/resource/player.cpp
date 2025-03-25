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

//************************************************************
//	定数宣言
//************************************************************
namespace
{
	const char*	SETUP_TXT	= "data\\CHARACTER\\player.txt";	// セットアップテキスト相対パス
	const int	PRIORITY	= 3;		// プレイヤーの優先順位
	const float	MOVE		= 1.5f;		// 移動量
	const float	GRAVITY		= 0.6f;		// 重力
	const float	RADIUS		= 20.0f;	// 半径
	const float HEIGHT		= 80.0f;	// 身長
	const float	REV_ROTA	= 0.25f;	// 向き変更の補正係数
	const float	JUMP_REV	= 0.18f;	// 通常状態時の空中の移動量の減衰係数
	const float	LAND_REV	= 0.18f;	// 通常状態時の地上の移動量の減衰係数

	namespace camera
	{
		const CCamera::SSwing HIT_SWING = CCamera::SSwing(8.0f, 1.8f, 0.14f);	// ヒット時のカメラ揺れ
	}

	namespace motion
	{
		const int BLEND_FRAME_OTHER = 5;	// モーションの基本的なブレンドフレーム
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
CPlayer::CPlayer() : CObjectChara(CObject::LABEL_PLAYER, CObject::DIM_3D, PRIORITY),
	m_oldPos	(VEC3_ZERO),	// 過去位置
	m_move		(VEC3_ZERO),	// 移動量
	m_destRot	(VEC3_ZERO),	// 目標向き
	m_state		(STATE_NONE),	// 状態
	m_bJump		(false)			// ジャンプ状況
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
	m_oldPos	= VEC3_ZERO;	// 過去位置
	m_move		= VEC3_ZERO;	// 移動量
	m_destRot	= VEC3_ZERO;	// 目標向き
	m_state		= STATE_NORMAL;	// 状態
	m_bJump		= true;			// ジャンプ状況

	// オブジェクトキャラクターの初期化
	if (FAILED(CObjectChara::Init()))
	{ // 初期化に失敗した場合

		// 失敗を返す
		assert(false);
		return E_FAIL;
	}

	// キャラクター情報の割当
	BindCharaData(SETUP_TXT);

	// 初期モーションの割当
	SetMotion(MOTION_IDOL);

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
	CObjectChara::Uninit();
}

//============================================================
//	更新処理
//============================================================
void CPlayer::Update(const float fDeltaTime)
{
	// 過去位置の更新
	UpdateOldPosition();

	// 状態の更新
	EMotion curMotion = (this->*(m_aFuncState[m_state]))(fDeltaTime);

	// モーション・オブジェクトキャラクターの更新
	UpdateMotion(curMotion, fDeltaTime);
}

//============================================================
//	描画処理
//============================================================
void CPlayer::Draw(CShader* pShader)
{
	// オブジェクトキャラクターの描画
	CObjectChara::Draw(pShader);
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
		pPlayer->m_destRot = rRot;	// 目標向きも同一の向きにする

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
//	半径取得処理
//============================================================
float CPlayer::GetRadius() const
{
	// 半径を返す
	return RADIUS;
}

//============================================================
//	縦幅取得処理
//============================================================
float CPlayer::GetHeight() const
{
	// 縦幅を返す
	return HEIGHT;
}

//============================================================
//	何もしない状態時の更新処理
//============================================================
CPlayer::EMotion CPlayer::UpdateNone(const float fDeltaTime)
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

	// 向き更新
	UpdateRotation(&rotPlayer, fDeltaTime);

	// 位置を反映
	SetVec3Position(posPlayer);

	// 向きを反映
	SetVec3Rotation(rotPlayer);

	// 待機モーションを返す
	return MOTION_IDOL;
}

//============================================================
//	通常状態時の更新処理
//============================================================
CPlayer::EMotion CPlayer::UpdateNormal(const float fDeltaTime)
{
	// 操作不可能な場合抜ける
	CGameManager* pGameManager = CSceneGame::GetGameManager();	// ゲームマネージャー
	if (pGameManager != nullptr)
	{ // ゲームマネージャーがある場合

		if (!pGameManager->IsControlOK()) { return MOTION_IDOL; }
	}

	EMotion curMotion = MOTION_IDOL;		// 現在のモーション
	VECTOR3 posPlayer = GetVec3Position();	// プレイヤー位置
	VECTOR3 rotPlayer = GetVec3Rotation();	// プレイヤー向き

	// 移動操作
	curMotion = UpdateMove(fDeltaTime);

	// 重力の更新
	UpdateGravity(fDeltaTime);

	// 位置更新
	UpdatePosition(&posPlayer, fDeltaTime);

	// 着地判定
	UpdateLanding(&posPlayer, fDeltaTime);

	// 位置補正
	CStage* pStage = GET_MANAGER->GetStage();	// ステージ情報
	pStage->LimitPosition(posPlayer, RADIUS);

	// 向き更新
	UpdateRotation(&rotPlayer, fDeltaTime);

	// 位置を反映
	SetVec3Position(posPlayer);

	// 向きを反映
	SetVec3Rotation(rotPlayer);

	// 現在のモーションを返す
	return curMotion;
}

//============================================================
//	死亡状態時の更新処理
//============================================================
CPlayer::EMotion CPlayer::UpdateDeath(const float fDeltaTime)
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

	// 向き更新
	UpdateRotation(&rotPlayer, fDeltaTime);

	// 位置を反映
	SetVec3Position(posPlayer);

	// 向きを反映
	SetVec3Rotation(rotPlayer);

	// 死亡モーションを返す
	//return MOTION_DEATH;	// TODO：死亡できたら変更
	return MOTION_IDOL;
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
//	移動量/目標向きの更新処理
//============================================================
CPlayer::EMotion CPlayer::UpdateMove(const float fDeltaTime)
{
	float fDeltaRate = fDeltaTime / (1.0f / (float)main::FPS);	// 経過時間の割合
	CInputKeyboard* pKey = GET_INPUTKEY;			// キーボード情報
	CInputPad* pPad = GET_INPUTPAD;					// パッド情報
	CCamera* pCamera = GET_MANAGER->GetCamera();	// カメラ情報
	float fCameraRotY = pCamera->GetRotation().y;	// カメラ向き
	EMotion curMotion = MOTION_IDOL;				// 現在のモーション
	if (pKey->IsPress(DIK_W) || pPad->IsPress(CInputPad::KEY_UP) || pPad->GetPressLStickY() < 0)
	{ // 奥移動の操作が行われた場合

		// 移動モーションにする
		curMotion = MOTION_WALK;

		if (pKey->IsPress(DIK_A) || pPad->IsPress(CInputPad::KEY_LEFT) || pPad->GetPressLStickX() < 0)
		{ // 左移動の操作も行われた場合 (左奥移動)

			// 移動量を更新
			m_move.x += sinf(fCameraRotY - (D3DX_PI * 0.25f)) * MOVE * fDeltaRate;
			m_move.z += cosf(fCameraRotY - (D3DX_PI * 0.25f)) * MOVE * fDeltaRate;

			// 目標向きを更新
			m_destRot.y = D3DXToRadian(135) + fCameraRotY;
		}
		else if (pKey->IsPress(DIK_D) || pPad->IsPress(CInputPad::KEY_RIGHT) || pPad->GetPressLStickX() > 0)
		{ // 右移動の操作も行われた場合 (右奥移動)

			// 移動量を更新
			m_move.x -= sinf(fCameraRotY - (D3DX_PI * 0.75f)) * MOVE * fDeltaRate;
			m_move.z -= cosf(fCameraRotY - (D3DX_PI * 0.75f)) * MOVE * fDeltaRate;

			// 目標向きを更新
			m_destRot.y = D3DXToRadian(225) + fCameraRotY;
		}
		else
		{ // 奥移動の操作だけが行われた場合 (奥移動)

			// 移動量を更新
			m_move.x += sinf(fCameraRotY) * MOVE * fDeltaRate;
			m_move.z += cosf(fCameraRotY) * MOVE * fDeltaRate;

			// 目標向きを更新
			m_destRot.y = D3DXToRadian(180) + fCameraRotY;
		}
	}
	else if (pKey->IsPress(DIK_S) || pPad->IsPress(CInputPad::KEY_DOWN) || pPad->GetPressLStickY() > 0)
	{ // 手前移動の操作が行われた場合

		// 移動モーションにする
		curMotion = MOTION_WALK;

		if (pKey->IsPress(DIK_A) || pPad->IsPress(CInputPad::KEY_LEFT) || pPad->GetPressLStickX() < 0)
		{ // 左移動の操作も行われた場合 (左手前移動)

			// 移動量を更新
			m_move.x += sinf(fCameraRotY - (D3DX_PI * 0.75f)) * MOVE * fDeltaRate;
			m_move.z += cosf(fCameraRotY - (D3DX_PI * 0.75f)) * MOVE * fDeltaRate;

			// 目標向きを更新
			m_destRot.y = D3DXToRadian(45) + fCameraRotY;
		}
		else if (pKey->IsPress(DIK_D) || pPad->IsPress(CInputPad::KEY_RIGHT) || pPad->GetPressLStickX() > 0)
		{ // 右移動の操作も行われた場合 (右手前移動)

			// 移動量を更新
			m_move.x -= sinf(fCameraRotY - (D3DX_PI * 0.25f)) * MOVE * fDeltaRate;
			m_move.z -= cosf(fCameraRotY - (D3DX_PI * 0.25f)) * MOVE * fDeltaRate;

			// 目標向きを更新
			m_destRot.y = D3DXToRadian(315) + fCameraRotY;
		}
		else
		{ // 手前移動の操作だけが行われた場合 (手前移動)

			// 移動量を更新
			m_move.x -= sinf(fCameraRotY) * MOVE * fDeltaRate;
			m_move.z -= cosf(fCameraRotY) * MOVE * fDeltaRate;

			// 目標向きを更新
			m_destRot.y = D3DXToRadian(0) + fCameraRotY;
		}
	}
	else if (pKey->IsPress(DIK_A) || pPad->IsPress(CInputPad::KEY_LEFT) || pPad->GetPressLStickX() < 0)
	{ // 左移動の操作が行われた場合

		// 移動モーションにする
		curMotion = MOTION_WALK;

		// 移動量を更新
		m_move.x += sinf(fCameraRotY - (D3DX_PI * 0.5f)) * MOVE * fDeltaRate;
		m_move.z += cosf(fCameraRotY - (D3DX_PI * 0.5f)) * MOVE * fDeltaRate;

		// 目標向きを更新
		m_destRot.y = D3DXToRadian(90) + fCameraRotY;
	}
	else if (pKey->IsPress(DIK_D) || pPad->IsPress(CInputPad::KEY_RIGHT) || pPad->GetPressLStickX() > 0)
	{ // 右移動の操作が行われた場合

		// 移動モーションにする
		curMotion = MOTION_WALK;

		// 移動量を更新
		m_move.x -= sinf(fCameraRotY - (D3DX_PI * 0.5f)) * MOVE * fDeltaRate;
		m_move.z -= cosf(fCameraRotY - (D3DX_PI * 0.5f)) * MOVE * fDeltaRate;

		// 目標向きを更新
		m_destRot.y = D3DXToRadian(270) + fCameraRotY;
	}

	// モーションを返す
	return curMotion;
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

		// 着地している状態にする
		bLand = true;

		// ジャンプしていない状態にする
		m_bJump = false;
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

		m_move.x += (0.0f - m_move.x) * JUMP_REV * fDeltaRate;
		m_move.z += (0.0f - m_move.z) * JUMP_REV * fDeltaRate;
	}
	else
	{ // 地上の場合

		m_move.x += (0.0f - m_move.x) * LAND_REV * fDeltaRate;
		m_move.z += (0.0f - m_move.z) * LAND_REV * fDeltaRate;
	}
}

//============================================================
//	向きの更新処理
//============================================================
void CPlayer::UpdateRotation(VECTOR3* pRot, const float fDeltaTime)
{
	float fDeltaRate = fDeltaTime / (1.0f / (float)main::FPS);	// 経過時間の割合
	float fDiffRot = 0.0f;	// 差分向き

	// 目標向きの正規化
	useful::NormalizeRot(m_destRot.y);

	// 目標向きまでの差分を計算
	fDiffRot = m_destRot.y - pRot->y;

	// 差分向きの正規化
	useful::NormalizeRot(fDiffRot);

	// 向きの更新
	pRot->y += fDiffRot * REV_ROTA * fDeltaRate;

	// 向きの正規化
	useful::NormalizeRot(pRot->y);
}

//============================================================
//	モーション/キャラクターの更新処理
//============================================================
void CPlayer::UpdateMotion(int nMotion, const float fDeltaTime)
{
	// 死んでたら抜ける
	if (IsDeath()) { return; }

	int nAnimMotion = GetMotionType();	// 現在再生中のモーション
	if (nMotion != NONE_IDX)
	{ // モーションが設定されている場合

		if (IsMotionLoop())
		{ // ループするモーション中の場合

			if (nAnimMotion != nMotion)
			{ // 現在のモーションが再生中のモーションと一致しない場合

				// 現在のモーションの設定
				SetMotion(nMotion, motion::BLEND_FRAME_OTHER);
			}
		}
		else
		{ // ループしないモーション中の場合

#if 0
			switch (GetMotionType())
			{ // モーションごとの処理
			case MOTION_:

				if (nMotion != MOTION_IDOL)
				{ // 待機モーション以外の場合

					// 現在のモーションの設定
					SetMotion(nMotion, motion::BLEND_FRAME_OTHER);
				}
				break;
			}
#endif
		}
	}

	// オブジェクトキャラクターの更新
	CObjectChara::Update(fDeltaTime);

	switch (GetMotionType())
	{ // モーションの種類ごとの処理
	case MOTION_IDOL:
		break;

	case MOTION_WALK:
		break;
	}
}
