//============================================================
//
//	タイマーヘッダー [timer.h]
//	Author：藤田勇一
//
//============================================================
//************************************************************
//	二重インクルード防止
//************************************************************
#ifndef _TIMER_H_
#define _TIMER_H_

//************************************************************
//	インクルードファイル
//************************************************************
#include "object.h"

//************************************************************
//	クラス定義
//************************************************************
// タイマークラス
class CTimer : public CObject
{
public:
	// 定数
	static constexpr float TIME_MIN = 0.0f;	// 最少タイム
	static constexpr float TIME_MAX = 60.0f * 99.0f + 59.0f + 0.999f;	// 最大タイム

	// 計測列挙
	enum EState
	{
		STATE_NONE = 0,	// 処理なし
		STATE_COUNT,	// 計測中
		STATE_END,		// 計測終了 
		STATE_MAX		// この列挙型の総数
	};

	// コンストラクタ
	CTimer();

	// デストラクタ
	~CTimer() override;

	// オーバーライド関数
	HRESULT Init() override;	// 初期化
	void Uninit() override;		// 終了
	void Update(const float fDeltaTime) override;	// 更新
	void Draw(CShader* pShader = nullptr) override;	// 描画

	// 静的メンバ関数
	static CTimer* Create	// 生成
	( // 引数
		const float fTime,	// 開始時間
		const float fLimit	// 制限時間
	);
	static CListManager<CTimer>* GetList();			// リスト取得
	static void EnableStopAll(const bool bStop);	// 全タイマー計測状況設定

	// メンバ関数
	void Start();	// 計測開始
	void End();		// 計測終了
	void EnableStop(const bool bStop);	// 計測停止設定
	void AddTime(const float fTime);	// 時間加算
	void SetTime(const float fTime);	// 時間設定
	void SetLimit(const float fLimit);	// 制限時間設定
	inline float GetTime() const	{ return m_fTime; }		// 時間取得
	inline float GetLimit() const	{ return m_fLimit; }	// 制限時間取得
	inline EState GetState() const	{ return m_state; }		// 計測状態取得
	inline int GetMin() const		{ return m_nMin; }		// 分取得
	inline int GetSec() const		{ return m_nSec; }		// 秒取得
	inline int GetMSec() const		{ return m_nMSec; }		// ミリ秒取得

private:
	// オーバーライド関数
	inline void Release() override { CObject::Release(); }	// 破棄

	// メンバ関数
	void CountUp(const float fDeltaTime);	// カウントアップ
	void CountDown(const float fDeltaTime);	// カウントダウン
	void CalcTime();	// 時間計算

	// 静的メンバ変数
	static CListManager<CTimer>* m_pList;	// オブジェクトリスト

	// メンバ変数
	CListManager<CTimer>::AIterator m_iterator;		// イテレーター
	std::function<void(const float)> m_funcCount;	// 計測関数ポインタ
	EState	m_state;	// 計測状態
	float	m_fTime;	// 計測時間
	float	m_fLimit;	// 制限時間
	bool	m_bStop;	// 計測停止状況
	long	m_lTime;	// 計測ミリ秒
	int		m_nMin;		// 分
	int		m_nSec;		// 秒
	int		m_nMSec;	// ミリ秒
};

#endif	// _TIMER_H_
