//============================================================
//
//	地面処理 [field.cpp]
//	Author：藤田勇一
//
//============================================================
//************************************************************
//	インクルードファイル
//************************************************************
#include "field.h"
#include "manager.h"
#include "renderer.h"
#include "texture.h"

//************************************************************
//	定数宣言
//************************************************************
namespace
{
	const char* TEXTURE_FILE[] =	// テクスチャファイル
	{
		"data\\TEXTURE\\soil000.png",	// 土テクスチャ
	};
	const int PRIORITY = 0;	// 地面の優先順位
}

//************************************************************
//	静的メンバ変数宣言
//************************************************************
CListManager<CField>* CField::m_pList = nullptr;	// オブジェクトリスト

//************************************************************
//	スタティックアサート
//************************************************************
static_assert(NUM_ARRAY(TEXTURE_FILE) == CField::TYPE_MAX, "ERROR : Type Count Mismatch");

//************************************************************
//	子クラス [CField] のメンバ関数
//************************************************************
//============================================================
//	コンストラクタ
//============================================================
CField::CField() : CObjectMeshField(CObject::LABEL_FIELD, CObject::DIM_3D, PRIORITY),
	m_type	(TYPE_SOIL)	// 種類
{

}

//============================================================
//	デストラクタ
//============================================================
CField::~CField()
{

}

//============================================================
//	初期化処理
//============================================================
HRESULT CField::Init()
{
	// メンバ変数を初期化
	m_type = TYPE_SOIL;	// 種類

	// オブジェクトメッシュフィールドの初期化
	if (FAILED(CObjectMeshField::Init()))
	{ // 初期化に失敗した場合

		// 失敗を返す
		assert(false);
		return E_FAIL;
	}

	if (m_pList == nullptr)
	{ // リストマネージャーが存在しない場合

		// リストマネージャーの生成
		m_pList = CListManager<CField>::Create();
		if (m_pList == nullptr)
		{ // 生成に失敗した場合

			// 失敗を返す
			assert(false);
			return E_FAIL;
		}
	}

	// リストに自身のオブジェクトを追加・イテレーターを取得
	m_iterator = m_pList->AddList(this);

	// 成功を返す
	return S_OK;
}

//============================================================
//	終了処理
//============================================================
void CField::Uninit()
{
	// リストから自身のオブジェクトを削除
	m_pList->DelList(m_iterator);

	if (m_pList->GetNumAll() == 0)
	{ // オブジェクトが一つもない場合

		// リストマネージャーの破棄
		m_pList->Release(m_pList);
	}

	// オブジェクトメッシュフィールドの終了
	CObjectMeshField::Uninit();
}

//============================================================
//	更新処理
//============================================================
void CField::Update(const float fDeltaTime)
{
	// オブジェクトメッシュフィールドの更新
	CObjectMeshField::Update(fDeltaTime);
}

//============================================================
//	描画処理
//============================================================
void CField::Draw(CShader* pShader)
{
	// オブジェクトメッシュフィールドの描画
	CObjectMeshField::Draw(pShader);
}

//============================================================
//	生成処理
//============================================================
CField* CField::Create
(
	const EType type,			// 種類
	const VECTOR3& rPos,	// 位置
	const VECTOR3& rRot,	// 向き
	const VECTOR2& rSize,	// 大きさ
	const COLOR& rCol,		// 色
	const POSGRID2& rPart,		// 分割数
	const POSGRID2& rTexPart	// テクスチャ分割数
)
{
	// 地面の生成
	CField* pField = new CField;
	if (pField == nullptr)
	{ // 生成に失敗した場合

		return nullptr;
	}
	else
	{ // 生成に成功した場合

		// 地面の初期化
		if (FAILED(pField->Init()))
		{ // 初期化に失敗した場合

			// 地面の破棄
			SAFE_DELETE(pField);
			return nullptr;
		}

		// 種類を設定
		pField->SetType(type);

		// 位置を設定
		pField->SetVec3Position(rPos);

		// 向きを設定
		pField->SetVec3Rotation(rRot);

		// 大きさを設定
		pField->SetVec2Size(rSize);

		// 色を設定
		pField->SetColor(rCol);

		// 分割数を設定
		if (FAILED(pField->SetPattern(rPart)))
		{ // 分割数の設定に失敗した場合

			// 地面の破棄
			SAFE_DELETE(pField);
			return nullptr;
		}

		// テクスチャ分割数を設定
		pField->SetTexPattern(rTexPart);

		// 確保したアドレスを返す
		return pField;
	}
}

//============================================================
//	リスト取得処理
//============================================================
CListManager<CField>* CField::GetList()
{
	// オブジェクトリストを返す
	return m_pList;
}

//============================================================
//	種類の設定処理
//============================================================
void CField::SetType(const EType type)
{
	if (type > NONE_IDX && type < TYPE_MAX)
	{ // インデックスが範囲内の場合

		// 種類を保存
		m_type = type;

		// テクスチャを登録・割当
		BindTexture(GET_MANAGER->GetTexture()->Regist(TEXTURE_FILE[type]));
	}
	else { assert(false); }	// 範囲外
}
