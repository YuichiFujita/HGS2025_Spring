//============================================================
//
//	テクスチャ処理 [texture.cpp]
//	Author：藤田勇一
//
//============================================================
//************************************************************
//	インクルードファイル
//************************************************************
#include "texture.h"
#include "manager.h"
#include "renderer.h"

//************************************************************
//	定数宣言
//************************************************************
namespace
{
	const char* LOAD_EXTENSION = "data\\TXT\\EXTENSION\\texture.txt";	// テクスチャ読込拡張子相対パス
	const char* LOAD_FOLDER = "data\\TEXTURE";	// テクスチャフォルダ相対パス
}

//************************************************************
//	親クラス [CTexture] のメンバ関数
//************************************************************
//============================================================
//	コンストラクタ
//============================================================
CTexture::CTexture()
{
	// テクスチャ連想配列をクリア
	m_mapTexture.clear();

	// 読み込み可能拡張子をクリア
	m_load.clear();
}

//============================================================
//	デストラクタ
//============================================================
CTexture::~CTexture()
{

}

//============================================================
//	テクスチャ初期化処理
//============================================================
HRESULT CTexture::Init()
{
	// テクスチャ連想配列を初期化
	m_mapTexture.clear();

	// 読み込み可能拡張子を初期化
	m_load.clear();

	// 読み込み可能拡張子の読込
	m_load = extension::LoadExtension(LOAD_EXTENSION);

	return S_OK;
}

//============================================================
//	テクスチャ終了処理
//============================================================
void CTexture::Uninit()
{
	for (auto& rMap : m_mapTexture)
	{ // テクスチャの要素数分繰り返す

		// テクスチャの破棄
		SAFE_RELEASE(rMap.second.textureData.pTexture);
	}

	// テクスチャ連想配列をクリア
	m_mapTexture.clear();

	// 読み込み可能拡張子をクリア
	m_load.clear();
}

//============================================================
//	テクスチャ全読込処理
//============================================================
HRESULT CTexture::LoadAll()
{
	// テクスチャの全読込
	if (FAILED(SearchFolderAll(LOAD_FOLDER)))
	{ // 読込に失敗した場合

		assert(false);
		return E_FAIL;
	}

	return S_OK;
}

//============================================================
//	テクスチャ登録処理 (生成)
//============================================================
int CTexture::Regist(const SInfo info)
{
	int nIdx = (int)m_mapTexture.size();	// テクスチャ読込番号
	HRESULT hr;				// 異常終了の確認用
	SMapInfo tempMapInfo;	// マップ情報

	// マップ情報のポインタを初期化
	tempMapInfo.textureData.pTexture = nullptr;	// テクスチャへのポインタ

	// 空のテクスチャを生成
	hr = D3DXCreateTexture
	( // 引数
		GET_DEVICE,		// Direct3Dデバイス
		info.Width,		// テクスチャ横幅
		info.Height,	// テクスチャ縦幅
		info.MipLevels,	// ミップマップレベル
		info.Usage,		// 性質・確保オプション
		info.Format,	// ピクセルフォーマット
		info.Pool,		// 格納メモリ
		&tempMapInfo.textureData.pTexture	// テクスチャへのポインタ
	);
	if (FAILED(hr))
	{ // テクスチャの生成に失敗した場合

		assert(false);
		return NONE_IDX;
	}

	// テクスチャステータスを設定
	D3DXIMAGE_INFO* pStatus = &tempMapInfo.textureData.status;	// ステータス情報
	pStatus->Width			 = info.Width;						// テクスチャ横幅
	pStatus->Height			 = info.Height;						// テクスチャ縦幅
	pStatus->Depth			 = 1;								// テクスチャ深度
	pStatus->MipLevels		 = info.MipLevels;					// ミップマップレベル
	pStatus->Format			 = info.Format;						// ピクセルフォーマット
	pStatus->ResourceType	 = D3DRTYPE_TEXTURE;				// リソース種類
	pStatus->ImageFileFormat = (D3DXIMAGE_FILEFORMAT)NONE_IDX;	// ファイル形式 (作成のため無し)

	// ファイルパス名を保存
	tempMapInfo.sFilePathName = NONE_STRING;	// 読込ではないのでパス無し

	// アスペクト比を計算
	tempMapInfo.textureData.aspect.x = (float)info.Width / (float)info.Height;
	tempMapInfo.textureData.aspect.y = (float)info.Height / (float)info.Width;

	// テクスチャ情報を保存
	m_mapTexture.insert(std::make_pair(nIdx, tempMapInfo));

	// 読み込んだテクスチャの配列番号を返す
	return nIdx;
}

//============================================================
//	テクスチャ登録処理 (パス)
//============================================================
int CTexture::Regist(std::string sFilePath)
{
	int nIdx = (int)m_mapTexture.size();	// テクスチャ読込番号
	HRESULT hr;				// 異常終了の確認用
	SMapInfo tempMapInfo;	// マップ情報

	// マップ情報のポインタを初期化
	tempMapInfo.textureData.pTexture = nullptr;	// テクスチャへのポインタ

	// ファイルパスを標準化
	useful::StandardizePathPart(&sFilePath);

	// 既に読み込んでいないかを確認
	int nCntTexture = 0;
	for (const auto& rMap : m_mapTexture)
	{ // テクスチャの要素数分繰り返す

		if (rMap.second.sFilePathName.compare(sFilePath) == 0)
		{ // 文字列が一致した場合

			// すでに読み込んでいるテクスチャの配列番号を返す
			return nCntTexture;
		}

		// 次のテクスチャ番号にする
		nCntTexture++;
	}

	// 画像情報の取得
	D3DXIMAGE_INFO size;	// テクスチャステータス
	if (FAILED(D3DXGetImageInfoFromFile(sFilePath.c_str(), &size)))
	{ // 画像情報の取得に失敗した場合

		// エラーメッセージの作成
		std::string sError = "画像情報の取得に失敗！\nパス：";
		sError.append(sFilePath.c_str());

		// エラーメッセージボックス
		MessageBox(nullptr, sError.c_str(), "警告！", MB_ICONWARNING);
		return NONE_IDX;
	}

	// テクスチャの読込
	hr = D3DXCreateTextureFromFileEx
	( // 引数
		GET_DEVICE,			// Direct3Dデバイス
		sFilePath.c_str(),	// テクスチャファイルパス
		size.Width,			// テクスチャ横幅
		size.Height,		// テクスチャ縦幅
		0,					// ミップマップレベル
		0,					// 性質・確保オプション
		D3DFMT_A8R8G8B8,	// ピクセルフォーマット
		D3DPOOL_MANAGED,	// 格納メモリ
		D3DX_DEFAULT,		// フィルタ
		D3DX_DEFAULT,		// ミップマップフィルタ
		0,					// カラーキー
		&tempMapInfo.textureData.status,	// テクスチャステータスへのポインタ
		nullptr,							// テクスチャパレットへのポインタ
		&tempMapInfo.textureData.pTexture	// テクスチャへのポインタ
	);
	if (FAILED(hr))
	{ // テクスチャの読込に失敗した場合

		// エラーメッセージの作成
		std::string sError = "テクスチャの読込に失敗！\nパス：";
		sError.append(sFilePath.c_str());

		// エラーメッセージボックス
		MessageBox(nullptr, sError.c_str(), "警告！", MB_ICONWARNING);
		return NONE_IDX;
	}

	// ファイルパス名を保存
	tempMapInfo.sFilePathName = sFilePath;

	// アスペクト比を計算
	D3DXIMAGE_INFO status = tempMapInfo.textureData.status;	// テクスチャステータス
	tempMapInfo.textureData.aspect.x = (float)status.Width / (float)status.Height;
	tempMapInfo.textureData.aspect.y = (float)status.Height / (float)status.Width;

	// テクスチャ情報を保存
	m_mapTexture.insert(std::make_pair(nIdx, tempMapInfo));

	// 読み込んだテクスチャの配列番号を返す
	return nIdx;
}

//============================================================
//	テクスチャ情報の取得処理
//============================================================
CTexture::STexture CTexture::GetInfo(const int nIdx)
{
	int nArray = (int)m_mapTexture.size();	// 配列要素数
	if (nIdx > NONE_IDX && nIdx < nArray)
	{ // テクスチャがある場合

		// 引数のテクスチャ情報を返す
		return m_mapTexture.find(nIdx)->second.textureData;
	}
	else
	{ // テクスチャがない場合

		// インデックスエラー
		assert(false);

		if (nArray > 0)
		{ // テクスチャ生成がされている場合

			// 先頭テクスチャを返す
			return m_mapTexture.find(0)->second.textureData;
		}
		else
		{ // テクスチャが一つもない場合

			// 空のテクスチャ情報を返す
			return {};
		}
	}
}

//============================================================
//	テクスチャポインタの取得処理
//============================================================
LPDIRECT3DTEXTURE9 CTexture::GetPtr(const int nIdx)
{
	if (nIdx >= 0 && nIdx < (int)m_mapTexture.size())
	{ // 引数のインデックスが範囲内の場合

		// 引数のテクスチャポインタを返す
		return m_mapTexture.find(nIdx)->second.textureData.pTexture;
	}
	else if (nIdx == NONE_IDX)
	{ // 引数のインデックスが -1の場合

		// nullptrを返す
		return nullptr;
	}
	else
	{ // 引数のインデックスが使用不可の場合

		// nullptrを返す
		assert(false);
		return nullptr;
	}
}

//============================================================
//	生成処理
//============================================================
CTexture* CTexture::Create()
{
	// テクスチャの生成
	CTexture* pTexture = new CTexture;
	if (pTexture == nullptr)
	{ // 生成に失敗した場合

		return nullptr;
	}
	else
	{ // 生成に成功した場合

		// テクスチャの初期化
		if (FAILED(pTexture->Init()))
		{ // テクスチャ初期化に失敗した場合

			// テクスチャの破棄
			SAFE_DELETE(pTexture);
			return nullptr;
		}

		// 確保したアドレスを返す
		return pTexture;
	}
}

//============================================================
//	破棄処理
//============================================================
void CTexture::Release(CTexture*& prTexture)
{
	// テクスチャの終了
	assert(prTexture != nullptr);
	prTexture->Uninit();

	// メモリ開放
	SAFE_DELETE(prTexture);
}

//============================================================
//	フォルダ全検索処理
//============================================================
HRESULT CTexture::SearchFolderAll(std::string sFolderPath)
{
	HANDLE hFile;	// 検索ハンドル
	WIN32_FIND_DATA findFileData;	// ファイル情報

	// 引数パスのディレクトリを取得
	std::string sAllLoadPath = sFolderPath + "\\*.*";	// 全読込パス
	hFile = FindFirstFile(sAllLoadPath.c_str(), &findFileData);
	if (INVALID_HANDLE_VALUE == hFile)
	{ // ハンドルが無効の場合

		assert(false);
		return E_FAIL;
	}

	do
	{ // ファイル内の情報全てを読み込む

		// 現在のディレクトリ、親ディレクトリの場合次のループに移行
		if (strcmp(findFileData.cFileName, ".") == 0)	{ continue; }
		if (strcmp(findFileData.cFileName, "..") == 0)	{ continue; }

		// ファイル名を相対パスに変換
		std::string sFullPath = sFolderPath;	// 現在の相対パスを設定
		sFullPath += "\\";						// パス区切り文字を追加
		sFullPath += findFileData.cFileName;	// ファイル名を追加

		if (findFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
		{ // ディレクトリだった場合

			// 新たなディレクトリを全検索
			SearchFolderAll(sFullPath);
		}
		else
		{ // ファイルだった場合

			if (extension::IsLoadOK(m_load, sFullPath.c_str()))
			{ // 読込可能な拡張子だった場合

				// テクスチャを登録
				Regist(sFullPath.c_str());
			}
		}

	} while (FindNextFile(hFile, &findFileData));	// 次のファイルを検索

	// 検索ハンドルを閉じる
	FindClose(hFile);

	return S_OK;
}
