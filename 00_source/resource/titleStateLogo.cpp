//============================================================
//
//	ロゴ表示状態処理 [titleStateLogo.cpp]
//	Author：藤田勇一
//
//============================================================
//************************************************************
//	インクルードファイル
//************************************************************
#include "titleStateLogo.h"
#include "titleManager.h"
#include "manager.h"
#include "fade.h"
#include "object2D.h"
#include "sound.h"

//************************************************************
//	定数宣言
//************************************************************
namespace
{
	const char* TEXTURE = "data\\TEXTURE\\logo000.png";	// テクスチャパス
	const float TRANS_TIME = 6.0f;	// 自動遷移時間
}

//************************************************************
//	子クラス [CTitleStateLogo] のメンバ関数
//************************************************************
//============================================================
//	コンストラクタ
//============================================================
CTitleStateLogo::CTitleStateLogo() :
	m_pLogo		(nullptr),	// ロゴ情報
	m_fCurTime	(0.0f)		// 現在の経過時間
{

}

//============================================================
//	デストラクタ
//============================================================
CTitleStateLogo::~CTitleStateLogo()
{

}

//============================================================
//	初期化処理
//============================================================
HRESULT CTitleStateLogo::Init()
{
	// メンバ変数を初期化
	m_pLogo		= nullptr;	// ロゴ情報
	m_fCurTime	= 0.0f;		// 現在の経過時間

	// ロゴの生成
	m_pLogo = CObject2D::Create(SCREEN_CENT, SCREEN_SIZE);
	if (m_pLogo == nullptr)
	{ // 生成に失敗した場合

		assert(false);
		return E_FAIL;
	}

	// ロゴテクスチャの割当
	m_pLogo->BindTexture(TEXTURE);

	return S_OK;
}

//============================================================
//	終了処理
//============================================================
void CTitleStateLogo::Uninit()
{
	// ロゴの終了
	SAFE_UNINIT(m_pLogo);

	// 自身の破棄
	delete this;
}

//============================================================
//	更新処理
//============================================================
void CTitleStateLogo::Update(const float fDeltaTime)
{
	CFade* pFade = GET_MANAGER->GetFade();	// フェード情報
	if (!pFade->IsFade())
	{ // フェードしていない場合

		// 決定の更新
		UpdateDecide();

		// タイトル遷移の更新
		UpdateTransSelect(fDeltaTime);
	}

	// ロゴの更新
	if (m_pLogo != nullptr) { m_pLogo->Update(fDeltaTime); }
}

//============================================================
//	決定の更新処理
//============================================================
void CTitleStateLogo::UpdateDecide()
{
	if (input::Decide())
	{
		// ラムダ式の作成
		auto pFunc = [this]()->HRESULT
		{
			// 遷移先選択状態にする
			return m_pContext->ChangeState(new CTitleStateSelect);
		};

		// 遷移先選択状態に遷移する
		CFade* pFade = GET_MANAGER->GetFade();	// フェード情報
		pFade->SetFade(CFade::DEF_LEVEL, CFade::DEF_LEVEL, pFunc);
	}
}

//============================================================
//	タイトル遷移の更新処理
//============================================================
void CTitleStateLogo::UpdateTransSelect(const float fDeltaTime)
{
	// 現在の経過時間を加算
	m_fCurTime += fDeltaTime;
	if (m_fCurTime >= TRANS_TIME)
	{ // 経過した場合

		// ラムダ式の作成
		auto pFunc = [this]()->HRESULT
		{
			// 遷移先選択状態にする
			return m_pContext->ChangeState(new CTitleStateSelect);
		};

		// 遷移先選択状態に遷移する
		CFade* pFade = GET_MANAGER->GetFade();	// フェード情報
		pFade->SetFade(CFade::DEF_LEVEL, CFade::DEF_LEVEL, pFunc);
	}
}
