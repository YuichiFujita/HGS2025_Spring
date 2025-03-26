//============================================================
//
//	ランキングマネージャー処理 [rankingManager.cpp]
//	Author：藤田勇一
//
//============================================================
//************************************************************
//	インクルードファイル
//************************************************************
#include "rankingManager.h"
#include "rankingState.h"
#include "manager.h"
#include "anim2D.h"
#include "retention.h"
#include "comboAreaManager.h"

#ifdef SCORE
#include "multiValue.h"
#else TIMER
#include "timeUI.h"
#endif

//************************************************************
//	定数宣言
//************************************************************
namespace
{
	namespace title
	{
		const char*		TEXTURE	= "data\\TEXTURE\\ranking_title000.png";	// テクスチャパス
		const VECTOR3	POS		= VECTOR3(SCREEN_CENT.x, 80.0f, 0.0f);		// 位置
		const float		HEIGHT	= 140.0f;	// 縦幅
	}

	namespace rank
	{
		const char*		TEXTURE = "data\\TEXTURE\\ranking000.png";			// テクスチャパス
		const POSGRID2	PTRN	= POSGRID2(CRankingManager::MAX_RANK, 1);	// テクスチャ分割数
		const VECTOR3	POS		= VECTOR3(290.0f, 190.0f, 0.0f);			// 位置
		const VECTOR3	OFFSET	= VECTOR3(0.0f, 112.0f, 0.0f);				// オフセット
		const float		WIDTH	= 160.0f * 1.3f;	// 横幅
	}

#ifdef SCORE
	const int	RANK_INIT[CRankingManager::MAX_RANK] = { 0, 0, 0, 0, 0 };	// 初期ランキング
	const char*	RANK_PATH	= "data\\BIN\\rank_score.bin";	// ランキングパス
	const COLOR	COL_UPDATE	= color::Yellow();	// ランキング更新色
	namespace score
	{
		const CValue::EType TYPE = CValue::TYPE_NORMAL;	// 数字種類
		const int DIGIT		 = 8;						// スコア桁数
		const VECTOR3 POS	 = VECTOR3(740.0f, 190.0f, 0.0f);				// スコア位置
		const VECTOR3 OFFSET = VECTOR3(0.0f, 112.0f, 0.0f);					// スコアオフセット
		const VECTOR3 SIZE	 = VECTOR3(52.8f, 62.4f, 0.0f) * 1.4f * 1.3f;	// スコア数字大きさ
		const VECTOR3 SPACE	 = VECTOR3(SIZE.x * 0.85f, 0.0f, 0.0f);			// スコア数字空白
	}
#else TIMER
	const float	RANK_INIT[CRankingManager::MAX_RANK] = { 0.0f, 0.0f, 0.0f, 0.0f, 0.0f };	// 初期ランキング
	const char*	RANK_PATH	= "data\\BIN\\rank_time.bin";	// ランキングパス
	const COLOR	COL_UPDATE	= color::Yellow();	// ランキング更新色
	namespace time
	{
		const CValue::EType TYPE = CValue::TYPE_NORMAL;	// 数字種類
		const VECTOR3 POS		 = VECTOR3(720.0f, 190.0f, 0.0f);				// タイマー位置
		const VECTOR3 OFFSET	 = VECTOR3(0.0f, 112.0f, 0.0f);					// タイマーオフセット
		const VECTOR3 VAL_SIZE	 = VECTOR3(52.8f, 62.4f, 0.0f) * 1.4f * 1.3f;			// タイマー数字大きさ
		const VECTOR3 PART_SIZE	 = VECTOR3(27.3f, 62.4f, 0.0f) * 1.2f * 1.3f;			// タイマー区切り大きさ
		const VECTOR3 VAL_SPACE	 = VECTOR3(VAL_SIZE.x * 0.85f, 0.0f, 0.0f);		// タイマー数字空白
		const VECTOR3 PART_SPACE = VECTOR3(PART_SIZE.x * 0.85f, 0.0f, 0.0f);	// タイマー区切り空白
	}
#endif
}

//************************************************************
//	親クラス [CRankingManager] のメンバ関数
//************************************************************
//============================================================
//	コンストラクタ
//============================================================
CRankingManager::CRankingManager() :
	m_pState	(nullptr)	// 状態
{
	memset(&m_apRank[0], 0, sizeof(m_apRank));		// 順位情報
#ifdef SCORE
	memset(&m_apScore[0], 0, sizeof(m_apScore));	// スコア情報
#else TIMER
	memset(&m_apTime[0], 0, sizeof(m_apTime));		// タイム情報
#endif
}

//============================================================
//	デストラクタ
//============================================================
CRankingManager::~CRankingManager()
{

}

//============================================================
//	初期化処理
//============================================================
HRESULT CRankingManager::Init()
{
	CRetention* pRetention = GET_MANAGER->GetRetention();	// データ保存マネージャー

	// メンバ変数を初期化
	memset(&m_apRank[0], 0, sizeof(m_apRank));		// 順位情報
#ifdef SCORE
	memset(&m_apScore[0], 0, sizeof(m_apScore));	// スコア情報
#else TIMER
	memset(&m_apTime[0], 0, sizeof(m_apTime));		// タイム情報
#endif
	m_pState = nullptr;	// 状態

#ifdef SCORE
	// ランキングの読込
	int aRankArray[MAX_RANK];	// ランキング数値配列
	LoadRank(RANK_PATH, &aRankArray[0]);
#else TIMER
	// ランキングの読込
	float aRankArray[MAX_RANK];	// ランキング数値配列
	LoadRank(RANK_PATH, &aRankArray[0]);
#endif

	// タイトルの生成
	m_pTitle = CObject2D::Create(title::POS);
	if (m_pTitle == nullptr)
	{ // 生成に失敗した場合

		assert(false);
		return E_FAIL;
	}

	// 自動ラベルを設定
	m_pTitle->SetLabel(CObject::LABEL_UI);

	// タイトルテクスチャの割当
	m_pTitle->BindTexture(title::TEXTURE);

	// タイトル大きさの設定
	float fTitleWidth = useful::GetTexWidthFromAspect(title::HEIGHT, title::TEXTURE);	// テクスチャ基準の横幅
	m_pTitle->SetVec3Size(VECTOR3(fTitleWidth, title::HEIGHT, 0.0f));

	for (int i = 0; i < MAX_RANK; i++)
	{ // ランキング上位表示数分繰り返す

		// 順位の生成
		m_apRank[i] = CAnim2D::Create
		( // 引数
			rank::PTRN.x,	// テクスチャの横の分割数
			rank::PTRN.y,	// テクスチャの縦の分割数
			rank::POS + (rank::OFFSET * (float)i)	// 位置
		);
		if (m_apRank[i] == nullptr)
		{ // 生成に失敗した場合

			assert(false);
			return E_FAIL;
		}

		// 順位テクスチャの割当
		m_apRank[i]->BindTexture(rank::TEXTURE);

		// 順位大きさの設定
		float fRankHeight = useful::GetTexHeightFromAspect(rank::WIDTH, rank::TEXTURE);	// テクスチャ基準の縦幅
		m_apRank[i]->SetVec3Size(VECTOR3(rank::WIDTH, fRankHeight * (float)rank::PTRN.x, 0.0f));

		// 順位テクスチャパターンの設定
		m_apRank[i]->SetPattern(i);

#ifdef SCORE
		// スコアの生成
		const VECTOR3 posScore = score::POS + (score::OFFSET * (float)i);	// スコア位置
		m_apScore[i] = CMultiValue::Create
		( // 引数
			score::TYPE,	// 数字種類
			aRankArray[i],	// 数字
			score::DIGIT,	// 桁数
			posScore,		// 位置
			score::SIZE,	// 大きさ
			score::SPACE	// 空白
		);
		if (m_apScore[i] == nullptr)
		{ // 生成に失敗した場合

			assert(false);
			return E_FAIL;
		}
#else TIMER
		// タイムの生成
		const VECTOR3 posTime = time::POS + (time::OFFSET * (float)i);	// タイム位置
		m_apTime[i] = CTimeUI::Create
		( // 引数
			aRankArray[i],		// 時間
			posTime,			// 位置
			time::VAL_SIZE,		// 数字の大きさ
			time::PART_SIZE,	// 区切りの大きさ
			time::VAL_SPACE,	// 数字の空白
			time::PART_SPACE,	// 区切りの空白
			time::TYPE			// 数字種類
		);
		if (m_apTime[i] == nullptr)
		{ // 生成に失敗した場合

			assert(false);
			return E_FAIL;
		}
#endif
	}

	const int nUpdateIdx = pRetention->GetUpdateRankIdx();	// ランキング更新インデックス
	if (nUpdateIdx > NONE_IDX)
	{ // ランキングが更新されていた場合

#ifdef SCORE
		// 色を変更する
		m_apScore[nUpdateIdx]->SetColor(COL_UPDATE);
#else TIMER
		// 色を変更する
		m_apTime[nUpdateIdx]->SetColor(COL_UPDATE);
#endif
	}

	// 通常状態にする
	ChangeState(new CRankingStateNormal);

	// コンボエリアマネージャーを生成
	CComboAreaManager::Create();

	return S_OK;
}

//============================================================
//	終了処理
//============================================================
void CRankingManager::Uninit()
{
	// 状態の終了
	SAFE_UNINIT(m_pState);

	for (int i = 0; i < MAX_RANK; i++)
	{ // ランキング上位表示数分繰り返す

		// 順位の終了
		SAFE_UNINIT(m_apRank[i]);

#ifdef SCORE
		// スコアの終了
		SAFE_UNINIT(m_apScore[i]);
#else TIMER
		// タイムの終了
		SAFE_UNINIT(m_apTime[i]);
#endif
	}
}

//============================================================
//	更新処理
//============================================================
void CRankingManager::Update(const float fDeltaTime)
{
	// 状態ごとの更新
	assert(m_pState != nullptr);
	m_pState->Update(fDeltaTime);

	if (input::Decide())
	{
		// タイトル画面に遷移する
		TransTitle();
	}

	for (int i = 0; i < MAX_RANK; i++)
	{ // ランキング上位表示数分繰り返す

		// 順位の更新
		m_apRank[i]->Update(fDeltaTime);

#ifdef SCORE
		// スコアの更新
		m_apScore[i]->Update(fDeltaTime);
#else TIMER
		// タイムの更新
		m_apTime[i]->Update(fDeltaTime);
#endif
	}
}

//============================================================
//	状態の変更処理
//============================================================
HRESULT CRankingManager::ChangeState(CRankingState* pState)
{
	// 状態の生成に失敗している場合抜ける
	if (pState == nullptr) { assert(false); return E_FAIL; }

	// 状態インスタンスを終了
	SAFE_UNINIT(m_pState);

	// 状態インスタンスを変更
	assert(m_pState == nullptr);
	m_pState = pState;

	// 状態にコンテキストを設定
	m_pState->SetContext(this);

	// 状態インスタンスを初期化
	if (FAILED(m_pState->Init()))
	{ // 初期化に失敗した場合

		assert(false);
		return E_FAIL;
	}

	return S_OK;
}

//============================================================
//	タイトル画面遷移処理
//============================================================
void CRankingManager::TransTitle()
{
	// フェード中の場合抜ける
	if (GET_MANAGER->GetFade()->IsFade()) { return; }

	// ランキング更新情報の初期化
	CRetention* pRetention = GET_MANAGER->GetRetention();	// データ保存マネージャー
	pRetention->SetUpdateRankIdx(NONE_IDX);

	// ゲーム画面に遷移する
	GET_MANAGER->SetLoadScene(CScene::MODE_TITLE);
}

//============================================================
//	生成処理
//============================================================
CRankingManager* CRankingManager::Create()
{
	// ランキングマネージャーの生成
	CRankingManager* pRankingManager = new CRankingManager;
	if (pRankingManager == nullptr)
	{ // 生成に失敗した場合

		return nullptr;
	}
	else
	{ // 生成に成功した場合

		// ランキングマネージャーの初期化
		if (FAILED(pRankingManager->Init()))
		{ // 初期化に失敗した場合

			// ランキングマネージャーの破棄
			SAFE_DELETE(pRankingManager);
			return nullptr;
		}

		// 確保したアドレスを返す
		return pRankingManager;
	}
}

//============================================================
//	破棄処理
//============================================================
void CRankingManager::Release(CRankingManager*& prRankingManager)
{
	// ランキングマネージャーの終了
	assert(prRankingManager != nullptr);
	prRankingManager->Uninit();

	// メモリ開放
	SAFE_DELETE(prRankingManager);
}

#ifdef SCORE
//============================================================
//	ランキングの設定処理
//============================================================
int CRankingManager::SetRank(const int nNewScore)
{
	int nLowIdx = MAX_RANK - 1;	// 最下位インデックス
	int nUpdateIdx = NONE_IDX;	// 更新順位インデックス
	int aRank[MAX_RANK];		// ランキング時間

	// ランキングの読込
	if (FAILED(LoadRank(RANK_PATH, &aRank[0])))
	{ // 読込に失敗した場合

		// ランキングの再読込
		if (FAILED(LoadRank(RANK_PATH, &aRank[0])))
		{ // 再読込に失敗した場合

			// 失敗を返す
			assert(false);
			return NONE_IDX;
		}
	}

	if (nNewScore > aRank[nLowIdx])
	{ // 最下位のスコアより高い場合

		// ランキングのソート
		nUpdateIdx = SortRank(nNewScore, &aRank[0]);

		// ランキングの保存
		SaveRank(RANK_PATH, &aRank[0]);
	}

	// 順位が更新されたインデックスを返す
	return nUpdateIdx;
}

//============================================================
//	ランキングのソート処理
//============================================================
int CRankingManager::SortRank(const int nNewScore, int* pRankArray)
{
	int nLowIdx = MAX_RANK - 1;	// 最下位インデックス
	int	nCurMaxIdx;	// 最大値のインデックス

	std::vector<int> idxRank;	// ランキングインデックス
	for (int i = 0; i < MAX_RANK; i++)
	{ idxRank.push_back(i); }	// 自身の要素番号を保存

	// 現在の最下位の情報と書き換え
	pRankArray[nLowIdx] = nNewScore;

	for (int nCntKeep = 0; nCntKeep < nLowIdx; nCntKeep++)
	{ // 入れ替える値の総数 -1回分繰り返す

		// 現在の繰り返し数を代入 (要素1とする)
		nCurMaxIdx = nCntKeep;

		for (int nCntSort = nCntKeep + 1; nCntSort < MAX_RANK; nCntSort++)
		{ // 入れ替える値の総数 -nCntKeep分繰り返す

			if (pRankArray[nCurMaxIdx] < pRankArray[nCntSort])
			{ // 最大値に設定されている値より、現在の値のほうが大きい場合

				// 現在の要素番号を最大値に設定
				nCurMaxIdx = nCntSort;
			}
		}

		if (nCntKeep != nCurMaxIdx)
		{ // 最大値の要素番号に変動があった場合

			// 要素の入れ替え
			int nKeepNum			= pRankArray[nCntKeep];
			pRankArray[nCntKeep]	= pRankArray[nCurMaxIdx];
			pRankArray[nCurMaxIdx]	= nKeepNum;

			// 要素の入れ替え
			int nKeepIdx		= idxRank[nCntKeep];
			idxRank[nCntKeep]	= idxRank[nCurMaxIdx];
			idxRank[nCurMaxIdx]	= nKeepIdx;
		}
	}

	// 最下位インデックスを検索
	auto itr = std::find(idxRank.begin(), idxRank.end(), nLowIdx);

	// 順位が更新されたインデックスを返す
	return std::distance(idxRank.begin(), itr);
}

//============================================================
//	ランキングの読込処理
//============================================================
HRESULT CRankingManager::LoadRank(const char* pPath, int* pRankArray)
{
	// ファイルを開く
	std::ifstream file(pPath, std::ios_base::binary);	// ファイルストリーム
	if (file.fail())
	{ // ファイルが開けなかった場合

#ifdef _DEBUG
		// エラーメッセージボックス
		MessageBox(nullptr, "ランキングの読み込みに失敗！", "警告！", MB_ICONWARNING);
#endif

		// 値をすべて最大タイムにする
		int aRank[MAX_RANK];	// ランキング生成用
		for (int i = 0; i < MAX_RANK; i++)
		{ aRank[i] = RANK_INIT[i]; }

		// クリアタイムの保存
		SaveRank(pPath, &aRank[0]);

		// 失敗を返す
		return E_FAIL;
	}

	// 引数の獲得状況を読み込み
	file.read((char*)pRankArray, sizeof(int) * MAX_RANK);

	// ファイルを閉じる
	file.close();

	// 成功を返す
	return S_OK;
}

//============================================================
//	ランキングの保存処理
//============================================================
HRESULT CRankingManager::SaveRank(const char* pPath, int* pRankArray)
{
	// ファイルを開く
	std::ofstream file(pPath, std::ios_base::binary);	// ファイルストリーム
	if (file.fail())
	{ // ファイルが開けなかった場合

		// エラーメッセージボックス
		MessageBox(nullptr, "ランキングの書き出しに失敗！", "警告！", MB_ICONWARNING);

		// 失敗を返す
		return E_FAIL;
	}

	// 引数の獲得状況を書き出し
	file.write((char*)pRankArray, sizeof(int) * MAX_RANK);

	// ファイルを閉じる
	file.close();

	// 成功を返す
	return S_OK;
}
#else TIMER
//============================================================
//	ランキングの設定処理
//============================================================
int CRankingManager::SetRank(const float fNewTime)
{
	int nLowIdx = MAX_RANK - 1;	// 最下位インデックス
	int nUpdateIdx = NONE_IDX;	// 更新順位インデックス
	float aRank[MAX_RANK];		// ランキング時間

	// ランキングの読込
	if (FAILED(LoadRank(RANK_PATH, &aRank[0])))
	{ // 読込に失敗した場合

		// ランキングの再読込
		if (FAILED(LoadRank(RANK_PATH, &aRank[0])))
		{ // 再読込に失敗した場合

			// 失敗を返す
			assert(false);
			return NONE_IDX;
		}
	}

	if (fNewTime > aRank[nLowIdx])
	{ // 最下位の時間より速い場合

		// ランキングのソート
		nUpdateIdx = SortRank(fNewTime, &aRank[0]);

		// ランキングの保存
		SaveRank(RANK_PATH, &aRank[0]);
	}

	// 順位が更新されたインデックスを返す
	return nUpdateIdx;
}

//============================================================
//	ランキングのソート処理
//============================================================
int CRankingManager::SortRank(const float fNewTime, float* pRankArray)
{
	int nLowIdx = MAX_RANK - 1;	// 最下位インデックス
	int	nCurMinIdx;	// 最小値のインデックス

	std::vector<int> idxRank;	// ランキングインデックス
	for (int i = 0; i < MAX_RANK; i++)
	{ idxRank.push_back(i); }	// 自身の要素番号を保存

	// 現在の最下位の情報と書き換え
	pRankArray[nLowIdx] = fNewTime;

	for (int nCntKeep = 0; nCntKeep < nLowIdx; nCntKeep++)
	{ // 入れ替える値の総数 -1回分繰り返す

		// 現在の繰り返し数を代入 (要素1とする)
		nCurMinIdx = nCntKeep;

		for (int nCntSort = nCntKeep + 1; nCntSort < MAX_RANK; nCntSort++)
		{ // 入れ替える値の総数 -nCntKeep分繰り返す

			if (pRankArray[nCurMinIdx] < pRankArray[nCntSort])
			{ // 最小値に設定されている値より、現在の値のほうが小さい場合

				// 現在の要素番号を最小値に設定
				nCurMinIdx = nCntSort;
			}
		}

		if (nCntKeep != nCurMinIdx)
		{ // 最小値の要素番号に変動があった場合

			// 要素の入れ替え
			float fKeepNum			= pRankArray[nCntKeep];
			pRankArray[nCntKeep]	= pRankArray[nCurMinIdx];
			pRankArray[nCurMinIdx]	= fKeepNum;

			// 要素の入れ替え
			int nKeepIdx		= idxRank[nCntKeep];
			idxRank[nCntKeep]	= idxRank[nCurMinIdx];
			idxRank[nCurMinIdx]	= nKeepIdx;
		}
	}

	// 最下位インデックスを検索
	auto itr = std::find(idxRank.begin(), idxRank.end(), nLowIdx);

	// 順位が更新されたインデックスを返す
	return std::distance(idxRank.begin(), itr);
}

//============================================================
//	ランキングの読込処理
//============================================================
HRESULT CRankingManager::LoadRank(const char* pPath, float* pRankArray)
{
	// ファイルを開く
	std::ifstream file(pPath, std::ios_base::binary);	// ファイルストリーム
	if (file.fail())
	{ // ファイルが開けなかった場合

#ifdef _DEBUG
		// エラーメッセージボックス
		MessageBox(nullptr, "ランキングの読み込みに失敗！", "警告！", MB_ICONWARNING);
#endif

		// 値をすべて最大タイムにする
		float aRank[MAX_RANK];	// ランキング生成用
		for (int i = 0; i < MAX_RANK; i++)
		{ aRank[i] = RANK_INIT[i]; }

		// クリアタイムの保存
		SaveRank(pPath, &aRank[0]);

		// 失敗を返す
		return E_FAIL;
	}

	// 引数の獲得状況を読み込み
	file.read((char*)pRankArray, sizeof(float) * MAX_RANK);

	// ファイルを閉じる
	file.close();

	// 成功を返す
	return S_OK;
}

//============================================================
//	ランキングの保存処理
//============================================================
HRESULT CRankingManager::SaveRank(const char* pPath, float* pRankArray)
{
	// ファイルを開く
	std::ofstream file(pPath, std::ios_base::binary);	// ファイルストリーム
	if (file.fail())
	{ // ファイルが開けなかった場合

		// エラーメッセージボックス
		MessageBox(nullptr, "ランキングの書き出しに失敗！", "警告！", MB_ICONWARNING);

		// 失敗を返す
		return E_FAIL;
	}

	// 引数の獲得状況を書き出し
	file.write((char*)pRankArray, sizeof(float) * MAX_RANK);

	// ファイルを閉じる
	file.close();

	// 成功を返す
	return S_OK;
}
#endif
