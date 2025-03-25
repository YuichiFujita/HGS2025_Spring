//============================================================
//
//	オブジェクト3D処理 [object3D.cpp]
//	Author：藤田勇一
//
//============================================================
//************************************************************
//	インクルードファイル
//************************************************************
#include "object3D.h"
#include "manager.h"
#include "renderer.h"
#include "texture.h"
#include "collision.h"

//************************************************************
//	定数宣言
//************************************************************
namespace
{
	const int MAX_VERTEX = 4;	// 頂点数
}

//************************************************************
//	子クラス [CObject3D] のメンバ関数
//************************************************************
//============================================================
//	コンストラクタ
//============================================================
CObject3D::CObject3D(const CObject::ELabel label, const CObject::EDim dimension, const int nPriority) : CObject(label, dimension, nPriority),
	m_pVtxBuff		(nullptr),			// 頂点バッファへのポインタ
	m_pRenderState	(nullptr),			// レンダーステートの情報
	m_pPosGapBuff	(nullptr),			// 座標のずれバッファ
	m_mtxWorld		(MTX_IDENT),		// ワールドマトリックス
	m_pos			(VEC3_ZERO),		// 位置
	m_rot			(VEC3_ZERO),		// 向き
	m_size			(VEC3_ZERO),		// 大きさ
	m_col			(color::White()),	// 色
	m_origin		(ORIGIN_CENTER),	// 原点
	m_nTextureIdx	(0)					// テクスチャインデックス
{

}

//============================================================
//	デストラクタ
//============================================================
CObject3D::~CObject3D()
{

}

//============================================================
//	初期化処理
//============================================================
HRESULT CObject3D::Init()
{
	LPDIRECT3DDEVICE9 pDevice = GET_DEVICE;	// デバイスのポインタ

	// メンバ変数を初期化
	m_pVtxBuff		= nullptr;			// 頂点バッファへのポインタ
	m_pRenderState	= nullptr;			// レンダーステートの情報
	m_pPosGapBuff	= nullptr;			// 座標のずれバッファ
	m_mtxWorld		= MTX_IDENT;		// ワールドマトリックス
	m_pos			= VEC3_ZERO;		// 位置
	m_rot			= VEC3_ZERO;		// 向き
	m_size			= VEC3_ZERO;		// 大きさ
	m_col			= color::White();	// 色
	m_origin		= ORIGIN_CENTER;	// 原点
	m_nTextureIdx	= NONE_IDX;			// テクスチャインデックス

	// 頂点バッファの生成
	if (FAILED(pDevice->CreateVertexBuffer
	( // 引数
		sizeof(VERTEX_3D) * MAX_VERTEX,	// 必要頂点数
		D3DUSAGE_WRITEONLY,				// 使用方法
		CObject::FVF_VERTEX_3D,			// 頂点フォーマット
		D3DPOOL_MANAGED,				// メモリの指定
		&m_pVtxBuff,					// 頂点バッファへのポインタ
		nullptr
	)))
	{ // 頂点バッファの生成に失敗した場合

		assert(false);
		return E_FAIL;
	}

	// レンダーステートの生成
	m_pRenderState = CRenderState::Create();
	if (m_pRenderState == nullptr)
	{ // 生成に失敗した場合

		assert(false);
		return E_FAIL;
	}

	// 座標のずれバッファのメモリ確保
	m_pPosGapBuff = new VECTOR3[MAX_VERTEX];
	if (m_pPosGapBuff != nullptr)
	{ // 確保に成功した場合

		// メモリクリア
		memset(m_pPosGapBuff, 0, sizeof(VECTOR3) * MAX_VERTEX);
	}
	else
	{ // 生成に失敗した場合

		assert(false);
		return E_FAIL;
	}

	// 頂点情報の設定
	SetVtx();

	return S_OK;
}

//============================================================
//	終了処理
//============================================================
void CObject3D::Uninit()
{
	// 頂点バッファの破棄
	SAFE_RELEASE(m_pVtxBuff);

	// レンダーステートの破棄
	SAFE_REF_RELEASE(m_pRenderState);

	// 座標のずれバッファの破棄
	SAFE_DEL_ARRAY(m_pPosGapBuff);

	// オブジェクト3Dを破棄
	Release();
}

//============================================================
//	更新処理
//============================================================
void CObject3D::Update(const float fDeltaTime)
{

}

//============================================================
//	描画処理
//============================================================
void CObject3D::Draw(CShader* pShader)
{
	LPDIRECT3DDEVICE9 pDevice = GET_DEVICE;	// デバイスのポインタ

	// レンダーステートを設定
	m_pRenderState->Set();

	// 描画マトリックスの計算
	CalcDrawMatrix();

	// ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

	// 頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_3D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(CObject::FVF_VERTEX_3D);

	if (pShader == nullptr)
	{ // シェーダーが使用されていない場合

		// 通常描画
		DrawNormal();
	}
	else
	{ // シェーダーが使用されている場合

		// シェーダー描画
		DrawShader(pShader);
	}

	// レンダーステートを再設定
	m_pRenderState->Reset();
}

//============================================================
//	生成処理
//============================================================
CObject3D* CObject3D::Create
(
	const VECTOR3& rPos,	// 位置
	const VECTOR3& rSize,	// 大きさ
	const VECTOR3& rRot,	// 向き
	const COLOR& rCol,		// 色
	const EOrigin origin	// 原点
)
{
	// オブジェクト3Dの生成
	CObject3D* pObject3D = new CObject3D;
	if (pObject3D == nullptr)
	{ // 生成に失敗した場合

		return nullptr;
	}
	else
	{ // 生成に成功した場合

		// オブジェクト3Dの初期化
		if (FAILED(pObject3D->Init()))
		{ // 初期化に失敗した場合

			// オブジェクト3Dの破棄
			SAFE_DELETE(pObject3D);
			return nullptr;
		}

		// 原点を設定
		pObject3D->SetOrigin(origin);

		// 位置を設定
		pObject3D->SetVec3Position(rPos);

		// 向きを設定
		pObject3D->SetVec3Rotation(rRot);

		// 大きさを設定
		pObject3D->SetVec3Size(rSize);

		// 色を設定
		pObject3D->SetColor(rCol);

		// 確保したアドレスを返す
		return pObject3D;
	}
}

//============================================================
//	位置の設定処理
//============================================================
void CObject3D::SetVec3Position(const VECTOR3& rPos)
{
	// 引数の位置を設定
	m_pos = rPos;

	// 頂点情報の設定
	SetVtx();
}

//============================================================
//	向きの設定処理
//============================================================
void CObject3D::SetVec3Rotation(const VECTOR3& rRot)
{
	// 引数の向きを設定
	m_rot = rRot;

	// 向きの正規化
	useful::NormalizeRot(m_rot);

	// 頂点情報の設定
	SetVtx();
}

//============================================================
//	大きさの設定処理
//============================================================
void CObject3D::SetVec3Size(const VECTOR3& rSize)
{
	// 引数の大きさを代入
	m_size = rSize;

	// 頂点情報の設定
	SetVtx();
}

//============================================================
//	レンダーステート情報の取得処理
//============================================================
CRenderState* CObject3D::GetRenderState()
{
	// インスタンス未使用
	assert(m_pRenderState != nullptr);

	// レンダーステートの情報を返す
	return m_pRenderState;
}

//============================================================
//	テクスチャ割当処理 (インデックス)
//============================================================
void CObject3D::BindTexture(const int nTextureIdx)
{
	if (nTextureIdx >= NONE_IDX)
	{ // テクスチャインデックスが使用可能な場合

		// テクスチャインデックスを代入
		m_nTextureIdx = nTextureIdx;
	}
	else { assert(false); }	// 範囲外
}

//============================================================
//	テクスチャ割当処理 (パス)
//============================================================
void CObject3D::BindTexture(const char* pTexturePath)
{
	if (pTexturePath != nullptr)
	{ // 割り当てるテクスチャパスがある場合

		// テクスチャインデックスを設定
		CTexture* pTexture = GET_MANAGER->GetTexture();	// テクスチャへのポインタ
		m_nTextureIdx = pTexture->Regist(pTexturePath);
	}
	else
	{ // 割り当てるテクスチャパスがない場合

		// テクスチャなしインデックスを設定
		m_nTextureIdx = NONE_IDX;
	}
}

//============================================================
//	透明度の設定処理
//============================================================
void CObject3D::SetAlpha(const float fAlpha)
{
	// 引数の透明度を代入
	m_col.a = fAlpha;

	// 色の設定
	SetColor(m_col);
}

//============================================================
//	色の設定処理
//============================================================
void CObject3D::SetColor(const COLOR& rCol)
{
	// 引数の色を代入
	m_col = rCol;

	// 頂点情報の設定
	SetVtx();
}

//============================================================
//	原点の設定処理
//============================================================
void CObject3D::SetOrigin(const EOrigin origin)
{
	// 引数の原点を設定
	m_origin = origin;

	// 頂点情報の設定
	SetVtx();
}

//============================================================
//	頂点位置の設定処理
//============================================================
void CObject3D::SetVertexPosition(const int nIdx, const VECTOR3& rPos)
{
	VERTEX_3D* pVtx;	// 頂点情報へのポインタ
	if (m_pVtxBuff != nullptr)
	{ // 使用中の場合

		if (nIdx < MAX_VERTEX)
		{ // インデックスが使用可能な場合

			// 頂点バッファをロックし、頂点情報へのポインタを取得
			m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

			// 頂点データのポインタを引数分進める
			pVtx += nIdx;

			// 頂点座標の設定
			pVtx[0].pos = rPos;

			// 頂点バッファをアンロックする
			m_pVtxBuff->Unlock();
		}
	}

	// 法線の設定・正規化
	NormalizeNormal();
}

//============================================================
//	頂点位置取得処理
//============================================================
VECTOR3 CObject3D::GetVertexPosition(const int nIdx)
{
	VERTEX_3D* pVtx;	// 頂点情報へのポインタ
	VECTOR3 pos;		// 頂点座標の代入用
	if (m_pVtxBuff != nullptr)
	{ // 使用中の場合

		if (nIdx < MAX_VERTEX)
		{ // インデックスが使用可能な場合

			// 頂点バッファをロックし、頂点情報へのポインタを取得
			m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

			// 頂点データのポインタを引数分進める
			pVtx += nIdx;

			// 頂点座標を代入
			pos = pVtx[0].pos;

			// 頂点バッファをアンロックする
			m_pVtxBuff->Unlock();
		}
	}

	// 引数のインデックスの頂点座標を返す
	return pos;
}

//============================================================
//	座標のずれの設定処理
//============================================================
void CObject3D::SetGapPosition(const int nIdx, const VECTOR3& rPos)
{
	if (m_pPosGapBuff != nullptr)
	{ // 使用中の場合

		if (nIdx < MAX_VERTEX)
		{ // インデックスが使用可能な場合

			// 頂点のずれを設定
			m_pPosGapBuff[nIdx] = rPos;

			// 頂点情報の設定
			SetVtx();
		}
		else { assert(false); }	// 使用不可
	}
}

//============================================================
//	座標のずれ取得処理
//============================================================
VECTOR3 CObject3D::GetGapPosition(const int nIdx)
{
	VECTOR3 pos = VEC3_ZERO;	// 頂点のずれの代入用
	if (m_pPosGapBuff != nullptr)
	{ // 使用中の場合

		if (nIdx < MAX_VERTEX)
		{ // インデックスが使用可能な場合

			// 頂点のずれを設定
			pos = m_pPosGapBuff[nIdx];
		}
		else { assert(false); }	// 使用不可
	}

	// 引数のインデックスの頂点のずれを返す
	return pos;
}

//============================================================
//	ポリゴンの着地取得処理
//============================================================
float CObject3D::GetPositionHeight(const VECTOR3& rPos)
{
	VECTOR3 aVtxPos[MAX_VERTEX];	// ポリゴンの頂点座標 [※] 0：左上 1：右上 2：左下 3：右下
	VECTOR3 nor;	// 法線ベクトル
	for (int nCnt = 0; nCnt < MAX_VERTEX; nCnt++)
	{ // 頂点数分繰り返す

		MATRIX mtxWorld, mtxRot, mtxTrans;		// 計算用マトリックス
		VECTOR3 pos = GetVertexPosition(nCnt);	// 頂点座標
		
		// ワールドマトリックスの初期化
		mtxWorld.Identity();

		// 頂点位置を反映
		mtxTrans.Translation(pos);
		mtxWorld.Multiply(mtxWorld, mtxTrans);

		// ポリゴン向きを反映
		mtxRot.Rotation(m_rot);
		mtxWorld.Multiply(mtxWorld, mtxRot);

		// ポリゴン位置を反映
		mtxTrans.Translation(m_pos);
		mtxWorld.Multiply(mtxWorld, mtxTrans);

		// 計算したマトリックスから座標を設定
		aVtxPos[nCnt] = mtxWorld.GetPosition();
	}

	if (collision::TriangleOuterPillar(aVtxPos[0], aVtxPos[1], aVtxPos[2], rPos))
	{ // ポリゴンの範囲内にいる場合

		// 法線を求める
		useful::NormalizeNormal(aVtxPos[1], aVtxPos[0], aVtxPos[2], nor);

		if (nor.y != 0.0f)
		{ // 法線が設定されている場合

			// プレイヤーの着地点を返す
			return (((rPos.x - aVtxPos[0].x) * nor.x + (-aVtxPos[0].y) * nor.y + (rPos.z - aVtxPos[0].z) * nor.z) * -1.0f) / nor.y;
		}
	}

	if (collision::TriangleOuterPillar(aVtxPos[3], aVtxPos[1], aVtxPos[2], rPos))
	{ // ポリゴンの範囲内にいる場合

		// 法線を求める
		useful::NormalizeNormal(aVtxPos[2], aVtxPos[3], aVtxPos[1], nor);

		if (nor.y != 0.0f)
		{ // 法線が設定されている場合

			// プレイヤーの着地点を返す
			return (((rPos.x - aVtxPos[3].x) * nor.x + (-aVtxPos[3].y) * nor.y + (rPos.z - aVtxPos[3].z) * nor.z) * -1.0f) / nor.y;
		}
	}

	// 着地範囲外の場合現在のy座標を返す
	return rPos.y;
}

//============================================================
//	描画マトリックスの計算処理
//============================================================
void CObject3D::CalcDrawMatrix()
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

//============================================================
//	頂点情報の設定処理
//============================================================
void CObject3D::SetVtx()
{
	VERTEX_3D* pVtx;	// 頂点情報へのポインタ

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	switch (m_origin)
	{ // 原点ごとの処理
	case ORIGIN_CENTER:	// 中央

		// 頂点座標の設定
		pVtx[0].pos = VECTOR3(-m_size.x * 0.5f,  m_size.y * 0.5f,  m_size.z * 0.5f);
		pVtx[1].pos = VECTOR3( m_size.x * 0.5f,  m_size.y * 0.5f,  m_size.z * 0.5f);
		pVtx[2].pos = VECTOR3(-m_size.x * 0.5f, -m_size.y * 0.5f, -m_size.z * 0.5f);
		pVtx[3].pos = VECTOR3( m_size.x * 0.5f, -m_size.y * 0.5f, -m_size.z * 0.5f);

		break;

	case ORIGIN_DOWN:	// 下

		// 頂点座標の設定
		pVtx[0].pos = VECTOR3(-m_size.x * 0.5f, m_size.y,  m_size.z * 0.5f);
		pVtx[1].pos = VECTOR3( m_size.x * 0.5f, m_size.y,  m_size.z * 0.5f);
		pVtx[2].pos = VECTOR3(-m_size.x * 0.5f,     0.0f, -m_size.z * 0.5f);
		pVtx[3].pos = VECTOR3( m_size.x * 0.5f,     0.0f, -m_size.z * 0.5f);

		break;

	default:	// 例外処理
		assert(false);
		break;
	}

	// 頂点からのずれ量を加算
	for (int nCntVtx = 0; nCntVtx < MAX_VERTEX; nCntVtx++)
	{ // 頂点数分繰り返す

		pVtx[nCntVtx].pos += m_pPosGapBuff[nCntVtx];
	}

	// 法線の設定・正規化
	NormalizeNormal();

	// 頂点カラーの設定
	pVtx[0].col = m_col;
	pVtx[1].col = m_col;
	pVtx[2].col = m_col;
	pVtx[3].col = m_col;

	// テクスチャ座標の設定
	pVtx[0].tex = VECTOR2(0.0f, 0.0f);
	pVtx[1].tex = VECTOR2(1.0f, 0.0f);
	pVtx[2].tex = VECTOR2(0.0f, 1.0f);
	pVtx[3].tex = VECTOR2(1.0f, 1.0f);

	// 頂点バッファをアンロックする
	m_pVtxBuff->Unlock();
}

//============================================================
//	アニメーションのテクスチャ座標の設定処理
//============================================================
void CObject3D::SetAnimTex
(
	const int nPattern,		// アニメーションパターン
	const int nWidthPtrn,	// テクスチャの横の分割数
	const int nHeightPtrn	// テクスチャの縦の分割数
)
{
	VERTEX_3D* pVtx;	// 頂点情報へのポインタ
	float fWidthRate	= 1.0f / nWidthPtrn;	// 横の分割数の割合
	float fHeightRate	= 1.0f / nHeightPtrn;	// 縦の分割数の割合
	int nWidthCurrent	= nPattern % nWidthPtrn;					// 現在の横のパターン
	int nHeightCurrent	= (nPattern / nWidthPtrn) % nHeightPtrn;	// 現在の縦のパターン
	if (m_pVtxBuff != nullptr)
	{ // 使用中の場合

		// 頂点バッファをロックし、頂点情報へのポインタを取得
		m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

		// テクスチャ座標の設定
		pVtx[0].tex = VECTOR2(fWidthRate *  nWidthCurrent,		fHeightRate *  nHeightCurrent);
		pVtx[1].tex = VECTOR2(fWidthRate * (nWidthCurrent + 1),	fHeightRate *  nHeightCurrent);
		pVtx[2].tex = VECTOR2(fWidthRate *  nWidthCurrent,		fHeightRate * (nHeightCurrent + 1));
		pVtx[3].tex = VECTOR2(fWidthRate * (nWidthCurrent + 1),	fHeightRate * (nHeightCurrent + 1));

		// 頂点バッファをアンロックする
		m_pVtxBuff->Unlock();
	}
}

//============================================================
//	スクロールのテクスチャ座標の設定処理
//============================================================
void CObject3D::SetScrollTex
(
	const float fTexU,		// テクスチャの横座標の開始位置
	const float fTexV,		// テクスチャの縦座標の開始位置
	const float fOffsetU,	// テクスチャの横座標のオフセット位置
	const float fOffsetV	// テクスチャの縦座標のオフセット位置
)
{
	VERTEX_3D* pVtx;	// 頂点情報へのポインタ
	if (m_pVtxBuff != nullptr)
	{ // 使用中の場合

		// 頂点バッファをロックし、頂点情報へのポインタを取得
		m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

		// テクスチャ座標の設定
		pVtx[0].tex = VECTOR2(fTexU,			fTexV);
		pVtx[1].tex = VECTOR2(fTexU + fOffsetU,	fTexV);
		pVtx[2].tex = VECTOR2(fTexU,			fTexV + fOffsetV);
		pVtx[3].tex = VECTOR2(fTexU + fOffsetU,	fTexV + fOffsetV);

		// 頂点バッファをアンロックする
		m_pVtxBuff->Unlock();
	}
}

//============================================================
//	通常描画処理
//============================================================
void CObject3D::DrawNormal()
{
	LPDIRECT3DDEVICE9 pDevice = GET_DEVICE;	// デバイスのポインタ

	// テクスチャの設定
	pDevice->SetTexture(0, GET_MANAGER->GetTexture()->GetPtr(m_nTextureIdx));

	// ポリゴンの描画
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
}

//============================================================
//	シェーダー描画処理
//============================================================
void CObject3D::DrawShader(CShader* pShader)
{
	LPDIRECT3DDEVICE9 pDevice = GET_DEVICE;	// デバイスのポインタ

	// 描画開始
	pShader->Begin();
	pShader->BeginPass(0);

	// マトリックス情報を設定
	pShader->SetMatrix(&m_mtxWorld);

	// ライト方向を設定
	pShader->SetLightDirect(&m_mtxWorld, 0);

	// 拡散光を設定
	pShader->SetOnlyDiffuse(m_col);

	// テクスチャを設定
	pShader->SetTexture(m_nTextureIdx);

	// 状態変更の伝達
	pShader->CommitChanges();

	// テクスチャの設定
	pDevice->SetTexture(0, GET_MANAGER->GetTexture()->GetPtr(m_nTextureIdx));

	// ポリゴンの描画
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);

	// 描画終了
	pShader->EndPass();
	pShader->End();
}

//============================================================
//	法線の正規化処理
//============================================================
void CObject3D::NormalizeNormal()
{
	VERTEX_3D* pVtx;	// 頂点情報へのポインタ

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// 法線の正規化
	useful::NormalizeNormal
	( // 引数
		pVtx[1].pos,	// 左位置
		pVtx[0].pos,	// 中心位置
		pVtx[2].pos,	// 右位置
		pVtx[0].nor		// 法線
	);

	// 法線の正規化
	useful::NormalizeNormal
	( // 引数
		pVtx[2].pos,	// 左位置
		pVtx[3].pos,	// 中心位置
		pVtx[1].pos,	// 右位置
		pVtx[3].nor		// 法線
	);

	// 法線ベクトルの設定
	pVtx[1].nor = (pVtx[0].nor + pVtx[3].nor) / 2;
	pVtx[2].nor = (pVtx[0].nor + pVtx[3].nor) / 2;

	// 法線を正規化
	D3DXVec3Normalize(&pVtx[1].nor, &pVtx[1].nor);
	D3DXVec3Normalize(&pVtx[2].nor, &pVtx[2].nor);

	// 頂点バッファをアンロックする
	m_pVtxBuff->Unlock();
}
