//============================================================
//
//	レンダラーヘッダー [renderer.h]
//	Author：藤田勇一
//
//============================================================
//************************************************************
//	二重インクルード防止
//************************************************************
#ifndef _RENDERER_H_
#define _RENDERER_H_

//************************************************************
//	インクルードファイル
//************************************************************
#include "renderTexture.h"

//************************************************************
//	前方宣言
//************************************************************
class CScreen;	// スクリーンクラス

//************************************************************
//	クラス定義
//************************************************************
// レンダラークラス
class CRenderer
{
public:
	// 定数
	static constexpr D3DCOLOR COL_CLEAR = D3DCOLOR_RGBA(0, 0, 0, 0);	// 画面クリア時の色

	// コンストラクタ
	CRenderer();

	// デストラクタ
	~CRenderer();

	// メンバ関数
	HRESULT Init(HWND hWnd, BOOL bWindow);	// 初期化
	void Uninit();							// 終了
	void Update(const float fDeltaTime);	// 更新
	void Draw();							// 描画
	HRESULT CreateRenderTexture();			// レンダーテクスチャー生成
	void DrawRenderTexture(LPDIRECT3DSURFACE9* pSurface);	// レンダーテクスチャ描画

	inline LPDIRECT3DDEVICE9 GetDevice() const	{ return m_pD3DDevice; }						// デバイス取得
	inline int GetRenderTextureIndex() const	{ return m_pRenderScene->GetTextureIndex(); }	// レンダーテクスチャインデックス取得

	// 静的メンバ関数
	static CRenderer* Create(HWND hWnd, BOOL bWindow);	// 生成
	static void Release(CRenderer*& prRenderer);		// 破棄

private:
	// メンバ関数
	HRESULT CreateDevice(HWND hWnd, D3DPRESENT_PARAMETERS d3dpp);	// デバイス生成

	// メンバ変数
	CRenderTexture*		m_pRenderScene;		// シーンレンダーテクスチャ
	CScreen*			m_pDrawScreen;		// スクリーン描画ポリゴン
	LPDIRECT3DSURFACE9	m_pDefSurScreen;	// 元の描画サーフェイス保存用
	LPDIRECT3D9			m_pD3D;				// Direct3Dオブジェクト
	LPDIRECT3DDEVICE9	m_pD3DDevice;		// Direct3Dデバイス
};

#endif	// _RENDERER_H_
