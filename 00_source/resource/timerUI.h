//============================================================
//
//	タイマーUIヘッダー [timerUI.h]
//	Author：藤田勇一
//
//============================================================
//************************************************************
//	二重インクルード防止
//************************************************************
#ifndef _TIMER_UI_H_
#define _TIMER_UI_H_

//************************************************************
//	インクルードファイル
//************************************************************
#include "timeUI.h"
#include "timer.h"
#include "value.h"

//************************************************************
//	クラス定義
//************************************************************
// タイマーUIクラス
class CTimerUI : public CTimeUI
{
public:
	// コンストラクタ
	CTimerUI();

	// デストラクタ
	~CTimerUI() override;

	// オーバーライド関数
	HRESULT Init() override;	// 初期化
	void Uninit() override;		// 終了
	void Update(const float fDeltaTime) override;	// 更新
	void Draw(CShader* pShader = nullptr) override;	// 描画
	void SetTime(const float fTime) override;		// 時間設定
	inline float GetTime() const override	{ return m_pTimer->GetTime(); }	// 時間取得
	inline int GetMin() const override		{ return m_pTimer->GetMin(); }	// 分取得
	inline int GetSec() const override		{ return m_pTimer->GetSec(); }	// 秒取得
	inline int GetMSec() const override		{ return m_pTimer->GetMSec(); }	// ミリ秒取得

	// 静的メンバ関数
	static CTimerUI* Create	// 生成
	( // 引数
		const float fTime,			// 開始時間
		const float fLimit,			// 制限時間
		const VECTOR3& rPos,		// 位置
		const VECTOR3& rSizeValue,	// 数字の大きさ
		const VECTOR3& rSizePart,	// 区切りの大きさ
		const VECTOR3& rSpaceValue,	// 数字の空白
		const VECTOR3& rSpacePart,	// 区切りの空白
		const CValue::EType type = CValue::TYPE_NORMAL,	// 数字種類
		const EAlignX alignX = XALIGN_CENTER,			// 横配置
		const EAlignY alignY = YALIGN_CENTER,			// 縦配置
		const VECTOR3& rRot = VEC3_ZERO,				// 向き
		const COLOR& rCol = color::White()				// 色
	);

	// メンバ関数
	inline void Start()	{ m_pTimer->Start(); }	// 計測開始
	inline void End()	{ m_pTimer->End(); }	// 計測終了
	inline void EnableStop(const bool bStop)	{ m_pTimer->EnableStop(bStop);}		// 計測停止設定
	inline void AddTime(const float fTime)		{ m_pTimer->AddTime(fTime); }		// 時間加算
	inline void SetLimit(const float fLimit)	{ m_pTimer->SetLimit(fLimit); }		// 制限時間設定
	inline float GetLimit() const				{ return m_pTimer->GetLimit(); }	// 制限時間取得
	inline CTimer::EState GetState() const		{ return m_pTimer->GetState(); }	// 計測状態取得

private:
	// メンバ変数
	CTimer* m_pTimer;	// タイマー管理情報
};

#endif	// _TIMER_UI_H_
