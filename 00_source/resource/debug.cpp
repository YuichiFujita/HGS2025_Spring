//============================================================
//
//	デバッグ処理 [debug.cpp]
//	Author：藤田勇一
//
//============================================================
//************************************************************
//	インクルードファイル
//************************************************************
#include "debug.h"
#include "manager.h"
#include "object.h"
#include "renderer.h"
#include "scene.h"
#include "sceneGame.h"
#include "gameManager.h"
#include "debugproc.h"
#include "pause.h"

//************************************************************
//	定数宣言
//************************************************************
namespace
{
	const int MEASURE_FPS	= 500;	// FPSの計測タイミング
	const int FILL_MAX		= 3;	// 塗りつぶしモードの総数
}

//************************************************************
//	マクロ定義
//************************************************************
#define KEY_DEBUG_DISP		(DIK_F1)	// デバッグ表示ON/OFFキー
#define NAME_DEBUG_DISP		("F1")		// デバッグ表示ON/OFF表示
#define KEY_CAMERA_CONTROL	(DIK_F2)	// カメラ操作ON/OFFキー
#define NAME_CAMERA_CONTROL	("F2")		// カメラ操作ON/OFF表示
#define KEY_FILLMODE		(DIK_F3)	// 塗りつぶしモード変更キー
#define NAME_FILLMODE		("F3")		// 塗りつぶしモード変更表示
#define KEY_2D_DISP			(DIK_F4)	// 2Dオブジェクト表示ON/OFFキー
#define NAME_2D_DISP		("F4")		// 2Dオブジェクト表示ON/OFF表示
#define KEY_PAUSE_DISP		(DIK_F5)	// ポーズ表示ON/OFFキー
#define NAME_PAUSE_DISP		("F5")		// ポーズ表示ON/OFF表示
#define KEY_RESULT_TRANS	(DIK_F6)	// リザルト遷移キー
#define NAME_RESULT_TRANS	("F6")		// リザルト遷移表示

//************************************************************
//	親クラス [CDebug] のメンバ関数
//************************************************************
//============================================================
//	コンストラクタ
//============================================================
#ifdef _DEBUG

CDebug::CDebug() :
	m_nFps				(0),					// FPSカウンタ
	m_bDisp2D			(false),				// 2D表示状況
	m_bDisp3D			(false),				// 3D表示状況
	m_dwFrameCount		(0),					// フレームカウント
	m_dwFPSLastTime		(0),					// 最後にFPSを計測した時刻
	m_fillMode			(D3DFILL_SOLID),		// 塗りつぶしモード
	m_oldCameraState	(CCamera::STATE_NONE)	// カメラの過去状態
{

}

#else	// NDEBUG

CDebug::CDebug() {}

#endif	// _DEBUG

//============================================================
//	デストラクタ
//============================================================
#ifdef _DEBUG

CDebug::~CDebug()
{

}

#else	// NDEBUG

CDebug::~CDebug() {}

#endif	// _DEBUG

//============================================================
//	初期化処理
//============================================================
#ifdef _DEBUG

HRESULT CDebug::Init()
{
	// メンバ変数を初期化
	m_nFps				= 0;					// FPSカウンタ
	m_bDisp2D			= true;					// 2D表示状況
	m_bDisp3D			= true;					// 3D表示状況
	m_dwFrameCount		= 0;					// フレームカウント
	m_dwFPSLastTime		= timeGetTime();		// 現在時刻を取得
	m_fillMode			= D3DFILL_SOLID;		// 塗りつぶしモード
	m_oldCameraState	= CCamera::STATE_NONE;	// カメラの過去状態

	// オブジェクトの表示状況を設定
	CObject::SetEnableDebugDispAll(m_bDisp2D, m_bDisp3D);

	return S_OK;
}

#else	// NDEBUG

HRESULT CDebug::Init() { return S_OK; }

#endif	// _DEBUG

//============================================================
//	終了処理
//============================================================
#ifdef _DEBUG

void CDebug::Uninit()
{

}

#else	// NDEBUG

void CDebug::Uninit() {}

#endif	// _DEBUG

//============================================================
//	FPS測定処理
//============================================================
#ifdef _DEBUG

void CDebug::MeasureFps(const DWORD dwCurrentTime)
{
	if ((dwCurrentTime - m_dwFPSLastTime) >= MEASURE_FPS)
	{ // 0.5秒経過

		// FPSを計測
		m_nFps = (m_dwFrameCount * 1000) / (dwCurrentTime - m_dwFPSLastTime);

		// FPSを測定した時刻を保存
		m_dwFPSLastTime = dwCurrentTime;

		// フレームカウントをクリア
		m_dwFrameCount = 0;
	}
}

#else	// NDEBUG

void CDebug::MeasureFps(const DWORD /*dwCurrentTime*/) {}

#endif	// _DEBUG

//============================================================
//	フレームカウント加算処理
//============================================================
#ifdef _DEBUG

void CDebug::AddFrameCount()
{
	// フレームカウントを加算
	m_dwFrameCount++;
}

#else	// NDEBUG

void CDebug::AddFrameCount() {}

#endif	// _DEBUG

//============================================================
//	FPS取得処理
//============================================================
#ifdef _DEBUG

int CDebug::GetFps() const
{
	// FPSを返す
	return m_nFps;
}

#else	// NDEBUG

int CDebug::GetFps() const { return 0; }

#endif	// _DEBUG

//============================================================
//	塗りつぶしモード設定処理
//============================================================
#ifdef _DEBUG

void CDebug::SetFillMode()
{
	// 塗りつぶしモードを現在のモードに設定
	GET_DEVICE->SetRenderState(D3DRS_FILLMODE, m_fillMode);
}

#else	// NDEBUG

void CDebug::SetFillMode() {}

#endif	// _DEBUG

//============================================================
//	デバッグ操作更新処理
//============================================================
#ifdef _DEBUG

void CDebug::UpdateDebugControl()
{
	// デバッグ表示変更
	ChangeDispDebug();

	// カメラ操作変更
	ChangeControlCamera();

	// 塗りつぶしモード変更
	ChangeFillMode();

	// 2Dオブジェクト表示変更
	ChangeDisp2D();

	switch (GET_MANAGER->GetMode())
	{ // モードごとの処理
	case CScene::MODE_TITLE:
		break;

	case CScene::MODE_GAME:

		// ポーズ表示変更
		ChangeDispPause();

		// リザルト遷移
		TransResult();
		break;

	case CScene::MODE_RESULT:
		break;

	case CScene::MODE_RANKING:
		break;

	default:
		assert(false);
		break;
	}
}

#else	// NDEBUG

void CDebug::UpdateDebugControl() {}

#endif	// _DEBUG

//============================================================
//	デバッグ操作表示処理
//============================================================
#ifdef _DEBUG

void CDebug::DrawDebugControl()
{
	CDebugProc* pDebugProc = GET_MANAGER->GetDebugProc();	// デバッグプロックの情報

	pDebugProc->Print(CDebugProc::POINT_LEFT, "======================================\n");
	pDebugProc->Print(CDebugProc::POINT_LEFT, "　[デバッグ操作]\n");
	pDebugProc->Print(CDebugProc::POINT_LEFT, "======================================\n");
	pDebugProc->Print(CDebugProc::POINT_LEFT, "[%s]：デバッグ表示のON/OFF\n", NAME_DEBUG_DISP);
	pDebugProc->Print(CDebugProc::POINT_LEFT, "[%s]：カメラ操作のON/OFF\n", NAME_CAMERA_CONTROL);
	pDebugProc->Print(CDebugProc::POINT_LEFT, "[%s]：塗りつぶしモードの変更\n", NAME_FILLMODE);
	pDebugProc->Print(CDebugProc::POINT_LEFT, "[%s]：2Dオブジェクト表示のON/OFF\n", NAME_2D_DISP);

	switch (GET_MANAGER->GetMode())
	{ // モードごとの処理
	case CScene::MODE_TITLE:
		break;

	case CScene::MODE_GAME:

		pDebugProc->Print(CDebugProc::POINT_LEFT, "[%s]：ポーズ描画のON/OFF\n", NAME_PAUSE_DISP);
		pDebugProc->Print(CDebugProc::POINT_LEFT, "[%s]：リザルト遷移\n", NAME_RESULT_TRANS);
		break;

	case CScene::MODE_RESULT:
		break;

	case CScene::MODE_RANKING:
		break;

	default:
		assert(false);
		break;
	}
}

#else	// NDEBUG

void CDebug::DrawDebugControl() {}

#endif	// _DEBUG

//============================================================
//	デバッグ情報表示処理
//============================================================
#ifdef _DEBUG

void CDebug::DrawDebugData()
{
	CDebugProc* pDebugProc = GET_MANAGER->GetDebugProc();	// デバッグプロックの情報

	pDebugProc->Print(CDebugProc::POINT_LEFT, "======================================\n");
	pDebugProc->Print(CDebugProc::POINT_LEFT, "　[デバッグ情報]\n");
	pDebugProc->Print(CDebugProc::POINT_LEFT, "======================================\n");

	pDebugProc->Print(CDebugProc::POINT_CENTER, "[FPS]：%d\n", m_nFps);
}

#else	// NDEBUG

void CDebug::DrawDebugData() {}

#endif	// _DEBUG

//============================================================
//	生成処理
//============================================================
CDebug* CDebug::Create()
{
	// デバッグの生成
	CDebug* pDebug = new CDebug;
	if (pDebug == nullptr)
	{ // 生成に失敗した場合

		return nullptr;
	}
	else
	{ // 生成に成功した場合

		// デバッグの初期化
		if (FAILED(pDebug->Init()))
		{ // 初期化に失敗した場合

			// デバッグの破棄
			SAFE_DELETE(pDebug);
			return nullptr;
		}

		// 確保したアドレスを返す
		return pDebug;
	}
}

//============================================================
//	破棄処理
//============================================================
void CDebug::Release(CDebug*& prDebug)
{
	// デバッグの終了
	assert(prDebug != nullptr);
	prDebug->Uninit();

	// メモリ開放
	SAFE_DELETE(prDebug);
}

//************************************************************
//	private メンバ関数 (デバッグ時以外存在しない)
//************************************************************
#ifdef _DEBUG

//============================================================
//	デバッグ表示変更処理
//============================================================
void CDebug::ChangeDispDebug()
{
	if (GET_INPUTKEY->IsTrigger(KEY_DEBUG_DISP))
	{
		// デバッグの表示フラグを反転
		GET_MANAGER->GetDebugProc()->SetDisp(!GET_MANAGER->GetDebugProc()->IsDisp());
	}
}

//============================================================
//	カメラ操作変更処理
//============================================================
void CDebug::ChangeControlCamera()
{
	if (GET_INPUTKEY->IsTrigger(KEY_CAMERA_CONTROL))
	{
		if (GET_MANAGER->GetCamera()->GetState() != CCamera::STATE_CONTROL)
		{ // カメラ操作がOFFの場合

			// 現在の状態を保存
			m_oldCameraState = GET_MANAGER->GetCamera()->GetState();

			// 操作カメラを設定
			GET_MANAGER->GetCamera()->SetState(CCamera::STATE_CONTROL);
		}
		else
		{ // カメラ操作がONの場合

			// 過去の状態を再設定
			GET_MANAGER->GetCamera()->SetState(m_oldCameraState);

			// 過去の状態を初期化
			m_oldCameraState = CCamera::STATE_NONE;
		}
	}
}

//============================================================
//	塗りつぶしモード変更処理
//============================================================
void CDebug::ChangeFillMode()
{
	if (GET_INPUTKEY->IsTrigger(KEY_FILLMODE))
	{
		// 塗りつぶしモードを変更する
		m_fillMode = (D3DFILLMODE)((m_fillMode + 1) % FILL_MAX);
	}
}

//============================================================
//	2Dオブジェクト表示変更処理
//============================================================
void CDebug::ChangeDisp2D()
{
	if (GET_INPUTKEY->IsTrigger(KEY_2D_DISP))
	{
		// 2Dの表示フラグを反転
		m_bDisp2D = !m_bDisp2D;

		// オブジェクトの表示状況を設定
		CObject::SetEnableDebugDispAll(m_bDisp2D, m_bDisp3D);
	}
}

//============================================================
//	ポーズ表示変更処理
//============================================================
void CDebug::ChangeDispPause()
{
	if (GET_INPUTKEY->IsTrigger(KEY_PAUSE_DISP))
	{
		// ポーズの表示状況を設定
		CSceneGame::GetPause()->SetEnableDebugDisp(!CSceneGame::GetPause()->IsDebugDisp());
	}
}

//============================================================
//	リザルト遷移処理
//============================================================
void CDebug::TransResult()
{
	if (GET_INPUTKEY->IsTrigger(KEY_RESULT_TRANS))
	{
		// リザルトに遷移する
		CSceneGame::GetGameManager()->TransResult();
	}
}

#endif	// _DEBUG
