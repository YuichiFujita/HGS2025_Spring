//============================================================
//
//	リザルトマネージャーヘッダー [resultManager.h]
//	Author：藤田勇一
//
//============================================================
//************************************************************
//	二重インクルード防止
//************************************************************
#ifndef _RESULTMANAGER_H_
#define _RESULTMANAGER_H_

//************************************************************
//	前方宣言
//************************************************************
class CResultState;		// リザルト状態クラス
class CObject2D;		// オブジェクト2Dクラス
class CScrollString2D;	// 文字送り文字列2Dクラス

#ifdef SCORE
class CMultiValue;		// マルチ数字クラス
#else TIMER
class CTimeUI;			// タイムUIクラス
#endif

//************************************************************
//	クラス定義
//************************************************************
// リザルトマネージャークラス
class CResultManager
{
public:
	// コンストラクタ
	CResultManager();

	// デストラクタ
	~CResultManager();

	// メンバ関数
	HRESULT Init();	// 初期化
	void Uninit();	// 終了
	void Update(const float fDeltaTime);		// 更新
	HRESULT ChangeState(CResultState* pState);	// 状態変更
	void TransGame();		// ゲーム画面遷移
	void TransRanking();	// ランキング画面遷移

	// 静的メンバ関数
	static CResultManager* Create();	// 生成
	static void Release(CResultManager*& prResultManager);	// 破棄

private:
	// 選択列挙
	enum ESelect
	{
		SELECT_CONTINUE = 0,	// コンテニュー
		SELECT_RANKING,			// ランキング
		SELECT_MAX				// この列挙型の総数
	};

	// メンバ関数
	void UpdateSelect();		// 選択更新
	void UpdateDecide();		// 決定更新
	bool IsEndScroll() const;	// 文字送り終了フラグ取得

	// メンバ変数
	CObject2D* m_apSelect[SELECT_MAX];	// 選択肢情報
	CScrollString2D* m_pUpdateRank;		// ランキング更新情報
	CObject2D* m_pContinue;	// コンテニュー情報
#ifdef SCORE
	CMultiValue* m_pScore;	// スコア情報
#else TIMER
	CTimeUI* m_pTime;		// タイム情報
#endif
	CObject2D* m_pTitle;	// タイトル情報
	CResultState* m_pState;	// 状態
	int m_nCurSelect;		// 現在の選択肢
	int m_nOldSelect;		// 前回の選択肢
};

#endif	// _RESULTMANAGER_H_
