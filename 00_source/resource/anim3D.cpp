//============================================================
//
//	アニメーション3D処理 [anim3D.cpp]
//	Author：藤田勇一
//
//============================================================
//************************************************************
//	インクルードファイル
//************************************************************
#include "anim3D.h"

//************************************************************
//	定数宣言
//************************************************************
namespace
{
	const POSGRID2 INIT_PTRN = GRID2_ONE;	// テクスチャ分割数の初期値
}

//************************************************************
//	子クラス [CAnim3D] のメンバ関数
//************************************************************
//============================================================
//	コンストラクタ
//============================================================
CAnim3D::CAnim3D(const CObject::ELabel label, const EDim dimension, const int nPriority) : CObject3D(label, dimension, nPriority),
	m_funcPattern	(nullptr),		// パターン変更関数ポインタ
	m_ptrn			(GRID2_ZERO),	// テクスチャ分割数
	m_pNextTime		(nullptr),		// パターン変更時間
	m_fCurTime		(0.0f),			// 現在の待機時間
	m_fCurWholeTime	(0.0f),			// 現在の全体時間
	m_fMaxWholeTime	(0.0f),			// 総全体時間
	m_nCurPtrn		(0),			// 現在のパターン
	m_nMaxPtrn		(0),			// パターンの総数
	m_nNumLoop		(0),			// パターン繰り返し数
	m_bPlay			(false),		// 再生フラグ
	m_bPlayBack		(false),		// 逆再生フラグ
	m_bLoop			(false),		// ループフラグ
	m_bFinish		(false)			// 終了フラグ
{

}

//============================================================
//	デストラクタ
//============================================================
CAnim3D::~CAnim3D()
{

}

//============================================================
//	初期化処理
//============================================================
HRESULT CAnim3D::Init()
{
	// メンバ変数を初期化
	m_funcPattern	= nullptr;		// パターン変更関数ポインタ
	m_ptrn			= GRID2_ZERO;	// テクスチャ分割数
	m_pNextTime		= nullptr;		// パターン変更時間
	m_fCurTime		= 0.0f;			// 現在の待機時間
	m_fCurWholeTime	= 0.0f;			// 現在の全体時間
	m_fMaxWholeTime	= 0.0f;			// 総全体時間
	m_nCurPtrn		= 0;			// 現在のパターン
	m_nMaxPtrn		= 0;			// パターン総数
	m_nNumLoop		= 0;			// パターン繰り返し数
	m_bPlay			= true;			// 再生フラグ
	m_bPlayBack		= false;		// 逆再生フラグ
	m_bLoop			= true;			// ループフラグ
	m_bFinish		= false;		// 終了フラグ

	// テクスチャ分割数を初期化
	SetTexPtrn(INIT_PTRN);

	// 通常再生を初期化
	SetEnablePlayBack(false);

	// オブジェクト3Dの初期化
	if (FAILED(CObject3D::Init()))
	{ // 初期化に失敗した場合

		assert(false);
		return E_FAIL;
	}

	// アニメーションのテクスチャ座標の設定
	CObject3D::SetAnimTex(m_nCurPtrn, m_ptrn.x, m_ptrn.y);

	return S_OK;
}

//============================================================
//	終了処理
//============================================================
void CAnim3D::Uninit()
{
	// パターン変更時間の破棄
	SAFE_DEL_ARRAY(m_pNextTime);

	// オブジェクト3Dの終了
	CObject3D::Uninit();
}

//============================================================
//	更新処理
//============================================================
void CAnim3D::Update(const float fDeltaTime)
{
	// 停止中の場合抜ける
	if (!m_bPlay) { return; }

	// パターンの更新
	assert(m_funcPattern != nullptr);
	m_funcPattern(fDeltaTime);

	// オブジェクト3Dの更新
	CObject3D::Update(fDeltaTime);

	// アニメーションのテクスチャ座標の設定
	CObject3D::SetAnimTex(m_nCurPtrn, m_ptrn.x, m_ptrn.y);
}

//============================================================
//	描画処理
//============================================================
void CAnim3D::Draw(CShader* pShader)
{
	// オブジェクト3Dの描画
	CObject3D::Draw(pShader);
}

//============================================================
//	生成処理
//============================================================
CAnim3D* CAnim3D::Create
(
	const POSGRID2& rPtrn,	// テクスチャ分割数
	const VECTOR3& rPos,	// 位置
	const float fNextTime,	// パターン変更時間
	const VECTOR3& rSize,	// 大きさ
	const VECTOR3& rRot,	// 向き
	const COLOR& rCol		// 色
)
{
	// アニメーション3Dの生成
	CAnim3D* pAnim3D = new CAnim3D;
	if (pAnim3D == nullptr)
	{ // 生成に失敗した場合

		return nullptr;
	}
	else
	{ // 生成に成功した場合

		// アニメーション3Dの初期化
		if (FAILED(pAnim3D->Init()))
		{ // 初期化に失敗した場合

			// アニメーション3Dの破棄
			SAFE_DELETE(pAnim3D);
			return nullptr;
		}

		// テクスチャ分割数を設定
		pAnim3D->SetTexPtrn(rPtrn);

		// パターン変更時間を設定
		pAnim3D->SetNextTime(fNextTime);

		// 位置を設定
		pAnim3D->SetVec3Position(rPos);

		// 向きを設定
		pAnim3D->SetVec3Rotation(rRot);

		// 大きさを設定
		pAnim3D->SetVec3Size(rSize);

		// 色を設定
		pAnim3D->SetColor(rCol);

		// 確保したアドレスを返す
		return pAnim3D;
	}
}

//============================================================
//	位置の設定処理
//============================================================
void CAnim3D::SetVec3Position(const VECTOR3& rPos)
{
	// 位置の設定
	CObject3D::SetVec3Position(rPos);

	// アニメーションのテクスチャ座標の設定
	CObject3D::SetAnimTex(m_nCurPtrn, m_ptrn.x, m_ptrn.y);
}

//============================================================
//	向きの設定処理
//============================================================
void CAnim3D::SetVec3Rotation(const VECTOR3& rRot)
{
	// 向きの設定
	CObject3D::SetVec3Rotation(rRot);

	// アニメーションのテクスチャ座標の設定
	CObject3D::SetAnimTex(m_nCurPtrn, m_ptrn.x, m_ptrn.y);
}

//============================================================
//	大きさの設定処理
//============================================================
void CAnim3D::SetVec3Size(const VECTOR3& rSize)
{
	// 大きさの設定
	CObject3D::SetVec3Size(rSize);

	// アニメーションのテクスチャ座標の設定
	CObject3D::SetAnimTex(m_nCurPtrn, m_ptrn.x, m_ptrn.y);
}

//============================================================
//	透明度の設定処理
//============================================================
void CAnim3D::SetAlpha(const float fAlpha)
{
	// 透明度の設定
	CObject3D::SetAlpha(fAlpha);

	// アニメーションのテクスチャ座標の設定
	CObject3D::SetAnimTex(m_nCurPtrn, m_ptrn.x, m_ptrn.y);
}

//============================================================
//	色の設定処理
//============================================================
void CAnim3D::SetColor(const COLOR& rCol)
{
	// 色の設定
	CObject3D::SetColor(rCol);

	// アニメーションのテクスチャ座標の設定
	CObject3D::SetAnimTex(m_nCurPtrn, m_ptrn.x, m_ptrn.y);
}

//============================================================
//	現在パターンの設定処理
//============================================================
void CAnim3D::SetCurPtrn(const int nPtrn)
{
	// 引数のパターン数を設定
	m_nCurPtrn = nPtrn;

	// 終了フラグをOFFにする
	m_bFinish = false;

	// 全体時間の初期化
	m_fCurWholeTime = 0.0f;

	// 全体時間をパターンの開始時間に設定
	int nAddTime = (!m_bPlayBack) ? 0 : 1;	// 逆再生の場合は現在パターンの待機時間も含む
	int nLoop = m_nCurPtrn + nAddTime;		// 繰り返し数を求める
	for (int i = 0; i < nLoop; i++)
	{ // 開始パターンまで繰り返す

		// 待機時間を加算
		m_fCurWholeTime += m_pNextTime[i];
	}

	// アニメーションのテクスチャ座標の設定
	CObject3D::SetAnimTex(m_nCurPtrn, m_ptrn.x, m_ptrn.y);
}

//============================================================
//	テクスチャ分割数の設定処理
//============================================================
void CAnim3D::SetTexPtrn(const POSGRID2& rPtrn)
{
	// 引数のテクスチャ分割数を設定
	m_ptrn = rPtrn;

	// パターン総数を設定
	SetMaxPtrn(m_ptrn.x * m_ptrn.y);
}

//============================================================
//	テクスチャ横分割数の設定処理
//============================================================
void CAnim3D::SetTexPtrnWidth(const int nTexPtrnW)
{
	// 引数のテクスチャ横分割数を設定
	m_ptrn.x = nTexPtrnW;

	// パターン総数を設定
	SetMaxPtrn(m_ptrn.x * m_ptrn.y);
}

//============================================================
//	テクスチャ縦分割数の設定処理
//============================================================
void CAnim3D::SetTexPtrnHeight(const int nTexPtrnH)
{
	// 引数のテクスチャ縦分割数を設定
	m_ptrn.y = nTexPtrnH;

	// パターン総数を設定
	SetMaxPtrn(m_ptrn.x * m_ptrn.y);
}

//============================================================
//	再生フラグの設定処理
//============================================================
void CAnim3D::SetEnablePlay(const bool bPlay)
{
	// 停止した場合に繰り返し数を初期化
	if (!bPlay) { m_nNumLoop = 0; }

	// 引数の再生状況を設定
	m_bPlay = bPlay;
}

//============================================================
//	逆再生フラグの設定処理
//============================================================
void CAnim3D::SetEnablePlayBack(const bool bPlayBack)
{
	// 再生/逆再生が反転した場合に繰り返し数と終了フラグを初期化
	if (m_bPlayBack != bPlayBack) { m_nNumLoop = 0; m_bFinish = false; }

	// 引数の逆再生状況を設定
	m_bPlayBack = bPlayBack;

	if (!m_bPlayBack)
	{ // 通常再生の場合

		// パターン加算関数を設定
		m_funcPattern = std::bind(&CAnim3D::NextPtrn, this, std::placeholders::_1);
	}
	else
	{ // 逆再生の場合

		// パターン減算関数を設定
		m_funcPattern = std::bind(&CAnim3D::BackPtrn, this, std::placeholders::_1);
	}
}

//============================================================
//	ループフラグの設定処理
//============================================================
void CAnim3D::SetEnableLoop(const bool bLoop)
{
	// ループする場合終了フラグを初期化
	if (bLoop) { m_bFinish = false; }

	// 引数のループ状況を設定
	m_bLoop = bLoop;
}

//============================================================
//	現在パターンの初期化処理
//============================================================
void CAnim3D::ResetCurPtrn()
{
	// 終了フラグをOFFにする
	m_bFinish = false;

	if (!m_bPlayBack)
	{ // 通常再生の場合

		// 開始パターンの初期化
		m_nCurPtrn = 0;

		// 待機時間の初期化
		m_fCurTime = 0.0f;
		m_fCurWholeTime = 0.0f;
	}
	else
	{ // 逆再生の場合

		// 開始パターンの初期化
		m_nCurPtrn = m_nMaxPtrn - 1;

		// 待機時間の初期化
		m_fCurTime = m_pNextTime[m_nCurPtrn];
		m_fCurWholeTime = m_fMaxWholeTime;
	}
}

//============================================================
//	パターン変更時間の設定処理 (パターン指定)
//============================================================
void CAnim3D::SetNextTime(const int nPtrnIdx, const float fNextTime)
{
	// パターンインデックスが範囲外の場合抜ける
	if (nPtrnIdx <= NONE_IDX || nPtrnIdx >= m_nMaxPtrn) { assert(false); return; }

	// 変更時間がプラスではない場合抜ける
	if (fNextTime <= 0.0f) { assert(false); return; }

	// 変更前のパターン変更時間を保存
	float fOldNextTime = m_pNextTime[nPtrnIdx];

	// 引数のパターン変更時間を設定
	m_pNextTime[nPtrnIdx] = fNextTime;

	// 総全体時間を変更
	m_fMaxWholeTime -= fOldNextTime;
	m_fMaxWholeTime += fNextTime;
}

//============================================================
//	パターン変更時間の設定処理 (全パターン)
//============================================================
void CAnim3D::SetNextTime(const float fNextTime)
{
	// 変更時間がプラスではない場合抜ける
	if (fNextTime <= 0.0f) { assert(false); return; }

	for (int i = 0; i < m_nMaxPtrn; i++)
	{ // パターンの総数分繰り返す

		// 引数のパターン変更時間を設定
		m_pNextTime[i] = fNextTime;
	}

	// 総全体時間を変更
	m_fMaxWholeTime = fNextTime * (float)m_nMaxPtrn;
}

//============================================================
//	パターン総数の設定処理
//============================================================
HRESULT CAnim3D::SetMaxPtrn(const int nMaxPtrn)
{
	// 引数のパターンの総数を設定
	m_nMaxPtrn = nMaxPtrn;

	// パターン変更時間の破棄
	SAFE_DEL_ARRAY(m_pNextTime);

	// パターン変更時間の再生成
	m_pNextTime = new float[m_nMaxPtrn];
	if (m_pNextTime == nullptr)
	{ // 生成に失敗した場合

		assert(false);
		return E_FAIL;
	}

	// パターン変更時間を初期化
	SetNextTime(DEF_NEXT);

	// アニメーションのテクスチャ座標の設定
	CObject3D::SetAnimTex(m_nCurPtrn, m_ptrn.x, m_ptrn.y);

	return S_OK;
}

//============================================================
//	パターン加算処理
//============================================================
void CAnim3D::NextPtrn(const float fDeltaTime)
{
	// アニメーションが終了している場合抜ける
	if (m_bFinish) { return; }

	// 現在の待機時間を加算
	m_fCurTime += fDeltaTime;
	m_fCurWholeTime += fDeltaTime;

	while (m_fCurTime >= m_pNextTime[m_nCurPtrn])
	{ // 待機し終わった場合

		// 現在の待機時間から今回の待機時間を減算
		m_fCurTime -= m_pNextTime[m_nCurPtrn];

		// パターンを加算
		m_nCurPtrn = (m_nCurPtrn + 1) % m_nMaxPtrn;
		if (m_nCurPtrn == 0)
		{ // 先頭パターンの場合

			if (m_bLoop)
			{ // ループする場合

				// 繰り返し数を加算
				m_nNumLoop++;

				// 全体時間を初期化
				m_fCurWholeTime = 0.0f;
			}
			else
			{ // ループしない場合

				// パターンを補正
				m_nCurPtrn = m_nMaxPtrn - 1;

				// 全体時間を初期化
				m_fCurWholeTime = m_fMaxWholeTime;

				// アニメーション終了を保存
				m_bFinish = true;
			}
		}
	}
}

//============================================================
//	パターン減算処理
//============================================================
void CAnim3D::BackPtrn(const float fDeltaTime)
{
	// アニメーションが終了している場合抜ける
	if (m_bFinish) { return; }

	// 現在の待機時間を加算
	m_fCurTime -= fDeltaTime;
	m_fCurWholeTime -= fDeltaTime;

	while (m_fCurTime <= 0.0f)
	{ // 待機し終わった場合

		// 現在の待機時間から今回の待機時間を減算
		m_fCurTime += m_pNextTime[m_nCurPtrn];

		// パターンを減算
		m_nCurPtrn = (m_nCurPtrn + (m_nMaxPtrn - 1)) % m_nMaxPtrn;

		if (m_nCurPtrn == m_nMaxPtrn - 1)
		{ // 最終パターンの場合

			if (m_bLoop)
			{ // ループする場合

				// 繰り返し数を加算
				m_nNumLoop++;

				// 全体時間を初期化
				m_fCurWholeTime = m_fMaxWholeTime;
			}
			else
			{ // ループしない場合

				// パターンを補正
				m_nCurPtrn = 0;

				// 全体時間を初期化
				m_fCurWholeTime = 0.0f;

				// アニメーション終了を保存
				m_bFinish = true;
			}
		}
	}
}
