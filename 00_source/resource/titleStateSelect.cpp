//============================================================
//
//	遷移先選択状態処理 [titleStateSelect.cpp]
//	Author：藤田勇一
//
//============================================================
//************************************************************
//	インクルードファイル
//************************************************************
#include "titleStateSelect.h"
#include "titleManager.h"
#include "manager.h"
#include "fade.h"
#include "object2D.h"

//************************************************************
//	定数宣言
//************************************************************
namespace
{
	const float TRANS_TIME = 6.0f;	// 自動遷移時間

	namespace title
	{
		const char*		TEXTURE	= "data\\TEXTURE\\TitleLogo.png";		// テクスチャパス
		const VECTOR3	POS		= VECTOR3(SCREEN_CENT.x, 235.0f, 0.0f);	// 位置
		const float		WIDTH	= 790.0f;	// 横幅
	}

	namespace select
	{
		const char* TEXTURE[] =	// テクスチャパス
		{
			"data\\TEXTURE\\titleSelect000.png",	// 開始
			"data\\TEXTURE\\titleSelect001.png",	// チュートリアル
		};
		const VECTOR3	POS		= VECTOR3(SCREEN_CENT.x - 260.0f, 565.0f, 0.0f);	// 位置
		const VECTOR3	OFFSET	= VECTOR3(260.0f * 2.0f, 0.0f, 0.0f);	// オフセット
		const float		WIDTH	= 420.0f;				// 横幅
		const COLOR		COL_DEFAULT	= color::White();	// 通常色
		const COLOR		COL_CHOICE	= color::Yellow();	// 選択色
	}
}

//************************************************************
//	子クラス [CTitleStateSelect] のメンバ関数
//************************************************************
//============================================================
//	コンストラクタ
//============================================================
CTitleStateSelect::CTitleStateSelect() :
	m_pTitle	 (nullptr),	// タイトル情報
	m_nCurSelect (0),		// 現在の選択肢
	m_nOldSelect (0),		// 前回の選択肢
	m_fCurTime	 (0.0f)		// 現在の経過時間
{
	// メンバ変数をクリア
	memset(&m_apSelect[0], 0, sizeof(m_apSelect));	// 選択肢情報
}

//============================================================
//	デストラクタ
//============================================================
CTitleStateSelect::~CTitleStateSelect()
{

}

//============================================================
//	初期化処理
//============================================================
HRESULT CTitleStateSelect::Init()
{
	// メンバ変数を初期化
	memset(&m_apSelect[0], 0, sizeof(m_apSelect));	// 選択肢情報
	m_pTitle	 = nullptr;	// タイトル情報
	m_nCurSelect = 0;		// 現在の選択肢
	m_nOldSelect = 0;		// 前回の選択肢
	m_fCurTime	 = 0.0f;	// 現在の経過時間

	// タイトルの生成
	m_pTitle = CObject2D::Create(title::POS);
	if (m_pTitle == nullptr)
	{ // 生成に失敗した場合

		assert(false);
		return E_FAIL;
	}

	// タイトルテクスチャの割当
	m_pTitle->BindTexture(title::TEXTURE);

	// タイトル大きさの設定
	float fTitleHeight = useful::GetTexHeightFromAspect(title::WIDTH, title::TEXTURE);	// テクスチャ基準の縦幅
	m_pTitle->SetVec3Size(VECTOR3(title::WIDTH, fTitleHeight, 0.0f));

	for (int i = 0; i < SELECT_MAX; i++)
	{ // 選択肢の総数分繰り返す

		// 選択肢の生成
		m_apSelect[i] = CObject2D::Create(select::POS + (select::OFFSET * (float)i));
		if (m_apSelect[i] == nullptr)
		{ // 生成に失敗した場合

			assert(false);
			return E_FAIL;
		}

		// 選択肢テクスチャの割当
		m_apSelect[i]->BindTexture(select::TEXTURE[i]);

		// 選択肢大きさの設定
		float fSelectHeight = useful::GetTexHeightFromAspect(select::WIDTH, select::TEXTURE[i]);	// テクスチャ基準の縦幅
		m_apSelect[i]->SetVec3Size(VECTOR3(select::WIDTH, fSelectHeight, 0.0f));
	}

	return S_OK;
}

//============================================================
//	終了処理
//============================================================
void CTitleStateSelect::Uninit()
{
	// タイトルの終了
	SAFE_UNINIT(m_pTitle);

	for (int i = 0; i < SELECT_MAX; i++)
	{ // 選択肢の総数分繰り返す

		// 選択肢の終了
		SAFE_UNINIT(m_apSelect[i]);
	}

	// 自身の破棄
	delete this;
}

//============================================================
//	更新処理
//============================================================
void CTitleStateSelect::Update(const float fDeltaTime)
{
	CFade* pFade = GET_MANAGER->GetFade();	// フェード情報
	if (!pFade->IsFade())
	{ // フェードしていない場合

		// 選択の更新
		UpdateSelect();

		// 決定の更新
		UpdateDecide();

		// ロゴ遷移の更新
		UpdateTransLogo(fDeltaTime);
	}

	// タイトルの更新
	if (m_pTitle != nullptr) { m_pTitle->Update(fDeltaTime); }

	for (int i = 0; i < SELECT_MAX; i++)
	{ // 選択肢の総数分繰り返す

		// 選択肢の更新
		if (m_apSelect[i] != nullptr) { m_apSelect[i]->Update(fDeltaTime); }
	}
}

//============================================================
//	選択の更新処理
//============================================================
void CTitleStateSelect::UpdateSelect()
{
	// 前回の選択肢を保存
	m_nOldSelect = m_nCurSelect;

	// 選択肢操作
	CInputKeyboard* pKey = GET_INPUTKEY;	// キーボード情報
	CInputPad* pPad = GET_INPUTPAD;			// パッド情報
	if (pKey->IsTrigger(DIK_LEFT) ||
		pPad->IsTrigger(CInputPad::KEY_LEFT))
	{
		// 左に選択をずらす
		m_nCurSelect = (m_nCurSelect + 1) % SELECT_MAX;

		// 経過時間を初期化
		m_fCurTime = 0.0f;
	}
	if (pKey->IsTrigger(DIK_RIGHT) ||
		pPad->IsTrigger(CInputPad::KEY_RIGHT))
	{
		// 右に選択をずらす
		m_nCurSelect = (m_nCurSelect + (SELECT_MAX - 1)) % SELECT_MAX;

		// 経過時間を初期化
		m_fCurTime = 0.0f;
	}

	// 前回の選択要素の色を白色に設定
	m_apSelect[m_nOldSelect]->SetColor(select::COL_DEFAULT);

	// 現在の選択要素の色を黄色に設定
	m_apSelect[m_nCurSelect]->SetColor(select::COL_CHOICE);
}

//============================================================
//	決定の更新処理
//============================================================
void CTitleStateSelect::UpdateDecide()
{
	if (input::Decide())
	{
		// 経過時間を初期化
		m_fCurTime = 0.0f;

		// 選択肢に応じて遷移先を変更
		switch (m_nCurSelect)
		{ // 現在の選択肢ごとの処理
		case SELECT_START:
		{
			// ゲーム画面に遷移する
			m_pContext->TransGame();
			break;
		}
		case SELECT_TUTORIAL:
		{
			// ラムダ式の作成
			auto pFunc = [this]()->HRESULT
			{
				// チュートリアル表示状態にする
				return m_pContext->ChangeState(new CTitleStateTutorial);
			};

			// 遷移先選択状態に遷移する
			CFade* pFade = GET_MANAGER->GetFade();	// フェード情報
			pFade->SetFade(CFade::DEF_LEVEL, CFade::DEF_LEVEL, pFunc);
			break;
		}
		default:
			assert(false);
			break;
		}
	}
}

//============================================================
//	ロゴ遷移の更新処理
//============================================================
void CTitleStateSelect::UpdateTransLogo(const float fDeltaTime)
{
	// 現在の経過時間を加算
	m_fCurTime += fDeltaTime;
	if (m_fCurTime >= TRANS_TIME)
	{ // 経過した場合

		// ラムダ式の作成
		auto pFunc = [this]()->HRESULT
		{
			// ロゴ表示状態にする
			return m_pContext->ChangeState(new CTitleStateLogo);
		};

		// 遷移先選択状態に遷移する
		CFade* pFade = GET_MANAGER->GetFade();	// フェード情報
		pFade->SetFade(CFade::DEF_LEVEL, CFade::DEF_LEVEL, pFunc);
	}
}
