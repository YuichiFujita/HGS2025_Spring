//============================================================
//
//	レンダーステートヘッダー [renderState.h]
//	Author：藤田勇一
//
//============================================================
//************************************************************
//	二重インクルード防止
//************************************************************
#ifndef _RENDER_STATE_H_
#define _RENDER_STATE_H_

//************************************************************
//	クラス定義
//************************************************************
// レンダーステートクラス
class CRenderState
{
public:
	// αブレンド列挙
	enum EBlend
	{
		BLEND_NORMAL = 0,	// 通常合成
		BLEND_ADD,			// 加算合成
		BLEND_SUB,			// 減算合成
		BLEND_MAX			// この列挙型の総数
	};

	// コンストラクタ
	CRenderState();

	// デストラクタ
	~CRenderState();

	// αブレンド情報
	struct SBlendAlpha
	{
	public:
		// デフォルトコンストラクタ
		SBlendAlpha() :
			op		(D3DBLENDOP_ADD),	// αブレンド情報
			scr		(D3DBLEND_ZERO),	// SCRブレンド
			dest	(D3DBLEND_ZERO),	// DESTブレンド
			bBlend	(false)				// αブレンド状況
		{}

		// 引数付きコンストラクタ
		SBlendAlpha(const D3DBLENDOP in_op, const D3DBLEND in_scr, const D3DBLEND in_dest) :
			op		(in_op),	// αブレンド情報
			scr		(in_scr),	// SCRブレンド
			dest	(in_dest),	// DESTブレンド
			bBlend	(true)		// αブレンド状況
		{}

		// デストラクタ
		~SBlendAlpha() {}

		// メンバ変数
		D3DBLENDOP	op;		// αブレンド情報
		D3DBLEND	scr;	// SCRブレンド
		D3DBLEND	dest;	// DESTブレンド
		bool		bBlend;	// αブレンド状況
	};

	// αテスト情報
	struct STestAlpha
	{
		// デフォルトコンストラクタ
		STestAlpha() :
			func	(D3DCMP_NEVER),	// αテスト情報
			nRef	(0),			// αテスト参照値
			bTest	(false)			// αテスト状況
		{}

		// デストラクタ
		~STestAlpha() {}

		// メンバ変数
		D3DCMPFUNC	func;	// αテスト情報
		int			nRef;	// αテスト参照値
		bool		bTest;	// αテスト状況
	};

	// Zテスト情報
	struct STestZ
	{
		// デフォルトコンストラクタ
		STestZ() :
			func	(D3DCMP_NEVER),	// Zテスト情報
			bUpdate	(false)			// Zバッファ更新状況
		{}

		// デストラクタ
		~STestZ() {}

		// メンバ変数
		D3DCMPFUNC	func;		// Zテスト情報
		bool		bUpdate;	// Zバッファ更新状況
	};

	// レンダーステート情報
	struct SInfo
	{
		// デフォルトコンストラクタ
		SInfo() :
			blendAlpha	({}),			// αブレンド情報
			testAlpha	({}),			// αテスト情報
			testZ		({}),			// Zテスト情報
			cull		(D3DCULL_NONE),	// カリング情報
			bLight		(false)			// ライティング状況
		{}

		// デストラクタ
		~SInfo() {}

		// メンバ変数
		SBlendAlpha	blendAlpha;	// αブレンド情報
		STestAlpha	testAlpha;	// αテスト情報
		STestZ		testZ;		// Zテスト情報
		D3DCULL		cull;		// カリング情報
		bool		bLight;		// ライティング状況
	};

	// メンバ関数
	HRESULT Init();	// 初期化
	void Uninit();	// 終了
	void Set();		// 設定
	void Reset();	// 再設定
	void SetInfoRenderState(const SInfo& rInfo);	// レンダーステート情報設定
	SInfo GetInfoRenderState();						// レンダーステート情報取得
	void SetAlphaBlend(const EBlend blend);			// αブレンド情報設定

	inline void SetAlphaFunc(const D3DCMPFUNC func)	{ m_info.testAlpha.func	 = func; }		// αテストの情報設定
	inline void SetAlphaNumRef(const int nRef)		{ m_info.testAlpha.nRef	 = nRef; }		// αテストの参照値設定
	inline void SetAlphaTest(const bool bTest)		{ m_info.testAlpha.bTest = bTest; }		// αテストの状況設定
	inline void SetZFunc(const D3DCMPFUNC func)		{ m_info.testZ.func		 = func; }		// Zテストの情報設定
	inline void SetZUpdate(const bool bUpdate)		{ m_info.testZ.bUpdate	 = bUpdate; }	// Zテストのバッファ更新設定
	inline void SetCulling(const D3DCULL cull)		{ m_info.cull	= cull; }	// カリング設定
	inline void SetLighting(const bool bLight)		{ m_info.bLight	= bLight; }	// ライティング設定

	// 静的メンバ関数
	static CRenderState* Create();	// 生成
	static void Release(CRenderState*& prRenderState);		// 破棄
	static void BindDevice(LPDIRECT3DDEVICE9* ppDevice);	// デバイス割当
	static void InitRenderState();		// レンダーステート情報初期化
	static SInfo GetSaveRenderState();	// 保存レンダーステート情報取得

private:
	// 静的メンバ関数
	static void SetRenderState(const SInfo& rInfo);	// レンダーステート反映

	// 静的メンバ変数
	static LPDIRECT3DDEVICE9* m_ppDevice;	// Direct3Dデバイスへのポインタ
	static SInfo m_save;	// 保存レンダーステート情報

	// メンバ変数
	SInfo m_info;	// レンダーステート情報
};

#endif	// _RENDER_STATE_H_
