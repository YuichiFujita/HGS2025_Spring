//============================================================
//
//	通常状態ヘッダー [resultStateNormal.h]
//	Author：藤田勇一
//
//============================================================
//************************************************************
//	二重インクルード防止
//************************************************************
#ifndef _RESULT_STATE_NORMAL_H_
#define _RESULT_STATE_NORMAL_H_

//************************************************************
//	インクルードファイル
//************************************************************
#include "resultState.h"

//************************************************************
//	クラス定義
//************************************************************
// 通常状態クラス
class CResultStateNormal : public CResultState
{
public:
	// コンストラクタ
	CResultStateNormal();

	// デストラクタ
	~CResultStateNormal() override;

	// オーバーライド関数
	HRESULT Init() override;	// 初期化
	void Uninit() override;		// 終了
	void Update(const float fDeltaTime) override;	// 更新
};

#endif	// _RESULT_STATE_NORMAL_H_
