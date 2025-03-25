//============================================================
//
//	モデルヘッダー [model.h]
//	Author：藤田勇一
//
//============================================================
//************************************************************
//	二重インクルード防止
//************************************************************
#ifndef _MODEL_H_
#define _MODEL_H_

//************************************************************
//	インクルードファイル
//************************************************************
#include "extension.h"

//************************************************************
//	クラス定義
//************************************************************
// モデルクラス
class CModel
{
public:
	// コンストラクタ
	CModel();

	// デストラクタ
	~CModel();

	// モデル構造体
	struct SModel
	{
		// デフォルトコンストラクタ
		SModel() :
			pMesh		(nullptr),				// メッシュ (頂点情報) へのポインタ
			pBuffMat	(nullptr),				// マテリアルへのポインタ
			dwNumMat	(0),					// マテリアルの数
			vtxMin		(VECTOR3( 9999.0f)),	// 最小の頂点座標
			vtxMax		(VECTOR3(-9999.0f)),	// 最大の頂点座標
			size		(VEC3_ZERO),			// 大きさ
			fRadius		(0.0f),					// 半径
			pTextureIdx	(nullptr)				// テクスチャインデックス
		{}

		// デストラクタ
		~SModel() {}

		// メンバ変数
		LPD3DXMESH pMesh;		// メッシュ (頂点情報) へのポインタ
		LPD3DXBUFFER pBuffMat;	// マテリアルへのポインタ
		DWORD dwNumMat;			// マテリアルの数
		VECTOR3 vtxMin;			// 最小の頂点座標
		VECTOR3 vtxMax;			// 最大の頂点座標
		VECTOR3 size;			// 大きさ
		float fRadius;			// 半径
		int* pTextureIdx;		// テクスチャインデックス
	};

	// マップ情報構造体
	struct SMapInfo
	{
		// デフォルトコンストラクタ
		SMapInfo() :
			modelData		({}),	// モデル情報
			sFilePathName	("")	// ファイルパス名
		{}

		// デストラクタ
		~SMapInfo() {}

		// メンバ変数
		SModel modelData;			// モデル情報
		std::string sFilePathName;	// ファイルパス名
	};

	// メンバ関数
	HRESULT Init();		// モデル初期化
	void Uninit();		// モデル終了
	HRESULT LoadAll();	// モデル全読込
	int Regist(std::string sFilePath);	// モデル登録
	SModel* GetInfo(const int nIdx);	// モデル情報取得

	// 静的メンバ関数
	static CModel* Create();				// 生成
	static void Release(CModel* &pModel);	// 破棄

private:
	// メンバ関数
	HRESULT LoadXFileModel(SMapInfo* pMapInfo, std::string sFilePath);	// xファイルの読込
	HRESULT LoadTextureModel(SMapInfo* pMapInfo);		// テクスチャの読込
	HRESULT SetCollisionModel(SMapInfo* pMapInfo);		// 当たり判定の作成
	HRESULT SearchFolderAll(std::string sFolderPath);	// フォルダ全検索

	// メンバ変数
	std::map<int, SMapInfo> m_mapModel;	// モデル連想配列
	extension::ALoad m_load;			// 読み込み可能拡張子
};

#endif	// _MODEL_H_
