//============================================================
//
//	ブロック処理 [block.cpp]
//	Author：小原立暉
//
//============================================================
//************************************************************
//	インクルードファイル
//************************************************************
#include "block.h"

#include "blockBreak.h"
#include "collision.h"

//************************************************************
//	定数宣言
//************************************************************
namespace
{
	const D3DXVECTOR3 RADIUS = D3DXVECTOR3(60.0f, 60.0f, 0.0f);	// 半径
}

//************************************************************
//	静的メンバ変数宣言
//************************************************************
CListManager<CBlock>* CBlock::m_pList = nullptr;	// オブジェクトリスト

//************************************************************
//	子クラス [CBlock] のメンバ関数
//************************************************************
//============================================================
//	コンストラクタ
//============================================================
CBlock::CBlock() : CObject3D(CObject::LABEL_BLOCK, CObject::DIM_3D, 4),
m_type(CBlock::TYPE_BREAK),		// 種類
m_fSpeed(0.0f)					// 速度
{

}

//============================================================
//	デストラクタ
//============================================================
CBlock::~CBlock()
{

}

//============================================================
//	初期化処理
//============================================================
HRESULT CBlock::Init()
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
		m_pList = CListManager<CBlock>::Create();
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
void CBlock::Uninit()
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
void CBlock::Update(const float fDeltaTime)
{
	if (useful::ScreenOut(GetVec3Position(),RADIUS.x)) 
	{ // 画面外に出た場合

		// 終了処理
		Uninit();
		return;
	}

	// 移動処理
	Move();

	// オブジェクト3Dの更新
	CObject3D::Update(fDeltaTime);
}

//============================================================
//	描画処理
//============================================================
void CBlock::Draw(CShader* pShader)
{
	// オブジェクト3Dの描画
	CObject3D::Draw(pShader);
}

//============================================================
//	生成処理
//============================================================
CBlock* CBlock::Create
(
	const VECTOR3& rPos,	// 位置
	const EType type,		// 種類
	const float fSpeed,		// 速度
	const bool bRight		// 右側
)
{
	// ブロックの生成
	CBlock* pBlock = nullptr;

	switch (type)
	{
	case CBlock::TYPE_BREAK:

		// 破壊可能ブロック
		pBlock = new CBlockBreak;

		break;

	default:

		// 停止
		assert(false);

		break;
	}

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

		// 位置を設定
		pBlock->SetVec3Position(rPos);

		// 向きを設定
		pBlock->SetVec3Rotation(VEC3_ZERO);

		// サイズを設定
		pBlock->SetVec3Size(RADIUS);

		// 種類を設定
		pBlock->m_type = type;

		// 速度の設定処理
		pBlock->SetSpeed(fSpeed, bRight);

		// 確保したアドレスを返す
		return pBlock;
	}
}

//============================================================
//	リスト取得処理
//============================================================
CListManager<CBlock>* CBlock::GetList()
{
	// オブジェクトリストを返す
	return m_pList;
}

//============================================================
//	当たり判定
//============================================================
bool CBlock::Collision(const VECTOR3& rPos, const VECTOR3& rSize)
{
	// ブロックとの当たり判定
	VECTOR3 posBlock = GetVec3Position();
	VECTOR3 sizeBlock = GetVec3Size() * 0.5f;
	return collision::BoxXY(posBlock, rPos, sizeBlock, sizeBlock, rSize, rSize);
}

//============================================================
// 速度の設定処理
//============================================================
void CBlock::SetSpeed(const float fSpeed, const bool bRight)
{
	if (bRight)
	{ // 右側移動の場合

		m_fSpeed = fSpeed;
	}
	else
	{ // 左側移動の場合

		m_fSpeed = -fSpeed;
	}
}

//============================================================
// 移動処理
//============================================================
void CBlock::Move(void)
{
	// 位置移動
	VECTOR3 pos = GetVec3Position();
	pos.x += m_fSpeed;
	SetVec3Position(pos);
}
