//============================================================
//
//	振動文字列2D処理 [shakeString2D.cpp]
//	Author：藤田勇一
//
//============================================================
//************************************************************
//	インクルードファイル
//************************************************************
#include "shakeString2D.h"
#include "string2D.h"

//************************************************************
//	子クラス [CShakeString2D] のメンバ関数
//************************************************************
//============================================================
//	コンストラクタ
//============================================================
CShakeString2D::CShakeString2D() :
	m_posSave		(VEC3_ZERO),	// 保存位置
	m_fShakeMove	(0.0f),			// 振動の移動量
	m_fNextTime		(0.0f),			// 次振動までの待機時間
	m_fCurTime		(0.0f)			// 現在の待機時間
{

}

//============================================================
//	デストラクタ
//============================================================
CShakeString2D::~CShakeString2D()
{

}

//============================================================
//	初期化処理
//============================================================
HRESULT CShakeString2D::Init()
{
	// メンバ変数を初期化
	m_posSave	 = VEC3_ZERO;	// 保存位置
	m_fShakeMove = 0.0f;		// 振動の移動量
	m_fNextTime	 = 0.0f;		// 次振動までの待機時間
	m_fCurTime	 = 0.0f;		// 現在の待機時間

	// 文字列2Dの初期化
	if (FAILED(CString2D::Init()))
	{ // 初期化に失敗した場合

		assert(false);
		return E_FAIL;
	}

	return S_OK;
}

//============================================================
//	終了処理
//============================================================
void CShakeString2D::Uninit()
{
	// 文字列2Dの終了
	CString2D::Uninit();
}

//============================================================
//	更新処理
//============================================================
void CShakeString2D::Update(const float fDeltaTime)
{
	// 振動の更新
	UpdateShake(fDeltaTime);

	// 文字列2Dの更新
	CString2D::Update(fDeltaTime);
}

//============================================================
//	描画処理
//============================================================
void CShakeString2D::Draw(CShader* pShader)
{
	// 文字列2Dの描画
	CString2D::Draw(pShader);
}

//============================================================
//	位置の設定処理
//============================================================
void CShakeString2D::SetVec3Position(const VECTOR3& rPos)
{
	// 親クラスの位置を設定
	CString2D::SetVec3Position(rPos);

	// 設定された位置を保存
	m_posSave = rPos;
}

//============================================================
//	生成処理 (マルチバイト文字列)
//============================================================
CShakeString2D* CShakeString2D::Create
(
	const std::string& rFilePath,	// フォントパス
	const bool bItalic,				// イタリック
	const std::string& rStr,		// 指定文字列
	const VECTOR3& rPos,			// 原点位置
	const float fNextTime,			// 文字振動の待機時間
	const float fMove,				// 振動移動量
	const float fHeight,			// 文字縦幅
	const EAlignX alignX,			// 横配置
	const VECTOR3& rRot,			// 原点向き
	const COLOR& rCol				// 色
)
{
	// 振動文字列2Dの生成
	CShakeString2D* pShakeString2D = new CShakeString2D;
	if (pShakeString2D == nullptr)
	{ // 生成に失敗した場合

		return nullptr;
	}
	else
	{ // 生成に成功した場合

		// 振動文字列2Dの初期化
		if (FAILED(pShakeString2D->Init()))
		{ // 初期化に失敗した場合

			// 振動文字列2Dの破棄
			SAFE_DELETE(pShakeString2D);
			return nullptr;
		}

		// フォントを設定
		pShakeString2D->SetFont(rFilePath, bItalic);

		// 文字列を設定
		if (FAILED(pShakeString2D->SetString(rStr)))
		{ // 設定に失敗した場合

			// 振動文字列2Dの破棄
			SAFE_DELETE(pShakeString2D);
			return nullptr;
		}

		// 原点位置を設定
		pShakeString2D->SetVec3Position(rPos);

		// 原点向きを設定
		pShakeString2D->SetVec3Rotation(rRot);

		// 色を設定
		pShakeString2D->SetColor(rCol);

		// 文字振動の待機時間を設定
		pShakeString2D->SetNextTime(fNextTime);

		// 振動の移動量を設定
		pShakeString2D->SetShakeMove(fMove);

		// 文字縦幅を設定
		pShakeString2D->SetCharHeight(fHeight);

		// 横配置を設定
		pShakeString2D->SetAlignX(alignX);

		// 確保したアドレスを返す
		return pShakeString2D;
	}
}

//============================================================
//	生成処理 (ワイド文字列)
//============================================================
CShakeString2D* CShakeString2D::Create
(
	const std::string& rFilePath,	// フォントパス
	const bool bItalic,				// イタリック
	const std::wstring& rStr,		// 指定文字列
	const VECTOR3& rPos,			// 原点位置
	const float fNextTime,			// 文字振動の待機時間
	const float fMove,				// 振動移動量
	const float fHeight,			// 文字縦幅
	const EAlignX alignX,			// 横配置
	const VECTOR3& rRot,			// 原点向き
	const COLOR& rCol				// 色
)
{
	// 振動文字列2Dの生成
	CShakeString2D* pShakeString2D = new CShakeString2D;
	if (pShakeString2D == nullptr)
	{ // 生成に失敗した場合

		return nullptr;
	}
	else
	{ // 生成に成功した場合

		// 振動文字列2Dの初期化
		if (FAILED(pShakeString2D->Init()))
		{ // 初期化に失敗した場合

			// 振動文字列2Dの破棄
			SAFE_DELETE(pShakeString2D);
			return nullptr;
		}

		// フォントを設定
		pShakeString2D->SetFont(rFilePath, bItalic);

		// 文字列を設定
		if (FAILED(pShakeString2D->SetString(rStr)))
		{ // 設定に失敗した場合

			// 振動文字列2Dの破棄
			SAFE_DELETE(pShakeString2D);
			return nullptr;
		}

		// 原点位置を設定
		pShakeString2D->SetVec3Position(rPos);

		// 原点向きを設定
		pShakeString2D->SetVec3Rotation(rRot);

		// 色を設定
		pShakeString2D->SetColor(rCol);

		// 文字振動の待機時間を設定
		pShakeString2D->SetNextTime(fNextTime);

		// 振動の移動量を設定
		pShakeString2D->SetShakeMove(fMove);

		// 文字縦幅を設定
		pShakeString2D->SetCharHeight(fHeight);

		// 横配置を設定
		pShakeString2D->SetAlignX(alignX);

		// 確保したアドレスを返す
		return pShakeString2D;
	}
}

//============================================================
//	振動の更新処理
//============================================================
void CShakeString2D::UpdateShake(const float fDeltaTime)
{
	// 現在の待機時間を加算
	m_fCurTime += fDeltaTime;
	if (m_fCurTime >= m_fNextTime)
	{ // 待機し終わった場合

		// 現在の待機時間を初期化
		m_fCurTime = 0.0f;

		// ランダムに振動させる
		VECTOR3 offset = VEC3_ZERO;	// 位置オフセット
		offset.x = m_fShakeMove * (float)(rand() % 3 - 1);
		offset.y = m_fShakeMove * (float)(rand() % 3 - 1);

		// 親クラスの位置を設定
		CString2D::SetVec3Position(m_posSave + offset);
	}
}
