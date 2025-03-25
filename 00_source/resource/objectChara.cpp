//============================================================
//
//	オブジェクトキャラクター処理 [objectChara.cpp]
//	Author：藤田勇一
//
//============================================================
//************************************************************
//	インクルードファイル
//************************************************************
#include "objectChara.h"
#include "manager.h"
#include "renderer.h"
#include "multiModel.h"
#include "motion.h"

//************************************************************
//	子クラス [CObjectChara] のメンバ関数
//************************************************************
//============================================================
//	コンストラクタ
//============================================================
CObjectChara::CObjectChara(const CObject::ELabel label, const CObject::EDim dimension, const int nPriority) : CObject(label, dimension, nPriority),
	m_pMotion	(nullptr),		// モーションの情報
	m_mtxWorld	(MTX_IDENT),	// ワールドマトリックス
	m_pos		(VEC3_ZERO),	// 位置
	m_rot		(VEC3_ZERO)		// 向き
{
	// パーツ情報配列をクリア
	m_vecParts.clear();
}

//============================================================
//	デストラクタ
//============================================================
CObjectChara::~CObjectChara()
{

}

//============================================================
//	初期化処理
//============================================================
HRESULT CObjectChara::Init()
{
	// メンバ変数を初期化
	m_pMotion	= nullptr;		// モーションの情報
	m_mtxWorld	= MTX_IDENT;	// ワールドマトリックス
	m_pos		= VEC3_ZERO;	// 位置
	m_rot		= VEC3_ZERO;	// 向き

	// パーツ情報配列を初期化
	m_vecParts.clear();

	// モーションの生成
	m_pMotion = CMotion::Create(this);
	if (m_pMotion == nullptr)
	{ // 非使用中の場合

		assert(false);
		return E_FAIL;
	}

	return S_OK;
}

//============================================================
//	終了処理
//============================================================
void CObjectChara::Uninit()
{
	// モーションの破棄
	SAFE_REF_RELEASE(m_pMotion);

	for (auto& rVec : m_vecParts)
	{ // パーツ数分繰り返す

		// パーツの終了
		SAFE_UNINIT(rVec);
	}

	// パーツ情報配列をクリア
	m_vecParts.clear();

	// オブジェクトキャラクターを破棄
	Release();
}

//============================================================
//	更新処理
//============================================================
void CObjectChara::Update(const float fDeltaTime)
{
	if (m_pMotion != nullptr)
	{ // モーションが存在する場合

		// モーションの更新
		m_pMotion->Update(fDeltaTime);
	}
}

//============================================================
//	描画処理
//============================================================
void CObjectChara::Draw(CShader* pShader)
{
	LPDIRECT3DDEVICE9 pDevice = GET_DEVICE;	// デバイスのポインタ

	// 描画マトリックスの計算
	CalcDrawMatrix();

	// ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

	for (auto& rVec : m_vecParts)
	{ // パーツ数分繰り返す

		// パーツの描画
		rVec->Draw(pShader);
	}
}

//============================================================
//	更新状況の設定処理
//============================================================
void CObjectChara::SetEnableUpdate(const bool bUpdate)
{
	// 引数の更新状況を設定
	CObject::SetEnableUpdate(bUpdate);	// 自身
	for (auto& rVec : m_vecParts)
	{ // パーツ数分繰り返す

		rVec->SetEnableUpdate(bUpdate);
	}
}

//============================================================
//	描画状況の設定処理
//============================================================
void CObjectChara::SetEnableDraw(const bool bDraw)
{
	// 引数の描画状況を設定
	CObject::SetEnableDraw(bDraw);	// 自身
	for (auto& rVec : m_vecParts)
	{ // パーツ数分繰り返す

		rVec->SetEnableDraw(bDraw);
	}
}

//============================================================
//	位置の設定処理
//============================================================
void CObjectChara::SetVec3Position(const VECTOR3& rPos)
{
	// 引数の位置を設定
	m_pos = rPos;
}

//============================================================
//	向きの設定処理
//============================================================
void CObjectChara::SetVec3Rotation(const VECTOR3& rRot)
{
	// 引数の向きを設定
	m_rot = rRot;

	// 向きの正規化
	useful::NormalizeRot(m_rot);
}

//============================================================
//	拡大率の設定処理
//============================================================
void CObjectChara::SetVec3Scale(const VECTOR3& rScale)
{
	for (auto& rVec : m_vecParts)
	{ // パーツ数分繰り返す

		if (rVec->GetParentObject() == nullptr)
		{ // 親がいない場合

			// パーツの拡大率を設定
			rVec->SetVec3Scale(rScale);
		}
	}
}

//============================================================
//	生成処理
//============================================================
CObjectChara* CObjectChara::Create(const VECTOR3& rPos, const VECTOR3& rRot)
{
	// オブジェクトキャラクターの生成
	CObjectChara* pObjectChara = new CObjectChara;
	if (pObjectChara == nullptr)
	{ // 生成に失敗した場合

		return nullptr;
	}
	else
	{ // 生成に成功した場合

		// オブジェクトキャラクターの初期化
		if (FAILED(pObjectChara->Init()))
		{ // 初期化に失敗した場合

			// オブジェクトキャラクターの破棄
			SAFE_DELETE(pObjectChara);
			return nullptr;
		}

		// 位置を設定
		pObjectChara->SetVec3Position(rPos);

		// 向きを設定
		pObjectChara->SetVec3Rotation(rRot);

		// 確保したアドレスを返す
		return pObjectChara;
	}
}

//============================================================
//	モーションの設定処理
//============================================================
void CObjectChara::SetMotion(const int nType, const int nBlendFrame)
{
	// モーションの設定
	m_pMotion->Set(nType, nBlendFrame);
}

//============================================================
//	パーツ情報の設定処理
//============================================================
void CObjectChara::SetPartsInfo
(
	const int nPartsIdx,	// パーツインデックス
	const int nParentIdx,	// 親インデックス
	const VECTOR3& rPos,	// 位置
	const VECTOR3& rRot,	// 向き
	const char* pFileName	// ファイル名
)
{
	// インデックスが非正規の場合抜ける
	int nNumParts = GetNumParts();	// パーツの総数
	if (nPartsIdx  <= NONE_IDX     || nPartsIdx  >= nNumParts) { assert(false); return; }
	if (nParentIdx <= NONE_IDX - 1 || nParentIdx >= nNumParts) { assert(false); return; }

	// ファイル指定がない場合抜ける
	if (pFileName == nullptr) { assert(false); return; }

	// モデルの原点位置・向きを設定
	m_pMotion->SetOriginPosition(rPos, nPartsIdx);
	m_pMotion->SetOriginRotation(rRot, nPartsIdx);

	// モデルの生成
	m_vecParts[nPartsIdx] = CMultiModel::Create(rPos, rRot);

	// モデルを割当
	m_vecParts[nPartsIdx]->BindModel(pFileName);

	// 親モデルの設定
	if (nParentIdx == NONE_IDX)
	{ // 親がない場合

		// nullptrを設定
		m_vecParts[nPartsIdx]->SetParentObject(nullptr);
	}
	else
	{ // 親がいる場合

		// 親のアドレスを設定
		m_vecParts[nPartsIdx]->SetParentObject(m_vecParts[nParentIdx]);
	}
}

//============================================================
//	キャラクター情報割当
//============================================================
void CObjectChara::BindCharaData(const char* pCharaPath)
{
	// 割り当てるモーションパスが存在しない場合抜ける
	if (pCharaPath == nullptr) { assert(false); return; }

	CCharacter::SCharaData data = GET_MANAGER->GetCharacter()->Regist(pCharaPath);	// キャラクター情報

	// モーション情報の動的配列のクリア
	m_pMotion->ClearVector();

	// 自身とモーションのパーツ数を設定
	SetNumParts(data.infoParts.GetNumParts());

	// 自身のパーツ情報を全設定
	SetPartsInfoAll(data.infoParts);

	// モーションにパーツ情報を割当
	m_pMotion->BindPartsData(&m_vecParts[0]);

	// モーションにモーション情報の全設定
	m_pMotion->SetAllInfo(data.infoMotion);
}

//============================================================
//	パーツ数の設定処理
//============================================================
void CObjectChara::SetNumParts(const int nNumParts)
{
	//--------------------------------------------------------
	//	現在のパーツを破棄
	//--------------------------------------------------------
	for (auto& rVec : m_vecParts)
	{ // パーツ数分繰り返す

		// パーツの終了
		SAFE_UNINIT(rVec);
	}

	// パーツ情報配列をクリア
	m_vecParts.clear();

	//--------------------------------------------------------
	//	パーツ数を設定
	//--------------------------------------------------------
	// モーションのパーツ数の設定
	m_pMotion->SetNumParts(nNumParts);

	// 自身のパーツ数の設定
	m_vecParts.resize(nNumParts);
}

//============================================================
//	パーツ情報の全設定処理
//============================================================
void CObjectChara::SetPartsInfoAll(CCharacter::SPartsInfo& rInfo)
{
	for (int nCntParts = 0; nCntParts < rInfo.GetNumParts(); nCntParts++)
	{ // パーツ数分繰り返す

		// パーツ情報の設定
		CCharacter::SParts* pParts = &rInfo.vecParts[nCntParts];	// パーツ情報
		SetPartsInfo
		( // 引数
			nCntParts,				// パーツインデックス
			pParts->nParentIdx,		// 親インデックス
			pParts->pos,			// 位置
			pParts->rot,			// 向き
			pParts->sPath.c_str()	// ファイル名
		);
	}
}

//============================================================
//	パーツ位置の設定処理
//============================================================
void CObjectChara::SetPartsPosition(const int nPartsIdx, const VECTOR3& rPos)
{
	if (nPartsIdx < GetNumParts())
	{ // 使用可能なインデックスの場合

		// 引数のインデックスの位置を設定
		m_vecParts[nPartsIdx]->SetVec3Position(rPos);
	}
	else { assert(false); }	// 使用不可
}

//============================================================
//	パーツ向きの設定処理
//============================================================
void CObjectChara::SetPartsRotation(const int nPartsIdx, const VECTOR3& rRot)
{
	if (nPartsIdx < GetNumParts())
	{ // 使用可能なインデックスの場合

		// 引数のインデックスの向きを設定
		m_vecParts[nPartsIdx]->SetVec3Rotation(rRot);
	}
	else { assert(false); }	// 使用不可
}

//============================================================
//	パーツ位置取得処理
//============================================================
VECTOR3 CObjectChara::GetPartsPosition(const int nPartsIdx) const
{
	if (nPartsIdx < GetNumParts())
	{ // 使用可能なインデックスの場合

		// 引数インデックスのパーツの位置を返す
		return m_vecParts[nPartsIdx]->GetVec3Position();
	}

	// インデックスエラー
	assert(false);
	return VEC3_ZERO;
}

//============================================================
//	パーツ向き取得処理
//============================================================
VECTOR3 CObjectChara::GetPartsRotation(const int nPartsIdx) const
{
	if (nPartsIdx < GetNumParts())
	{ // 使用可能なインデックスの場合

		// 引数インデックスのパーツの向きを返す
		return m_vecParts[nPartsIdx]->GetVec3Rotation();
	}

	// インデックスエラー
	assert(false);
	return VEC3_ZERO;
}

//============================================================
//	パーツ取得処理
//============================================================
CMultiModel* CObjectChara::GetParts(const int nPartsIdx) const
{
	if (nPartsIdx < GetNumParts())
	{ // 使用可能なインデックスの場合

		// パーツの情報を返す
		return m_vecParts[nPartsIdx];
	}
	
	// インデックスエラー
	assert(false);
	return nullptr;
}

//============================================================
//	マテリアルの設定処理
//============================================================
void CObjectChara::SetMaterial(const D3DXMATERIAL& rMat, const int nPartsIdx, const int nMatIdx)
{
	if (nPartsIdx < GetNumParts())
	{ // 使用可能なインデックスの場合

		// 引数のマテリアルを設定
		m_vecParts[nPartsIdx]->SetMaterial(rMat, nMatIdx);
	}
	else { assert(false); }	// 使用不可
}

//============================================================
//	マテリアル全設定処理
//============================================================
void CObjectChara::SetAllMaterial(const D3DXMATERIAL& rMat)
{
	for (auto& rVec : m_vecParts)
	{ // パーツ数分繰り返す

		// 引数のマテリアルを全マテリアルに設定
		rVec->SetAllMaterial(rMat);
	}
}

//============================================================
//	マテリアル再設定処理
//============================================================
void CObjectChara::ResetMaterial()
{
	for (auto& rVec : m_vecParts)
	{ // パーツ数分繰り返す

		// 全マテリアルに初期マテリアルを再設定
		rVec->ResetMaterial();
	}
}

//============================================================
//	透明度の設定処理
//============================================================
void CObjectChara::SetAlpha(const float fAlpha)
{
	for (auto& rVec : m_vecParts)
	{ // パーツ数分繰り返す

		// 引数の透明度を設定
		rVec->SetAlpha(fAlpha);
	}
}

//============================================================
//	透明度取得処理
//============================================================
float CObjectChara::GetAlpha() const
{
	// 最も不透明な透明度を探す
	float fAlpha = 0.0f;	// 最も不透明なマテリアルの透明度
	for (auto& rVec : m_vecParts)
	{ // パーツ数分繰り返す

		float fCurAlpha = rVec->GetAlpha();	// 現在のモデルの透明度
		if (fCurAlpha > fAlpha)
		{ // マテリアルの透明度がより不透明だった場合

			// 現在のマテリアルの透明度を保存
			fAlpha = fCurAlpha;
		}
	}

	// 全パーツ内で最も不透明だったマテリアルの透明度を返す
	return fAlpha;
}

//============================================================
//	最大透明度取得処理
//============================================================
float CObjectChara::GetMaxAlpha() const
{
	// 最も不透明な透明度を探す
	float fAlpha = 0.0f;	// 最も不透明なマテリアルの透明度
	for (int nCntParts = 0; nCntParts < GetNumParts(); nCntParts++)
	{ // パーツの最大数分繰り返す

		if (m_vecParts[nCntParts]->GetMaxAlpha() > fAlpha)
		{ // マテリアルの透明度がより不透明だった場合

			// 現在のマテリアルの透明度を保存
			fAlpha = m_vecParts[nCntParts]->GetMaxAlpha();
		}
	}

	// 全パーツ内で最も不透明だったマテリアルの透明度を返す
	return fAlpha;
}

//============================================================
//	描画マトリックスの計算処理
//============================================================
void CObjectChara::CalcDrawMatrix()
{
	MATRIX mtxRot, mtxTrans;	// 計算用マトリックス

	// ワールドマトリックスの初期化
	m_mtxWorld.Identity();

	// 向きを反映
	mtxRot.Rotation(m_rot);
	m_mtxWorld.Multiply(m_mtxWorld, mtxRot);

	// 位置を反映
	mtxTrans.Translation(m_pos);
	m_mtxWorld.Multiply(m_mtxWorld, mtxTrans);
}
