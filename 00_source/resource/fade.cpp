//============================================================
//
//	フェード処理 [fade.cpp]
//	Author：藤田勇一
//
//============================================================
//************************************************************
//	インクルードファイル
//************************************************************
#include "fade.h"
#include "manager.h"
#include "renderer.h"
#include "loading.h"
#include "sceneGame.h"

//************************************************************
//	定数宣言
//************************************************************
namespace
{
#ifdef _DEBUG

	const CScene::EMode INIT_SCENE = CScene::MODE_TITLE;	// 初期シーン

#else	// NDEBUG

	const CScene::EMode INIT_SCENE = CScene::MODE_TITLE;	// 初期シーン

#endif	// _DEBUG

	const int PRIORITY = 7;	// フェードの優先順位
}

//************************************************************
//	親クラス [CFade] のメンバ関数
//************************************************************
//============================================================
//	コンストラクタ
//============================================================
CFade::CFade() :
	m_funcSetMode	(nullptr),		// モード設定関数ポインタ
	m_modeNext		(INIT_SCENE),	// 遷移先モード
	m_fade			(FADE_NONE),	// フェード状態
	m_type			(TYPE_NONE),	// フェード種類
	m_fWaitTime		(0.0f),			// 現在の余韻時間
	m_fSubIn		(0.0f),			// インのα値減少量
	m_fAddOut		(0.0f)			// アウトのα値増加量
{

}

//============================================================
//	デストラクタ
//============================================================
CFade::~CFade()
{

}

//============================================================
//	初期化処理
//============================================================
HRESULT CFade::Init()
{
	// メンバ変数を初期化
	m_funcSetMode	= nullptr;		// モード設定関数ポインタ
	m_modeNext		= INIT_SCENE;	// 遷移先モード
	m_fade			= FADE_IN;		// フェード状態
	m_type			= TYPE_NONE;	// フェード種類
	m_fWaitTime		= 0.0f;			// 現在の余韻時間
	m_fSubIn		= 1.0f;			// インのα値減少量
	m_fAddOut		= 1.0f;			// アウトのα値増加量

	// オブジェクト2Dの初期化
	if (FAILED(CObject2D::Init()))
	{ // 初期化に失敗した場合

		assert(false);
		return E_FAIL;
	}

	// 位置をスクリーン中央にする
	SetVec3Position(SCREEN_CENT);

	// 大きさをスクリーンサイズにする
	SetVec3Size(SCREEN_SIZE);

	// 色を黒にする
	SetColor(color::Black());

	// 優先順位を一番上にする
	SetPriority(PRIORITY);

	// ラベル指定なしにする
	SetLabel(CObject::LABEL_NONE);	// 自動破棄・更新を停止する

	// シーンの初期化
	if (FAILED(GET_MANAGER->InitScene(m_modeNext)))
	{ // 初期化に失敗した場合

		assert(false);
		return E_FAIL;
	}

	return S_OK;
}

//============================================================
//	終了処理
//============================================================
void CFade::Uninit()
{
	// オブジェクト2Dの終了
	CObject2D::Uninit();
}

//============================================================
//	更新処理
//============================================================
void CFade::Update(const float fDeltaTime)
{
	// フェードしていない場合抜ける
	if (m_fade == FADE_NONE) { m_type = TYPE_NONE; return; }

	COLOR colFade = GetColor();	// フェード色
	switch (m_fade)
	{ // フェード状態ごとの処理
	case FADE_WAIT:	// フェード余韻状態

		// 余韻時刻を減らす
		m_fWaitTime -= fDeltaTime;
		if (m_fWaitTime <= 0.0f)
		{ // 余韻終了した場合

			// 余韻時間を初期化
			m_fWaitTime = 0.0f;

			// フェードアウト状態にする
			m_fade = FADE_OUT;
		}

		break;

	case FADE_IN:	// フェードイン状態

		// ロードが完了していない場合抜ける
		if (GET_MANAGER->GetLoading()->GetState() != CLoading::LOAD_NONE) { break; }

		// 透明にしていく
		colFade.a -= m_fSubIn * fDeltaTime;
		if (colFade.a <= 0.0f)
		{ // 透明になった場合

			// α値を補正
			colFade.a = 0.0f;

			// 何もしない状態にする
			m_fade = FADE_NONE;
		}

		break;

	case FADE_OUT:	// フェードアウト状態

		// 不透明にしていく
		colFade.a += m_fAddOut * fDeltaTime;
		if (colFade.a >= 1.0f)
		{ // 不透明になった場合

			// α値を補正
			colFade.a = 1.0f;

			// フェードイン状態にする
			m_fade = FADE_IN;

			if (m_funcSetMode != nullptr)
			{ // モード設定関数が指定されている場合

				// モードの設定
				m_funcSetMode();
			}
		}

		break;

	default:	// 例外処理
		assert(false);
		break;
	}

	// 色を反映
	SetColor(colFade);

	// オブジェクト2Dの更新
	CObject2D::Update(fDeltaTime);
}

//============================================================
//	描画処理
//============================================================
void CFade::Draw(CShader* pShader)
{
	// オブジェクト2Dの描画
	CObject2D::Draw(pShader);
}

//============================================================
//	フェードの開始処理
//============================================================
void CFade::SetFade
(
	const float fAddOut,			// アウトのα値増加量
	const float fSubIn,				// インのα値減少量
	std::function<HRESULT()> pFunc,	// モード設定関数ポインタ
	const int nPriority,			// 優先順位
	const COLOR colFade				// フェード色
)
{
	// フェード中の場合抜ける
	if (m_fade != FADE_NONE) { return; }

	// α値加減量を設定
	m_fSubIn  = fSubIn;
	m_fAddOut = fAddOut;

	// 優先順位を設定
	SetPriority(nPriority);

	// 色を設定
	SetColor(colFade);

	// モード設定関数ポインタを設定
	m_funcSetMode = pFunc;

	// フェード種類を設定
	m_type = TYPE_FADE;

	// フェードアウト状態にする
	m_fade = FADE_OUT;
}

//============================================================
//	遷移先モードの設定処理 (フェードのみ)
//============================================================
void CFade::SetModeFade
(
	const CScene::EMode mode,	// 遷移先モード
	const float fWaitTime,		// 余韻時間
	const float fAddOut,		// アウトのα値増加量
	const float fSubIn,			// インのα値減少量
	const COLOR colFade			// フェード色
)
{
	// フェード中の場合抜ける
	if (m_fade != FADE_NONE) { return; }

	// 余韻フレームオーバー
	assert(fWaitTime >= 0.0f);

	// 引数のモードを設定
	m_modeNext = mode;

	// 余韻時間を設定
	m_fWaitTime = fWaitTime;

	// α値加減量を設定
	m_fSubIn  = fSubIn;
	m_fAddOut = fAddOut;

	// 色を設定
	SetColor(colFade);

	// 優先順位を設定
	SetPriority(PRIORITY);

	// ロード画面を挟まないモード設定関数を設定
	m_funcSetMode = std::bind(&CManager::SetMode, GET_MANAGER);

	// フェード種類を設定
	m_type = TYPE_MODE;

	if (m_fWaitTime <= 0.0f)
	{ // カウンターが未設定の場合

		// フェードアウト状態にする
		m_fade = FADE_OUT;
	}
	else
	{ // カウンターが設定された場合

		// フェード余韻状態にする
		m_fade = FADE_WAIT;
	}
}

//============================================================
//	遷移先モードの設定処理 (ロード画面付き)
//============================================================
void CFade::SetLoadFade
(
	const CScene::EMode mode,	// 遷移先モード
	const float fWaitTime,		// 余韻時間
	const float fAddOut,		// アウトのα値増加量
	const float fSubIn,			// インのα値減少量
	const COLOR colFade			// フェード色
)
{
	// フェード中の場合抜ける
	if (m_fade != FADE_NONE) { return; }

	// 余韻フレームオーバー
	assert(fWaitTime >= 0.0f);

	// 引数のモードを設定
	m_modeNext = mode;

	// 余韻時間を設定
	m_fWaitTime = fWaitTime;

	// α値加減量を設定
	m_fSubIn  = fSubIn;
	m_fAddOut = fAddOut;

	// 色を設定
	SetColor(colFade);

	// 優先順位を設定
	SetPriority(PRIORITY);

	// ロード画面を挟むモード設定関数を設定
	m_funcSetMode = std::bind(&CManager::SetLoadMode, GET_MANAGER);

	// フェード種類を設定
	m_type = TYPE_LOAD;

	if (m_fWaitTime <= 0.0f)
	{ // カウンターが未設定の場合

		// フェードアウト状態にする
		m_fade = FADE_OUT;
	}
	else
	{ // カウンターが設定された場合

		// フェード余韻状態にする
		m_fade = FADE_WAIT;
	}
}

//============================================================
//	生成処理
//============================================================
CFade* CFade::Create()
{
	// フェードの生成
	CFade* pFade = new CFade;
	if (pFade == nullptr)
	{ // 生成に失敗した場合

		return nullptr;
	}
	else
	{ // 生成に成功した場合

		// フェードの初期化
		if (FAILED(pFade->Init()))
		{ // 初期化に失敗した場合

			// フェードの破棄
			SAFE_DELETE(pFade);
			return nullptr;
		}

		// 確保したアドレスを返す
		return pFade;
	}
}
