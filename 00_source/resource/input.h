//============================================================
//
//	入力ヘッダー [input.h]
//	Author：藤田勇一
//
//============================================================
//************************************************************
//	二重インクルード防止
//************************************************************
#ifndef _INPUT_H_
#define _INPUT_H_

//************************************************************
//	クラス定義
//************************************************************
// 入力クラス
class CInput
{
public:
	// コンストラクタ
	CInput();

	// デストラクタ
	virtual ~CInput();

	// 仮想関数
	virtual HRESULT Init(HINSTANCE hInstance, HWND hWnd);	// 初期化
	virtual void Uninit();	// 終了

	// 純粋仮想関数
	virtual void Update() = 0;	// 更新

protected:
	// メンバ変数
	static int m_nNumAll;				// 入力デバイスの総数
	static LPDIRECTINPUT8 m_pInput;		// DirectInputオブジェクト
	LPDIRECTINPUTDEVICE8  m_pDevice;	// DirectInputデバイス
};

// キーボードクラス
class CInputKeyboard : public CInput
{
public:
	// 定数
	static constexpr int MAX_KEY = 256;	// キーの最大数

	// コンストラクタ
	CInputKeyboard();

	// デストラクタ
	~CInputKeyboard();

	// オーバーライド関数
	HRESULT Init(HINSTANCE hInstance, HWND hWnd) override;	// 初期化
	void Uninit() override;	// 終了
	void Update() override;	// 更新

	// 静的メンバ関数
	static CInputKeyboard* Create(HINSTANCE hInstance, HWND hWnd);	// 生成
	static void Release(CInputKeyboard*& prKeyboard);				// 破棄

	// メンバ関数
	void InitPress();			// プレス初期化
	void InitTrigger();			// トリガー初期化
	void InitRelease();			// リリース初期化
	bool IsPress(int nKey);		// プレス取得
	bool IsTrigger(int nKey);	// トリガー取得
	bool IsRelease(int nKey);	// リリース取得
	bool IsAnyPress();			// 全プレス取得
	bool IsAnyTrigger();		// 全トリガー取得
	bool IsAnyRelease();		// 全リリース取得

private:
	// メンバ変数
	BYTE m_aKeyStatePress[MAX_KEY];		// プレス情報
	BYTE m_aKeyStateTrigger[MAX_KEY];	// トリガー情報
	BYTE m_aKeyStateRelease[MAX_KEY];	// リリース情報
};

// マウスクラス
class CInputMouse : public CInput
{
public:
	// コンストラクタ
	CInputMouse();

	// デストラクタ
	~CInputMouse();

	// マウスキー列挙
	enum EKey
	{
		KEY_LEFT = 0,	// マウス右クリック
		KEY_RIGHT,		// マウス左クリック
		KEY_WHEEL,		// マウスホイール押し込み
		KEY_MAX			// この列挙型の総数
	};

	// オーバーライド関数
	HRESULT Init(HINSTANCE hInstance, HWND hWnd) override;	// 初期化
	void Uninit() override;	// 終了
	void Update() override;	// 更新

	// 静的メンバ関数
	static CInputMouse* Create(HINSTANCE hInstance, HWND hWnd);	// 生成
	static void Release(CInputMouse*& prMouse);					// 破棄

	// メンバ関数
	bool IsPress(EKey mouKey);		// プレス取得
	bool IsTrigger(EKey mouKey);	// トリガー取得
	bool IsRelease(EKey mouKey);	// リリース取得
	bool IsAnyPress();				// 全プレス取得
	bool IsAnyTrigger();			// 全トリガー取得
	bool IsAnyRelease();			// 全リリース取得
	VECTOR3 GetMove();				// 移動量取得

private:
	// メンバ変数
	DIMOUSESTATE m_keyStatePress;	// プレス情報
	DIMOUSESTATE m_keyStateTrigger;	// トリガー情報
	DIMOUSESTATE m_keyStateRelease;	// リリース情報
};

// パッドクラス
class CInputPad
{
public:
	static constexpr int	MAX_NUM = 4;	// パッドの最大数
	static constexpr float	DEAD_ZONE = (float)USHRT_MAX * 0.1f;	// スティックの無視する傾き量

	// コンストラクタ
	CInputPad();

	// デストラクタ
	~CInputPad();

	// パッドキー列挙
	enum EKey
	{
		KEY_UP = 0,		// 方向キー 上
		KEY_DOWN,		// 方向キー 下
		KEY_LEFT,		// 方向キー 左
		KEY_RIGHT,		// 方向キー 右
		KEY_START,		// STARTボタン
		KEY_BACK,		// BACKボタン
		KEY_LSTICKPUSH,	// 左スティック押し込み
		KEY_RSTICKPUSH,	// 右スティック押し込み
		KEY_LB,			// LBボタン
		KEY_RB,			// RBボタン
		KEY_LT,			// LTボタン
		KEY_RT,			// RTボタン
		KEY_A,			// Aボタン
		KEY_B,			// Bボタン
		KEY_X,			// Xボタン
		KEY_Y,			// Yボタン
		KEY_LSTICK_X,	// 左スティック 左右
		KEY_LSTICK_Y,	// 左スティック 上下
		KEY_RSTICK_X,	// 右スティック 左右
		KEY_RSTICK_Y,	// 右スティック 上下
		KEY_MAX			// この列挙型の総数
	};
	
	// 振動種類列挙
	enum EType
	{
		TYPE_NONE = 0,		// 何もしていない
		TYPE_DAMAGE,		// ダメージ
		TYPE_BIG_DAMAGE,	// 大ダメージ
		TYPE_DEATH,			// 死亡
		TYPE_WALLDASH,		// 壁走り
		TYPE_MAX			// この列挙型の総数
	};

	// 振動構造体
	struct SVibration
	{
		// デフォルトコンストラクタ
		SVibration() :
			vibration	({}),			// バイブ情報
			type		(TYPE_NONE),	// バイブ種類
			nTime		(0)				// バイブ時間
		{}

		// デストラクタ
		~SVibration() {}

		// メンバ変数
		_XINPUT_VIBRATION vibration;	// バイブ情報
		EType type;	// バイブ種類
		int nTime;	// バイブ時間
	};

	// 静的メンバ関数
	static CInputPad* Create();				// 生成
	static void Release(CInputPad*& prPad);	// 破棄

	// メンバ関数
	HRESULT Init();	// 初期化
	void Uninit();	// 終了
	void Update();	// 更新
	void SetVibration(EType type, int nPadIdx = 0);	// バイブ設定
	bool IsPress(EKey joyKey, int nPadIdx = 0);		// プレス取得	(ボタン)
	bool IsTrigger(EKey joyKey, int nPadIdx = 0);	// トリガー取得	(ボタン)
	bool IsRelease(EKey joyKey, int nPadIdx = 0);	// リリース取得	(ボタン)

	bool IsPressAll(EKey joyKey);		// 全プレス取得		(ボタン)
	bool IsTriggerAll(EKey joyKey);		// 全トリガー取得	(ボタン)
	bool IsReleaseAll(EKey joyKey);		// 全リリース取得	(ボタン)
	bool IsAnyPress(int nPadIdx = 0);	// 全プレス取得		(ボタン)
	bool IsAnyTrigger(int nPadIdx = 0);	// 全トリガー取得	(ボタン)
	bool IsAnyRelease(int nPadIdx = 0);	// 全リリース取得	(ボタン)
	void InitPress(int nPadIdx = 0);	// プレス初期化		(ボタン)
	void InitTrigger(int nPadIdx = 0);	// トリガー初期化	(ボタン)
	void InitRelease(int nPadIdx = 0);	// リリース初期化	(ボタン)

	SHORT GetPressLStickX(int nPadIdx = 0);		// プレス取得 (LスティックX)
	SHORT GetPressLStickY(int nPadIdx = 0);		// プレス取得 (LスティックY)
	SHORT GetPressRStickX(int nPadIdx = 0);		// プレス取得 (RスティックX)
	SHORT GetPressRStickY(int nPadIdx = 0);		// プレス取得 (RスティックY)
	float GetPressLStickRot(int nPadIdx = 0);	// プレス取得 (Lスティック向き)
	float GetPressRStickRot(int nPadIdx = 0);	// プレス取得 (Rスティック向き)
	float GetPressLStickTilt(int nPadIdx = 0);	// プレス取得 (Lスティック傾き)
	float GetPressRStickTilt(int nPadIdx = 0);	// プレス取得 (Rスティック傾き)

private:
	// メンバ関数
	void UpdateVibration(SVibration* pVibration, int nPadIdx);	// バイブレーション更新

	// メンバ変数
	SVibration   m_aVibration[MAX_NUM];			// バイブ情報
	XINPUT_STATE m_aKeyStatePress[MAX_NUM];		// プレス情報
	XINPUT_STATE m_aKeyStateTrigger[MAX_NUM];	// トリガー情報
	XINPUT_STATE m_aKeyStateRelease[MAX_NUM];	// リリース情報
};

#endif	// _INPUT_H_
