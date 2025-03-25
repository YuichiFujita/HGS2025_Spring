//============================================================
//
//	モーションヘッダー [motion.h]
//	Author：藤田勇一
//
//============================================================
//************************************************************
//	二重インクルード防止
//************************************************************
#ifndef _MOTION_H_
#define _MOTION_H_

//************************************************************
//	前方宣言
//************************************************************
class CMultiModel;	// マルチモデルクラス
class CObjectChara;	// オブジェクトキャラクタークラス

//************************************************************
//	クラス定義
//************************************************************
// モーションクラス
class CMotion
{
public:
	// コンストラクタ
	explicit CMotion(std::function<int()> funcGetNumParts);

	// デストラクタ
	~CMotion();

	// 判定カウント管理構造体
	struct SCollTime
	{
		// デフォルトコンストラクタ
		SCollTime() :
			nMin (NONE_IDX),	// 攻撃判定の開始カウント
			nMax (NONE_IDX)		// 攻撃判定の終了カウント
		{}

		// デストラクタ
		~SCollTime() {}

		// メンバ変数
		int nMin;	// 攻撃判定の開始カウント
		int nMax;	// 攻撃判定の終了カウント
	};

	// パーツ管理構造体
	struct SParts
	{
		// デフォルトコンストラクタ
		SParts() :
			pos	(VEC3_ZERO),	// モデル位置
			rot	(VEC3_ZERO)		// モデル向き
		{}

		// デストラクタ
		~SParts() {}

		// メンバ変数
		VECTOR3 pos;	// モデル位置
		VECTOR3 rot;	// モデル向き
	};

	// キー管理構造体
	struct SKey
	{
		// デフォルトコンストラクタ
		SKey() :
			vecParts ({}),			// パーツ情報
			move	 (VEC3_ZERO),	// キー移動量
			nFrame	 (0)			// キー再生フレーム数
		{}

		// デストラクタ
		~SKey() {}

		// メンバ変数
		std::vector<SParts> vecParts;	// パーツ情報
		VECTOR3 move;	// キー移動量
		int nFrame;		// キー再生フレーム数
	};

	// モーション管理構造体
	struct SMotion
	{
		// デフォルトコンストラクタ
		SMotion() :
			vecKey		 ({}),			// キー情報
			collLeft	 ({}),			// 左攻撃判定のカウント
			collRight	 ({}),			// 右攻撃判定のカウント
			nWholeFrame	 (0),			// モーション全体フレーム数
			nCancelFrame (NONE_IDX),	// キャンセル可能フレーム
			nComboFrame	 (NONE_IDX),	// コンボ可能フレーム
			bLoop		 (false),		// ループON/OFF
			bWeaponDisp	 (false)		// 武器表示ON/OFF
		{}

		// デストラクタ
		~SMotion() {}

		// メンバ関数
		inline int GetNumKey() { return (int)vecKey.size(); }	// キー情報の総数取得

		// メンバ変数
		std::vector<SKey> vecKey;	// キー情報
		SCollTime collLeft;			// 左攻撃判定のカウント
		SCollTime collRight;		// 右攻撃判定のカウント
		int  nWholeFrame;			// モーション全体フレーム数
		int  nCancelFrame;			// キャンセル可能フレーム
		int  nComboFrame;			// コンボ可能フレーム
		bool bLoop;					// ループON/OFF
		bool bWeaponDisp;			// 武器表示ON/OFF
	};

	// モーション情報構造体
	struct SInfo
	{
		// デフォルトコンストラクタ
		SInfo() :
			vecMotion		({}),	// モーション情報
			vecOriginParts	({}),	// パーツ原点情報
			nType			(0),	// モーション種類
			nKey			(0),	// モーションキー番号
			nKeyCounter		(0),	// モーションキーカウンター
			nWholeCounter	(0),	// モーション全体カウンター
			bFinish			(false)	// モーション終了状況
		{}

		// デストラクタ
		~SInfo() {}

		// メンバ関数
		inline int GetNumMotion() { return (int)vecMotion.size(); }	// モーション情報の総数取得

		// メンバ変数
		std::vector<SMotion> vecMotion;		// モーション情報
		std::vector<SParts> vecOriginParts;	// パーツ原点情報
		int  nType;			// モーション種類
		int  nKey;			// モーションキー番号
		int  nKeyCounter;	// モーションキーカウンター
		int  nWholeCounter;	// モーション全体カウンター
		bool bFinish;		// モーション終了状況
	};

	// ブレンド情報構造体
	struct SBlend
	{
		// デフォルトコンストラクタ
		SBlend() :
			vecParts		({}),	// ブレンド開始パーツ情報
			nFrame			(0),	// ブレンド再生フレーム数
			nWholeCounter	(0)		// ブレンド全体カウンター
		{}

		// デストラクタ
		~SBlend() {}

		// メンバ変数
		std::vector<SParts> vecParts;	// ブレンド開始パーツ情報
		int nFrame;			// ブレンド再生フレーム数
		int nWholeCounter;	// ブレンド全体カウンター
	};

	// メンバ関数
	HRESULT Init();	// 初期化
	void Uninit();	// 終了
	void Update(const float fDeltaTime);		// 更新
	void BindPartsData(CMultiModel** ppModel);	// パーツ情報設定
	void SetAllInfo(const SInfo& rInfo);		// モーション情報全設定
	void AddInfo(const SMotion& rMotion);		// モーション情報追加
	void SetEnableUpdate(const bool bUpdate);	// 更新状況設定
	void SetNumParts(const int nNumParts);		// パーツ数設定
	void ClearVector();							// モーション情報の動的配列クリア
	void Set(const int nType, const int nBlendFrame = 0);			// 設定
	void SetOriginPosition(const VECTOR3& rPos, const int nParts);	// 原点位置の設定
	void SetOriginRotation(const VECTOR3& rRot, const int nParts);	// 原点向きの設定

	int GetNumType();						// 種類総数取得
	int GetNumKey(const int nType);			// キー総数取得
	bool IsCancel(const int nType) const;	// キャンセル取得
	bool IsCombo(const int nType) const;	// コンボ取得
	bool IsLeftWeaponCollision();			// 左の攻撃判定フラグ取得
	bool IsRightWeaponCollision();			// 右の攻撃判定フラグ取得
	VECTOR3 GetOriginPosition(const int nParts);	// 原点位置の取得
	VECTOR3 GetOriginRotation(const int nParts);	// 原点向きの取得

	inline int GetType() const			{ return m_info.nType; }			// 種類取得
	inline int GetKey() const			{ return m_info.nKey; }				// キー番号取得
	inline int GetKeyCounter() const	{ return m_info.nKeyCounter; }		// モーションキーカウンター取得
	inline int GetWholeCounter() const	{ return m_info.nWholeCounter; }	// モーション全体カウンター取得
	inline bool IsFinish() const		{ return m_info.bFinish; }			// 終了取得
	inline bool IsLoop(const int nType) const			{ return m_info.vecMotion[nType].bLoop; }			// ループ取得
	inline bool IsWeaponDisp(const int nType) const		{ return m_info.vecMotion[nType].bWeaponDisp; }		// 武器表示取得
	inline int GetWholeFrame(const int nType) const		{ return m_info.vecMotion[nType].nWholeFrame; }		// モーション全体フレーム数取得
	inline int GetCancelFrame(const int nType) const	{ return m_info.vecMotion[nType].nCancelFrame; }	// モーションキャンセルフレーム取得
	inline int GetComboFrame(const int nType) const		{ return m_info.vecMotion[nType].nComboFrame; }		// モーションコンボフレーム取得

	// 静的メンバ関数
	static CMotion* Create(CObjectChara* pChara);	// 生成
	static void Release(CMotion*& prMotion);		// 破棄

private:
	// メンバ関数
	void UpdateMove();		// 移動更新
	void UpdateMotion();	// モーション更新
	void UpdateBlend();		// ブレンド更新

	// メンバ変数
	const std::function<int()> m_funcGetNumParts;	// パーツ数取得関数ポインタ
	CMultiModel** m_ppModel;	// モデル情報
	CObjectChara* m_pChara;		// オブジェクトキャラクター情報
	SInfo	m_info;		// モーション情報
	SBlend	m_blend;	// ブレンド情報
	bool	m_bUpdate;	// 更新状況
};

#endif	// _MOTION_H_
