//============================================================
//
//	マルチモデル処理 [multiModel.cpp]
//	Author：藤田勇一
//
//============================================================
//************************************************************
//	インクルードファイル
//************************************************************
#include "multiModel.h"
#include "manager.h"
#include "renderer.h"
#include "texture.h"
#include "renderState.h"

//************************************************************
//	子クラス [CMultiModel] のメンバ関数
//************************************************************
//============================================================
//	コンストラクタ
//============================================================
CMultiModel::CMultiModel(const CObject::ELabel label, const CObject::EDim dimension, const int nPriority) : CObjectModel(label, dimension, nPriority), m_pParent(nullptr)
{

}

//============================================================
//	デストラクタ
//============================================================
CMultiModel::~CMultiModel()
{

}

//============================================================
//	初期化処理
//============================================================
HRESULT CMultiModel::Init()
{
	// メンバ変数を初期化
	m_pParent = nullptr;	// 親モデルへのポインタ

	// オブジェクトモデルの初期化
	if (FAILED(CObjectModel::Init()))
	{ // 初期化に失敗した場合

		assert(false);
		return E_FAIL;
	}

	// 自動描画をOFFにする
	SetEnableDraw(false);

	return S_OK;
}

//============================================================
//	終了処理
//============================================================
void CMultiModel::Uninit()
{
	// オブジェクトモデルの終了
	CObjectModel::Uninit();
}

//============================================================
//	更新処理
//============================================================
void CMultiModel::Update(const float fDeltaTime)
{
	// オブジェクトモデルの更新
	CObjectModel::Update(fDeltaTime);
}

//============================================================
//	描画処理
//============================================================
void CMultiModel::Draw(CShader* pShader)
{
	// オブジェクトモデルの描画
	CObjectModel::Draw(pShader);
}

//============================================================
//	生成処理
//============================================================
CMultiModel* CMultiModel::Create(const VECTOR3& rPos, const VECTOR3& rRot, const VECTOR3& rScale)
{
	// マルチモデルの生成
	CMultiModel* pMultiModel = new CMultiModel;
	if (pMultiModel == nullptr)
	{ // 生成に失敗した場合

		return nullptr;
	}
	else
	{ // 生成に成功した場合

		// マルチモデルの初期化
		if (FAILED(pMultiModel->Init()))
		{ // 初期化に失敗した場合

			// マルチモデルの破棄
			SAFE_DELETE(pMultiModel);
			return nullptr;
		}

		// 位置を設定
		pMultiModel->SetVec3Position(rPos);

		// 向きを設定
		pMultiModel->SetVec3Rotation(rRot);

		// 拡大率を設定
		pMultiModel->SetVec3Scale(rScale);

		// 確保したアドレスを返す
		return pMultiModel;
	}
}

//============================================================
//	親オブジェクトの設定処理
//============================================================
void CMultiModel::SetParentObject(CObject* pObject)
{
	// 引数のオブジェクトポインタを親に設定
	m_pParent = pObject;
}

//============================================================
//	親オブジェクト削除処理
//============================================================
void CMultiModel::DeleteParentObject()
{
	// 親オブジェクトをnullptrにする
	m_pParent = nullptr;
}

//============================================================
//	描画マトリックスの計算処理
//============================================================
void CMultiModel::CalcDrawMatrix()
{
	LPDIRECT3DDEVICE9 pDevice = GET_DEVICE;	// デバイスのポインタ
	MATRIX* pMtxWorld = GetPtrMtxWorld();	// ワールドマトリックス
	MATRIX mtxScale, mtxRot, mtxTrans;		// 計算用マトリックス
	MATRIX mtxParent;						// 親のマトリックス
	VECTOR3 pos = GetVec3Position();		// モデルの位置
	VECTOR3 rot = GetVec3Rotation();		// モデルの向き
	VECTOR3 scale = GetVec3Scale();			// モデルの拡大率

	// ワールドマトリックスの初期化
	pMtxWorld->Identity();

	// 拡大率を反映
	mtxScale.Scaling(scale);
	pMtxWorld->Multiply(*pMtxWorld, mtxScale);

	// 向きを反映
	mtxRot.Rotation(rot);
	pMtxWorld->Multiply(*pMtxWorld, mtxRot);

	// 位置を反映
	mtxTrans.Translation(pos);
	pMtxWorld->Multiply(*pMtxWorld, mtxTrans);

	// 親マトリックスを設定
	if (m_pParent == nullptr)
	{ // 親が存在しない場合

		// 現在のマトリックスを取得
		pDevice->GetTransform(D3DTS_WORLD, &mtxParent);	// 設定された最新のマトリックス (実体のマトリックス)
	}
	else
	{ // 親が存在する場合

		// 親のマトリックスを設定
		mtxParent = *m_pParent->GetPtrMtxWorld();
	}

	// ワールドマトリックスと親マトリックスを掛け合わせる
	pMtxWorld->Multiply(*pMtxWorld, mtxParent);
}
