//============================================================
//
//	キャラクターヘッダー [character.h]
//	Author：藤田勇一
//
//============================================================
//************************************************************
//	二重インクルード防止
//************************************************************
#ifndef _CHARACTER_H_
#define _CHARACTER_H_

//************************************************************
//	インクルードファイル
//************************************************************
#include "motion.h"
#include "extension.h"

//************************************************************
//	クラス定義
//************************************************************
// キャラクタークラス
class CCharacter
{
public:
	// コンストラクタ
	CCharacter();

	// デストラクタ
	~CCharacter();

	// パーツ構造体
	struct SParts
	{
		// デフォルトコンストラクタ
		SParts() :
			sPath		(""),			// モデルパスをクリア
			pos			(VEC3_ZERO),	// 位置
			rot			(VEC3_ZERO),	// 向き
			nParentIdx	(NONE_IDX)		// 親インデックス
		{}

		// デストラクタ
		~SParts() {}

		// メンバ変数
		std::string sPath;	// モデルパス
		VECTOR3 pos;		// 位置
		VECTOR3 rot;		// 向き
		int nParentIdx;		// 親インデックス
	};

	// パーツ情報構造体
	struct SPartsInfo
	{
		// デフォルトコンストラクタ
		SPartsInfo() :
			vecParts ({})	// パーツ情報
		{}

		// デストラクタ
		~SPartsInfo() {}

		// メンバ関数
		inline int GetNumParts() { return vecParts.size(); }	// パーツ情報の総数取得

		// メンバ変数
		std::vector<SParts> vecParts;	// パーツ情報
	};

	// キャラクター情報構造体
	struct SCharaData
	{
		// デフォルトコンストラクタ
		SCharaData() :
			infoMotion	({}),	// モーション情報
			infoParts	({})	// パーツ情報
		{}

		// デストラクタ
		~SCharaData() {}

		// メンバ変数
		CMotion::SInfo infoMotion;	// モーション情報
		SPartsInfo infoParts;		// パーツ情報
	};

	// メンバ関数
	HRESULT Init();		// キャラクター初期化
	void Uninit();		// キャラクター終了
	HRESULT LoadAll();	// キャラクター全読込
	SCharaData Regist(const char* pCharaPath);	// キャラクター登録

	// 静的メンバ関数
	static CCharacter* Create();	// 生成
	static void Release(CCharacter*& prCharacter);	// 破棄

private:
	// メンバ関数
	HRESULT SearchFolderAll(std::string sFolderPath);	// フォルダ全検索

	// メンバ関数
	HRESULT LoadSetup(SCharaData* pInfoChara, const char* pCharaPath);	// キャラクター情報セットアップ
	HRESULT LoadMotionSetup(CMotion::SInfo* pInfoMotion, const SPartsInfo* pInfoParts, const char* pMotionPath);	// モーション情報セットアップ

	// メンバ変数
	std::map<std::string, SCharaData> m_mapCharacter;	// キャラクター連想配列
	extension::ALoad m_load;	// 読み込み可能拡張子
};

#endif	// _CHARACTER_H_
