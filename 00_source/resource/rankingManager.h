//============================================================
//
//	ランキングマネージャーヘッダー [rankingManager.h]
//	Author：藤田勇一
//
//============================================================
//************************************************************
//	二重インクルード防止
//************************************************************
#ifndef _RANKINGMANAGER_H_
#define _RANKINGMANAGER_H_

//************************************************************
//	前方宣言
//************************************************************
class CRankingState;	// ランキング状態クラス
class CAnim2D;			// アニメーション2Dクラス
class CObject2D;		// オブジェクト2Dクラス

#ifdef SCORE
class CMultiValue;	// マルチ数字クラス
#else TIMER
class CTimeUI;		// タイムUIクラス
#endif

//************************************************************
//	クラス定義
//************************************************************
// ランキングマネージャークラス
class CRankingManager
{
public:
	// 定数
	static constexpr int MAX_RANK = 5;	// ランキング上位表示数

	// コンストラクタ
	CRankingManager();

	// デストラクタ
	~CRankingManager();

	// メンバ関数
	HRESULT Init();	// 初期化
	void Uninit();	// 終了
	void Update(const float fDeltaTime);		// 更新
	HRESULT ChangeState(CRankingState* pState);	// 状態変更
	void TransTitle();	// タイトル画面遷移

	// 静的メンバ関数
	static CRankingManager* Create();	// 生成
	static void Release(CRankingManager*& prRankingManager);	// 破棄

#ifdef SCORE
	static int SetRank(const int nNewScore);	// ランキング設定
#else TIMER
	static int SetRank(const float fNewTime);	// ランキング設定
#endif

private:
	// 静的メンバ関数
#ifdef SCORE
	static int SortRank(const int nNewScore, int* pRankArray);		// ランキングソート
	static HRESULT LoadRank(const char* pPath, int* pRankArray);	// ランキング読込
	static HRESULT SaveRank(const char* pPath, int* pRankArray);	// ランキング保存
#else TIMER
	static int SortRank(const float fNewTime, float* pRankArray);	// ランキングソート
	static HRESULT LoadRank(const char* pPath, float* pRankArray);	// ランキング読込
	static HRESULT SaveRank(const char* pPath, float* pRankArray);	// ランキング保存
#endif

	// メンバ変数
	CAnim2D* m_apRank[MAX_RANK];		// 順位情報
#ifdef SCORE
	CMultiValue* m_apScore[MAX_RANK];	// スコア情報
#else TIMER
	CTimeUI* m_apTime[MAX_RANK];		// タイム情報
#endif
	CObject2D* m_pTitle;				// タイトル情報
	CRankingState* m_pState;			// 状態
};

#endif	// _RANKINGMANAGER_H_
