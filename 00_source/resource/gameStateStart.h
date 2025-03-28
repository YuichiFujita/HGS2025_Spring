//============================================================
//
//	開始演出状態ヘッダー [gameStateStart.h]
//	Author：藤田勇一
//
//============================================================
//************************************************************
//	二重インクルード防止
//************************************************************
#ifndef _GAME_STATE_START_H_
#define _GAME_STATE_START_H_

//************************************************************
//	インクルードファイル
//************************************************************
#include "gameState.h"

//************************************************************
//	前方宣言
//************************************************************
class CStart;

//************************************************************
//	クラス定義
//************************************************************
// 開始演出状態クラス
class CGameStateStart : public CGameState
{
public:
	// コンストラクタ
	CGameStateStart();

	// デストラクタ
	~CGameStateStart() override;

	// オーバーライド関数
	HRESULT Init() override;	// 初期化
	void Uninit() override;		// 終了
	void Update(const float fDeltaTime) override;	// 更新

private:
	// メンバ変数
	CStart* m_pStart;	// 開始演出情報
};

#endif	// _GAME_STATE_START_H_
