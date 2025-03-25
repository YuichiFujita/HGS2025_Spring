//============================================================
//
//	チュートリアル表示状態処理 [titleStateTutorial.cpp]
//	Author：藤田勇一
//
//============================================================
//************************************************************
//	インクルードファイル
//************************************************************
#include "titleStateTutorial.h"
#include "titleManager.h"
#include "manager.h"
#include "fade.h"
#include "object2D.h"

//************************************************************
//	定数宣言
//************************************************************
namespace
{
	const char* TEXTURE = "data\\TEXTURE\\tutorial000.png";	// テクスチャパス
}

//************************************************************
//	子クラス [CTitleStateTutorial] のメンバ関数
//************************************************************
//============================================================
//	コンストラクタ
//============================================================
CTitleStateTutorial::CTitleStateTutorial() :
	m_pTutorial	(nullptr)	// チュートリアル情報
{

}

//============================================================
//	デストラクタ
//============================================================
CTitleStateTutorial::~CTitleStateTutorial()
{

}

//============================================================
//	初期化処理
//============================================================
HRESULT CTitleStateTutorial::Init()
{
	// メンバ変数を初期化
	m_pTutorial = nullptr;	// チュートリアル情報

	// チュートリアルの生成
	m_pTutorial = CObject2D::Create(SCREEN_CENT, SCREEN_SIZE);
	if (m_pTutorial == nullptr)
	{ // 生成に失敗した場合

		assert(false);
		return E_FAIL;
	}

	// チュートリアルテクスチャの割当
	m_pTutorial->BindTexture(TEXTURE);

	return S_OK;
}

//============================================================
//	終了処理
//============================================================
void CTitleStateTutorial::Uninit()
{
	// チュートリアルの終了
	SAFE_UNINIT(m_pTutorial);

	// 自身の破棄
	delete this;
}

//============================================================
//	更新処理
//============================================================
void CTitleStateTutorial::Update(const float fDeltaTime)
{
	CFade* pFade = GET_MANAGER->GetFade();	// フェード情報
	if (!pFade->IsFade() && input::Decide())
	{ // 遷移操作が行われた場合

		// ラムダ式の作成
		auto pFunc = [this]()->HRESULT
		{
			// 遷移先選択状態にする
			return m_pContext->ChangeState(new CTitleStateSelect);
		};

		// 遷移先選択状態に遷移する
		pFade->SetFade(CFade::DEF_LEVEL, CFade::DEF_LEVEL, pFunc);
	}

	// チュートリアルの更新
	if (m_pTutorial != nullptr) { m_pTutorial->Update(fDeltaTime); }
}
