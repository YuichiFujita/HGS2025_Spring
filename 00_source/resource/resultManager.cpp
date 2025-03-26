//============================================================
//
//	リザルトマネージャー処理 [resultManager.cpp]
//	Author：藤田勇一
//
//============================================================
//************************************************************
//	インクルードファイル
//************************************************************
#include "resultManager.h"
#include "resultState.h"
#include "manager.h"
#include "retention.h"
#include "object2D.h"
#include "scrollString2D.h"
#include "comboAreaManager.h"

#ifdef SCORE
#include "multiValue.h"
#else TIMER
#include "timeUI.h"
#endif

//************************************************************
//	定数宣言
//************************************************************
namespace
{
	namespace title
	{
		const char*		TEXTURE	= "data\\TEXTURE\\timer_title000.png";	// テクスチャパス
		const VECTOR3	POS		= VECTOR3(SCREEN_CENT.x, 120.0f, 0.0f);	// 位置
		const float		HEIGHT	= 120.0f;	// 縦幅
	}

	namespace rank
	{
		const char*	FONT = "data\\FONT\\JFドット東雲ゴシック14.ttf";	// フォントパス
		const int	PRIORITY	= 6;			// テキストの優先順位
		const bool	ITALIC		= false;		// イタリック
		const float	HEIGHT		= 82.0f;		// 文字縦幅
		const float	WAIT_TIME	= 0.15f;		// 文字表示の待機時間
		const EAlignX ALIGN_X	= XALIGN_LEFT;	// 横配置
		const EAlignY ALIGN_Y	= YALIGN_TOP;	// 縦配置
		const VECTOR3 POS		= VECTOR3(500.0f, 305.0f, 0.0f);	// 位置
	}

	namespace cont
	{
		const char*		TEXTURE	= "data\\TEXTURE\\continue000.png";		// テクスチャパス
		const VECTOR3	POS		= VECTOR3(SCREEN_CENT.x, 445.0f, 0.0f);	// 位置
		const float		WIDTH	= 700.0f;	// 横幅
	}

	namespace select
	{
		const char* TEXTURE[] =	// テクスチャパス
		{
			"data\\TEXTURE\\continueSelect000.png",	// YES
			"data\\TEXTURE\\continueSelect001.png",	// NO
		};
		const VECTOR3	POS		= VECTOR3(SCREEN_CENT.x - 160.0f, 585.0f, 0.0f);	// 位置
		const VECTOR3	OFFSET	= VECTOR3(160.0f * 2.0f, 0.0f, 0.0f);	// オフセット
		const float		WIDTH	= 340.0f;				// 横幅
		const COLOR		COL_DEFAULT	= color::White();	// 通常色
		const COLOR		COL_CHOICE	= color::Yellow();	// 選択色
	}

#ifdef SCORE
	namespace score
	{
		const CValue::EType TYPE = CValue::TYPE_NORMAL;	// 数字種類
		const float DIGIT	= 8;						// スコア桁数
		const VECTOR3 POS	= VECTOR3(SCREEN_CENT.x, 240.0f, 0.0f);	// スコア位置
		const VECTOR3 SIZE	= VECTOR3(52.8f, 62.4f, 0.0f) * 1.4f;	// スコア数字大きさ
		const VECTOR3 SPACE	= VECTOR3(SIZE.x * 0.85f, 0.0f, 0.0f);	// スコア数字空白
	}
#else TIMER
	namespace time
	{
		const CValue::EType TYPE = CValue::TYPE_NORMAL;	// 数字種類
		const VECTOR3 POS		 = VECTOR3(SCREEN_CENT.x, 240.0f, 0.0f);		// タイマー位置
		const VECTOR3 VAL_SIZE	 = VECTOR3(52.8f, 62.4f, 0.0f) * 1.4f * 1.6f;			// タイマー数字大きさ
		const VECTOR3 PART_SIZE	 = VECTOR3(27.3f, 62.4f, 0.0f) * 1.2f * 1.6f;			// タイマー区切り大きさ
		const VECTOR3 VAL_SPACE	 = VECTOR3(VAL_SIZE.x * 0.85f, 0.0f, 0.0f);		// タイマー数字空白
		const VECTOR3 PART_SPACE = VECTOR3(PART_SIZE.x * 0.85f, 0.0f, 0.0f);	// タイマー区切り空白
	}
#endif
}

//************************************************************
//	親クラス [CResultManager] のメンバ関数
//************************************************************
//============================================================
//	コンストラクタ
//============================================================
CResultManager::CResultManager() :
	m_pUpdateRank	(nullptr),	// ランキング更新情報
	m_pContinue		(nullptr),	// コンテニュー情報
#ifdef SCORE
	m_pScore		(nullptr),	// スコア情報
#else TIMER
	m_pTime			(nullptr),	// タイム情報
#endif
	m_pTitle		(nullptr),	// タイトル情報
	m_pState		(nullptr),	// 状態
	m_nCurSelect	(0),		// 現在の選択肢
	m_nOldSelect	(0)			// 前回の選択肢
{
	// メンバ変数をクリア
	memset(&m_apSelect[0], 0, sizeof(m_apSelect));	// 選択肢情報
}

//============================================================
//	デストラクタ
//============================================================
CResultManager::~CResultManager()
{

}

//============================================================
//	初期化処理
//============================================================
HRESULT CResultManager::Init()
{
	CRetention* pRetention = GET_MANAGER->GetRetention();	// データ保存マネージャー

	// メンバ変数を初期化
	memset(&m_apSelect[0], 0, sizeof(m_apSelect));	// 選択肢情報
	m_pUpdateRank	= nullptr;	// ランキング更新情報
	m_pContinue		= nullptr;	// コンテニュー情報
#ifdef SCORE
	m_pScore		= nullptr;	// スコア情報
#else TIMER
	m_pTime			= nullptr;	// タイム情報
#endif
	m_pTitle		= nullptr;	// タイトル情報
	m_pState		= nullptr;	// 状態
	m_nCurSelect	= 0;		// 現在の選択肢
	m_nOldSelect	= 0;		// 前回の選択肢

	// 通常状態にする
	ChangeState(new CResultStateNormal);

	// タイトルの生成
	m_pTitle = CObject2D::Create(title::POS);
	if (m_pTitle == nullptr)
	{ // 生成に失敗した場合

		assert(false);
		return E_FAIL;
	}

	// 自動ラベルを設定
	m_pTitle->SetLabel(CObject::LABEL_UI);

	// タイトルテクスチャの割当
	m_pTitle->BindTexture(title::TEXTURE);

	// タイトル大きさの設定
	float fTitleWidth = useful::GetTexWidthFromAspect(title::HEIGHT, title::TEXTURE);	// テクスチャ基準の横幅
	m_pTitle->SetVec3Size(VECTOR3(fTitleWidth, title::HEIGHT, 0.0f));

#ifdef SCORE
	// スコアの生成
	m_pScore = CMultiValue::Create
	( // 引数
		score::TYPE,			// 数字種類
		pRetention->GetScore(),	// 数字
		score::DIGIT,			// 桁数
		score::POS,				// 位置
		score::SIZE,			// 大きさ
		score::SPACE			// 空白
	);
	if (m_pScore == nullptr)
	{ // 生成に失敗した場合

		assert(false);
		return E_FAIL;
	}
#else TIMER
	// タイムの生成
	m_pTime = CTimeUI::Create
	( // 引数
		pRetention->GetTime(),	// 時間
		time::POS,				// 位置
		time::VAL_SIZE,			// 数字の大きさ
		time::PART_SIZE,		// 区切りの大きさ
		time::VAL_SPACE,		// 数字の空白
		time::PART_SPACE,		// 区切りの空白
		time::TYPE				// 数字種類
	);
	if (m_pTime == nullptr)
	{ // 生成に失敗した場合

		assert(false);
		return E_FAIL;
	}
#endif

	const int nUpdateIdx = pRetention->GetUpdateRankIdx();	// ランキング更新インデックス
	if (nUpdateIdx > NONE_IDX)
	{ // ランキングが更新されている場合

		// 更新文字列を作成
		const std::string sRank = std::to_string(nUpdateIdx + 1);		// 更新順位文字列
		const std::string sUpdate = "ランキング" + sRank + "位更新";	// 更新文字列

		// ランキング更新の生成
		m_pUpdateRank = CScrollString2D::Create
		( // 引数
			rank::FONT,			// フォントパス
			rank::ITALIC,		// イタリック
			sUpdate,			// 表示文字列
			rank::POS,			// 原点位置
			rank::WAIT_TIME,	// 文字表示の待機時間
			rank::HEIGHT,		// 文字縦幅
			rank::ALIGN_X,		// 横配置
			rank::ALIGN_Y		// 縦配置
		);
		if (m_pUpdateRank == nullptr)
		{ // 生成に失敗した場合

			assert(false);
			return E_FAIL;
		}

		// 文字送りを開始する
		m_pUpdateRank->SetEnableScroll(true);

		// 文字送り時の再生SEを設定
		//m_pUpdateRank->SetScrollSE(CSound::LABEL_SE_TEXT01);

		// 色を黄色にする
		m_pUpdateRank->SetColor(color::Yellow());
	}

	// コンテニューの生成
	m_pContinue = CObject2D::Create(cont::POS);
	if (m_pContinue == nullptr)
	{ // 生成に失敗した場合

		assert(false);
		return E_FAIL;
	}

	// コンテニューテクスチャの割当
	m_pContinue->BindTexture(cont::TEXTURE);

	// コンテニュー大きさの設定
	float fContinueHeight = useful::GetTexHeightFromAspect(cont::WIDTH, cont::TEXTURE);	// テクスチャ基準の縦幅
	m_pContinue->SetVec3Size(VECTOR3(cont::WIDTH, fContinueHeight, 0.0f));

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

	// コンボエリアマネージャーを生成
	CComboAreaManager::Create();

	return S_OK;
}

//============================================================
//	終了処理
//============================================================
void CResultManager::Uninit()
{
	// 状態の終了
	SAFE_UNINIT(m_pState);

#ifdef SCORE
	// スコアの終了
	//SAFE_UNINIT(m_pScore);
#else TIMER
	// タイムの終了
	//SAFE_UNINIT(m_pTimer);
#endif

	// ランキング更新の終了
	SAFE_UNINIT(m_pUpdateRank);

	// コンテニューの終了
	SAFE_UNINIT(m_pContinue);

	for (int i = 0; i < SELECT_MAX; i++)
	{ // 選択肢の総数分繰り返す

		// 選択肢の終了
		SAFE_UNINIT(m_apSelect[i]);
	}
}

//============================================================
//	更新処理
//============================================================
void CResultManager::Update(const float fDeltaTime)
{
	// 状態ごとの更新
	assert(m_pState != nullptr);
	m_pState->Update(fDeltaTime);

	CFade* pFade = GET_MANAGER->GetFade();	// フェード情報
	if (!pFade->IsFade())
	{ // フェードしていない場合

		if (IsEndScroll())
		{ // 文字送りが終わっている場合

			// 選択の更新
			UpdateSelect();

			// 決定の更新
			UpdateDecide();
		}
	}

	// ランキング更新の更新
	if (m_pUpdateRank != nullptr) { m_pUpdateRank->Update(fDeltaTime); }

	// コンテニューの更新
	if (m_pContinue != nullptr) { m_pContinue->Update(fDeltaTime); }

	for (int i = 0; i < SELECT_MAX; i++)
	{ // 選択肢の総数分繰り返す

		// 選択肢の更新
		if (m_apSelect[i] != nullptr) { m_apSelect[i]->Update(fDeltaTime); }
	}

#ifdef SCORE
	// スコアの更新
	//m_pScore->Update(fDeltaTime);
#else TIMER
	// タイムの更新
	//m_pTime->Update(fDeltaTime);
#endif
}

//============================================================
//	状態の変更処理
//============================================================
HRESULT CResultManager::ChangeState(CResultState* pState)
{
	// 状態の生成に失敗している場合抜ける
	if (pState == nullptr) { assert(false); return E_FAIL; }

	// 状態インスタンスを終了
	SAFE_UNINIT(m_pState);

	// 状態インスタンスを変更
	assert(m_pState == nullptr);
	m_pState = pState;

	// 状態にコンテキストを設定
	m_pState->SetContext(this);

	// 状態インスタンスを初期化
	if (FAILED(m_pState->Init()))
	{ // 初期化に失敗した場合

		assert(false);
		return E_FAIL;
	}

	return S_OK;
}

//============================================================
//	ゲーム画面遷移処理
//============================================================
void CResultManager::TransGame()
{
	// フェード中の場合抜ける
	if (GET_MANAGER->GetFade()->IsFade()) { return; }

	// ランキング更新情報の初期化
	CRetention* pRetention = GET_MANAGER->GetRetention();	// データ保存マネージャー
	pRetention->SetUpdateRankIdx(NONE_IDX);

	// ゲーム画面に遷移する
	GET_MANAGER->SetLoadScene(CScene::MODE_GAME);
}

//============================================================
//	ランキング画面遷移処理
//============================================================
void CResultManager::TransRanking()
{
	// フェード中の場合抜ける
	if (GET_MANAGER->GetFade()->IsFade()) { return; }

	// ランキング画面に遷移する
	GET_MANAGER->SetLoadScene(CScene::MODE_RANKING);
}

//============================================================
//	生成処理
//============================================================
CResultManager* CResultManager::Create()
{
	// リザルトマネージャーの生成
	CResultManager* pResultManager = new CResultManager;
	if (pResultManager == nullptr)
	{ // 生成に失敗した場合

		return nullptr;
	}
	else
	{ // 生成に成功した場合

		// リザルトマネージャーの初期化
		if (FAILED(pResultManager->Init()))
		{ // 初期化に失敗した場合

			// リザルトマネージャーの破棄
			SAFE_DELETE(pResultManager);
			return nullptr;
		}

		// 確保したアドレスを返す
		return pResultManager;
	}
}

//============================================================
//	破棄処理
//============================================================
void CResultManager::Release(CResultManager*& prResultManager)
{
	// リザルトマネージャーの終了
	assert(prResultManager != nullptr);
	prResultManager->Uninit();

	// メモリ開放
	SAFE_DELETE(prResultManager);
}

//============================================================
//	選択の更新処理
//============================================================
void CResultManager::UpdateSelect()
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

		PLAY_SOUND(CSound::LABEL_SE_SELECT);
	}
	if (pKey->IsTrigger(DIK_RIGHT) ||
		pPad->IsTrigger(CInputPad::KEY_RIGHT))
	{
		// 右に選択をずらす
		m_nCurSelect = (m_nCurSelect + (SELECT_MAX - 1)) % SELECT_MAX;

		PLAY_SOUND(CSound::LABEL_SE_SELECT);
	}

	// 前回の選択要素の色を白色に設定
	m_apSelect[m_nOldSelect]->SetColor(select::COL_DEFAULT);

	// 現在の選択要素の色を黄色に設定
	m_apSelect[m_nCurSelect]->SetColor(select::COL_CHOICE);
}

//============================================================
//	決定の更新処理
//============================================================
void CResultManager::UpdateDecide()
{
	if (input::Decide())
	{
		// 選択肢に応じて遷移先を変更
		switch (m_nCurSelect)
		{ // 現在の選択肢ごとの処理
		case SELECT_CONTINUE:
		{
			// ゲーム画面に遷移する
			TransGame();
			break;
		}
		case SELECT_RANKING:
		{
			// ランキング画面に遷移する
			TransRanking();
			break;
		}
		default:
			assert(false);
			break;
		}

		PLAY_SOUND(CSound::LABEL_SE_DESITION);
	}
}

//============================================================
//	文字送り終了フラグの取得処理
//============================================================
bool CResultManager::IsEndScroll() const
{
	// 文字が未生成の場合文字送り終了済みとする
	if (m_pUpdateRank == nullptr) { return true; }
	
	// 文字送り終了フラグを返す
	return m_pUpdateRank->IsEndScroll();
}
