//============================================================
//
//	爆発処理 [explosion.cpp]
//	Author：藤田勇一
//
//============================================================
//************************************************************
//	インクルードファイル
//************************************************************
#include "explosion.h"

//************************************************************
//	定数宣言
//************************************************************
namespace
{
	const char* TEXTURE_FILE[] =	// テクスチャファイル
	{
		"data\\TEXTURE\\explosion000.png",	// 火テクスチャ
		"data\\TEXTURE\\explosion001.png",	// 煙テクスチャ
	};
	const POSGRID2 PTRN[] =		// 分割数
	{
		POSGRID2(10, 1),	// 火分割数
		POSGRID2(8, 1),		// 煙分割数
	};
	const float NEXT_TIME[] =	// 遷移時間
	{
		0.06f,	// 火遷移時間
		0.08f,	// 煙遷移時間
	};

	const int PRIORITY = 6;	// 爆発の優先順位
}

//************************************************************
//	スタティックアサート
//************************************************************
static_assert(NUM_ARRAY(TEXTURE_FILE) == CExplosion::TYPE_MAX, "ERROR : Type Count Mismatch");

//************************************************************
//	子クラス [CExplosion] のメンバ関数
//************************************************************
//============================================================
//	コンストラクタ
//============================================================
CExplosion::CExplosion() : CAnim3D(CObject::LABEL_EXPLOSION, CObject::DIM_3D, PRIORITY),
	m_type	(TYPE_FIRE)	// 種類
{

}

//============================================================
//	デストラクタ
//============================================================
CExplosion::~CExplosion()
{

}

//============================================================
//	初期化処理
//============================================================
HRESULT CExplosion::Init()
{
	// メンバ変数を初期化
	m_type = TYPE_FIRE;	// 種類

	// アニメーション3Dの初期化
	if (FAILED(CAnim3D::Init()))
	{ // 初期化に失敗した場合

		// 失敗を返す
		assert(false);
		return E_FAIL;
	}

	// テクスチャを割当
	BindTexture(TEXTURE_FILE[m_type]);

	// テクスチャ分割数設定
	SetTexPtrn(PTRN[m_type]);

	// パターン変更時間設定
	SetNextTime(NEXT_TIME[m_type]);

	// 再生開始
	SetEnablePlay(true);

	// 成功を返す
	return S_OK;
}

//============================================================
//	終了処理
//============================================================
void CExplosion::Uninit()
{
	// アニメーション3Dの終了
	CAnim3D::Uninit();
}

//============================================================
//	更新処理
//============================================================
void CExplosion::Update(const float fDeltaTime)
{
	// アニメーション3Dの更新
	CAnim3D::Update(fDeltaTime);
	if (GetLoopAnim() > 0)
	{ // 一度ループしきった場合

		// 自身の終了
		Uninit();
		return;
	}
}

//============================================================
//	描画処理
//============================================================
void CExplosion::Draw(CShader* pShader)
{
	// アニメーション3Dの描画
	CAnim3D::Draw(pShader);
}

//============================================================
//	生成処理
//============================================================
CExplosion* CExplosion::Create
(
	const EType type,		// 種類
	const VECTOR3& rPos,	// 位置
	const VECTOR3& rSize	// 大きさ
)
{
	// 爆発の生成
	CExplosion* pExplosion = new CExplosion;
	if (pExplosion == nullptr)
	{ // 生成に失敗した場合

		return nullptr;
	}
	else
	{ // 生成に成功した場合

		// 爆発の初期化
		if (FAILED(pExplosion->Init()))
		{ // 初期化に失敗した場合

			// 爆発の破棄
			SAFE_DELETE(pExplosion);
			return nullptr;
		}

		// 種類を設定
		pExplosion->SetType(type);

		// 位置を設定
		pExplosion->SetVec3Position(rPos);

		// 大きさを設定
		pExplosion->SetVec3Size(rSize);

		// 確保したアドレスを返す
		return pExplosion;
	}
}

//============================================================
//	種類の設定処理
//============================================================
void CExplosion::SetType(const EType type)
{
	if (type > NONE_IDX && type < TYPE_MAX)
	{ // インデックスが範囲内の場合

		// 種類を保存
		m_type = type;

		// テクスチャを割当
		BindTexture(TEXTURE_FILE[m_type]);

		// テクスチャ分割数設定
		SetTexPtrn(PTRN[m_type]);

		// パターン変更時間設定
		SetNextTime(NEXT_TIME[m_type]);
	}
	else { assert(false); }	// 範囲外
}
