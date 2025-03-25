//============================================================
//
//	遷移先選択状態ヘッダー [titleStateSelect.h]
//	Author：藤田勇一
//
//============================================================
//************************************************************
//	二重インクルード防止
//************************************************************
#ifndef _TITLE_STATE_SELECT_H_
#define _TITLE_STATE_SELECT_H_

//************************************************************
//	インクルードファイル
//************************************************************
#include "titleState.h"

//************************************************************
//	前方宣言
//************************************************************
class CObject2D;	// オブジェクト2Dクラス

//************************************************************
//	クラス定義
//************************************************************
// 遷移先選択状態クラス
class CTitleStateSelect : public CTitleState
{
public:
	// コンストラクタ
	CTitleStateSelect();

	// デストラクタ
	~CTitleStateSelect() override;

	// オーバーライド関数
	HRESULT Init() override;	// 初期化
	void Uninit() override;		// 終了
	void Update(const float fDeltaTime) override;	// 更新

private:
	// 選択列挙
	enum ESelect
	{
		SELECT_START = 0,	// 開始
		SELECT_TUTORIAL,	// チュートリアル
		SELECT_MAX			// この列挙型の総数
	};

	// メンバ関数
	void UpdateSelect();	// 選択更新
	void UpdateDecide();	// 決定更新
	void UpdateTransLogo(const float fDeltaTime);	// ロゴ遷移更新

	// メンバ変数
	CObject2D* m_apSelect[SELECT_MAX];	// 選択肢情報
	CObject2D* m_pTitle;	// タイトル情報
	int m_nCurSelect;		// 現在の選択肢
	int m_nOldSelect;		// 前回の選択肢
	float m_fCurTime;		// 現在の経過時間
};

#endif	// _TITLE_STATE_SELECT_H_
