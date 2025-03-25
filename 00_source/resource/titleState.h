//============================================================
//
//	タイトル状態ヘッダー [titleState.h]
//	Author：藤田勇一
//
//============================================================
//************************************************************
//	二重インクルード防止
//************************************************************
#ifndef _TITLE_STATE_H_
#define _TITLE_STATE_H_

//************************************************************
//	前方宣言
//************************************************************
class CTitleManager;	// タイトルマネージャークラス

//************************************************************
//	クラス定義
//************************************************************
// タイトル状態クラス
class CTitleState
{
public:
	// コンストラクタ
	CTitleState();

	// デストラクタ
	virtual ~CTitleState();

	// 純粋仮想関数
	virtual HRESULT Init()	= 0;	// 初期化
	virtual void Uninit()	= 0;	// 終了
	virtual void Update(const float fDeltaTime) = 0;	// 更新

	// メンバ関数
	inline void SetContext(CTitleManager* pContext) { m_pContext = pContext; }	// コンテキスト設定

protected:
	// メンバ変数
	CTitleManager* m_pContext;	// コンテキスト
};

//************************************************************
//	インクルードファイル
//************************************************************
#include "titleStateLogo.h"
#include "titleStateSelect.h"
#include "titleStateTutorial.h"

#endif	// _TITLE_STATE_H_
