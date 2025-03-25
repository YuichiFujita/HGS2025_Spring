//============================================================
//
//	マネージャーヘッダー [manager.h]
//	Author：藤田勇一
//
//============================================================
//************************************************************
//	二重インクルード防止
//************************************************************
#ifndef _MANAGER_H_
#define _MANAGER_H_

//************************************************************
//	インクルードファイル
//************************************************************
#include "scene.h"
#include "fade.h"

//************************************************************
//	前方宣言
//************************************************************
class CDeltaTime;		// デルタタイムクラス
class CRenderer;		// レンダラークラス
class CInputKeyboard;	// キーボードクラス
class CInputMouse;		// マウスクラス
class CInputPad;		// パッドクラス
class CSound;			// サウンドクラス
class CCamera;			// カメラクラス
class CLightManager;	// ライトマネージャークラス
class CTexture;			// テクスチャクラス
class CModel;			// モデルクラス
class CFont;			// フォントクラス
class CCharacter;		// キャラクタークラス
class CCharacterAnim3D;	// キャラクターアニメーション3Dクラス
class CStage;			// ステージクラス
class CLoading;			// ローディングクラス
class CRetention;		// データ保存クラス
class CDebugProc;		// デバッグ表示クラス
class CDebug;			// デバッグクラス

//************************************************************
//	クラス定義
//************************************************************
// マネージャークラス
class CManager
{
public:
	// コンストラクタ
	CManager();

	// デストラクタ
	~CManager();

	// メンバ関数
	HRESULT Init(HINSTANCE hInstance, HWND hWnd, BOOL bWindow);	// 初期化
	HRESULT Load();	// 読込
	void Uninit();	// 終了
	void Update();	// 更新
	void Draw();	// 描画

	// 静的メンバ関数
	static CManager* Create(HINSTANCE hInstance, HWND hWnd, BOOL bWindow);	// 生成
	static CManager* GetInstance();				// 取得
	static void Release(CManager*& prManager);	// 破棄
	static void ReleaseWindow();				// ウインドウ破棄

	// メンバ関数
	HRESULT InitScene(const CScene::EMode mode);	// シーン初期化
	HRESULT SetScene(const CScene::EMode mode);		// シーン設定 (フェード･ロード：OFF)

	void SetFadeScene	// シーン設定 (フェード：ON, ロード：OFF)
	( // 引数
		const CScene::EMode mode,		// 遷移先モード
		const float fWaitTime	= 0.0f,	// 余韻時間
		const float fAddOut		= CFade::DEF_LEVEL,		// アウトのα値増加量
		const float fSubIn		= CFade::DEF_LEVEL,		// インのα値減少量
		const COLOR colFade		= color::Black(0.0f)	// フェード色
	);
	void SetLoadScene	// シーン設定 (フェード･ロード：ON)
	( // 引数
		const CScene::EMode mode,		// 遷移先モード
		const float fWaitTime	= 0.0f,	// 余韻時間
		const float fAddOut		= CFade::DEF_LEVEL,		// アウトのα値増加量
		const float fSubIn		= CFade::DEF_LEVEL,		// インのα値減少量
		const COLOR colFade		= color::Black(0.0f)	// フェード色
	);

	HRESULT SetMode();			// モード設定 (ロード：OFF)
	HRESULT SetLoadMode();		// モード設定 (ロード：ON)
	CScene::EMode GetMode();	// モード取得

	CDeltaTime*			GetDeltaTime();			// デルタタイム取得
	CRenderer*			GetRenderer();			// レンダラー取得
	CInputKeyboard*		GetKeyboard();			// キーボード取得
	CInputMouse*		GetMouse();				// マウス取得
	CInputPad*			GetPad();				// パッド取得
	CSound*				GetSound();				// サウンド取得
	CCamera*			GetCamera();			// カメラ取得
	CLightManager*		GetLight();				// ライトマネージャー取得
	CTexture*			GetTexture();			// テクスチャ取得
	CModel*				GetModel();				// モデル取得
	CFont*				GetFont();				// フォント取得
	CCharacter*			GetCharacter();			// キャラクター取得
	CCharacterAnim3D*	GetCharacterAnim3D();	// キャラクターアニメーション3D取得
	CStage*				GetStage();				// ステージ取得
	CFade*				GetFade();				// フェード取得
	CLoading*			GetLoading();			// ローディング取得
	CScene*				GetScene();				// シーン取得
	CRetention*			GetRetention();			// データ保存取得
	CDebugProc*			GetDebugProc();			// デバッグ表示取得
	CDebug*				GetDebug();				// デバッグ取得

private:
	// 静的メンバ変数
	static CManager* m_pManager;	// マネージャー

	// メンバ変数
	HINSTANCE			m_hInstance;		// インスタンスハンドル
	HWND				m_hWnd;				// ウインドウハンドル
	CDeltaTime*			m_pDeltaTime;		// デルタタイムインスタンス
	CRenderer*			m_pRenderer;		// レンダラーインスタンス
	CInputKeyboard*		m_pKeyboard;		// キーボードインスタンス
	CInputMouse*		m_pMouse;			// マウスインスタンス
	CInputPad*			m_pPad;				// パッドインスタンス
	CSound*				m_pSound;			// サウンドインスタンス
	CCamera*			m_pCamera;			// カメラインスタンス
	CLightManager*		m_pLight;			// ライトマネージャーインスタンス
	CTexture*			m_pTexture;			// テクスチャインスタンス
	CModel*				m_pModel;			// モデルインスタンス
	CFont*				m_pFont;			// フォントインスタンス
	CCharacter*			m_pCharacter;		// キャラクターインスタンス
	CCharacterAnim3D*	m_pCharacterAnim3D;	// キャラクターアニメーション3Dインスタンス
	CStage*				m_pStage;			// ステージインスタンス
	CFade*				m_pFade;			// フェードインスタンス
	CLoading*			m_pLoading;			// ローディングインスタンス
	CScene*				m_pScene;			// シーンインスタンス
	CRetention*			m_pRetention;		// データ保存インスタンス
	CDebugProc*			m_pDebugProc;		// デバッグ表示
	CDebug*				m_pDebug;			// デバッグ
};

#endif	// _MANAGER_H_
