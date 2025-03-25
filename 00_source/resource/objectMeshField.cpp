//============================================================
//
//	オブジェクトメッシュフィールド処理 [objectMeshField.cpp]
//	Author：藤田勇一
//
//============================================================
//************************************************************
//	インクルードファイル
//************************************************************
#include "objectMeshField.h"
#include "manager.h"
#include "renderer.h"
#include "texture.h"
#include "collision.h"

//************************************************************
//	定数宣言
//************************************************************
namespace
{
	const int	NUM_VTX_TRIANGLE = 3;		// 三角形ポリゴンの頂点数
	const float	COLL_RADIUS = 100.0f;		// 当たり判定を行う距離
	const POSGRID2 MIN_PART = GRID2_ONE;	// 分割数の最小値
	const POSGRID2 MIN_TEXPART = GRID2_ONE;	// テクスチャ分割数の最小値
}

//************************************************************
//	子クラス [CObjectMeshField] のメンバ関数
//************************************************************
//============================================================
//	コンストラクタ
//============================================================
CObjectMeshField::CObjectMeshField(const CObject::ELabel label, const CObject::EDim dimension, const int nPriority) : CObject(label, dimension, nPriority),
	m_pVtxBuff		(nullptr),			// 頂点バッファ
	m_pIdxBuff		(nullptr),			// インデックスバッファ
	m_pRenderState	(nullptr),			// レンダーステートの情報
	m_pPosGapBuff	(nullptr),			// 座標のずれバッファ
	m_pNorBuff		(nullptr),			// 法線バッファ
	m_pNumNorBuff	(nullptr),			// 法線の使用数バッファ
	m_mtxWorld		(MTX_IDENT),		// ワールドマトリックス
	m_pos			(VEC3_ZERO),		// 位置
	m_rot			(VEC3_ZERO),		// 向き
	m_size			(VEC2_ZERO),		// 大きさ
	m_col			(color::White()),	// 色
	m_part			(GRID2_ZERO),		// 分割数
	m_texPart		(GRID2_ZERO),		// テクスチャ分割数
	m_nNumVtx		(0),				// 必要頂点数
	m_nNumIdx		(0),				// 必要インデックス数
	m_nTextureIdx	(0)					// テクスチャインデックス
{

}

//============================================================
//	デストラクタ
//============================================================
CObjectMeshField::~CObjectMeshField()
{

}

//============================================================
//	初期化処理
//============================================================
HRESULT CObjectMeshField::Init()
{
	// メンバ変数を初期化
	m_pVtxBuff		= nullptr;			// 頂点バッファ
	m_pIdxBuff		= nullptr;			// インデックスバッファ
	m_pRenderState	= nullptr;			// レンダーステートの情報
	m_pPosGapBuff	= nullptr;			// 座標のずれバッファ
	m_pNorBuff		= nullptr;			// 法線バッファ
	m_pNumNorBuff	= nullptr;			// 法線の使用数バッファ
	m_mtxWorld		= MTX_IDENT;		// ワールドマトリックス
	m_pos			= VEC3_ZERO;		// 位置
	m_rot			= VEC3_ZERO;		// 向き
	m_size			= VEC2_ZERO;		// 大きさ
	m_col			= color::White();	// 色
	m_part			= GRID2_ZERO;		// 分割数
	m_texPart		= GRID2_ZERO;		// テクスチャ分割数
	m_nNumVtx		= 0;				// 必要頂点数
	m_nNumIdx		= 0;				// 必要インデックス数
	m_nTextureIdx	= NONE_IDX;			// テクスチャインデックス

	// 分割数を設定
	if (FAILED(SetPattern(GRID2_ONE)))
	{ // 分割数の設定に失敗した場合

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

	return S_OK;
}

//============================================================
//	終了処理
//============================================================
void CObjectMeshField::Uninit()
{
	// 頂点バッファの破棄
	SAFE_RELEASE(m_pVtxBuff);

	// インデックスバッファの破棄
	SAFE_RELEASE(m_pIdxBuff);

	// レンダーステートの破棄
	SAFE_REF_RELEASE(m_pRenderState);

	// 座標のずれバッファの破棄
	SAFE_DEL_ARRAY(m_pPosGapBuff);

	// 法線バッファの破棄
	SAFE_DEL_ARRAY(m_pNorBuff);

	// 法線の使用数バッファの破棄
	SAFE_DEL_ARRAY(m_pNumNorBuff);

	// オブジェクトメッシュフィールドを破棄
	Release();
}

//============================================================
//	更新処理
//============================================================
void CObjectMeshField::Update(const float fDeltaTime)
{

}

//============================================================
//	描画処理
//============================================================
void CObjectMeshField::Draw(CShader* pShader)
{
	LPDIRECT3DDEVICE9 pDevice = GET_DEVICE;	// デバイスのポインタ
	MATRIX mtxRot, mtxTrans;	// 計算用マトリックス

	// レンダーステートを設定
	m_pRenderState->Set();

	// 描画マトリックスの計算
	CalcDrawMatrix();

	// ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

	// 頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_3D));

	// インデックスバッファをデータストリームに設定
	pDevice->SetIndices(m_pIdxBuff);

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
//	位置の設定処理
//============================================================
void CObjectMeshField::SetVec3Position(const VECTOR3& rPos)
{
	// 引数の位置を設定
	m_pos = rPos;
}

//============================================================
//	向きの設定処理
//============================================================
void CObjectMeshField::SetVec3Rotation(const VECTOR3& rRot)
{
	// 引数の向きを設定
	m_rot = rRot;

	// 向きの正規化
	useful::NormalizeRot(m_rot);
}

//============================================================
//	大きさの設定処理
//============================================================
void CObjectMeshField::SetVec2Size(const VECTOR2& rSize)
{
	// 引数の大きさを設定
	m_size = rSize;

	// 頂点情報の設定
	SetVtx(false);
}

//============================================================
//	生成処理
//============================================================
CObjectMeshField* CObjectMeshField::Create
(
	const VECTOR3& rPos,		// 位置
	const VECTOR3& rRot,		// 向き
	const VECTOR2& rSize,		// 大きさ
	const COLOR& rCol,			// 色
	const POSGRID2& rPart,		// 分割数
	const POSGRID2& rTexPart	// テクスチャ分割数
)
{
	// オブジェクトメッシュフィールドの生成
	CObjectMeshField* pMeshField = new CObjectMeshField;
	if (pMeshField == nullptr)
	{ // 生成に失敗した場合

		return nullptr;
	}
	else
	{ // 生成に成功した場合

		// オブジェクトメッシュフィールドの初期化
		if (FAILED(pMeshField->Init()))
		{ // 初期化に失敗した場合

			// オブジェクトメッシュフィールドの破棄
			SAFE_DELETE(pMeshField);
			return nullptr;
		}

		// 位置を設定
		pMeshField->SetVec3Position(rPos);

		// 向きを設定
		pMeshField->SetVec3Rotation(rRot);

		// 大きさを設定
		pMeshField->SetVec2Size(rSize);

		// 色を設定
		pMeshField->SetColor(rCol);

		// 分割数を設定
		if (FAILED(pMeshField->SetPattern(rPart)))
		{ // 分割数の設定に失敗した場合

			// オブジェクトメッシュフィールドの破棄
			SAFE_DELETE(pMeshField);
			return nullptr;
		}

		// テクスチャ分割数を設定
		pMeshField->SetTexPattern(rTexPart);

		// 確保したアドレスを返す
		return pMeshField;
	}
}

//============================================================
//	レンダーステート情報の取得処理
//============================================================
CRenderState* CObjectMeshField::GetRenderState()
{
	// インスタンス未使用
	assert(m_pRenderState != nullptr);

	// レンダーステートの情報を返す
	return m_pRenderState;
}

//============================================================
//	テクスチャ割当処理 (インデックス)
//============================================================
void CObjectMeshField::BindTexture(const int nTextureIdx)
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
void CObjectMeshField::BindTexture(const char* pTexturePath)
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
void CObjectMeshField::SetAlpha(const float fAlpha)
{
	// 引数の透明度を設定
	m_col.a = fAlpha;

	// 色の設定
	SetColor(m_col);
}

//============================================================
//	色の設定処理
//============================================================
void CObjectMeshField::SetColor(const COLOR& rCol)
{
	// 引数の色を設定
	m_col = rCol;

	// 頂点情報の設定
	SetVtx(false);
}

//============================================================
//	分割数の設定処理
//============================================================
HRESULT CObjectMeshField::SetPattern(const POSGRID2& rPart)
{
	LPDIRECT3DDEVICE9 pDevice = GET_DEVICE;	// デバイスのポインタ
	int nNorBuff = 0;	// 法線バッファの確保数

	//--------------------------------------------------------
	//	変更情報を設定
	//--------------------------------------------------------
	// 分割数の設定不可
	assert(rPart.x >= MIN_PART.x);
	assert(rPart.y >= MIN_PART.y);

	// 引数の分割数を設定
	m_part = rPart;

	// 必要頂点・インデックス数を求める
	m_nNumVtx = (m_part.x + 1) * (m_part.y + 1); // 必要頂点数
	m_nNumIdx = (m_part.x + 1) * (((m_part.y + 1) * 2) - 2) + (m_part.y * 2) - 2; // 必要インデックス数

	// 法線バッファの確保数を求める
	nNorBuff  = 6 + (6 * (m_part.x - 1)) + (6 * (m_part.y - 1)) + (6 * ((m_part.x - 1) * (m_part.y - 1)));

	//--------------------------------------------------------
	//	頂点バッファの破棄・生成
	//--------------------------------------------------------
	// 頂点バッファの破棄
	SAFE_RELEASE(m_pVtxBuff);
	assert(m_pVtxBuff == nullptr);

	// 頂点バッファの生成
	if (FAILED(pDevice->CreateVertexBuffer
	( // 引数
		sizeof(VERTEX_3D) * m_nNumVtx,	// 必要頂点数
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

	//--------------------------------------------------------
	//	インデックスバッファの破棄・生成
	//--------------------------------------------------------
	// インデックスバッファの破棄
	SAFE_RELEASE(m_pIdxBuff);
	assert(m_pIdxBuff == nullptr);

	// インデックスバッファの生成
	if (FAILED(pDevice->CreateIndexBuffer
	( // 引数
		sizeof(WORD) * m_nNumIdx,	// 必要インデックス数
		D3DUSAGE_WRITEONLY,	// 使用方法
		D3DFMT_INDEX16,		// インデックスバッファのフォーマット
		D3DPOOL_MANAGED,	// メモリの指定
		&m_pIdxBuff,		// インデックスバッファへのポインタ
		nullptr
	)))
	{ // インデックスバッファの生成に失敗した場合

		assert(false);
		return E_FAIL;
	}

	//--------------------------------------------------------
	//	座標のずれバッファの破棄・生成
	//--------------------------------------------------------
	// 座標のずれバッファの破棄
	SAFE_DEL_ARRAY(m_pPosGapBuff);
	assert(m_pPosGapBuff == nullptr);

	// 座標のずれバッファのメモリ確保
	m_pPosGapBuff = new VECTOR3[m_nNumVtx];
	if (m_pPosGapBuff != nullptr)
	{ // 確保に成功した場合

		// メモリクリア
		memset(m_pPosGapBuff, 0, sizeof(VECTOR3) * m_nNumVtx);
	}
	else
	{ // 確保に失敗した場合

		assert(false);
		return E_FAIL;
	}

	//--------------------------------------------------------
	//	法線バッファの破棄・生成
	//--------------------------------------------------------
	// 法線バッファの破棄
	SAFE_DEL_ARRAY(m_pNorBuff);
	assert(m_pNorBuff == nullptr);

	// 法線バッファのメモリ確保
	m_pNorBuff = new VECTOR3[nNorBuff];
	if (m_pNorBuff != nullptr)
	{ // 確保に成功した場合
	
		// メモリクリア
		memset(m_pNorBuff, 0, sizeof(VECTOR3) * nNorBuff);
	}
	else
	{ // 確保に失敗した場合

		assert(false);
		return E_FAIL;
	}

	//--------------------------------------------------------
	//	法線の使用数バッファの破棄・生成
	//--------------------------------------------------------
	// 法線の使用数バッファの破棄
	SAFE_DEL_ARRAY(m_pNumNorBuff);
	assert(m_pNumNorBuff == nullptr);

	// 法線の使用数バッファのメモリ確保
	m_pNumNorBuff = new int[m_nNumVtx];
	if (m_pNumNorBuff != nullptr)
	{ // 確保に成功した場合

		// メモリクリア
		memset(m_pNumNorBuff, 0, sizeof(int) * m_nNumVtx);
	}
	else
	{ // 確保に失敗した場合

		assert(false);
		return E_FAIL;
	}

	//--------------------------------------------------------
	//	分割数を反映
	//--------------------------------------------------------
	// 頂点・インデックス情報の設定
	SetVtx(true);
	SetIdx();

	return S_OK;
}

//============================================================
//	テクスチャ分割数の設定処理
//============================================================
void CObjectMeshField::SetTexPattern(const POSGRID2& rTexPart)
{
	if (rTexPart.x >= MIN_TEXPART.x
	&&  rTexPart.y >= MIN_TEXPART.y)
	{ // テクスチャ分割数が最低値以上の場合

		// 引数のテクスチャ分割数を設定
		m_texPart = rTexPart;

		// 頂点情報の設定
		SetVtx(true);
	}
	else { assert(false); }	// 最低値未満
}

//============================================================
//	座標のずれの設定処理
//============================================================
void CObjectMeshField::SetGapPosition(const int nIdx, const VECTOR3& rPos)
{
	if (m_pPosGapBuff != nullptr)
	{ // 使用中の場合

		if (nIdx < (m_part.x + 1) * (m_part.y + 1))
		{ // インデックスが使用可能な場合

			// 頂点のずれを設定
			m_pPosGapBuff[nIdx] = rPos;
		}
		else
		{ // インデックスが使用不可な場合

			// 例外処理
			assert(false);
		}
	}
}

//============================================================
//	座標のずれ取得処理
//============================================================
VECTOR3 CObjectMeshField::GetGapPosition(const int nIdx)
{
	VECTOR3 pos = VEC3_ZERO;	// 頂点のずれの代入用
	if (m_pPosGapBuff != nullptr)
	{ // 使用中の場合

		if (nIdx < (m_part.x + 1) * (m_part.y + 1))
		{ // インデックスが使用可能な場合

			// 頂点のずれを設定
			pos = m_pPosGapBuff[nIdx];
		}
		else
		{ // インデックスが使用不可な場合

			// 例外処理
			assert(false);
		}
	}

	// 引数のインデックスの頂点のずれを返す
	return pos;
}

//============================================================
//	メッシュの頂点位置の設定処理
//============================================================
void CObjectMeshField::SetMeshVertexPosition(const int nIdx, const VECTOR3& rPos)
{
	VERTEX_3D* pVtx;	// 頂点情報へのポインタ
	if (m_pVtxBuff != nullptr)
	{ // 使用中の場合

		if (nIdx < (m_part.x + 1) * (m_part.y + 1))
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
}

//============================================================
//	メッシュの頂点位置の設定処理
//============================================================
VECTOR3 CObjectMeshField::GetMeshVertexPosition(const int nIdx)
{
	VERTEX_3D* pVtx;	// 頂点情報へのポインタ
	VECTOR3 pos;		// 頂点座標の代入用
	if (m_pVtxBuff != nullptr)
	{ // 使用中の場合

		if (nIdx < (m_part.x + 1) * (m_part.y + 1))
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
//	メッシュの着地処理
//============================================================
bool CObjectMeshField::LandPosition(VECTOR3& rPos, VECTOR3& rMove)
{
	float fLandHeight = GetPositionHeight(rPos);	// 着地位置
	if (rPos.y < fLandHeight)
	{ // 位置が地面より下の場合

		// 位置を補正
		rPos.y = fLandHeight;

		// 移動量を初期化
		rMove.y = 0.0f;

		// 着地している状況を返す
		return true;
	}

	// 着地していない状況を返す
	return false;
}

//============================================================
//	地形の設定処理
//============================================================
void CObjectMeshField::SetTerrain(const POSGRID2& rPart, VECTOR3* pPosGap)
{
	if (rPart != m_part)
	{ // 分割数が違う場合

		// 分割数を設定
		SetPattern(rPart);
	}

	if (m_pPosGapBuff != nullptr && rPart == m_part)
	{ // 座標のずれバッファが使用中且つ、同じ分割数の場合

		for (int nCntVtx = 0; nCntVtx < m_nNumVtx; nCntVtx++, pPosGap++)
		{ // 頂点数分繰り返す

			// 引数の座標のずれ量を設定
			m_pPosGapBuff[nCntVtx] = *pPosGap;
		}
	}
	else { assert(false); }	// 非使用中

	// 頂点・インデックス情報の設定
	SetVtx(true);
	SetIdx();
}

//============================================================
//	メッシュの範囲内取得処理
//============================================================
bool CObjectMeshField::IsPositionRange(const VECTOR3&rPos)
{
	VECTOR3 aVtxPos[4];		// ポリゴンの頂点座標
	VECTOR3 aVtxMtxPos[4];	// ポリゴンの位置・向き反映を行った頂点座標

	// メッシュの頂点位置を設定
	aVtxPos[0] = VECTOR3(-m_size.x * 0.5f, 0.0f, +m_size.y * 0.5f);	// 左上
	aVtxPos[1] = VECTOR3(-m_size.x * 0.5f, 0.0f, -m_size.y * 0.5f);	// 左下
	aVtxPos[2] = VECTOR3(+m_size.x * 0.5f, 0.0f, -m_size.y * 0.5f);	// 右下
	aVtxPos[3] = VECTOR3(+m_size.x * 0.5f, 0.0f, +m_size.y * 0.5f);	// 右上

	for (int nCntVtx = 0; nCntVtx < 4; nCntVtx++)
	{ // 三角形ポリゴンの頂点数分繰り返す

		MATRIX mtxWorld, mtxRot, mtxTrans;	// 計算用マトリックス

		// ワールドマトリックスの初期化
		mtxWorld.Identity();

		// 頂点位置を反映
		mtxTrans.Translation(aVtxPos[nCntVtx]);
		mtxWorld.Multiply(mtxWorld, mtxTrans);

		// ポリゴン向きを反映
		mtxRot.Rotation(m_rot);
		mtxWorld.Multiply(mtxWorld, mtxRot);

		// ポリゴン位置を反映
		mtxTrans.Translation(m_pos);
		mtxWorld.Multiply(mtxWorld, mtxTrans);

		// 計算したマトリックスから座標を設定
		aVtxMtxPos[nCntVtx] = mtxWorld.GetPosition();
	}

	// 判定を返す
	return collision::BoxOuterPillar(aVtxMtxPos[0], aVtxMtxPos[1], aVtxMtxPos[2], aVtxMtxPos[3], rPos);
}

//============================================================
//	メッシュの着地位置の取得処理
//============================================================
float CObjectMeshField::GetPositionHeight(const VECTOR3& rPos)
{
	VECTOR3 aVtxPos[NUM_VTX_TRIANGLE];		// ポリゴンの頂点座標
	VECTOR3 aVtxMtxPos[NUM_VTX_TRIANGLE];	// ポリゴンの位置・向き反映を行った頂点座標
	VECTOR3 pos;	// ターゲット位置
	VECTOR3 nor;	// 法線ベクトル
	int nNumVtx;	// 法線を求める頂点番号
	int nNumCul;	// 法線ベクトル用の頂点計算用

	for (int nCntVtx = 0; nCntVtx < 2; nCntVtx++)
	{ // 四角ポリゴンに使用する三角の数分繰り返す

		// 法線ベクトル用の頂点計算用の値を設定
		nNumCul = (nCntVtx == 0) ? 1 : -1;

		for (int nCntHeight = 0; nCntHeight < m_part.y; nCntHeight++)
		{ // 縦の分割数分繰り返す

			for (int nCntWidth = 0; nCntWidth < m_part.x; nCntWidth++)
			{ // 横の分割数分繰り返す

				// 法線を求める頂点番号を設定
				nNumVtx = ((nCntWidth + 1) + (nCntHeight * (m_part.x + 1))) + (nCntVtx * m_part.x);

				// ターゲット位置を設定
				pos = rPos;

				// ポリゴンの頂点位置を取得
				aVtxPos[0] = GetMeshVertexPosition(nNumVtx);
				aVtxPos[1] = GetMeshVertexPosition(nNumVtx - (1 * nNumCul));
				aVtxPos[2] = GetMeshVertexPosition(nNumVtx + ((m_part.x + 1) * nNumCul));

				for (int nCntTriangle = 0; nCntTriangle < NUM_VTX_TRIANGLE; nCntTriangle++)
				{ // 三角形ポリゴンの頂点数分繰り返す

					MATRIX mtxWorld, mtxRot, mtxTrans;	// 計算用マトリックス

					// ワールドマトリックスの初期化
					mtxWorld.Identity();

					// 頂点位置を反映
					mtxTrans.Translation(aVtxPos[nCntTriangle]);
					mtxWorld.Multiply(mtxWorld, mtxTrans);

					// ポリゴン向きを反映
					mtxRot.Rotation(m_rot);
					mtxWorld.Multiply(mtxWorld, mtxRot);

					// ポリゴン位置を反映
					mtxTrans.Translation(m_pos);
					mtxWorld.Multiply(mtxWorld, mtxTrans);

					// 計算したマトリックスから座標を設定
					aVtxMtxPos[nCntTriangle] = mtxWorld.GetPosition();
				}

				if (collision::TriangleOuterPillar(aVtxMtxPos[0], aVtxMtxPos[1], aVtxMtxPos[2], rPos))
				{ // ポリゴンの範囲内にいる場合

					// 法線を求める
					useful::NormalizeNormal(aVtxMtxPos[1], aVtxMtxPos[0], aVtxMtxPos[2], nor);

					if (nor.y != 0.0f)
					{ // 法線が設定されている場合

						// プレイヤーの着地点を返す
						return (((rPos.x - aVtxMtxPos[0].x) * nor.x + (-aVtxMtxPos[0].y) * nor.y + (rPos.z - aVtxMtxPos[0].z) * nor.z) * -1.0f) / nor.y;
					}
				}
			}
		}
	}

	// 着地範囲外の場合現在のy座標を返す
	return rPos.y;
}

//============================================================
//	法線の正規化処理
//============================================================
void CObjectMeshField::NormalizeNormal()
{
	VERTEX_3D* pVtx;	// 頂点情報へのポインタ
	int nNumNor = 0;	// 法線データ格納用

	//--------------------------------------------------------
	//	法線の設定
	//--------------------------------------------------------
	// 頂点バッファをロックし、頂点情報へのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntHeight = 0; nCntHeight < m_part.y + 1; nCntHeight++)
	{ // 縦の分割数 +1回繰り返す

		for (int nCntWidth = 0; nCntWidth < m_part.x + 1; nCntWidth++)
		{ // 横の分割数 +1回繰り返す

			int nNumVtx = nCntWidth + (nCntHeight * (m_part.x + 1));	// 現在の頂点番号
			if (nNumVtx == 0)
			{ // 頂点番号が左上の場合

				// 法線の使用数を 2に設定
				m_pNumNorBuff[nNumVtx] = 2;

				// 法線の設定
				m_pNorBuff[nNumNor + 0] = GetNormalRightTop(pVtx);
				m_pNorBuff[nNumNor + 1] = GetNormalRightBottom(pVtx);

				// 法線データを 2つ分進める
				nNumNor += 2;
			}
			else if (nNumVtx == m_nNumVtx - 1)
			{ // 頂点番号が右下の場合

				// 法線の使用数を 2に設定
				m_pNumNorBuff[nNumVtx] = 2;

				// 法線の設定
				m_pNorBuff[nNumNor + 0] = GetNormalLeftTop(pVtx);
				m_pNorBuff[nNumNor + 1] = GetNormalLeftBottom(pVtx);

				// 法線データを 2つ分進める
				nNumNor += 2;
			}
			else if (nNumVtx == m_part.x)
			{ // 頂点番号が右上の場合

				// 法線の使用数を 1に設定
				m_pNumNorBuff[nNumVtx] = 1;

				// 法線の設定
				m_pNorBuff[nNumNor + 0] = GetNormalLeft(pVtx);

				// 法線データを 1つ分進める
				nNumNor += 1;
			}
			else if (nNumVtx == (m_nNumVtx - 1) - m_part.x)
			{ // 頂点番号が左下の場合

				// 法線の使用数を 1に設定
				m_pNumNorBuff[nNumVtx] = 1;

				// 法線の設定
				m_pNorBuff[nNumNor + 0] = GetNormalRight(pVtx);

				// 法線データを 1つ分進める
				nNumNor += 1;
			}
			else if (nNumVtx < m_part.x)
			{ // 頂点番号が角を除いた上一行の頂点の場合

				// 法線の使用数を 3に設定
				m_pNumNorBuff[nNumVtx] = 3;

				// 法線の設定
				m_pNorBuff[nNumNor + 0] = GetNormalRightTop(pVtx);
				m_pNorBuff[nNumNor + 1] = GetNormalRightBottom(pVtx);
				m_pNorBuff[nNumNor + 2] = GetNormalLeft(pVtx);

				// 法線データを 3つ分進める
				nNumNor += 3;
			}
			else if (nNumVtx > (m_part.x + 1) * m_part.y)
			{ // 頂点番号が角を除いた下一行の頂点の場合

				// 法線の使用数を 3に設定
				m_pNumNorBuff[nNumVtx] = 3;

				// 法線の設定
				m_pNorBuff[nNumNor + 0] = GetNormalRight(pVtx);
				m_pNorBuff[nNumNor + 1] = GetNormalLeftTop(pVtx);
				m_pNorBuff[nNumNor + 2] = GetNormalLeftBottom(pVtx);

				// 法線データを 3つ分進める
				nNumNor += 3;
			}
			else if (nCntWidth == 0)
			{ // 頂点番号が角を除いた左一行の頂点の場合

				// 法線の使用数を 3に設定
				m_pNumNorBuff[nNumVtx] = 3;

				// 法線の設定
				m_pNorBuff[nNumNor + 0] = GetNormalRight(pVtx);
				m_pNorBuff[nNumNor + 1] = GetNormalRightTop(pVtx);
				m_pNorBuff[nNumNor + 2] = GetNormalRightBottom(pVtx);

				// 法線データを 3つ分進める
				nNumNor += 3;
			}
			else if (nCntWidth == m_part.x)
			{ // 頂点番号が角を除いた右一行の頂点の場合

				// 法線の使用数を 3に設定
				m_pNumNorBuff[nNumVtx] = 3;

				// 法線の設定
				m_pNorBuff[nNumNor + 0] = GetNormalLeft(pVtx);
				m_pNorBuff[nNumNor + 1] = GetNormalLeftTop(pVtx);
				m_pNorBuff[nNumNor + 2] = GetNormalLeftBottom(pVtx);

				// 法線データを 3つ分進める
				nNumNor += 3;
			}
			else
			{ // 頂点番号が外周内の頂点の場合

				// 法線の使用数を 6に設定
				m_pNumNorBuff[nNumVtx] = 6;

				// 法線の設定
				m_pNorBuff[nNumNor + 0] = GetNormalRight(pVtx);
				m_pNorBuff[nNumNor + 1] = GetNormalRightTop(pVtx);
				m_pNorBuff[nNumNor + 2] = GetNormalRightBottom(pVtx);
				m_pNorBuff[nNumNor + 3] = GetNormalLeft(pVtx);
				m_pNorBuff[nNumNor + 4] = GetNormalLeftTop(pVtx);
				m_pNorBuff[nNumNor + 5] = GetNormalLeftBottom(pVtx);

				// 法線データを 6つ分進める
				nNumNor += 6;
			}

			// 頂点データのポインタを 1つ分進める
			pVtx += 1;
		}
	}

	// 頂点バッファをアンロックする
	m_pVtxBuff->Unlock();

	//--------------------------------------------------------
	//	法線の平均化
	//--------------------------------------------------------
	// 法線データ格納用変数を初期化
	nNumNor = 0;

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntVtx = 0; nCntVtx < m_nNumVtx; nCntVtx++)
	{ // 頂点数分繰り返す

		VECTOR3 nor = VEC3_ZERO;	// 法線設定用
		for (int nCntNor = 0; nCntNor < m_pNumNorBuff[nCntVtx]; nCntNor++)
		{ // 設定されている法線数分繰り返す

			// 法線を加算
			nor += m_pNorBuff[nNumNor];

			// 法線データを 1つ分進める
			nNumNor++;
		}

		// 法線を平均化
		nor /= (float)m_pNumNorBuff[nCntVtx];

		// 法線を設定
		pVtx->nor = nor;

		// 頂点データのポインタを 1つ分進める
		pVtx += 1;
	}

	// 頂点バッファをアンロックする
	m_pVtxBuff->Unlock();
}

//============================================================
//	描画マトリックスの計算処理
//============================================================
void CObjectMeshField::CalcDrawMatrix()
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
void CObjectMeshField::SetVtx(bool bNor)
{
	VERTEX_3D* pVtx;	// 頂点情報へのポインタ
	int nNumVtx = 0;	// 現在の頂点番号

	// テクスチャ分割数の割合を計算
	VECTOR2 texRate = VECTOR2
	(
		(float)m_texPart.x / (float)m_part.x,
		(float)m_texPart.y / (float)m_part.y
	);

	if (m_pVtxBuff != nullptr)
	{ // 使用中の場合

		// 頂点バッファをロックし、頂点情報へのポインタを取得
		m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

		for (int nCntHeight = 0; nCntHeight < m_part.y + 1; nCntHeight++)
		{ // 縦の分割数 +1回繰り返す

			for (int nCntWidth = 0; nCntWidth < m_part.x + 1; nCntWidth++)
			{ // 横の分割数 +1回繰り返す

				// 現在の頂点番号を設定
				nNumVtx = nCntWidth + (nCntHeight * (m_part.x + 1));

				// 頂点座標の設定
				pVtx[0].pos = VECTOR3
				( // 引数
					nCntWidth * (m_size.x / (float)m_part.x) - (m_size.x * 0.5f),		// x
					0.0f,																// y
					-(nCntHeight * (m_size.y / (float)m_part.y) - (m_size.y * 0.5f))	// z
				);
				pVtx[0].pos += m_pPosGapBuff[nNumVtx];	// 頂点からのずれ量を加算

				// 頂点カラーの設定
				pVtx[0].col = m_col;

				// テクスチャ座標の設定
				pVtx[0].tex = VECTOR2(texRate.x * nCntWidth, texRate.y * nCntHeight);

				// 頂点データのポインタを 1つ分進める
				pVtx += 1;
			}
		}

		// 頂点バッファをアンロックする
		m_pVtxBuff->Unlock();
	}

	if (bNor)
	{ // 法線の正規化も行う場合

		// 法線の設定・正規化
		NormalizeNormal();
	}
}

//============================================================
//	インデックス情報の設定処理
//============================================================
void CObjectMeshField::SetIdx()
{
	WORD* pIdx;	// インデックス情報へのポインタ
	if (m_pIdxBuff != nullptr)
	{ // 使用中の場合

		// インデックスバッファをロックし、頂点番号データへのポインタを取得
		m_pIdxBuff->Lock(0, 0, (void**)&pIdx, 0);

		for (int nCntHeight = 0, nCntWidth = 0; nCntHeight < m_part.y; nCntHeight++)
		{ // 縦の分割数 +1回繰り返す

			for (nCntWidth = 0; nCntWidth < m_part.x + 1; nCntWidth++)
			{ // 横の分割数 +1回繰り返す

				pIdx[0] = (WORD)((m_part.x + 1) * (nCntHeight + 1) + nCntWidth);
				pIdx[1] = (WORD)((m_part.x + 1) * nCntHeight + nCntWidth);

				// インデックスデータのポインタを 2つ分進める
				pIdx += 2;
			}

			if (nCntHeight != m_part.y - 1)
			{ // 一番手前の分割場所ではない場合

				pIdx[0] = (WORD)((m_part.x + 1) * nCntHeight + nCntWidth - 1);
				pIdx[1] = (WORD)((m_part.x + 1) * (nCntHeight + 2));

				// インデックスデータのポインタを 2つ分進める
				pIdx += 2;
			}
		}

		// インデックスバッファをアンロックする
		m_pIdxBuff->Unlock();
	}
}

//============================================================
//	スクロールのテクスチャ座標の設定処理
//============================================================
void CObjectMeshField::SetScrollTex(const float fTexU, const float fTexV)
{
	VERTEX_3D* pVtx;	// 頂点情報へのポインタ
	if (m_pVtxBuff != nullptr)
	{ // 使用中の場合

		// 頂点バッファをロックし、頂点情報へのポインタを取得
		m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

		for (int nCntHeight = 0; nCntHeight < m_part.y + 1; nCntHeight++)
		{ // 縦の分割数 +1回繰り返す

			for (int nCntWidth = 0; nCntWidth < m_part.x + 1; nCntWidth++)
			{ // 横の分割数 +1回繰り返す

				// テクスチャ座標の設定
				pVtx[0].tex = VECTOR2((float)nCntWidth + fTexU, (float)nCntHeight + fTexV);

				// 頂点データのポインタを 1つ分進める
				pVtx += 1;
			}
		}

		// 頂点バッファをアンロックする
		m_pVtxBuff->Unlock();
	}
}

//============================================================
//	通常描画処理
//============================================================
void CObjectMeshField::DrawNormal()
{
	LPDIRECT3DDEVICE9 pDevice = GET_DEVICE;	// デバイスのポインタ

	// テクスチャの設定
	pDevice->SetTexture(0, GET_MANAGER->GetTexture()->GetPtr(m_nTextureIdx));

	// ポリゴンの描画
	pDevice->DrawIndexedPrimitive
	( // 引数
		D3DPT_TRIANGLESTRIP,	// プリミティブの種類
		0,
		0,
		m_nNumVtx,		// 使用する頂点数
		0,				// インデックスバッファの開始地点
		m_nNumIdx - 2	// プリミティブ (ポリゴン) 数
	);
}

//============================================================
//	シェーダー描画処理
//============================================================
void CObjectMeshField::DrawShader(CShader* pShader)
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
	pDevice->DrawIndexedPrimitive
	( // 引数
		D3DPT_TRIANGLESTRIP,	// プリミティブの種類
		0,
		0,
		m_nNumVtx,		// 使用する頂点数
		0,				// インデックスバッファの開始地点
		m_nNumIdx - 2	// プリミティブ (ポリゴン) 数
	);

	// 描画終了
	pShader->EndPass();
	pShader->End();
}

//============================================================
//	法線の取得処理 (左)
//============================================================
VECTOR3 CObjectMeshField::GetNormalLeft(VERTEX_3D* pVtx)
{
	// 法線の正規化
	VECTOR3 nor;	// 法線代入用
	useful::NormalizeNormal
	( // 引数
		pVtx[m_part.x + 1].pos,	// 左位置
		pVtx[0].pos,			// 中心位置
		pVtx[-1].pos,			// 右位置
		nor						// 法線
	);

	// 法線を返す
	return nor;
}

//============================================================
//	法線の取得処理 (左上)
//============================================================
VECTOR3 CObjectMeshField::GetNormalLeftTop(VERTEX_3D* pVtx)
{
	// 法線の正規化
	VECTOR3 nor;	// 法線代入用
	useful::NormalizeNormal
	( // 引数
		pVtx[-(m_part.x + 2)].pos,	// 左位置
		pVtx[0].pos,				// 中心位置
		pVtx[-(m_part.x + 1)].pos,	// 右位置
		nor							// 法線
	);

	// 法線を返す
	return nor;
}

//============================================================
//	法線の取得処理 (左下)
//============================================================
VECTOR3 CObjectMeshField::GetNormalLeftBottom(VERTEX_3D* pVtx)
{
	// 法線の正規化
	VECTOR3 nor;	// 法線代入用
	useful::NormalizeNormal
	( // 引数
		pVtx[-1].pos,				// 左位置
		pVtx[0].pos,				// 中心位置
		pVtx[-(m_part.x + 2)].pos,	// 右位置
		nor							// 法線
	);

	// 法線を返す
	return nor;
}

//============================================================
//	法線の取得処理 (右)
//============================================================
VECTOR3 CObjectMeshField::GetNormalRight(VERTEX_3D* pVtx)
{
	// 法線の正規化
	VECTOR3 nor;	// 法線代入用
	useful::NormalizeNormal
	( // 引数
		pVtx[-(m_part.x + 1)].pos,	// 左位置
		pVtx[0].pos,				// 中心位置
		pVtx[1].pos,				// 右位置
		nor							// 法線
	);

	// 法線を返す
	return nor;
}

//============================================================
//	法線の取得処理 (右上)
//============================================================
VECTOR3 CObjectMeshField::GetNormalRightTop(VERTEX_3D* pVtx)
{
	// 法線の正規化
	VECTOR3 nor;	// 法線代入用
	useful::NormalizeNormal
	( // 引数
		pVtx[1].pos,			// 左位置
		pVtx[0].pos,			// 中心位置
		pVtx[m_part.x + 2].pos,	// 右位置
		nor						// 法線
	);

	// 法線を返す
	return nor;
}

//============================================================
//	法線の取得処理 (右下)
//============================================================
VECTOR3 CObjectMeshField::GetNormalRightBottom(VERTEX_3D* pVtx)
{
	// 法線の正規化
	VECTOR3 nor;	// 法線代入用
	useful::NormalizeNormal
	( // 引数
		pVtx[m_part.x + 2].pos,	// 左位置
		pVtx[0].pos,			// 中心位置
		pVtx[m_part.x + 1].pos,	// 右位置
		nor						// 法線
	);

	// 法線を返す
	return nor;
}
