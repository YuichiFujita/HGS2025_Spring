//============================================================
//
//	通常状態ヘッダー [rankingStateNormal.h]
//	Author：藤田勇一
//
//============================================================
//************************************************************
//	二重インクルード防止
//************************************************************
#ifndef _RANKING_STATE_NORMAL_H_
#define _RANKING_STATE_NORMAL_H_

//************************************************************
//	インクルードファイル
//************************************************************
#include "rankingState.h"

//************************************************************
//	クラス定義
//************************************************************
// 通常状態クラス
class CRankingStateNormal : public CRankingState
{
public:
	// コンストラクタ
	CRankingStateNormal();

	// デストラクタ
	~CRankingStateNormal() override;

	// オーバーライド関数
	HRESULT Init() override;	// 初期化
	void Uninit() override;		// 終了
	void Update(const float fDeltaTime) override;	// 更新
};

#endif	// _RANKING_STATE_NORMAL_H_
