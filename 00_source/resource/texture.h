//============================================================
//
//	テクスチャヘッダー [texture.h]
//	Author：藤田勇一
//
//============================================================
//************************************************************
//	二重インクルード防止
//************************************************************
#ifndef _TEXTURE_H_
#define _TEXTURE_H_

//************************************************************
//	インクルードファイル
//************************************************************
#include "extension.h"

//************************************************************
//	クラス定義
//************************************************************
// テクスチャクラス
class CTexture
{
public:
	// コンストラクタ
	CTexture();

	// デストラクタ
	~CTexture();

	// テクスチャ生成情報構造体
	struct SInfo
	{
	public:
		// デフォルトコンストラクタ
		SInfo() :
			Width		(0),	// テクスチャ横幅
			Height		(0),	// テクスチャ縦幅
			MipLevels	(0),	// ミップマップレベル
			Usage		(0),	// 性質・確保オプション
			Format		(D3DFMT_UNKNOWN),	// ピクセルフォーマット
			Pool		(D3DPOOL_DEFAULT)	// 格納メモリ
		{}

		// 引数付きコンストラクタ
		SInfo(const UINT in_nWidth, const UINT in_nHeight, const UINT in_nMip, const DWORD in_dwUsage, const D3DFORMAT in_format, const D3DPOOL in_pool) :
			Width		(in_nWidth),	// テクスチャ横幅
			Height		(in_nHeight),	// テクスチャ縦幅
			MipLevels	(in_nMip),		// ミップマップレベル
			Usage		(in_dwUsage),	// 性質・確保オプション
			Format		(in_format),	// ピクセルフォーマット
			Pool		(in_pool)		// 格納メモリ
		{}

		// デストラクタ
		~SInfo() {}

		// メンバ変数
		UINT		Width;		// テクスチャ横幅
		UINT		Height;		// テクスチャ縦幅
		UINT		MipLevels;	// ミップマップレベル
		DWORD		Usage;		// 性質・確保オプション
		D3DFORMAT	Format;		// ピクセルフォーマット
		D3DPOOL		Pool;		// 格納メモリ
	};

	// テクスチャ構造体
	struct STexture
	{
		// デフォルトコンストラクタ
		STexture() :
			pTexture (nullptr),		// テクスチャへのポインタ
			status	 ({}),			// テクスチャステータスへのポインタ
			aspect	 (VEC2_ZERO)	// アスペクト比
		{}

		// デストラクタ
		~STexture() {}

		// メンバ変数
		LPDIRECT3DTEXTURE9 pTexture;	// テクスチャへのポインタ
		D3DXIMAGE_INFO status;			// テクスチャステータスへのポインタ
		VECTOR2 aspect;					// アスペクト比
	};

	// マップ情報構造体
	struct SMapInfo
	{
		// デフォルトコンストラクタ
		SMapInfo() :
			textureData		({}),	// テクスチャ情報
			sFilePathName	("")	// ファイルパス名
		{}

		// デストラクタ
		~SMapInfo() {}

		// メンバ変数
		STexture textureData;		// テクスチャ情報
		std::string sFilePathName;	// ファイルパス名
	};

	// メンバ関数
	HRESULT Init();		// テクスチャ初期化
	void Uninit();		// テクスチャ終了
	HRESULT LoadAll();	// テクスチャ全読込
	int Regist(const SInfo info);				// テクスチャ登録 (生成)
	int Regist(std::string sFilePath);			// テクスチャ登録 (パス)
	STexture GetInfo(const int nIdx);			// テクスチャ情報取得
	LPDIRECT3DTEXTURE9 GetPtr(const int nIdx);	// テクスチャポインタ取得

	// 静的メンバ関数
	static CTexture* Create();					// 生成
	static void Release(CTexture*& prTexture);	// 破棄

private:
	// メンバ関数
	HRESULT SearchFolderAll(std::string sFolderPath);	// フォルダ全検索

	// メンバ変数
	std::map<int, SMapInfo> m_mapTexture;	// テクスチャ連想配列
	extension::ALoad m_load;				// 読み込み可能拡張子
};

#endif	// _TEXTURE_H_
