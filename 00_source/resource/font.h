//============================================================
//
//	フォントヘッダー [font.h]
//	Author：藤田勇一
//
//============================================================
//************************************************************
//	二重インクルード防止
//************************************************************
#ifndef _FONT_H_
#define _FONT_H_

//************************************************************
//	インクルードファイル
//************************************************************
#include "fontChar.h"
#include "extension.h"

//************************************************************
//	クラス定義
//************************************************************
// フォントクラス
class CFont
{
public:
	// コンストラクタ
	CFont();

	// デストラクタ
	~CFont();

	// キー構造体
	struct SKey
	{
		// デフォルトコンストラクタ
		SKey() :
			sFilePath	(""),	// フォントファイルパス
			bItalic		(false)	// イタリック
		{}

		// 引数付きコンストラクタ
		SKey(std::string in_sFontName, bool in_bItalic) :
			sFilePath	(in_sFontName),	// フォントファイルパス
			bItalic		(in_bItalic)	// イタリック
		{}

		// 比較演算子オーバーロード
		inline bool operator<(const SKey& rKey) const
		{
			// 情報をまとめて比較
			return std::tie(sFilePath, bItalic) < std::tie(rKey.sFilePath, rKey.bItalic);
		}

		// メンバ変数
		std::string sFilePath;	// フォントファイルパス
		bool bItalic;			// イタリック
	};

	// フォント構造体
	struct SFont
	{
		// デフォルトコンストラクタ
		SFont() :
			sFontName	(""),		// フォント名をクリア
			pFontChar	(nullptr),	// フォント文字インスタンス
			pFont		(nullptr)	// フォントへのポインタ
		{}

		// メンバ変数
		std::string sFontName;	// フォント名
		CFontChar* pFontChar;	// フォント文字インスタンス
		HFONT pFont;			// フォントへのポインタ
	};

	// メンバ関数
	HRESULT Init();		// フォント初期化
	void Uninit();		// フォント終了
	HRESULT LoadAll();	// フォント全読込
	HRESULT Load(const std::string& rFilePath);	// フォント読込
	SFont Regist(const std::string& rFilePath, const bool bItalic = false);	// フォント登録
	CFontChar::SChar RegistChar	// フォント文字登録
	( // 引数
		const wchar_t wcChar,			// 指定文字
		const std::string& rFilePath,	// ファイルパス
		const bool bItalic = false		// イタリック
	);

	// 静的メンバ関数
	static CFont* Create();					// 生成
	static void Release(CFont*& prFont);	// 破棄

private:
	// メンバ関数
	void RegistPrepare(const std::string& rFilePath);			// フォント・フォント文字の事前登録
	HRESULT SearchFolderAll(const std::string& rFolderPath);	// フォルダ全検索

	// メンバ変数
	std::map<SKey, SFont> m_mapFont;		// フォント連想配列
	std::vector<std::string> m_vecFilePath;	// 読込済みファイルパス
	extension::ALoad m_load;				// 読み込み可能拡張子
};

#endif	// _FONT_H_
