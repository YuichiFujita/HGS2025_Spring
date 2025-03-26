//============================================================
//
//	銃弾処理 [bullet.cpp]
//	Author：小原立暉
//
//============================================================
//************************************************************
//	インクルードファイル
//************************************************************
#include "bullet.h"
#include "block.h"
#include "sceneGame.h"
#include "gameManager.h"

//************************************************************
//	定数宣言
//************************************************************
namespace
{
	const D3DXVECTOR3 RADIUS = D3DXVECTOR3(40.0f, 20.0f, 0.0f);		// 半径
	const float SPEED = 9.0f;		// 速度
}

//************************************************************
//	静的メンバ変数宣言
//************************************************************
CListManager<CBullet>* CBullet::m_pList = nullptr;	// オブジェクトリスト

//************************************************************
//	子クラス [CBullet] のメンバ関数
//************************************************************
//============================================================
//	コンストラクタ
//============================================================
CBullet::CBullet() : CObject3D(CObject::LABEL_BLOCK, CObject::DIM_3D, 5),
m_move(VEC3_ZERO)	// 移動量
{

}

//============================================================
//	デストラクタ
//============================================================
CBullet::~CBullet()
{

}

//============================================================
//	初期化処理
//============================================================
HRESULT CBullet::Init()
{
	// オブジェクト3Dの初期化
	if (FAILED(CObject3D::Init()))
	{ // 初期化に失敗した場合

		// 失敗を返す
		assert(false);
		return E_FAIL;
	}

	if (m_pList == nullptr)
	{ // リストマネージャーが存在しない場合

		// リストマネージャーの生成
		m_pList = CListManager<CBullet>::Create();
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
void CBullet::Uninit()
{
	// リストから自身のオブジェクトを削除
	m_pList->DelList(m_iterator);

	if (m_pList->GetNumAll() == 0)
	{ // オブジェクトが一つもない場合

		// リストマネージャーの破棄
		m_pList->Release(m_pList);
	}

	// オブジェクト3Dの終了
	CObject3D::Uninit();
}

//============================================================
//	更新処理
//============================================================
void CBullet::Update(const float fDeltaTime)
{
	if (useful::ScreenOut(GetVec3Position(), RADIUS.x))
	{ // 画面外に出た場合

		// 終了処理
		Uninit();
		return;
	}

	// 移動処理
	Move();

	// オブジェクト3Dの更新
	CObject3D::Update(fDeltaTime);

	// ブロックとの当たり判定
	BlockCollision();
}

//============================================================
//	描画処理
//============================================================
void CBullet::Draw(CShader* pShader)
{
	// オブジェクト3Dの描画
	CObject3D::Draw(pShader);
}

//============================================================
//	生成処理
//============================================================
CBullet* CBullet::Create
(
	const VECTOR3& rPos,	// 位置
	const bool bRight		// 右側
)
{
	// 銃弾の生成
	CBullet* pBlock = new CBullet;

	if (pBlock == nullptr)
	{ // 生成に失敗した場合

		return nullptr;
	}
	else
	{ // 生成に成功した場合

		// プレイヤーの初期化
		if (FAILED(pBlock->Init()))
		{ // 初期化に失敗した場合

			// プレイヤーの破棄
			SAFE_DELETE(pBlock);
			return nullptr;
		}

		// テクスチャを割当
		if (bRight) { pBlock->BindTexture("data\\TEXTURE\\bullet_right000.png"); }
		else		{ pBlock->BindTexture("data\\TEXTURE\\bullet_left000.png"); }

		// 位置を設定
		pBlock->SetVec3Position(rPos);

		// 向きを設定
		pBlock->SetVec3Rotation(VEC3_ZERO);

		// サイズを設定
		pBlock->SetVec3Size(RADIUS);

		// 移動量を設定
		pBlock->SetMove(bRight);

		// 確保したアドレスを返す
		return pBlock;
	}
}

//============================================================
//	リスト取得処理
//============================================================
CListManager<CBullet>* CBullet::GetList()
{
	// オブジェクトリストを返す
	return m_pList;
}

//============================================================
// 移動量の設定処理
//============================================================
void CBullet::SetMove(const bool bRight)
{
	if (bRight)
	{ // 右側移動の場合

		m_move.x = SPEED;
	}
	else
	{ // 左側移動の場合

		m_move.x = -SPEED;
	}
}

//============================================================
// ブロックとの当たり判定
//============================================================
void CBullet::BlockCollision(void)
{
	VECTOR3 pos = GetVec3Position();
	VECTOR3 size = GetVec3Size() * 0.5f;

	// ブロックがない場合抜ける
	CListManager<CBlock>* pList = CBlock::GetList();
	if (pList == nullptr) { return; }

	// ブロックのリストを取得
	std::list<CBlock*> list = pList->GetList();

	for (auto pBlock : list)
	{
		if (pBlock->Collision(pos, size))
		{ // ブロックに当たった場合

			// ヒット処理
			if (pBlock->Hit())
			{ // 崩せた場合

				CGameManager* pGameManager = CSceneGame::GetGameManager();	// ゲームマネージャー
				if (pGameManager != nullptr)
				{ // ゲームマネージャーがある場合

					// 基礎スコアを加算
					pGameManager->AddBaseScore(1);	// TODO：固定値のスコア加算で(●｀･ω･)ゞ＜ok？
				}

				// 自身の終了
				Uninit();

				// 関数を抜ける
				return;
			}
		}
	}
}

//============================================================
// 移動処理
//============================================================
void CBullet::Move(void)
{
	// 位置移動
	VECTOR3 pos = GetVec3Position();
	pos.x += m_move.x;
	SetVec3Position(pos);
}
