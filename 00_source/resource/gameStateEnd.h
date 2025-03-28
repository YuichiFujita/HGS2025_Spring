//============================================================
//
//	終了状態ヘッダー [gameStateEnd.h]
//	Author：藤田勇一
//
//============================================================
//************************************************************
//	二重インクルード防止
//************************************************************
#ifndef _GAME_STATE_END_H_
#define _GAME_STATE_END_H_

//************************************************************
//	インクルードファイル
//************************************************************
#include "gameState.h"

//************************************************************
//	クラス定義
//************************************************************
// 終了状態クラス
class CGameStateEnd : public CGameState
{
public:
	// コンストラクタ
	CGameStateEnd();

	// デストラクタ
	~CGameStateEnd() override;

	// オーバーライド関数
	HRESULT Init() override;	// 初期化
	void Uninit() override;		// 終了
	void Update(const float fDeltaTime) override;	// 更新

private:
	// メンバ変数
	float m_fCurTime;	// 経過時間
};

#endif	// _GAME_STATE_END_H_
