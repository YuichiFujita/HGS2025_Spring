//============================================================
//
//	オブジェクトキャラクターアニメーション3D処理 [objectCharaAnim3D.cpp]
//	Author：藤田勇一
//
//============================================================
//************************************************************
//	インクルードファイル
//************************************************************
#include "objectCharaAnim3D.h"
#include "manager.h"
#include "renderer.h"
#include "characterAnim3D.h"

//************************************************************
//	子クラス [CObjectCharaAnim3D] のメンバ関数
//************************************************************
//============================================================
//	コンストラクタ
//============================================================
CObjectCharaAnim3D::CObjectCharaAnim3D(const CObject::ELabel label, const CObject::EDim dimension, const int nPriority) : CAnim3D(label, dimension, nPriority)
{

}

//============================================================
//	デストラクタ
//============================================================
CObjectCharaAnim3D::~CObjectCharaAnim3D()
{

}

//============================================================
//	初期化処理
//============================================================
HRESULT CObjectCharaAnim3D::Init()
{
	// アニメーション3Dの初期化
	if (FAILED(CAnim3D::Init()))
	{ // 初期化に失敗した場合

		assert(false);
		return E_FAIL;
	}

	return S_OK;
}

//============================================================
//	終了処理
//============================================================
void CObjectCharaAnim3D::Uninit()
{
	// アニメーション3Dの終了
	CAnim3D::Uninit();
}

//============================================================
//	更新処理
//============================================================
void CObjectCharaAnim3D::Update(const float fDeltaTime)
{
	// モーションがない場合抜ける
	if (m_info.vecMotion.empty()) { assert(false); return; }

	// アニメーション3Dの更新
	CAnim3D::Update(fDeltaTime);
}

//============================================================
//	描画処理
//============================================================
void CObjectCharaAnim3D::Draw(CShader* pShader)
{
	// アニメーション3Dの描画
	CAnim3D::Draw(pShader);
}

//============================================================
//	生成処理
//============================================================
CObjectCharaAnim3D* CObjectCharaAnim3D::Create(const VECTOR3& rPos, const VECTOR3& rRot)
{
	// オブジェクトキャラクターアニメーション3Dの生成
	CObjectCharaAnim3D* pObjectCharaAnim3D = new CObjectCharaAnim3D;
	if (pObjectCharaAnim3D == nullptr)
	{ // 生成に失敗した場合

		return nullptr;
	}
	else
	{ // 生成に成功した場合

		// オブジェクトキャラクターアニメーション3Dの初期化
		if (FAILED(pObjectCharaAnim3D->Init()))
		{ // 初期化に失敗した場合

			// オブジェクトキャラクターアニメーション3Dの破棄
			SAFE_DELETE(pObjectCharaAnim3D);
			return nullptr;
		}

		// 位置を設定
		pObjectCharaAnim3D->SetVec3Position(rPos);

		// 向きを設定
		pObjectCharaAnim3D->SetVec3Rotation(rRot);

		// 確保したアドレスを返す
		return pObjectCharaAnim3D;
	}
}

//============================================================
//	キャラクター情報割当
//============================================================
void CObjectCharaAnim3D::BindCharaData(const char* pCharaPath)
{
	// 割り当てるモーションパスが存在しない場合抜ける
	if (pCharaPath == nullptr) { assert(false); return; }

	// モーション情報の全設定
	SetAllInfo(GET_MANAGER->GetCharacterAnim3D()->Regist(pCharaPath));
}

//============================================================
//	モーションの設定処理
//============================================================
void CObjectCharaAnim3D::SetMotion(const int nType)
{
	// 指定されたモーションが存在しない場合抜ける
	if (nType <= NONE_IDX || nType >= m_info.GetNumMotion()) { assert(false); return; }

	// 引数のモーションの種類を設定
	m_info.nType = nType;

	// 変数をポインタ化し簡略化
	SChara* pInfoChara = &m_info.vecMotion[nType].infoChara;	// キャラクター情報

	// 指定テクスチャの割当
	BindTexture(pInfoChara->sPathTexture.c_str());

	// キャラクター情報を設定
	SetTexPtrn(pInfoChara->ptrnTexture);	// テクスチャ分割数
	SetEnableLoop(pInfoChara->bLoop);		// ループON/OFF
	SetVec3Size(pInfoChara->size);			// 大きさ

	assert((int)pInfoChara->vecNextTime.size() == pInfoChara->nMaxPtrn);
	for (int i = 0; i < pInfoChara->nMaxPtrn; i++)
	{
		// パターン変更時間を設定
		SetNextTime(pInfoChara->vecNextTime[i]);
	}

	// キャラクター情報を初期化
	ResetCurPtrn();	// 開始パターン
	ResetNumLoop();	// 繰り返し数
}

//============================================================
//	モーション情報の追加処理
//============================================================
void CObjectCharaAnim3D::AddInfo(const SMotion& rMotion)
{
	int nSetMotionIdx = m_info.GetNumMotion();	// モーションを設定する配列番号

	// 空の要素を最後尾に追加
	m_info.vecMotion.emplace_back();

	// 引数のモーション情報を設定
	m_info.vecMotion[nSetMotionIdx] = rMotion;
}

//============================================================
//	モーション情報全設定処理
//============================================================
void CObjectCharaAnim3D::SetAllInfo(const SInfo& rInfo)
{
	// モーション情報をクリア
	m_info.vecMotion.clear();

	for (auto& rVec : rInfo.vecMotion)
	{ // 読み込んだモーション数分繰り返す

		// モーション情報の追加
		AddInfo(rVec);
	}
}

//============================================================
//	キャンセル取得処理
//============================================================
bool CObjectCharaAnim3D::IsCancel() const
{
	float fCancelTime = m_info.vecMotion[m_info.nType].fCancelTime;	// キャンセル可能時間
	if (fCancelTime >= 0.0f)
	{ // キャンセル可能時間が設定されている場合

		// モーションのキャンセル状況を返す
		return (GetCurWholeTime() >= fCancelTime);
	}

	// キャンセル不可を返す
	return false;
}

//============================================================
//	コンボ取得処理
//============================================================
bool CObjectCharaAnim3D::IsCombo() const
{
	float fComboTime = m_info.vecMotion[m_info.nType].fComboTime;	// コンボ可能時間
	if (fComboTime >= 0.0f)
	{ // コンボ可能時間が設定されている場合

		// モーションのコンボ状況を返す
		return (GetCurWholeTime() >= fComboTime);
	}

	// コンボ不可を返す
	return false;
}

//============================================================
//	原点オフセット反映位置の計算処理
//============================================================
VECTOR3 CObjectCharaAnim3D::CalcOriginOffsetPosition(const VECTOR3& rPos, const VECTOR3& rRot) const
{
	// オフセット反映位置の計算結果を返す
	const VECTOR3& rOffset = m_info.vecMotion[m_info.nType].infoChara.offset;	// オフセット
	return CalcOffsetPosition(rPos, rRot, rOffset);
}

//============================================================
//	判定原点オフセット反映位置の計算処理
//============================================================
VECTOR3 CObjectCharaAnim3D::CalcCollOffsetPosition(const VECTOR3& rPos, const VECTOR3& rRot) const
{
	// オフセット反映位置の計算結果を返す
	const VECTOR3& rOffset = m_info.vecMotion[m_info.nType].infoColl.offset;	// オフセット
	return CalcOffsetPosition(rPos, rRot, rOffset);
}

//============================================================
//	原点オフセット反映位置の取得処理
//============================================================
VECTOR3 CObjectCharaAnim3D::GetOriginOffsetPosition() const
{
	// 原点オフセット反映位置の計算結果を返す
	const VECTOR3& rPos = GetVec3Position();	// 位置
	const VECTOR3& rRot = GetVec3Rotation();	// 向き
	return CalcOriginOffsetPosition(rPos, rRot);
}

//============================================================
//	判定原点オフセット反映位置の取得処理
//============================================================
VECTOR3 CObjectCharaAnim3D::GetCollOffsetPosition() const
{
	// 判定原点オフセット反映位置の計算結果を返す
	const VECTOR3& rPos = GetVec3Position();	// 位置
	const VECTOR3& rRot = GetVec3Rotation();	// 向き
	return CalcCollOffsetPosition(rPos, rRot);
}

//============================================================
//	描画マトリックスの計算処理
//============================================================
void CObjectCharaAnim3D::CalcDrawMatrix()
{
	VECTOR3 offset = m_info.vecMotion[m_info.nType].infoChara.offset;	// オフセット
	MATRIX* pMtxWorld = GetPtrMtxWorld();	// ワールドマトリックス
	MATRIX mtxRot, mtxTrans, mtxOffset;		// 計算用マトリックス
	VECTOR3 pos = GetVec3Position();		// 位置
	VECTOR3 rot = GetVec3Rotation();		// 向き

	// ワールドマトリックスの初期化
	pMtxWorld->Identity();

	// オフセットを反映
	mtxOffset.Translation(offset);
	pMtxWorld->Multiply(*pMtxWorld, mtxOffset);

	// 向きを反映
	mtxRot.Rotation(rot);
	pMtxWorld->Multiply(*pMtxWorld, mtxRot);

	// 位置を反映
	mtxTrans.Translation(pos);
	pMtxWorld->Multiply(*pMtxWorld, mtxTrans);
}

//============================================================
//	オフセット反映位置の計算処理
//============================================================
VECTOR3 CObjectCharaAnim3D::CalcOffsetPosition
(
	const VECTOR3& rPos,	// 位置
	const VECTOR3& rRot,	// 向き
	const VECTOR3& rOffset	// オフセット
) const
{
	MATRIX mtxWorld, mtxRot, mtxTrans, mtxOffset;	// 計算用マトリックス

	// ワールドマトリックスの初期化
	mtxWorld.Identity();

	// オフセットを反映
	mtxOffset.Translation(rOffset);
	mtxWorld.Multiply(mtxWorld, mtxOffset);

	// 向きを反映
	mtxRot.Rotation(rRot);
	mtxWorld.Multiply(mtxWorld, mtxRot);

	// 位置を反映
	mtxTrans.Translation(rPos);
	mtxWorld.Multiply(mtxWorld, mtxTrans);

	// 算出したマトリックスの位置を返す
	return mtxWorld.GetPosition();
}
