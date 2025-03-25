//============================================================
//
//	フェードヘッダー [fade.h]
//	Author：藤田勇一
//
//============================================================
//************************************************************
//	二重インクルード防止
//************************************************************
#ifndef _FADE_H_
#define _FADE_H_

//************************************************************
//	インクルードファイル
//************************************************************
#include "scene.h"
#include "object2D.h"

//************************************************************
//	クラス定義
//************************************************************
// フェードクラス
class CFade : public CObject2D
{
public:
	// 定数
	static constexpr int	PRIORITY	= 7;		// 優先順位
	static constexpr float	DEF_LEVEL	= 2.0f;		// α値加減量 (デフォルト)
	static constexpr float	SKIP_LEVEL	= 120.0f;	// α値加減量 (スキップ)

	// フェード状態列挙
	enum EFade
	{
		FADE_NONE = 0,	// 何もしていない状態
		FADE_WAIT,		// フェード余韻状態
		FADE_IN,		// フェードイン状態
		FADE_OUT,		// フェードアウト状態
		FADE_MAX		// この列挙型の総数
	};

	// フェード種類列挙
	enum EType
	{
		TYPE_NONE = 0,	// 何もしていない
		TYPE_FADE,		// 遷移なし
		TYPE_MODE,		// モード遷移
		TYPE_LOAD,		// ロード付きモード遷移
		TYPE_MAX		// この列挙型の総数
	};

	// コンストラクタ
	CFade();

	// デストラクタ
	~CFade();

	// オーバーライド関数
	HRESULT Init() override;	// 初期化
	void Uninit() override;		// 終了
	void Update(const float fDeltaTime) override;	// 更新
	void Draw(CShader* pShader = nullptr) override;	// 描画

	// 静的メンバ関数
	static CFade* Create();	// 生成

	// メンバ関数
	inline void SetNextMode(const CScene::EMode next)	{ m_modeNext = next; }	// 遷移先モード設定
	inline CScene::EMode GetNextMode() const			{ return m_modeNext; }	// 遷移先モード取得
	inline void SetSubIn(const float fSubIn)	{ m_fSubIn = fSubIn; }			// インのα値減少量
	inline float GetSubIn() const				{ return m_fSubIn; }			// インのα値減少量
	inline void SetAddOut(const float fAddOut)	{ m_fAddOut = fAddOut; }		// アウトのα値増加量
	inline float GetAddOut() const				{ return m_fAddOut; }			// アウトのα値増加量
	inline EFade GetState() const	{ return m_fade; }							// フェード状態取得
	inline EType GetType() const	{ return m_type; }							// フェード種類取得
	inline bool IsFade() const		{ return m_fade != FADE_NONE; }				// フェード状況取得

	void SetFade	// フェード開始設定
	( // 引数
		const float fAddOut	= DEF_LEVEL,	// アウトのα値増加量
		const float fSubIn	= DEF_LEVEL,	// インのα値減少量
		std::function<HRESULT()> pFunc = nullptr,	// モード設定関数ポインタ
		const int nPriority	= PRIORITY,				// 優先順位
		const COLOR colFade	= color::Black(0.0f)	// フェード色
	);
	void SetModeFade	// 遷移先モード設定 (フェードのみ)
	( // 引数
		const CScene::EMode mode,				// 遷移先モード
		const float fWaitTime	= 0.0f,			// 余韻時間
		const float fAddOut		= DEF_LEVEL,	// アウトのα値増加量
		const float fSubIn		= DEF_LEVEL,	// インのα値減少量
		const COLOR colFade		= color::Black(0.0f)	// フェード色
	);
	void SetLoadFade	// 遷移先モード設定 (ロード画面付き)
	( // 引数
		const CScene::EMode mode,				// 遷移先モード
		const float fWaitTime	= 0.0f,			// 余韻時間
		const float fAddOut		= DEF_LEVEL,	// アウトのα値増加量
		const float fSubIn		= DEF_LEVEL,	// インのα値減少量
		const COLOR colFade		= color::Black(0.0f)	// フェード色
	);

private:
	// メンバ変数
	std::function<HRESULT()> m_funcSetMode;	// モード設定関数ポインタ
	CScene::EMode m_modeNext;	// 遷移先モード
	EFade m_fade;		// フェード状態
	EType m_type;		// フェード種類
	float m_fWaitTime;	// 現在の余韻時間
	float m_fSubIn;		// インのα値減少量
	float m_fAddOut;	// アウトのα値増加量
};

#endif	// _FADE_H_
