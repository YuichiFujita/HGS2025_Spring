//============================================================
//
//	データ保存ヘッダー [retention.h]
//	Author：藤田勇一
//
//============================================================
//************************************************************
//	二重インクルード防止
//************************************************************
#ifndef _RETENTION_H_
#define _RETENTION_H_

//************************************************************
//	クラス定義
//************************************************************
// データ保存クラス
class CRetention
{
public:
	// コンストラクタ
	CRetention();

	// デストラクタ
	~CRetention();

	// メンバ関数
	HRESULT Init();	// 初期化
	void Uninit();	// 終了

#ifdef SCORE
	inline void SetScore(const int nScore)	{ m_nScore = nScore; }	// クリアスコア設定
	inline int GetScore() const				{ return m_nScore; }	// クリアスコア取得
#else TIMER
	inline void SetTime(const float fTime)	{ m_fTime = fTime; }	// クリアタイム設定
	inline float GetTime() const			{ return m_fTime; }		// クリアタイム取得
#endif
	inline void SetUpdateRankIdx(const int nUpdateIdx)	{ m_nUpdateRankIdx = nUpdateIdx; }	// ランキング更新インデックス設定
	inline int GetUpdateRankIdx() const					{ return m_nUpdateRankIdx; }		// ランキング更新インデックス取得

	// 静的メンバ関数
	static CRetention* Create();	// 生成
	static void Release(CRetention*& prRetention);	// 破棄

private:
	// メンバ変数
#ifdef SCORE
	int m_nScore;			// クリアスコア
#else TIMER
	float m_fTime;			// クリアタイム
#endif
	int m_nUpdateRankIdx;	// ランキング更新インデックス
};

#endif	// _RETENTION_H_
