//============================================================
//
//	コンボエリア処理 [comboArea.cpp]
//	Author：小原立暉
//
//============================================================
//************************************************************
//	インクルードファイル
//************************************************************
#include "comboArea.h"

//************************************************************
//	定数宣言
//************************************************************
namespace
{
	const std::string TEXTURE[] =	// テクスチャ
	{
		"data\\TEXTURE\\ComboArea000.png",
		"data\\TEXTURE\\ComboArea001.png",
		"data\\TEXTURE\\ComboArea002.png",
	};
	const D3DXCOLOR COL[] =	// テクスチャ
	{
		D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f),
		D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f),
		D3DXCOLOR(0.0f, 0.0f, 1.0f, 1.0f),
	};
	const D3DXVECTOR3 RADIUS = D3DXVECTOR3(1440.0f, 240.0f, 0.0f);		// 半径
	const float MULTI_RATE = 0.5f;		// コンボ倍率
}

//************************************************************
//	静的メンバ変数宣言
//************************************************************
CListManager<CComboArea>* CComboArea::m_pList = nullptr;	// オブジェクトリスト

//************************************************************
//	子クラス [CComboArea] のメンバ関数
//************************************************************
//============================================================
//	コンストラクタ
//============================================================
CComboArea::CComboArea() : CObject3D(CObject::LABEL_BLOCK, CObject::DIM_3D, 0),
m_nIdx(0)		// 番号
{

}

//============================================================
//	デストラクタ
//============================================================
CComboArea::~CComboArea()
{

}

//============================================================
//	初期化処理
//============================================================
HRESULT CComboArea::Init()
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
		m_pList = CListManager<CComboArea>::Create();
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
void CComboArea::Uninit()
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
void CComboArea::Update(const float fDeltaTime)
{
	// 頂点座標の設定処理
	SetVtx();
}

//============================================================
//	描画処理
//============================================================
void CComboArea::Draw(CShader* pShader)
{
	// オブジェクト3Dの描画
	CObject3D::Draw(pShader);
}

//============================================================
// 倍率の取得処理
//============================================================
float CComboArea::GetMulti(void) const
{
	// コンボ倍率を返す
	return (float)m_nIdx * MULTI_RATE;
}

//============================================================
//	生成処理
//============================================================
CComboArea* CComboArea::Create
(
	const VECTOR3& rPos,	// 位置
	const int nMul			// コンボ倍率
)
{
	// コンボの最大値を超えた場合、停止
	assert(nMul - 1 < NUM_ARRAY(TEXTURE));

	// コンボエリアの生成
	CComboArea* pBlock = new CComboArea;

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

		// 倍率を設定
		pBlock->m_nIdx = nMul;

		// テクスチャの割り当て処理
		pBlock->BindTexture(TEXTURE[nMul - 1].c_str());

		pBlock->SetColor(COL[nMul - 1]);

		// 確保したアドレスを返す
		return pBlock;
	}
}

//============================================================
//	リスト取得処理
//============================================================
CListManager<CComboArea>* CComboArea::GetList()
{
	// オブジェクトリストを返す
	return m_pList;
}
