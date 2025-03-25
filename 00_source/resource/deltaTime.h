//============================================================
//
//	デルタタイムヘッダー [deltaTime.h]
//	Author：藤田勇一
//
//============================================================
//************************************************************
//	二重インクルード防止
//************************************************************
#ifndef _DELTATIME_H_
#define _DELTATIME_H_

//************************************************************
//	クラス定義
//************************************************************
// デルタタイムクラス
class CDeltaTime
{
public:
	// コンストラクタ
	CDeltaTime();

	// デストラクタ
	~CDeltaTime();

	// メンバ関数
	HRESULT Init();	// 初期化
	void Uninit();	// 終了
	void Update();	// 更新
	inline float GetDeltaTime() const			{ return m_fDeltaTime; }	// 経過時間取得
	inline float GetDeltaRate() const			{ return m_fDeltaRate; }	// 経過時間の割合取得
	inline float GetSlowRate() const			{ return m_fSlowRate; }		// 速度低下の割合取得
	inline void SetSlowRate(const float fRate)	{ m_fSlowRate = fRate; }	// 速度低下の割合設定

	// 静的メンバ関数
	static CDeltaTime* Create();					// 生成
	static void Release(CDeltaTime*& prDeltaTime);	// 破棄

private:
	// メンバ変数
	DWORD m_dwOldTime;	// 前回の処理開始時刻
	DWORD m_dwCurTime;	// 今回の処理開始時刻
	float m_fDeltaTime;	// 経過時間
	float m_fDeltaRate;	// 経過時間の割合
	float m_fSlowRate;	// 速度低下の割合
};

#endif	// _DELTATIME_H_
