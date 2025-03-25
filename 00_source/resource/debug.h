//============================================================
//
//	デバッグヘッダー [debug.h]
//	Author：藤田勇一
//
//============================================================
//************************************************************
//	二重インクルード防止
//************************************************************
#ifndef _DEBUG_H_
#define _DEBUG_H_

//************************************************************
//	インクルードファイル
//************************************************************
#include "camera.h"

//************************************************************
//	クラス定義
//************************************************************
// デバッグクラス
class CDebug
{
public:
	// コンストラクタ
	CDebug();

	// デストラクタ
	~CDebug();

	// メンバ関数
	HRESULT Init();	// 初期化
	void Uninit();	// 終了
	void MeasureFps(const DWORD dwCurrentTime);	// FPS測定
	void AddFrameCount();		// フレームカウント加算
	int  GetFps() const;		// FPS取得
	void SetFillMode();			// 塗りつぶしモード設定
	void UpdateDebugControl();	// デバッグ操作更新
	void DrawDebugControl();	// デバッグ操作表示
	void DrawDebugData();		// デバッグ情報表示

	// 静的メンバ関数
	static CDebug* Create();				// 生成
	static void Release(CDebug*& prDebug);	// 破棄

private:

#ifdef _DEBUG

	// メンバ関数
	void ChangeDispDebug();		// デバッグ表示変更
	void ChangeControlCamera();	// カメラ操作変更
	void ChangeFillMode();		// 塗りつぶしモード変更
	void ChangeDisp2D();		// 2Dオブジェクト表示変更
	void ChangeDispPause();		// ポーズ表示変更
	void TransResult();			// リザルト遷移

	// メンバ変数
	int  m_nFps;	// FPSカウンタ
	bool m_bDisp2D;	// 2D表示状況
	bool m_bDisp3D;	// 3D表示状況
	DWORD m_dwFrameCount;	// フレームカウント
	DWORD m_dwFPSLastTime;	// 最後にFPSを計測した時刻
	D3DFILLMODE m_fillMode;	// 塗りつぶしモード
	CCamera::EState m_oldCameraState;	// カメラの過去状態

#endif	// _DEBUG

};

#endif	// _DEBUG_H_
