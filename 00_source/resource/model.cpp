//============================================================
//
//	モデル処理 [model.cpp]
//	Author：藤田勇一
//
//============================================================
//************************************************************
//	インクルードファイル
//************************************************************
#include "model.h"
#include "manager.h"
#include "renderer.h"
#include "texture.h"

//************************************************************
//	定数宣言
//************************************************************
namespace
{
	const char* LOAD_EXTENSION = "data\\TXT\\EXTENSION\\model.txt";	// モデル読込拡張子相対パス
	const char* LOAD_FOLDER = "data\\MODEL";	// モデルフォルダ相対パス
}

//************************************************************
//	親クラス [CModel] のメンバ関数
//************************************************************
//============================================================
//	コンストラクタ
//============================================================
CModel::CModel()
{
	// モデル連想配列をクリア
	m_mapModel.clear();

	// 読み込み可能拡張子をクリア
	m_load.clear();
}

//============================================================
//	デストラクタ
//============================================================
CModel::~CModel()
{

}

//============================================================
//	モデル初期化処理
//============================================================
HRESULT CModel::Init()
{
	// モデル連想配列を初期化
	m_mapModel.clear();

	// 読み込み可能拡張子を初期化
	m_load.clear();

	// 読み込み可能拡張子の読込
	m_load = extension::LoadExtension(LOAD_EXTENSION);

	return S_OK;
}

//============================================================
//	モデル終了処理
//============================================================
void CModel::Uninit()
{
	for (auto& rMap : m_mapModel)
	{ // モデルの要素数分繰り返す

		// テクスチャインデックスの破棄
		SAFE_FREE(rMap.second.modelData.pTextureIdx);

		// メッシュの破棄
		SAFE_RELEASE(rMap.second.modelData.pMesh);

		// マテリアルの破棄
		SAFE_RELEASE(rMap.second.modelData.pBuffMat);
	}

	// モデル連想配列をクリア
	m_mapModel.clear();

	// 読み込み可能拡張子をクリア
	m_load.clear();
}

//============================================================
//	モデル全読込処理
//============================================================
HRESULT CModel::LoadAll()
{
	// モデルの全読込
	if (FAILED(SearchFolderAll(LOAD_FOLDER)))
	{ // 読込に失敗した場合

		assert(false);
		return E_FAIL;
	}

	return S_OK;
}

//============================================================
//	モデル登録処理
//============================================================
int CModel::Regist(std::string sFilePath)
{
	int nIdx = (int)m_mapModel.size();	// モデル読込番号
	SMapInfo tempMapInfo;	// マップ情報

	// ファイルパスを標準化
	useful::StandardizePathPart(&sFilePath);

	// 既に読み込んでいないかを確認
	int nCntModel = 0;
	for (const auto& rMap : m_mapModel)
	{ // モデルの要素数分繰り返す

		if (rMap.second.sFilePathName.compare(sFilePath) == 0)
		{ // 文字列が一致した場合

			// すでに読み込んでいるモデルの配列番号を返す
			return nCntModel;
		}

		// 次のモデル番号にする
		nCntModel++;
	}

	// xファイルの読込
	if (FAILED(LoadXFileModel(&tempMapInfo, sFilePath)))
	{ // xファイルの読込に失敗した場合

		assert(false);
		return NONE_IDX;
	}

	// テクスチャの読込
	if (FAILED(LoadTextureModel(&tempMapInfo)))
	{ // テクスチャの読込に失敗した場合

		assert(false);
		return NONE_IDX;
	}

	// 当たり判定の作成
	if (FAILED(SetCollisionModel(&tempMapInfo)))
	{ // 当たり判定の作成に失敗した場合

		assert(false);
		return NONE_IDX;
	}

	// ファイルパス名を保存
	tempMapInfo.sFilePathName = sFilePath;

	// モデル情報を生成
	m_mapModel.insert(std::make_pair(nIdx, tempMapInfo));

	// 読み込んだモデルの配列番号を返す
	return nIdx;
}

//============================================================
//	モデル情報取得処理
//============================================================
CModel::SModel* CModel::GetInfo(const int nIdx)
{
	if (nIdx > NONE_IDX && nIdx < (int)m_mapModel.size())
	{ // モデルがある場合

		// 引数のモデル情報を返す
		return &m_mapModel.find(nIdx)->second.modelData;
	}
	else
	{ // モデルがない場合

		// nullptrを返す
		assert(false);
		return nullptr;
	}
}

//============================================================
//	生成処理
//============================================================
CModel* CModel::Create()
{
	// モデルの生成
	CModel* pModel = new CModel;
	if (pModel == nullptr)
	{ // 生成に失敗した場合

		return nullptr;
	}
	else
	{ // 生成に成功した場合

		// モデルの初期化
		if (FAILED(pModel->Init()))
		{ // モデル初期化に失敗した場合

			// モデルの破棄
			SAFE_DELETE(pModel);
			return nullptr;
		}

		// 確保したアドレスを返す
		return pModel;
	}
}

//============================================================
//	破棄処理
//============================================================
void CModel::Release(CModel*& prModel)
{
	// モデルの終了
	assert(prModel != nullptr);
	prModel->Uninit();

	// メモリ開放
	SAFE_DELETE(prModel);
}

//============================================================
//	xファイルの読み込み
//============================================================
HRESULT CModel::LoadXFileModel(SMapInfo* pMapInfo, std::string sFilePath)
{
	// マップ情報の指定がない場合エラー
	if (pMapInfo == nullptr) { return E_FAIL; }

	// xファイルの読込
	HRESULT hr;
	hr = D3DXLoadMeshFromX
	( // 引数
		sFilePath.c_str(),				// モデルの相対パス
		D3DXMESH_SYSTEMMEM,				// メッシュ作成用オプション
		GET_DEVICE,						// デバイスへのポインタ
		nullptr,						// 隣接性データ
		&pMapInfo->modelData.pBuffMat,	// マテリアルへのポインタ
		nullptr,						// エフェクトデータ
		&pMapInfo->modelData.dwNumMat,	// マテリアルの数
		&pMapInfo->modelData.pMesh		// メッシュ (頂点情報) へのポインタ
	);
	if (FAILED(hr))
	{ // xファイルの読込に失敗した場合

		// エラーメッセージの作成
		std::string sError = "xファイルの読込に失敗！\nパス：";
		sError.append(sFilePath.c_str());

		// エラーメッセージボックス
		MessageBox(nullptr, sError.c_str(), "警告！", MB_ICONWARNING);
		return E_FAIL;
	}

	// マテリアル数分メモリ確保
	pMapInfo->modelData.pTextureIdx = (int*)malloc(pMapInfo->modelData.dwNumMat * sizeof(int*));
	if (pMapInfo->modelData.pTextureIdx == nullptr)
	{ // 動的確保に失敗した場合

		// エラーメッセージの作成
		std::string sError = "マテリアルのメモリ確保に失敗！\nパス：";
		sError.append(sFilePath.c_str());

		// エラーメッセージボックス
		MessageBox(nullptr, sError.c_str(), "警告！", MB_ICONWARNING);
		return E_FAIL;
	}

	return S_OK;
}

//============================================================
//	テクスチャの読み込み
//============================================================
HRESULT CModel::LoadTextureModel(SMapInfo* pMapInfo)
{
	// マップ情報の指定がない場合エラー
	if (pMapInfo == nullptr) { return E_FAIL; }

	CTexture* pTexture = GET_MANAGER->GetTexture();	// テクスチャへのポインタ
	D3DXMATERIAL* pMat;	// マテリアルへのポインタ

	// マテリアル情報に対するポインタを取得
	pMat = (D3DXMATERIAL*)pMapInfo->modelData.pBuffMat->GetBufferPointer();

	for (int nCntMat = 0; nCntMat < (int)pMapInfo->modelData.dwNumMat; nCntMat++)
	{ // マテリアルの数分繰り返す

		if (pMat[nCntMat].pTextureFilename != nullptr)
		{ // テクスチャファイルが存在する場合

			// テクスチャを登録
			pMapInfo->modelData.pTextureIdx[nCntMat] = pTexture->Regist(pMat[nCntMat].pTextureFilename);
		}
		else
		{ // テクスチャファイルが存在しない場合

			// テクスチャを登録
			pMapInfo->modelData.pTextureIdx[nCntMat] = NONE_IDX;	// テクスチャなし
		}
	}

	return S_OK;
}

//============================================================
//	当たり判定の作成
//============================================================
HRESULT CModel::SetCollisionModel(SMapInfo* pMapInfo)
{
	// マップ情報の指定がない場合エラー
	if (pMapInfo == nullptr) { return E_FAIL; }

	BYTE*	pVtxBuff;	// モデルの頂点バッファへのポインタ
	DWORD	dwSizeFVF;	// モデルの頂点フォーマットのサイズ
	VECTOR3	vtx;		// モデルの頂点座標
	int		nNumVtx;	// モデルの頂点数

	// モデルの頂点数を取得
	nNumVtx = pMapInfo->modelData.pMesh->GetNumVertices();

	// モデルの頂点フォーマットのサイズを取得
	dwSizeFVF = D3DXGetFVFVertexSize(pMapInfo->modelData.pMesh->GetFVF());

	// モデルの頂点バッファをロック
	pMapInfo->modelData.pMesh->LockVertexBuffer(D3DLOCK_READONLY, (void**)&pVtxBuff);

	for (int nCntVtx = 0; nCntVtx < nNumVtx; nCntVtx++)
	{ // モデルの頂点数分繰り返す

		// モデルの頂点座標を代入
		vtx = *(VECTOR3*)pVtxBuff;

		// x頂点座標の設定
		if (vtx.x < pMapInfo->modelData.vtxMin.x)
		{ // 現状の x頂点座標よりも小さい場合

			// x頂点情報を代入
			pMapInfo->modelData.vtxMin.x = vtx.x;
		}
		else if (vtx.x > pMapInfo->modelData.vtxMax.x)
		{ // 現状の x頂点座標よりも大きい場合

			// x頂点情報を代入
			pMapInfo->modelData.vtxMax.x = vtx.x;
		}

		// y頂点座標の設定
		if (vtx.y < pMapInfo->modelData.vtxMin.y)
		{ // 現状の y頂点座標よりも小さい場合

			// y頂点情報を代入
			pMapInfo->modelData.vtxMin.y = vtx.y;
		}
		else if (vtx.y > pMapInfo->modelData.vtxMax.y)
		{ // 現状の y頂点座標よりも大きい場合

			// y頂点情報を代入
			pMapInfo->modelData.vtxMax.y = vtx.y;
		}

		// z頂点座標の設定
		if (vtx.z < pMapInfo->modelData.vtxMin.z)
		{ // 現状の z頂点座標よりも小さい場合

			// z頂点情報を代入
			pMapInfo->modelData.vtxMin.z = vtx.z;
		}
		else if (vtx.z > pMapInfo->modelData.vtxMax.z)
		{ // 現状の z頂点座標よりも大きい場合

			// z頂点情報を代入
			pMapInfo->modelData.vtxMax.z = vtx.z;
		}

		// 頂点フォーマットのサイズ分ポインタを進める
		pVtxBuff += dwSizeFVF;
	}

	// モデルの頂点バッファをアンロック
	pMapInfo->modelData.pMesh->UnlockVertexBuffer();

	// モデルサイズを求める
	pMapInfo->modelData.size = pMapInfo->modelData.vtxMax - pMapInfo->modelData.vtxMin;

	// モデルの円の当たり判定を作成
	pMapInfo->modelData.fRadius = ((pMapInfo->modelData.size.x * 0.5f) + (pMapInfo->modelData.size.z * 0.5f)) * 0.5f;

	return S_OK;
}

//============================================================
//	フォルダ全検索処理
//============================================================
HRESULT CModel::SearchFolderAll(std::string sFolderPath)
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

				// モデルを登録
				Regist(sFullPath.c_str());
			}
		}

	} while (FindNextFile(hFile, &findFileData));	// 次のファイルを検索

	// 検索ハンドルを閉じる
	FindClose(hFile);

	return S_OK;
}
