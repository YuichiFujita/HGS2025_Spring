//============================================================
//
//	開始演出ヘッダー [start.h]
//	Author：藤田勇一
//
//============================================================
//************************************************************
//	二重インクルード防止
//************************************************************
#ifndef _START_H_
#define _START_H_

//************************************************************
//	インクルードファイル
//************************************************************
#include "object2D.h"

//************************************************************
//	クラス定義
//************************************************************
// 開始演出クラス
class CStart : public CObject2D
{
public:
	// コンストラクタ
	CStart();

	// デストラクタ
	~CStart() override;

	// オーバーライド関数
	HRESULT Init() override;	// 初期化
	void Uninit() override;		// 終了
	void Update(const float fDeltaTime) override;	// 更新
	void Draw(CShader* pShader = nullptr) override;	// 描画

	// 静的メンバ関数
	static CStart* Create();

	// メンバ関数
	bool IsStagEnd() const;	// 演出終了フラグ取得

	inline float EaseOutBack(float start, float end, float startTime, float endTime, float currentTime, float c = 1.70158f)
	{
		// 割合
		float ratio = (currentTime - startTime) / (endTime - startTime);
		useful::LimitNum(ratio, 0.0f, 1.0f);

		ratio -= 1; // t を (t - 1) に変換

		float cal = ratio * ratio * ((c + 1) * ratio + c) + 1;
		return start + (end - start) * cal;
	}

	inline float EasingCubicInOut(float start, float end, float startTime, float endTime, float currentTime)
	{
		float ratio = easing::InOutCubic(currentTime, startTime, endTime);
		useful::LimitNum(ratio, 0.0f, 1.0f);
		return start + (end - start) * ratio;
	}

private:
	// メンバ変数
	float m_fStateTime;	// 経過時間
};

#endif	// _START_H_
