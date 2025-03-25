//============================================================
//
//	オブジェクト竜巻処理 [objectTornado.cpp]
//	Author：藤田勇一
//
//============================================================
//************************************************************
//	インクルードファイル
//************************************************************
#include "objectTornado.h"
#include "manager.h"
#include "renderer.h"
#include "texture.h"

//************************************************************
//	定数宣言
//************************************************************
namespace
{
	const int MAX_OFFSET = 2;	// オフセットの数
}

//************************************************************
//	子クラス [CObjectTornado] のメンバ関数
//************************************************************
//============================================================
//	コンストラクタ
//============================================================
CObjectTornado::CObjectTornado() : CObject(CObject::LABEL_NONE, CObject::DIM_3D, CObject::DEFAULT_PRIO),
	m_pVtxBuff		(nullptr),			// 頂点バッファ
	m_pRenderState	(nullptr),			// レンダーステートの情報
	m_pMtxParent	(nullptr),			// 親のマトリックス
	m_mtxWorld		(MTX_IDENT),		// ワールドマトリックス
	m_pos			(VEC3_ZERO),		// 位置
	m_rot			(VEC3_ZERO),		// 向き
	m_growRot		(VEC3_ZERO),		// 成長向き
	m_col			(color::White()),	// 色
	m_fMoveRot		(0.0f),				// 向きの変更量
	m_fThickness	(0.0f),				// ポリゴンの太さ
	m_fOuterPlusY	(0.0f),				// ポリゴン外周のY座標加算量
	m_fSetWidth		(0.0f),				// 生成時の横ずれ量
	m_fSetAlpha		(0.0f),				// 生成時の透明度
	m_fAddWidth		(0.0f),				// 横ずれの加算量
	m_fAddHeight	(0.0f),				// 縦ずれの加算量
	m_fSubAlpha		(0.0f),				// 透明度の減算量
	m_fGrowWidth	(0.0f),				// 横ずれの成長量
	m_fGrowHeight	(0.0f),				// 縦ずれの成長量
	m_fGrowAlpha	(0.0f),				// 透明度の成長量
	m_nNumVtx		(0),				// 必要頂点数
	m_nNumAround	(0),				// 渦の周回数
	m_nPattern		(0),				// 渦の分割数
	m_nTextureIdx	(0)					// テクスチャインデックス
{

}

//============================================================
//	デストラクタ
//============================================================
CObjectTornado::~CObjectTornado()
{

}

//============================================================
//	初期化処理
//============================================================
HRESULT CObjectTornado::Init()
{
	// メンバ変数を初期化
	m_pVtxBuff		= nullptr;			// 頂点バッファ
	m_pRenderState	= nullptr;			// レンダーステートの情報
	m_pMtxParent	= nullptr;			// 親のマトリックス
	m_mtxWorld		= MTX_IDENT;		// ワールドマトリックス
	m_pos			= VEC3_ZERO;		// 位置
	m_rot			= VEC3_ZERO;		// 向き
	m_growRot		= VEC3_ZERO;		// 成長向き
	m_col			= color::White();	// 色
	m_fMoveRot		= 0.0f;				// 向きの変更量
	m_fThickness	= 0.0f;				// ポリゴンの太さ
	m_fOuterPlusY	= 0.0f;				// ポリゴン外周のY座標加算量
	m_fSetWidth		= 0.0f;				// 生成時の横ずれ量
	m_fSetAlpha		= 0.0f;				// 生成時の透明度
	m_fAddWidth		= 0.0f;				// 横ずれの加算量
	m_fAddHeight	= 0.0f;				// 縦ずれの加算量
	m_fSubAlpha		= 0.0f;				// 透明度の減算量
	m_fGrowWidth	= 0.0f;				// 横ずれの成長量
	m_fGrowHeight	= 0.0f;				// 縦ずれの成長量
	m_fGrowAlpha	= 0.0f;				// 透明度の成長量
	m_nNumVtx		= 0;				// 必要頂点数
	m_nNumAround	= 0;				// 渦の周回数
	m_nPattern		= 0;				// 渦の分割数
	m_nTextureIdx	= NONE_IDX;			// テクスチャインデックス

	// 渦を設定
	if (FAILED(SetVortex(1, 1)))
	{ // 渦の設定に失敗した場合

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

	// レンダーステートの情報を取得
	CRenderState* pRenderState = GetRenderState();

	// αテストの設定
	pRenderState->SetAlphaTest(true);			// αテストの有効 / 無効の設定
	pRenderState->SetAlphaFunc(D3DCMP_GREATER);	// αテストの設定

	// ポリゴンの両面を表示状態にする
	pRenderState->SetCulling(D3DCULL_NONE);

	// ライティングをOFFにする
	pRenderState->SetLighting(false);

	return S_OK;
}

//============================================================
//	終了処理
//============================================================
void CObjectTornado::Uninit()
{
	// 頂点バッファの破棄
	SAFE_RELEASE(m_pVtxBuff);

	// レンダーステートの破棄
	SAFE_REF_RELEASE(m_pRenderState);

	// オブジェクト竜巻を破棄
	Release();
}

//============================================================
//	更新処理
//============================================================
void CObjectTornado::Update(const float fDeltaTime)
{
	// 竜巻の向きを変更
	m_rot.y -= m_fMoveRot;

	// 向きを正規化
	useful::NormalizeRot(m_rot.y);

	// 竜巻を成長させる
	m_fSetWidth  += m_fGrowWidth * m_nNumVtx;	// 原点からの横ずれ量を加算
	m_fAddWidth  += m_fGrowWidth;	// 竜巻の横の広がりを加算
	m_fAddHeight += m_fGrowHeight;	// 竜巻の縦の広がりを加算
	m_fSetAlpha  -= m_fGrowAlpha;	// 竜巻の透明度を減算 (透明にしていく)

	// 頂点情報の設定
	SetVtx();

	if (m_fSetAlpha <= 0.0f)
	{ // 竜巻が透明になった場合

		// 竜巻オブジェクトの終了
		Uninit();

		// 処理を抜ける
		return;
	}
}

//============================================================
//	描画処理
//============================================================
void CObjectTornado::Draw(CShader* pShader)
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
//	位置の設定処理
//============================================================
void CObjectTornado::SetVec3Position(const VECTOR3& rPos)
{
	// 引数の位置を設定
	m_pos = rPos;
}

//============================================================
//	生成処理
//============================================================
CObjectTornado* CObjectTornado::Create
(
	const VECTOR3& rPos,		// 位置
	const VECTOR3& rGrowRot,	// 成長向き
	const COLOR& rCol,			// 色
	MATRIX*		pMtxParent,		// 親のマトリックス
	const int	nNumAround,		// 渦の周回数
	const int	nPattern,		// 渦の分割数
	const float	fMoveRot,		// 向きの変更量
	const float	fThickness,		// ポリゴンの太さ
	const float	fOuterPlusY,	// ポリゴン外周のY座標加算量
	const float	fSetWidth,		// 生成時の横ずれ量
	const float	fSetAlpha,		// 生成時の透明度
	const float	fAddWidth,		// 横ずれの加算量
	const float	fAddHeight,		// 縦ずれの加算量
	const float	fSubAlpha,		// 透明度の減算量
	const float	fGrowWidth,		// 横ずれの成長量
	const float	fGrowHeight,	// 縦ずれの成長量
	const float	fGrowAlpha		// 透明度の成長量
)
{
	// オブジェクト竜巻の生成
	CObjectTornado* pTornado = new CObjectTornado;
	if (pTornado == nullptr)
	{ // 生成に失敗した場合

		return nullptr;
	}
	else
	{ // 生成に成功した場合

		// オブジェクト竜巻の初期化
		if (FAILED(pTornado->Init()))
		{ // 初期化に失敗した場合

			// オブジェクト竜巻の破棄
			SAFE_DELETE(pTornado);
			return nullptr;
		}

		// 位置を設定
		pTornado->SetVec3Position(rPos);

		// 向きを設定
		pTornado->SetRotationGrow(rGrowRot);

		// 色を設定
		pTornado->SetColor(rCol);

		// 親のマトリックスを設定
		pTornado->SetMatrixParent(pMtxParent);

		// ポリゴンの太さを設定
		pTornado->SetThickness(fThickness);

		// ポリゴン外周のY座標加算量を設定
		pTornado->SetOuterPlusY(fOuterPlusY);

		// 生成時の横ずれ量を設定
		pTornado->SetCreateWidth(fSetWidth);

		// 生成時の透明度を設定
		pTornado->SetCreateAlpha(fSetAlpha);

		// 横ずれの加算量を設定
		pTornado->SetAddWidth(fAddWidth);

		// 縦ずれの加算量を設定
		pTornado->SetAddHeight(fAddHeight);

		// 透明度の減算量を設定
		pTornado->SetSubAlpha(fSubAlpha);

		// 成長性を設定
		pTornado->SetGrow(fMoveRot, fGrowWidth, fGrowHeight, fGrowAlpha);

		// 渦を設定
		if (FAILED(pTornado->SetVortex(nNumAround, nPattern)))
		{ // 渦の設定に失敗した場合

			// オブジェクト竜巻の破棄
			SAFE_DELETE(pTornado);
			return nullptr;
		}

		// 確保したアドレスを返す
		return pTornado;
	}
}

//============================================================
//	成長の設定処理
//============================================================
void CObjectTornado::SetGrow
(
	const float fMoveRot,		// 向きの変更量
	const float fGrowWidth,		// 横ずれの成長量
	const float fGrowHeight,	// 縦ずれの成長量
	const float fGrowAlpha		// 透明度の成長量
)
{
	// 引数の情報を設定
	m_fMoveRot		= fMoveRot;		// 向きの変更量
	m_fGrowWidth	= fGrowWidth;	// 横ずれの成長量
	m_fGrowHeight	= fGrowHeight;	// 縦ずれの成長量
	m_fGrowAlpha	= fGrowAlpha;	// 透明度の成長量
}

//============================================================
//	渦の設定処理
//============================================================
HRESULT CObjectTornado::SetVortex(const int nNumAround, const int nPattern)
{
	// 引数の渦を設定
	m_nNumAround = nNumAround;
	m_nPattern = nPattern;

	// 必要頂点数を求める
	m_nNumVtx = m_nNumAround * m_nPattern * MAX_OFFSET;

	// 頂点バッファの破棄
	SAFE_RELEASE(m_pVtxBuff);
	assert(m_pVtxBuff == nullptr);

	// 頂点バッファの生成
	if (FAILED(GET_DEVICE->CreateVertexBuffer
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

	// 頂点情報の設定
	SetVtx();

	return S_OK;
}

//============================================================
//	レンダーステート情報の取得処理
//============================================================
CRenderState* CObjectTornado::GetRenderState()
{
	// インスタンス未使用
	assert(m_pRenderState != nullptr);

	// レンダーステートの情報を返す
	return m_pRenderState;
}

//============================================================
//	テクスチャ割当処理 (インデックス)
//============================================================
void CObjectTornado::BindTexture(const int nTextureIdx)
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
void CObjectTornado::BindTexture(const char* pTexturePath)
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
//	親マトリックス削除処理
//============================================================
void CObjectTornado::DeleteMatrixParent()
{
	// 親マトリックスをnullptrにする
	m_pMtxParent = nullptr;
}

//============================================================
//	親のマトリックスの設定処理
//============================================================
void CObjectTornado::SetMatrixParent(MATRIX* pMtxParent)
{
	// 引数の親マトリックスを設定
	m_pMtxParent = pMtxParent;
}

//============================================================
//	透明度の設定処理
//============================================================
void CObjectTornado::SetAlpha(const float fAlpha)
{
	// 引数の透明度を設定
	m_col.a = fAlpha;

	// 色の設定
	SetColor(m_col);
}

//============================================================
//	色の設定処理
//============================================================
void CObjectTornado::SetColor(const COLOR& rCol)
{
	// 引数の色を設定
	m_col = rCol;

	// 頂点情報の設定
	SetVtx();
}

//============================================================
//	成長向きの設定処理
//============================================================
void CObjectTornado::SetRotationGrow(const VECTOR3& rRot)
{
	// 引数の成長向きを設定
	m_growRot = rRot;

	// 成長向きの正規化
	useful::NormalizeRot(m_growRot);
}

//============================================================
//	ポリゴンの太さの設定処理
//============================================================
void CObjectTornado::SetThickness(const float fThickness)
{
	// 引数のポリゴンの太さを設定
	m_fThickness = fThickness;

	// 頂点情報の設定
	SetVtx();
}

//============================================================
//	ポリゴン外周のY座標加算量の設定処理
//============================================================
void CObjectTornado::SetOuterPlusY(const float fOuterPlusY)
{
	// 引数のポリゴン外周のY座標加算量を設定
	m_fOuterPlusY = fOuterPlusY;

	// 頂点情報の設定
	SetVtx();
}

//============================================================
//	生成時の横ずれ量の設定処理
//============================================================
void CObjectTornado::SetCreateWidth(const float fSetWidth)
{
	// 引数の生成時の横ずれ量を設定
	m_fSetWidth = fSetWidth;

	// 頂点情報の設定
	SetVtx();
}

//============================================================
//	生成時の透明度の設定処理
//============================================================
void CObjectTornado::SetCreateAlpha(const float fSetAlpha)
{
	// 引数の生成時の透明度を設定
	m_fSetAlpha = fSetAlpha;

	// 頂点情報の設定
	SetVtx();
}

//============================================================
//	横ずれの加算量の設定処理
//============================================================
void CObjectTornado::SetAddWidth(const float fAddWidth)
{
	// 引数の横ずれの加算量を設定
	m_fAddWidth = fAddWidth;

	// 頂点情報の設定
	SetVtx();
}

//============================================================
//	縦ずれの加算量の設定処理
//============================================================
void CObjectTornado::SetAddHeight(const float fAddHeight)
{
	// 引数の縦ずれの加算量を設定
	m_fAddHeight = fAddHeight;

	// 頂点情報の設定
	SetVtx();
}

//============================================================
//	透明度の減算量の設定処理
//============================================================
void CObjectTornado::SetSubAlpha(const float fSubAlpha)
{
	// 引数の透明度の減算量を設定
	m_fSubAlpha = fSubAlpha;

	// 頂点情報の設定
	SetVtx();
}

//============================================================
//	描画マトリックスの計算処理
//============================================================
void CObjectTornado::CalcDrawMatrix()
{
	MATRIX mtxRot, mtxTrans;	// 計算用マトリックス
	MATRIX mtxOrigin;			// 発生源のマトリックス

	//--------------------------------------------------------
	//	発生源のマトリックスを求める
	//--------------------------------------------------------
	// 発生源のマトリックスの初期化
	mtxOrigin.Identity();

	// 向きを反映
	mtxRot.Rotation(m_growRot);
	mtxOrigin.Multiply(mtxOrigin, mtxRot);	// 成長向き

	// 位置を反映
	mtxTrans.Translation(m_pos);
	mtxOrigin.Multiply(mtxOrigin, mtxTrans);	// 発生位置

	//--------------------------------------------------------
	//	ワールドマトリックスを求める
	//--------------------------------------------------------
	// ワールドマトリックスの初期化
	m_mtxWorld.Identity();

	// 向きを反映
	mtxRot.Rotation(m_rot);
	m_mtxWorld.Multiply(m_mtxWorld, mtxRot);	// 回転量

	//--------------------------------------------------------
	//	マトリックスを掛け合わせる
	//--------------------------------------------------------
	if (m_pMtxParent != nullptr)
	{ // 親のマトリックスが存在する場合

		// 親のマトリックスと掛け合わせる
		mtxOrigin.Multiply(mtxOrigin, *m_pMtxParent);
	}

	// 発生源のマトリックスと掛け合わせる
	m_mtxWorld.Multiply(m_mtxWorld, mtxOrigin);
}

//============================================================
//	頂点情報の設定処理
//============================================================
void CObjectTornado::SetVtx()
{
	VERTEX_3D* pVtx;	// 頂点情報へのポインタ
	VECTOR3 vecPos;		// 竜巻の頂点方向ベクトル
	float fWidth  = m_fSetWidth;	// 頂点方向の横ずれ量
	float fHeight = 0.0f;			// 頂点方向の縦ずれ量
	float fAlpha  = m_fSetAlpha;	// 頂点カラーの透明度
	if (m_pVtxBuff != nullptr)
	{ // 竜巻の頂点バッファが使用中の場合

		// 頂点バッファをロックし、頂点情報へのポインタを取得
		m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

		for (int nCntWidth = 0; nCntWidth < m_nNumAround; nCntWidth++)
		{ // 渦の周回の総数分繰り返す

			for (int nCntHeight = 0; nCntHeight < m_nPattern; nCntHeight++)
			{ // 渦の分割数分繰り返す

				// 頂点方向ベクトルを求める
				vecPos.x = sinf(nCntHeight * ((D3DX_PI * 2.0f) / m_nPattern));
				vecPos.y = 1.0f;
				vecPos.z = cosf(nCntHeight * ((D3DX_PI * 2.0f) / m_nPattern));

				for (int nCntSet = 0; nCntSet < MAX_OFFSET; nCntSet++)
				{ // オフセットの総数分繰り返す

					// 頂点座標の設定
					pVtx[0].pos.x = 0.0f + vecPos.x * (fWidth + (nCntSet * m_fThickness));		// x
					pVtx[0].pos.y = 0.0f + vecPos.y * (fHeight + (nCntSet * m_fOuterPlusY));	// y
					pVtx[0].pos.z = 0.0f + vecPos.z * (fWidth + (nCntSet * m_fThickness));		// z

					// 法線ベクトルの設定
					pVtx[0].nor = VEC3_ZERO;

					if (fAlpha < 0.0f)
					{ // 透明度が範囲外の場合

						// 透明度を補正
						fAlpha = 0.0f;
					}

					// 頂点カラーの設定
					pVtx[0].col = COLOR(m_col.r, m_col.g, m_col.b, fAlpha);

					// テクスチャ座標の設定
					pVtx[0].tex = VECTOR2
					( // 引数
						1.0f,			// u
						1.0f * nCntSet	// v
					);

					// 頂点データのポインタを進める
					pVtx += 1;
				}

				// 縦横を広げる
				fWidth  += m_fAddWidth;
				fHeight += m_fAddHeight;

				// α値を減算
				fAlpha -= m_fSubAlpha;
			}
		}

		// 頂点バッファをアンロックする
		m_pVtxBuff->Unlock();
	}
	else { assert(false); }	// 非使用中
}

//============================================================
//	通常描画処理
//============================================================
void CObjectTornado::DrawNormal()
{
	LPDIRECT3DDEVICE9 pDevice = GET_DEVICE;	// デバイスのポインタ

	// テクスチャの設定
	pDevice->SetTexture(0, GET_MANAGER->GetTexture()->GetPtr(m_nTextureIdx));

	// ポリゴンの描画
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, m_nNumVtx - 2);
}

//============================================================
//	シェーダー描画処理
//============================================================
void CObjectTornado::DrawShader(CShader* pShader)
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
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, m_nNumVtx - 2);

	// 描画終了
	pShader->EndPass();
	pShader->End();
}
