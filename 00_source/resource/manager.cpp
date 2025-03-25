//============================================================
//
//	マネージャー処理 [manager.cpp]
//	Author：藤田勇一
//
//============================================================
//************************************************************
//	インクルードファイル
//************************************************************
#include "manager.h"
#include "object.h"
#include "deltaTime.h"
#include "renderer.h"
#include "sound.h"
#include "camera.h"
#include "lightManager.h"
#include "loading.h"
#include "texture.h"
#include "model.h"
#include "font.h"
#include "character.h"
#include "characterAnim3D.h"
#include "stage.h"
#include "shader.h"
#include "retention.h"
#include "debug.h"

//************************************************************
//	静的メンバ変数宣言
//************************************************************
CManager* CManager::m_pManager = nullptr;	// マネージャーオブジェクト

//************************************************************
//	親クラス [CManager] のメンバ関数
//************************************************************
//============================================================
//	コンストラクタ
//============================================================
CManager::CManager() :
	m_hInstance			(nullptr),	// インスタンスハンドル
	m_hWnd				(nullptr),	// ウインドウハンドル
	m_pDeltaTime		(nullptr),	// デルタタイムインスタンス
	m_pRenderer			(nullptr),	// レンダラーインスタンス
	m_pKeyboard			(nullptr),	// キーボードインスタンス
	m_pMouse			(nullptr),	// マウスインスタンス
	m_pPad				(nullptr),	// パッドインスタンス
	m_pSound			(nullptr),	// サウンドインスタンス
	m_pCamera			(nullptr),	// カメラインスタンス
	m_pLight			(nullptr),	// ライトインスタンス
	m_pTexture			(nullptr),	// テクスチャインスタンス
	m_pModel			(nullptr),	// モデルインスタンス
	m_pFont				(nullptr),	// フォントインスタンス
	m_pCharacter		(nullptr),	// キャラクターインスタンス
	m_pCharacterAnim3D	(nullptr),	// キャラクターアニメーション3Dインスタンス
	m_pStage			(nullptr),	// ステージインスタンス
	m_pFade				(nullptr),	// フェードインスタンス
	m_pLoading			(nullptr),	// ローディングインスタンス
	m_pScene			(nullptr),	// シーンインスタンス
	m_pRetention		(nullptr),	// データ保存マネージャー
	m_pDebugProc		(nullptr),	// デバッグ表示
	m_pDebug			(nullptr)	// デバッグ
{

}

//============================================================
//	デストラクタ
//============================================================
CManager::~CManager()
{

}

//============================================================
//	初期化処理
//============================================================
HRESULT CManager::Init(HINSTANCE hInstance, HWND hWnd, BOOL bWindow)
{
	//--------------------------------------------------------
	//	メンバ変数を初期化
	//--------------------------------------------------------
	m_hInstance			= hInstance;	// インスタンスハンドル
	m_hWnd				= hWnd;			// ウインドウハンドル
	m_pDeltaTime		= nullptr;		// デルタタイムインスタンス
	m_pRenderer			= nullptr;		// レンダラーインスタンス
	m_pKeyboard			= nullptr;		// キーボードインスタンス
	m_pMouse			= nullptr;		// マウスインスタンス
	m_pPad				= nullptr;		// パッドインスタンス
	m_pSound			= nullptr;		// サウンドインスタンス
	m_pCamera			= nullptr;		// カメラインスタンス
	m_pLight			= nullptr;		// ライトインスタンス
	m_pTexture			= nullptr;		// テクスチャインスタンス
	m_pModel			= nullptr;		// モデルインスタンス
	m_pFont				= nullptr;		// フォントインスタンス
	m_pCharacter		= nullptr;		// キャラクターインスタンス
	m_pCharacterAnim3D	= nullptr;		// キャラクターアニメーション3Dインスタンス
	m_pStage			= nullptr;		// ステージインスタンス
	m_pFade				= nullptr;		// フェードインスタンス
	m_pLoading			= nullptr;		// ローディングインスタンス
	m_pScene			= nullptr;		// シーンインスタンス
	m_pRetention		= nullptr;		// データ保存マネージャー
	m_pDebugProc		= nullptr;		// デバッグ表示
	m_pDebug			= nullptr;		// デバッグ

	//--------------------------------------------------------
	//	システムの生成
	//--------------------------------------------------------
	// デルタタイムの生成
	m_pDeltaTime = CDeltaTime::Create();
	if (m_pDeltaTime == nullptr)
	{ // 生成に失敗した場合

		assert(false);
		return E_FAIL;
	}

	// レンダラーの生成
	m_pRenderer = CRenderer::Create(hWnd, bWindow);
	if (m_pRenderer == nullptr)
	{ // 生成に失敗した場合

		assert(false);
		return E_FAIL;
	}

	// キーボードの生成
	m_pKeyboard = CInputKeyboard::Create(hInstance, hWnd);
	if (m_pKeyboard == nullptr)
	{ // 生成に失敗した場合

		assert(false);
		return E_FAIL;
	}

	// マウスの生成
	m_pMouse = CInputMouse::Create(hInstance, hWnd);
	if (m_pMouse == nullptr)
	{ // 生成に失敗した場合

		assert(false);
		return E_FAIL;
	}

	// パッドの生成
	m_pPad = CInputPad::Create();
	if (m_pPad == nullptr)
	{ // 生成に失敗した場合

		assert(false);
		return E_FAIL;
	}

	// サウンドの生成
	m_pSound = CSound::Create(hWnd);
	if (m_pSound == nullptr)
	{ // 生成に失敗した場合

		assert(false);
		return E_FAIL;
	}

	// カメラの生成
	m_pCamera = CCamera::Create();
	if (m_pCamera == nullptr)
	{ // 生成に失敗した場合

		assert(false);
		return E_FAIL;
	}

	// ライトマネージャーの生成
	m_pLight = CLightManager::Create();
	if (m_pLight == nullptr)
	{ // 生成に失敗した場合

		assert(false);
		return E_FAIL;
	}

	// データ保存の生成
	m_pRetention = CRetention::Create();
	if (m_pRetention == nullptr)
	{ // 生成に失敗した場合

		assert(false);
		return E_FAIL;
	}

	//--------------------------------------------------------
	//	情報の読込・設定
	//--------------------------------------------------------
	// テクスチャの生成
	m_pTexture = CTexture::Create();
	if (m_pTexture == nullptr)
	{ // 生成に失敗した場合

		assert(false);
		return E_FAIL;
	}

	// モデルの生成
	m_pModel = CModel::Create();
	if (m_pModel == nullptr)
	{ // 生成に失敗した場合

		assert(false);
		return E_FAIL;
	}

	// フォントの生成
	m_pFont = CFont::Create();
	if (m_pFont == nullptr)
	{ // 生成に失敗した場合

		assert(false);
		return E_FAIL;
	}

	// キャラクターの生成
	m_pCharacter = CCharacter::Create();
	if (m_pCharacter == nullptr)
	{ // 生成に失敗した場合

		assert(false);
		return E_FAIL;
	}

	// キャラクターアニメーション3Dの生成
	m_pCharacterAnim3D = CCharacterAnim3D::Create();
	if (m_pCharacterAnim3D == nullptr)
	{ // 生成に失敗した場合

		assert(false);
		return E_FAIL;
	}

	// ステージの生成
	m_pStage = CStage::Create();
	if (m_pStage == nullptr)
	{ // 生成に失敗した場合

		assert(false);
		return E_FAIL;
	}

	// レンダーテクスチャーの生成
	if (FAILED(m_pRenderer->CreateRenderTexture()))
	{ // 生成に失敗した場合

		assert(false);
		return E_FAIL;
	}

	// シェーダーの生成
	if (FAILED(CShader::Create()))
	{ // 生成に失敗した場合

		assert(false);
		return E_FAIL;
	}

	// ローディングの生成
	m_pLoading = CLoading::Create();
	if (m_pLoading == nullptr)
	{ // 生成に失敗した場合

		assert(false);
		return E_FAIL;
	}

	// フェードの生成・シーンの設定
	m_pFade = CFade::Create();
	if (m_pFade == nullptr)
	{ // 生成に失敗した場合

		assert(false);
		return E_FAIL;
	}

	//--------------------------------------------------------
	//	デバッグ用
	//--------------------------------------------------------
	// デバッグ表示の生成
	m_pDebugProc = CDebugProc::Create();
	if (m_pDebugProc == nullptr)
	{ // 生成に失敗した場合

		assert(false);
		return E_FAIL;
	}

	// デバッグの生成
	m_pDebug = CDebug::Create();
	if (m_pDebug == nullptr)
	{ // 生成に失敗した場合

		assert(false);
		return E_FAIL;
	}

	return S_OK;
}

//============================================================
//	読込処理
//============================================================
HRESULT CManager::Load()
{
#if NDEBUG	// Release版以外では全読込を行わない

	// テクスチャの全読込
	assert(m_pTexture != nullptr);
	if (FAILED(m_pTexture->LoadAll()))
	{ // 全読込に失敗した場合

		assert(false);
		return E_FAIL;
	}

	// モデルの全読込
	assert(m_pModel != nullptr);
	if (FAILED(m_pModel->LoadAll()))
	{ // 全読込に失敗した場合

		assert(false);
		return E_FAIL;
	}

	// キャラクターの全読込
	assert(m_pCharacter != nullptr);
	if (FAILED(m_pCharacter->LoadAll()))
	{ // 全読込に失敗した場合

		assert(false);
		return E_FAIL;
	}

	// キャラクターアニメーション3Dの全読込
	assert(m_pCharacterAnim3D != nullptr);
	if (FAILED(m_pCharacterAnim3D->LoadAll()))
	{ // 全読込に失敗した場合

		assert(false);
		return E_FAIL;
	}

#endif	// NDEBUG

	// フォントの全読込
	assert(m_pFont != nullptr);
	if (FAILED(m_pFont->LoadAll()))
	{ // 全読込に失敗した場合

		assert(false);
		return E_FAIL;
	}

	// サウンドの全読込
	assert(m_pSound != nullptr);
	if (FAILED(m_pSound->LoadAll(m_hWnd)))
	{ // 全読込に失敗した場合

		assert(false);
		return E_FAIL;
	}

	return S_OK;
}

//============================================================
//	終了処理
//============================================================
void CManager::Uninit()
{
	//--------------------------------------------------------
	//	デバッグ用
	//--------------------------------------------------------
	// デバッグ表示の破棄
	SAFE_REF_RELEASE(m_pDebugProc);

	// デバッグの破棄
	SAFE_REF_RELEASE(m_pDebug);

	//--------------------------------------------------------
	//	スレッドの破棄
	//--------------------------------------------------------
	// ローディングの破棄
	SAFE_REF_RELEASE(m_pLoading);

	//--------------------------------------------------------
	//	システムの破棄
	//--------------------------------------------------------
	// データ保存の破棄
	SAFE_REF_RELEASE(m_pRetention);

	// シーンの破棄
	SAFE_REF_RELEASE(m_pScene);

	// フェードの終了
	SAFE_UNINIT(m_pFade);

	// ライトの破棄
	SAFE_REF_RELEASE(m_pLight);

	// カメラの破棄
	SAFE_REF_RELEASE(m_pCamera);

	// サウンドの破棄
	SAFE_REF_RELEASE(m_pSound);

	// パッドの破棄
	SAFE_REF_RELEASE(m_pPad);

	// マウスの破棄
	SAFE_REF_RELEASE(m_pMouse);

	// キーボードの破棄
	SAFE_REF_RELEASE(m_pKeyboard);

	// レンダラーの破棄
	SAFE_REF_RELEASE(m_pRenderer);

	// デルタタイムの破棄
	SAFE_REF_RELEASE(m_pDeltaTime);

	//--------------------------------------------------------
	//	情報の破棄
	//--------------------------------------------------------
	// テクスチャの破棄
	SAFE_REF_RELEASE(m_pTexture);

	// モデルの破棄
	SAFE_REF_RELEASE(m_pModel);

	// フォントの破棄
	SAFE_REF_RELEASE(m_pFont);

	// キャラクターの破棄
	SAFE_REF_RELEASE(m_pCharacter);

	// キャラクターアニメーション3Dの破棄
	SAFE_REF_RELEASE(m_pCharacterAnim3D);

	// ステージの破棄
	SAFE_REF_RELEASE(m_pStage);

	// シェーダーの破棄
	CShader::Release();

	// オブジェクトの全破棄
	CObject::ReleaseAll();

	// 例外処理
	assert(CObject::GetNumAll() == 0);	// 破棄の失敗
}

//============================================================
//	更新処理
//============================================================
void CManager::Update()
{
	// デバッグ情報の更新・描画
	assert(m_pDebug != nullptr);
	m_pDebug->UpdateDebugControl();
	m_pDebug->DrawDebugControl();
	m_pDebug->DrawDebugData();

	// デルタタイムの更新
	assert(m_pDeltaTime != nullptr);
	m_pDeltaTime->Update();

	const float fDeltaTime = m_pDeltaTime->GetDeltaTime();	// 経過時間
	const float fDeltaRate = m_pDeltaTime->GetDeltaRate();	// 経過時間の割合
	const float fSlowRate = m_pDeltaTime->GetSlowRate();	// 速度低下の割合

	// パッドの更新
	assert(m_pPad != nullptr);
	m_pPad->Update();

	// マウスの更新
	assert(m_pMouse != nullptr);
	m_pMouse->Update();

	// キーボードの更新
	assert(m_pKeyboard != nullptr);
	m_pKeyboard->Update();

	// ローディングの更新
	assert(m_pLoading != nullptr);
	m_pLoading->Update(fDeltaTime);

	// フェードの更新
	assert(m_pFade != nullptr);
	m_pFade->Update(fDeltaTime);

	if (m_pLoading->GetState() == CLoading::LOAD_NONE)
	{ // ロードしていない場合

		// シーンの更新
		assert(m_pScene != nullptr);
		m_pScene->Update(fDeltaTime);
	}

	// デバッグ表示の更新
	assert(m_pDebugProc != nullptr);
	m_pDebugProc->Update();
}

//============================================================
//	描画処理
//============================================================
void CManager::Draw()
{
	// レンダラーの描画
	assert(m_pRenderer != nullptr);
	m_pRenderer->Draw();
}

//============================================================
//	生成処理
//============================================================
CManager* CManager::Create(HINSTANCE hInstance, HWND hWnd, BOOL bWindow)
{
	// インスタンス使用中
	assert(m_pManager == nullptr);

	// マネージャーの生成
	m_pManager = new CManager;
	if (m_pManager == nullptr)
	{ // 生成に失敗した場合

		assert(false);
		return nullptr;
	}
	else
	{ // 生成に成功した場合

		// マネージャーの初期化
		if (FAILED(m_pManager->Init(hInstance, hWnd, bWindow)))
		{ // 初期化に失敗した場合

			// マネージャーの破棄
			SAFE_DELETE(m_pManager);
			return nullptr;
		}

		// 確保したアドレスを返す
		return m_pManager;
	}
}

//============================================================
//	取得処理
//============================================================
CManager* CManager::GetInstance()
{
	// インスタンス未使用
	assert(m_pManager != nullptr);

	// マネージャーのポインタを返す
	return m_pManager;
}

//============================================================
//	破棄処理
//============================================================
void CManager::Release(CManager*& prManager)
{
	// マネージャーの終了
	assert(prManager != nullptr);
	prManager->Uninit();

	// メモリ開放
	SAFE_DELETE(prManager);
}

//============================================================
//	ウインドウ破棄処理
//============================================================
void CManager::ReleaseWindow()
{
	// ウインドウを破棄する
	DestroyWindow(m_pManager->m_hWnd);	// WM_DESTROYメッセージを送る
}

//============================================================
//	シーンの初期化処理
//============================================================
HRESULT CManager::InitScene(const CScene::EMode mode)
{
	// シーンの生成
	assert(m_pScene == nullptr);
	m_pScene = CScene::Create(mode);
	if (m_pScene == nullptr)
	{ // 非使用中の場合

		assert(false);
		return E_FAIL;
	}

	// ラムダ式の作成
	auto func = [this](bool* pFuncEnd) -> HRESULT
	{
		// 例外処理
		assert(pFuncEnd != nullptr);	// フラグアドレスが指定なし
		assert(!(*pFuncEnd));			// フラグが既にオン

		// マネージャーの読込
		if (FAILED(m_pManager->Load()))
		{ // 読込に失敗した場合

			*pFuncEnd = true;	// 処理の終了を設定
			return E_FAIL;
		}

		// シーンの初期化
		if (FAILED(m_pScene->Init()))
		{ // 初期化に失敗した場合

			*pFuncEnd = true;	// 処理の終了を設定
			return E_FAIL;
		}

		*pFuncEnd = true;	// 処理の終了を設定
		return S_OK;
	};

	// 実行する初期化関数を渡しロード開始
	assert(m_pLoading != nullptr);
	if (FAILED(m_pLoading->Set(func)))
	{ // ローディング開始に失敗した場合

		assert(false);
		return E_FAIL;
	}

	return S_OK;
}

//============================================================
//	シーンの設定処理 (フェード･ロード：OFF)
//============================================================
HRESULT CManager::SetScene(const CScene::EMode mode)
{
	// カメラ更新をオンにする
	m_pCamera->SetEnableUpdate(true);

	// サウンドを停止
	assert(m_pSound != nullptr);
	m_pSound->Stop();

	// シーンを破棄
	SAFE_REF_RELEASE(m_pScene);

	// オブジェクトの全破棄
	CObject::ReleaseAll();

	// 遷移先モードの保存
	m_pFade->SetNextMode(mode);

	// シーンの生成
	assert(m_pScene == nullptr);
	m_pScene = CScene::Create(mode);
	if (m_pScene == nullptr)
	{ // 非使用中の場合

		assert(false);
		return E_FAIL;
	}

	// シーンの初期化
	if (FAILED(m_pScene->Init()))
	{ // 初期化に失敗した場合

		assert(false);
		return E_FAIL;
	}

	return S_OK;
}

//============================================================
//	シーンの設定処理 (フェード：ON, ロード：OFF)
//============================================================
void CManager::SetFadeScene
(
	const CScene::EMode mode,	// 遷移先モード
	const float fWaitTime,		// 余韻時間
	const float fAddOut,		// アウトのα値増加量
	const float fSubIn,			// インのα値減少量
	const COLOR colFade			// フェード色
)
{
	// インスタンス未使用
	assert(m_pFade != nullptr);

	// 次のシーンを設定
	m_pFade->SetModeFade(mode, fWaitTime, fAddOut, fSubIn, colFade);
}

//============================================================
//	シーンの設定処理 (フェード･ロード：ON)
//============================================================
void CManager::SetLoadScene
(
	const CScene::EMode mode,	// 遷移先モード
	const float fWaitTime,		// 余韻時間
	const float fAddOut,		// アウトのα値増加量
	const float fSubIn,			// インのα値減少量
	const COLOR colFade			// フェード色
)
{
	// インスタンス未使用
	assert(m_pFade != nullptr);

	// 次のシーンを設定
	m_pFade->SetLoadFade(mode, fWaitTime, fAddOut, fSubIn, colFade);
}

//============================================================
//	モードの設定処理 (ロード：OFF)
//============================================================
HRESULT CManager::SetMode()
{
	// カメラ更新をオンにする
	m_pCamera->SetEnableUpdate(true);

	// サウンドの停止
	assert(m_pSound != nullptr);
	m_pSound->Stop();

	// シーンの破棄
	SAFE_REF_RELEASE(m_pScene);

	// オブジェクトの全破棄
	CObject::ReleaseAll();

	// シーンの生成
	assert(m_pScene == nullptr);
	m_pScene = CScene::Create(m_pFade->GetNextMode());
	if (m_pScene == nullptr)
	{ // 非使用中の場合

		assert(false);
		return E_FAIL;
	}

	// シーンの初期化
	if (FAILED(m_pScene->Init()))
	{ // 初期化に失敗した場合

		assert(false);
		return E_FAIL;
	}

	return S_OK;
}

//============================================================
//	モードの設定処理 (ロード：ON)
//============================================================
HRESULT CManager::SetLoadMode()
{
	// カメラ更新をオンにする
	m_pCamera->SetEnableUpdate(true);

	// サウンドの停止
	assert(m_pSound != nullptr);
	m_pSound->Stop();

	// シーンの破棄
	SAFE_REF_RELEASE(m_pScene);

	// オブジェクトの全破棄
	CObject::ReleaseAll();

	// シーンの生成
	assert(m_pScene == nullptr);
	m_pScene = CScene::Create(m_pFade->GetNextMode());
	if (m_pScene == nullptr)
	{ // 非使用中の場合

		assert(false);
		return E_FAIL;
	}

	// ラムダ式の作成
	auto func = [this](bool* pFuncEnd) -> HRESULT
	{
		// 例外処理
		assert(pFuncEnd != nullptr);	// フラグアドレスが指定なし
		assert(!(*pFuncEnd));			// フラグが既にオン

		// シーンの初期化
		if (FAILED(m_pScene->Init()))
		{ // 初期化に失敗した場合

			*pFuncEnd = true;	// 処理の終了を設定
			return E_FAIL;
		}

		*pFuncEnd = true;	// 処理の終了を設定
		return S_OK;
	};

	// 実行する初期化関数を渡しロード開始
	assert(m_pLoading != nullptr);
	if (FAILED(m_pLoading->Set(func)))
	{ // ローディング開始に失敗した場合

		assert(false);
		return E_FAIL;
	}

	return S_OK;
}

//============================================================
//	モードの取得処理
//============================================================
CScene::EMode CManager::GetMode()
{
	// インスタンス未使用
	assert(m_pScene != nullptr);

	// 現在のモードを返す
	return m_pScene->GetMode();
}

//============================================================
//	デルタタイム取得処理
//============================================================
CDeltaTime* CManager::GetDeltaTime()
{
	// インスタンス未使用
	assert(m_pDeltaTime != nullptr);

	// デルタタイムのポインタを返す
	return m_pDeltaTime;
}

//============================================================
//	レンダラー取得処理
//============================================================
CRenderer* CManager::GetRenderer()
{
	// インスタンス未使用
	assert(m_pRenderer != nullptr);

	// レンダラーのポインタを返す
	return m_pRenderer;
}

//============================================================
//	キーボード取得処理
//============================================================
CInputKeyboard* CManager::GetKeyboard()
{
	// インスタンス未使用
	assert(m_pKeyboard != nullptr);

	// キーボードのポインタを返す
	return m_pKeyboard;
}

//============================================================
//	マウス取得処理
//============================================================
CInputMouse* CManager::GetMouse()
{
	// インスタンス未使用
	assert(m_pMouse != nullptr);

	// マウスのポインタを返す
	return m_pMouse;
}

//============================================================
//	パッド取得処理
//============================================================
CInputPad* CManager::GetPad()
{
	// インスタンス未使用
	assert(m_pPad != nullptr);

	// パッドのポインタを返す
	return m_pPad;
}

//============================================================
//	サウンド取得処理
//============================================================
CSound* CManager::GetSound()
{
	// インスタンス未使用
	assert(m_pSound != nullptr);

	// サウンドのポインタを返す
	return m_pSound;
}

//============================================================
//	カメラ取得処理
//============================================================
CCamera* CManager::GetCamera()
{
	// インスタンス未使用
	assert(m_pCamera != nullptr);

	// カメラのポインタを返す
	return m_pCamera;
}

//============================================================
//	ライト取得処理
//============================================================
CLightManager* CManager::GetLight()
{
	// インスタンス未使用
	assert(m_pLight != nullptr);

	// ライトのポインタを返す
	return m_pLight;
}

//============================================================
//	テクスチャ取得処理
//============================================================
CTexture* CManager::GetTexture()
{
	// インスタンス未使用
	assert(m_pTexture != nullptr);

	// テクスチャのポインタを返す
	return m_pTexture;
}

//============================================================
//	モデル取得処理
//============================================================
CModel* CManager::GetModel()
{
	// インスタンス未使用
	assert(m_pModel != nullptr);

	// モデルのポインタを返す
	return m_pModel;
}

//============================================================
//	フォント取得処理
//============================================================
CFont* CManager::GetFont()
{
	// インスタンス未使用
	assert(m_pFont != nullptr);

	// フォントのポインタを返す
	return m_pFont;
}

//============================================================
//	キャラクター取得処理
//============================================================
CCharacter* CManager::GetCharacter()
{
	// インスタンス未使用
	assert(m_pCharacter != nullptr);

	// キャラクターを返す
	return m_pCharacter;
}

//============================================================
//	キャラクターアニメーション3D取得処理
//============================================================
CCharacterAnim3D* CManager::GetCharacterAnim3D()
{
	// インスタンス未使用
	assert(m_pCharacterAnim3D != nullptr);

	// キャラクターアニメーション3Dを返す
	return m_pCharacterAnim3D;
}

//============================================================
//	ステージ取得処理
//============================================================
CStage* CManager::GetStage()
{
	// インスタンス未使用
	assert(m_pStage != nullptr);

	// ステージのポインタを返す
	return m_pStage;
}

//============================================================
//	フェード取得処理
//============================================================
CFade* CManager::GetFade()
{
	// インスタンス未使用
	assert(m_pFade != nullptr);

	// フェードのポインタを返す
	return m_pFade;
}

//============================================================
//	ローディング取得処理
//============================================================
CLoading* CManager::GetLoading()
{
	// インスタンス未使用
	assert(m_pLoading != nullptr);

	// ローディングのポインタを返す
	return m_pLoading;
}

//============================================================
//	シーン取得処理
//============================================================
CScene* CManager::GetScene()
{
	// インスタンス未使用
	assert(m_pScene != nullptr);

	// シーンのポインタを返す
	return m_pScene;
}

//============================================================
//	データ保存取得処理
//============================================================
CRetention* CManager::GetRetention()
{
	// インスタンス未使用
	assert(m_pRetention != nullptr);

	// データ保存マネージャーを返す
	return m_pRetention;
}

//============================================================
//	デバッグ表示取得処理
//============================================================
CDebugProc* CManager::GetDebugProc()
{
	// インスタンス未使用
	assert(m_pDebugProc != nullptr);

	// デバッグ表示のポインタを返す
	return m_pDebugProc;
}

//============================================================
//	デバッグ取得処理
//============================================================
CDebug* CManager::GetDebug()
{
	// インスタンス未使用
	assert(m_pDebug != nullptr);

	// デバッグのポインタを返す
	return m_pDebug;
}
