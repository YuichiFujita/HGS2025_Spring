//============================================================
//
//	ポーズ処理 [pause.cpp]
//	Author：藤田勇一
//
//============================================================
//************************************************************
//	インクルードファイル
//************************************************************
#include "pause.h"
#include "renderer.h"
#include "manager.h"
#include "sceneGame.h"
#include "sound.h"
#include "fade.h"
#include "texture.h"
#include "object2D.h"
#include "timerUI.h"

//************************************************************
//	定数宣言
//************************************************************
namespace
{
	const char* TEXTURE_FILE[] =	// テクスチャファイル
	{
		"data\\TEXTURE\\pause000.png",	// 再開テクスチャ
		"data\\TEXTURE\\pause001.png",	// リトライテクスチャ
		"data\\TEXTURE\\pause002.png",	// 終了テクスチャ
	};

	const int PRIORITY = 6;	// ポーズ表示の優先順位

	const COLOR BG_COL		= COLOR(0.0f, 0.0f, 0.0f, 0.6f);	// 背景カラー
	const COLOR CHOICE_COL	= COLOR(1.0f, 1.0f, 1.0f, 1.0f);	// 選択中カラー
	const COLOR DEFAULT_COL	= COLOR(0.5f, 0.5f, 0.5f, 1.0f);	// 非選択中カラー

	namespace select
	{
		const VECTOR3 POS	= VECTOR3(640.0f, 210.0f, 0.0f);	// セレクトメニューの位置
		const VECTOR3 SIZE	= VECTOR3(440.0f, 110.0f, 0.0f);	// セレクトメニューの大きさ
		const VECTOR3 SPACE	= VECTOR3(0.0f, 150.0f, 0.0f);		// セレクトメニューの空白
	}
}

//************************************************************
//	スタティックアサート
//************************************************************
static_assert(NUM_ARRAY(TEXTURE_FILE) == CPause::SELECT_MAX, "ERROR : Texture Count Mismatch");

//************************************************************
//	子クラス [CPause] のメンバ関数
//************************************************************
//============================================================
//	コンストラクタ
//============================================================
CPause::CPause() :
	m_pBG			(nullptr),			// 背景情報
	m_nSelect		(SELECT_RESUME),	// 現在の選択
	m_nOldSelect	(SELECT_RESUME),	// 前回の選択
	m_bPause		(false)				// ポーズ状況
{
	// メンバ変数をクリア
	memset(&m_apSelect[0], 0, sizeof(m_apSelect));	// 選択情報
}

//============================================================
//	デストラクタ
//============================================================
CPause::~CPause()
{

}

//============================================================
//	初期化処理
//============================================================
HRESULT CPause::Init()
{
	// メンバ変数を初期化
	memset(&m_apSelect[0], 0, sizeof(m_apSelect));	// 選択情報
	m_pBG		 = nullptr;			// 背景情報
	m_nSelect	 = SELECT_RESUME;	// 現在の選択
	m_nOldSelect = SELECT_RESUME;	// 前回の選択
	m_bPause	 = false;			// ポーズ状況

	// 背景情報の生成
	m_pBG = CObject2D::Create
	( // 引数
		SCREEN_CENT,	// 位置
		SCREEN_SIZE,	// 大きさ
		VEC3_ZERO,		// 向き
		BG_COL			// 色
	);
	if (m_pBG == nullptr)
	{ // 非使用中の場合

		assert(false);
		return E_FAIL;
	}

	// 優先順位を設定
	m_pBG->SetPriority(PRIORITY);

	for (int nCntPause = 0; nCntPause < SELECT_MAX; nCntPause++)
	{ // 選択肢の項目数分繰り返す

		// 選択情報の生成
		m_apSelect[nCntPause] = CObject2D::Create
		( // 引数
			select::POS + (select::SPACE * (float)nCntPause),	// 位置
			select::SIZE,	// 大きさ
			VEC3_ZERO,		// 向き
			DEFAULT_COL		// 色
		);
		if (m_apSelect[nCntPause] == nullptr)
		{ // 非使用中の場合

			assert(false);
			return E_FAIL;
		}

		// 優先順位を設定
		m_apSelect[nCntPause]->SetPriority(PRIORITY);

		// テクスチャを登録・割当
		m_apSelect[nCntPause]->BindTexture(GET_MANAGER->GetTexture()->Regist(TEXTURE_FILE[nCntPause]));
	}

	// 描画状況の設定
	SetEnableDraw(m_bPause);

	return S_OK;
}

//============================================================
//	終了処理
//============================================================
void CPause::Uninit()
{
	// 背景情報の終了
	SAFE_UNINIT(m_pBG);

	for (int nCntPause = 0; nCntPause < SELECT_MAX; nCntPause++)
	{ // 選択肢の項目数分繰り返す

		// 選択情報の終了
		SAFE_UNINIT(m_apSelect[nCntPause]);
	}
}

//============================================================
//	更新処理
//============================================================
void CPause::Update(const float fDeltaTime)
{
	if (!GET_MANAGER->GetFade()->IsFade())
	{ // フェードしていない場合

		if (GET_INPUTKEY->IsTrigger(DIK_P)
		||  GET_INPUTPAD->IsTrigger(CInputPad::KEY_START))
		{
			// ポーズ状況を切り替え
			m_bPause = !m_bPause;

			// 全タイマーの計測状況の設定
			CTimer::EnableStopAll(m_bPause);

			// 現在の選択を初期化
			m_nSelect = SELECT_RESUME;

			// 描画状況の設定
			SetEnableDraw(m_bPause);
		}
	}

	if (m_bPause)
	{ // ポーズ中の場合

		// 選択操作
		Select();

		// 前回の選択要素の色を黒に設定
		m_apSelect[m_nOldSelect]->SetColor(DEFAULT_COL);

		// 現在の選択要素の色を白に設定
		m_apSelect[m_nSelect]->SetColor(CHOICE_COL);

		// 現在の選択要素を代入
		m_nOldSelect = m_nSelect;
	}

	// 背景情報の更新
	m_pBG->Update(fDeltaTime);

	for (int nCntPause = 0; nCntPause < SELECT_MAX; nCntPause++)
	{ // 選択肢の項目数分繰り返す

		// 選択情報の更新
		m_apSelect[nCntPause]->Update(fDeltaTime);
	}
}

//============================================================
//	描画処理
//============================================================
void CPause::Draw()
{

}

//============================================================
//	ポーズ状況取得処理
//============================================================
bool CPause::IsPause()
{
	// ポーズ状況を返す
	return m_bPause;
}

//============================================================
//	描画状況の設定処理
//============================================================
void CPause::SetEnableDraw(const bool bDraw)
{
	// 背景情報の描画状況を設定
	m_pBG->SetEnableDraw(bDraw);

	for (int nCntPause = 0; nCntPause < SELECT_MAX; nCntPause++)
	{ // 選択肢の項目数分繰り返す

		// 選択情報の描画状況を設定
		m_apSelect[nCntPause]->SetEnableDraw(bDraw);
	}
}

//============================================================
//	生成処理
//============================================================
CPause* CPause::Create()
{
	// ポーズの生成
	CPause* pPause = new CPause;
	if (pPause == nullptr)
	{ // 生成に失敗した場合

		return nullptr;
	}
	else
	{ // 生成に成功した場合

		// ポーズの初期化
		if (FAILED(pPause->Init()))
		{ // 初期化に失敗した場合

			// ポーズの破棄
			SAFE_DELETE(pPause);
			return nullptr;
		}

		// 確保したアドレスを返す
		return pPause;
	}
}

//============================================================
//	破棄処理
//============================================================
void CPause::Release(CPause*& prPause)
{
	// ポーズの終了
	assert(prPause != nullptr);
	prPause->Uninit();

	// メモリ開放
	SAFE_DELETE(prPause);
}

//============================================================
//	デバッグ表示状況の設定処理
//============================================================
#ifdef _DEBUG

void CPause::SetEnableDebugDisp(const bool bDisp)
{
	// 背景情報の描画状況を設定
	m_pBG->SetEnableDebugDisp(bDisp);

	for (int nCntPause = 0; nCntPause < SELECT_MAX; nCntPause++)
	{ // 選択肢の項目数分繰り返す

		// 選択情報の描画状況を設定
		m_apSelect[nCntPause]->SetEnableDebugDisp(bDisp);
	}
}

#endif	// _DEBUG

//============================================================
//	デバッグ表示状況の取得処理
//============================================================
#ifdef _DEBUG

bool CPause::IsDebugDisp() const
{
	// デバッグ表示状況を返す
	return m_pBG->IsDebugDisp();
}

#endif	// _DEBUG

//============================================================
//	選択操作処理
//============================================================
void CPause::Select()
{
	// フェード中の場合抜ける
	if (GET_MANAGER->GetFade()->IsFade()) { return; }

	CInputKeyboard*	pKeyboard	= GET_INPUTKEY;	// キーボード
	CInputPad*		pPad		= GET_INPUTPAD;	// パッド
	if (pKeyboard->IsTrigger(DIK_W)
	||  pKeyboard->IsTrigger(DIK_UP)
	||  pPad->IsTrigger(CInputPad::KEY_UP))
	{ // 上移動の操作が行われた場合

		// 上に選択をずらす
		m_nSelect = (m_nSelect + (SELECT_MAX - 1)) % SELECT_MAX;
	}
	if (pKeyboard->IsTrigger(DIK_S)
	||  pKeyboard->IsTrigger(DIK_DOWN)
	||  pPad->IsTrigger(CInputPad::KEY_DOWN))
	{ // 下移動の操作が行われた場合

		// 下に選択をずらす
		m_nSelect = (m_nSelect + 1) % SELECT_MAX;
	}

	if (pKeyboard->IsTrigger(DIK_RETURN)  || pKeyboard->IsTrigger(DIK_SPACE)
	||  pPad->IsTrigger(CInputPad::KEY_A) || pPad->IsTrigger(CInputPad::KEY_B)
	||  pPad->IsTrigger(CInputPad::KEY_X) || pPad->IsTrigger(CInputPad::KEY_Y))
	{ // 決定の操作が行われた場合

		switch (m_nSelect)
		{ // 選択ごとの処理
		case SELECT_RESUME:	// 再開

			// ポーズを終了する
			m_bPause = false;

			// 全タイマーの計測状況の設定
			CTimer::EnableStopAll(m_bPause);

			// 描画状況の設定
			SetEnableDraw(m_bPause);
			break;

		case SELECT_RETRY:	// リトライ

			// ゲーム画面に遷移
			GET_MANAGER->SetLoadScene(CScene::MODE_GAME);
			break;

		case SELECT_EXIT:	// 終了

			// タイトル画面に遷移
			GET_MANAGER->SetLoadScene(CScene::MODE_TITLE);
			break;
		}
	}
}
