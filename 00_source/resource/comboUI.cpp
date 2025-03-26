//============================================================
//
//	コンボUI処理 [comboUI.cpp]
//	Author：小原立暉
//
//============================================================
//************************************************************
//	インクルードファイル
//************************************************************
#include "comboUI.h"

#include "multiValue.h"

//************************************************************
//	定数宣言
//************************************************************
namespace
{
	const int PRIORITY = 6;	// タイムUIの優先順位

	// コンボ関係
	namespace combo
	{
		const int DIGIT = 2;	// 桁数
		const VECTOR3 POS = VECTOR3(1000.0f, 50.0f, 0.0f);			// 位置
		const VECTOR3 SIZE = VECTOR3(30.0f, 45.0f, 0.0f);			// サイズ
		const VECTOR3 SPACE = VECTOR3(SIZE.x * 0.85f, 0.0f, 0.0f);	// スコア数字空白
	}

	// スコア関係
	namespace score
	{
		const int DIGIT = 3;	// 桁数
		const VECTOR3 POS = VECTOR3(1100.0f, 80.0f, 0.0f);			// 位置
		const VECTOR3 SIZE = VECTOR3(40.0f, 60.0f, 0.0f);			// サイズ
		const VECTOR3 SPACE = VECTOR3(SIZE.x * 0.85f, 0.0f, 0.0f);	// スコア数字空白
	}
}

//************************************************************
//	子クラス [CComboUI] のメンバ関数
//************************************************************
//============================================================
//	コンストラクタ
//============================================================
CComboUI::CComboUI() : CObject(CObject::LABEL_UI, CObject::DIM_2D, PRIORITY),
m_pMulti(nullptr),		// 倍率の情報
m_pScore(nullptr)		// スコアの情報
{

}

//============================================================
//	デストラクタ
//============================================================
CComboUI::~CComboUI()
{

}

//============================================================
//	初期化処理
//============================================================
HRESULT CComboUI::Init()
{
	return S_OK;
}

//============================================================
//	終了処理
//============================================================
void CComboUI::Uninit()
{
	if (m_pMulti != nullptr)
	{ // 倍率が NULL じゃない場合

		// 終了処理
		m_pMulti->Uninit();
		m_pMulti = nullptr;
	}

	if (m_pScore != nullptr)
	{ // スコアが NULL じゃない場合

		// 終了処理
		m_pScore->Uninit();
		m_pScore = nullptr;
	}

	// オブジェクトを破棄
	CObject::Release();
}

//============================================================
//	更新処理
//============================================================
void CComboUI::Update(const float fDeltaTime)
{

}

//============================================================
//	描画処理
//============================================================
void CComboUI::Draw(CShader* pShader)
{

}

//============================================================
// 情報の設定処理
//============================================================
void CComboUI::SetData()
{
	// コンボ倍率の情報を生成
	m_pMulti = CMultiValue::Create
	(
		CValue::TYPE_NORMAL,		// 種類
		0,							// 値
		combo::DIGIT,				// 桁数
		combo::POS,					// 位置
		combo::SIZE,				// サイズ
		combo::SPACE				// 余白
	);
	m_pMulti->SetLabel(CObject::LABEL_NONE);

	// スコアの情報を生成
	m_pScore = CMultiValue::Create
	(
		CValue::TYPE_NORMAL,		// 種類
		0,							// 値
		score::DIGIT,				// 桁数
		score::POS,					// 位置
		score::SIZE,				// サイズ
		score::SPACE				// 余白
	);
	m_pScore->SetLabel(CObject::LABEL_NONE);
}

//============================================================
//	生成処理
//============================================================
CComboUI* CComboUI::Create()
{
	// タイムUIの生成
	CComboUI* pTimeUI = new CComboUI;
	if (pTimeUI == nullptr)
	{ // 生成に失敗した場合

		return nullptr;
	}
	else
	{ // 生成に成功した場合

		// タイムUIの初期化
		if (FAILED(pTimeUI->Init()))
		{ // 初期化に失敗した場合

			// タイムUIの破棄
			SAFE_DELETE(pTimeUI);
			return nullptr;
		}

		// 確保したアドレスを返す
		return pTimeUI;
	}
}
