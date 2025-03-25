//==========================================================
//
//デバッグ表示処理 [debugproc.h]
//Author Ibuki Okusada
//
//==========================================================
#ifndef _DEBUGPROC_H_		//このマクロが定義されていない場合
#define _DEBUGPROC_H_

//**********************************************************
//クラスの定義
//**********************************************************
class CDebugProc
{
public:
	// 定数
	static constexpr int MAX_STR = 2048;	// デバッグ表示の最大文字数

	// デバッグ位置列挙
	enum EPoint
	{
		POINT_CENTER = 0,	// 中央
		POINT_LEFT,			// 左
		POINT_RIGHT,		// 右
		POINT_MAX			// この列挙型の総数
	};

	// コンストラクタ
	CDebugProc();

	// デストラクタ
	~CDebugProc();

	// メンバ関数
	HRESULT Init();
	void Uninit();
	void Update();
	void Draw();
	void Print(const EPoint point, const char* fmt, ...);
	
	inline void SetDisp(const bool bDisp)	{ m_bDisp = bDisp; }	// 表示設定
	inline bool IsDisp() const				{ return m_bDisp; }		// 表示取得

	// 静的メンバ関数
	static CDebugProc* Create();					// 生成
	static void Release(CDebugProc*& prDebugProc);	// 破棄

private:
	static LPD3DXFONT m_pFont;	// フォントへのポインタ
	bool m_bDisp;				// デバッグ表示のON/OFF
	char m_aStr[POINT_MAX][MAX_STR];	// デバッグ表示用の文字列
};

#endif	// _DEBUGPROC_H_
