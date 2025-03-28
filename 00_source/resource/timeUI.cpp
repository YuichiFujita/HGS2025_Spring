//============================================================
//
//	タイムUI処理 [timeUI.cpp]
//	Author：藤田勇一
//
//============================================================
//************************************************************
//	インクルードファイル
//************************************************************
#include "timeUI.h"
#include "manager.h"
#include "renderer.h"
#include "texture.h"
#include "timer.h"
#include "value.h"
#include "object2D.h"

//************************************************************
//	定数宣言
//************************************************************
namespace
{
	const char* TEXTURE_FILE[] =	// テクスチャファイル
	{
		"data\\TEXTURE\\timePart000.png",	// 通常区切り
	};

	const int PRIORITY = 6;	// タイムUIの優先順位
}

//************************************************************
//	スタティックアサート
//************************************************************
static_assert(NUM_ARRAY(TEXTURE_FILE) == CValue::TYPE_MAX, "ERROR : Type Count Mismatch");

//************************************************************
//	子クラス [CTimeUI] のメンバ関数
//************************************************************
//============================================================
//	コンストラクタ
//============================================================
CTimeUI::CTimeUI() : CObject(CObject::LABEL_UI, CObject::DIM_2D, PRIORITY),
	m_type			(CValue::TYPE_NORMAL),	// 数字種類
	m_pos			(VEC3_ZERO),			// 原点位置
	m_rot			(VEC3_ZERO),			// 原点向き
	m_sizeValue		(VEC3_ZERO),			// 数字の大きさ
	m_sizePart		(VEC3_ZERO),			// 区切りの大きさ
	m_spaceValue	(VEC3_ZERO),			// 数字の空白
	m_spacePart		(VEC3_ZERO),			// 区切りの空白
	m_col			(color::White()),		// 色
	m_alignX		(XALIGN_CENTER),		// 横配置
	m_alignY		(YALIGN_CENTER),		// 縦配置
	m_fTime			(0.0f)					// 表示時間
{
	// メンバ変数をクリア
	memset(&m_apValue[0], 0, sizeof(m_apValue));	// 数値の情報
	memset(&m_apPart[0], 0, sizeof(m_apPart));		// 区切りの情報
}

//============================================================
//	デストラクタ
//============================================================
CTimeUI::~CTimeUI()
{

}

//============================================================
//	初期化処理
//============================================================
HRESULT CTimeUI::Init()
{
	// メンバ変数を初期化
	memset(&m_apValue[0], 0, sizeof(m_apValue));	// 数値の情報
	memset(&m_apPart[0], 0, sizeof(m_apPart));		// 区切りの情報
	m_type			= CValue::TYPE_NORMAL;			// 数字種類
	m_pos			= VEC3_ZERO;		// 原点位置
	m_rot			= VEC3_ZERO;		// 原点向き
	m_sizeValue		= VEC3_ZERO;		// 数字の大きさ
	m_sizePart		= VEC3_ZERO;		// 区切りの大きさ
	m_spaceValue	= VEC3_ZERO;		// 数字の空白
	m_spacePart		= VEC3_ZERO;		// 区切りの空白
	m_col			= color::White();	// 色
	m_alignX		= XALIGN_CENTER;	// 横配置
	m_alignY		= YALIGN_CENTER;	// 縦配置
	m_fTime			= 0.0f;				// 表示時間

	for (int nCntValue = 0; nCntValue < CTimeUI::MAX_DIGIT; nCntValue++)
	{ // 数字の数分繰り返す

		// 数字の生成
		m_apValue[nCntValue] = CValue::Create();
		if (m_apValue[nCntValue] == nullptr)
		{ // 生成に失敗した場合

			assert(false);
			return E_FAIL;
		}
	}

	for (int nCntPart = 0; nCntPart < CTimeUI::MAX_PART; nCntPart++)
	{ // 区切りの数分繰り返す

		// 区切りの生成
		m_apPart[nCntPart] = CObject2D::Create(VEC3_ZERO);
		if (m_apPart[nCntPart] == nullptr)
		{ // 生成に失敗した場合

			assert(false);
			return E_FAIL;
		}
	}

	// 優先順位の設定
	SetPriority(PRIORITY);

	// 更新状況の設定
	SetEnableUpdate(false);
	CObject::SetEnableUpdate(true);	// 自身はONにする

	// 描画状況の設定
	SetEnableDraw(false);
	CObject::SetEnableDraw(true);	// 自身はONにする

	return S_OK;
}

//============================================================
//	終了処理
//============================================================
void CTimeUI::Uninit()
{
	for (int nCntValue = 0; nCntValue < CTimeUI::MAX_DIGIT; nCntValue++)
	{ // 数字の数分繰り返す

		// 数字の終了
		SAFE_UNINIT(m_apValue[nCntValue]);
	}

	for (int nCntPart = 0; nCntPart < CTimeUI::MAX_PART; nCntPart++)
	{ // 区切りの数分繰り返す

		// 区切りの終了
		SAFE_UNINIT(m_apPart[nCntPart]);
	}

	// オブジェクトを破棄
	Release();
}

//============================================================
//	更新処理
//============================================================
void CTimeUI::Update(const float fDeltaTime)
{
	// 相対位置の設定
	SetPositionRelative();

	// 数字のテクスチャ座標の設定
	SetTexNum();

	for (int nCntValue = 0; nCntValue < CTimeUI::MAX_DIGIT; nCntValue++)
	{ // 数字の数分繰り返す

		// 数字の更新
		assert(m_apValue[nCntValue] != nullptr);
		m_apValue[nCntValue]->Update(fDeltaTime);
	}

	for (int nCntPart = 0; nCntPart < CTimeUI::MAX_PART; nCntPart++)
	{ // 区切りの数分繰り返す

		// 区切りの更新
		assert(m_apPart[nCntPart] != nullptr);
		m_apPart[nCntPart]->Update(fDeltaTime);
	}
}

//============================================================
//	描画処理
//============================================================
void CTimeUI::Draw(CShader* pShader)
{
	for (int nCntValue = 0; nCntValue < CTimeUI::MAX_DIGIT; nCntValue++)
	{ // 数字の数分繰り返す

		// 数字の更新
		assert(m_apValue[nCntValue] != nullptr);
		m_apValue[nCntValue]->Draw(pShader);
	}

	for (int nCntPart = 0; nCntPart < CTimeUI::MAX_PART; nCntPart++)
	{ // 区切りの数分繰り返す

		// 区切りの更新
		assert(m_apPart[nCntPart] != nullptr);
		m_apPart[nCntPart]->Draw(pShader);
	}
}

//============================================================
//	優先順位の設定処理
//============================================================
void CTimeUI::SetPriority(const int nPriority)
{
	// 自身の優先順位を設定
	CObject::SetPriority(nPriority);

	for (int nCntValue = 0; nCntValue < CTimeUI::MAX_DIGIT; nCntValue++)
	{ // 数字の数分繰り返す

		// 数字の優先順位を設定
		assert(m_apValue[nCntValue] != nullptr);
		m_apValue[nCntValue]->SetPriority(nPriority);
	}

	for (int nCntPart = 0; nCntPart < CTimeUI::MAX_PART; nCntPart++)
	{ // 区切りの数分繰り返す

		// 区切りの優先順位を設定
		assert(m_apPart[nCntPart] != nullptr);
		m_apPart[nCntPart]->SetPriority(nPriority);
	}
}

//============================================================
//	更新状況の設定処理
//============================================================
void CTimeUI::SetEnableUpdate(const bool bUpdate)
{
	// 自身の更新状況を設定
	CObject::SetEnableUpdate(bUpdate);

	for (int nCntValue = 0; nCntValue < CTimeUI::MAX_DIGIT; nCntValue++)
	{ // 数字の数分繰り返す

		// 数字の更新状況を設定
		assert(m_apValue[nCntValue] != nullptr);
		m_apValue[nCntValue]->SetEnableUpdate(bUpdate);
	}

	for (int nCntPart = 0; nCntPart < CTimeUI::MAX_PART; nCntPart++)
	{ // 区切りの数分繰り返す

		// 区切りの更新状況を設定
		assert(m_apPart[nCntPart] != nullptr);
		m_apPart[nCntPart]->SetEnableUpdate(bUpdate);
	}
}

//============================================================
//	描画状況の設定処理
//============================================================
void CTimeUI::SetEnableDraw(const bool bDraw)
{
	// 自身の描画状況を設定
	CObject::SetEnableDraw(bDraw);

	for (int nCntValue = 0; nCntValue < CTimeUI::MAX_DIGIT; nCntValue++)
	{ // 数字の数分繰り返す

		// 数字の描画状況を設定
		assert(m_apValue[nCntValue] != nullptr);
		m_apValue[nCntValue]->SetEnableDraw(bDraw);
	}

	for (int nCntPart = 0; nCntPart < CTimeUI::MAX_PART; nCntPart++)
	{ // 区切りの数分繰り返す

		// 区切りの描画状況を設定
		assert(m_apPart[nCntPart] != nullptr);
		m_apPart[nCntPart]->SetEnableDraw(bDraw);
	}
}

//============================================================
//	位置の設定処理
//============================================================
void CTimeUI::SetVec3Position(const VECTOR3& rPos)
{
	// 引数の位置を設定
	m_pos = rPos;

	// 相対位置の設定
	SetPositionRelative();
}

//============================================================
//	向きの設定処理
//============================================================
void CTimeUI::SetVec3Rotation(const VECTOR3& rRot)
{
	// 引数の向きを設定
	m_rot = rRot;

	for (int nCntValue = 0; nCntValue < CTimeUI::MAX_DIGIT; nCntValue++)
	{ // 数字の数分繰り返す

		// 数字の向きを設定
		assert(m_apValue[nCntValue] != nullptr);
		m_apValue[nCntValue]->SetVec3Rotation(rRot);
	}

	for (int nCntPart = 0; nCntPart < CTimeUI::MAX_PART; nCntPart++)
	{ // 区切りの数分繰り返す

		// 区切りの向きを設定
		assert(m_apPart[nCntPart] != nullptr);
		m_apPart[nCntPart]->SetVec3Rotation(rRot);
	}

	// 相対位置の設定
	SetPositionRelative();
}

//============================================================
//	生成処理
//============================================================
CTimeUI* CTimeUI::Create
(
	const float fTime,			// 表示時間
	const VECTOR3& rPos,		// 位置
	const VECTOR3& rSizeValue,	// 数字の大きさ
	const VECTOR3& rSizePart,	// 区切りの大きさ
	const VECTOR3& rSpaceValue,	// 数字の空白
	const VECTOR3& rSpacePart,	// 区切りの空白
	const CValue::EType type,	// 数字種類
	const EAlignX alignX,		// 横配置
	const EAlignY alignY,		// 縦配置
	const VECTOR3& rRot,		// 向き
	const COLOR& rCol			// 色
)
{
	// タイムUIの生成
	CTimeUI* pTimeUI = new CTimeUI;
	if (pTimeUI == nullptr)
	{ // 生成に失敗した場合

		return nullptr;
	}
	else
	{ // 生成に成功した場合

		// タイムUIの初期化
		if (FAILED(pTimeUI->Init()))
		{ // 初期化に失敗した場合

			// タイムUIの破棄
			SAFE_DELETE(pTimeUI);
			return nullptr;
		}

		// 表示時間を設定
		pTimeUI->SetTime(fTime);

		// 数字種類を設定
		pTimeUI->SetValueType(type);

		// 原点位置を設定
		pTimeUI->SetVec3Position(rPos);

		// 原点向きを設定
		pTimeUI->SetVec3Rotation(rRot);

		// 数字の大きさを設定
		pTimeUI->SetSizeValue(rSizeValue);

		// 区切りの大きさを設定
		pTimeUI->SetSizePart(rSizePart);

		// 数字の空白を設定
		pTimeUI->SetSpaceValue(rSpaceValue);

		// 区切りの空白を設定
		pTimeUI->SetSpacePart(rSpacePart);

		// 色を設定
		pTimeUI->SetColor(rCol);

		// 横配置を設定
		pTimeUI->SetAlignX(alignX);

		// 縦配置を設定
		pTimeUI->SetAlignY(alignY);

		// 確保したアドレスを返す
		return pTimeUI;
	}
}

//============================================================
//	表示時間の設定処理
//============================================================
void CTimeUI::SetTime(const float fTime)
{
	// 引数の表示時間を設定
	m_fTime = fTime;

	// 表示時間の補正
	useful::LimitNum(m_fTime, CTimer::TIME_MIN, CTimer::TIME_MAX);

	// 数字のテクスチャ座標の設定
	SetTexNum();
}

//============================================================
//	数字の種類の設定処理
//============================================================
void CTimeUI::SetValueType(const CValue::EType type)
{
	// 設定された数字の種類を保存
	m_type = type;

	for (int nCntValue = 0; nCntValue < CTimeUI::MAX_DIGIT; nCntValue++)
	{ // 数字の数分繰り返す

		// 数字の種類を設定
		assert(m_apValue[nCntValue] != nullptr);
		m_apValue[nCntValue]->SetType(type);
	}

	for (int nCntPart = 0; nCntPart < CTimeUI::MAX_PART; nCntPart++)
	{ // 区切りの数分繰り返す

		// テクスチャを割当
		assert(m_apPart[nCntPart] != nullptr);
		m_apPart[nCntPart]->BindTexture(TEXTURE_FILE[(int)type]);
	}
}

//============================================================
//	数字の大きさの設定処理
//============================================================
void CTimeUI::SetSizeValue(const VECTOR3& rSize)
{
	// 設定された数字の大きさを保存
	m_sizeValue = rSize;

	for (int nCntValue = 0; nCntValue < CTimeUI::MAX_DIGIT; nCntValue++)
	{ // 数字の数分繰り返す

		// 数字の大きさを設定
		assert(m_apValue[nCntValue] != nullptr);
		m_apValue[nCntValue]->SetVec3Size(rSize);
	}

	// 相対位置の設定
	SetPositionRelative();
}

//============================================================
//	区切りの大きさの設定処理
//============================================================
void CTimeUI::SetSizePart(const VECTOR3& rSize)
{
	// 設定された区切りの大きさを保存
	m_sizePart = rSize;

	for (int nCntPart = 0; nCntPart < CTimeUI::MAX_PART; nCntPart++)
	{ // 区切りの数分繰り返す

		// 区切りの大きさを設定
		assert(m_apPart[nCntPart] != nullptr);
		m_apPart[nCntPart]->SetVec3Size(rSize);
	}

	// 相対位置の設定
	SetPositionRelative();
}

//============================================================
//	数字の空白の設定処理
//============================================================
void CTimeUI::SetSpaceValue(const VECTOR3& rSpace)
{
	// 引数の数字の空白を設定
	m_spaceValue = rSpace;

	// 相対位置の設定
	SetPositionRelative();
}

//============================================================
//	区切りの空白の設定処理
//============================================================
void CTimeUI::SetSpacePart(const VECTOR3& rSpace)
{
	// 引数の区切りの空白を設定
	m_spacePart = rSpace;

	// 相対位置の設定
	SetPositionRelative();
}

//============================================================
//	透明度の設定処理
//============================================================
void CTimeUI::SetAlpha(const float fAlpha)
{
	// 引数の透明度を設定
	m_col.a = fAlpha;

	// 色の設定
	SetColor(m_col);
}

//============================================================
//	色の設定処理
//============================================================
void CTimeUI::SetColor(const COLOR& rCol)
{
	// 設定された色を保存
	m_col = rCol;

	for (int nCntValue = 0; nCntValue < CTimeUI::MAX_DIGIT; nCntValue++)
	{ // 数字の数分繰り返す

		// 数字の色を設定
		assert(m_apValue[nCntValue] != nullptr);
		m_apValue[nCntValue]->SetColor(rCol);
	}

	for (int nCntPart = 0; nCntPart < CTimeUI::MAX_PART; nCntPart++)
	{ // 区切りの数分繰り返す

		// 区切りの色を設定
		assert(m_apPart[nCntPart] != nullptr);
		m_apPart[nCntPart]->SetColor(rCol);
	}
}

//============================================================
//	横配置の設定処理
//============================================================
void CTimeUI::SetAlignX(const EAlignX align)
{
	// 引数の横配置を設定
	m_alignX = align;

	// 相対位置の設定
	SetPositionRelative();
}

//============================================================
//	縦配置の設定処理
//============================================================
void CTimeUI::SetAlignY(const EAlignY align)
{
	// 引数の縦配置を設定
	m_alignY = align;

	// 相対位置の設定
	SetPositionRelative();
}

//============================================================
//	タイム全体の横幅取得処理
//============================================================
float CTimeUI::GetTimeWidth() const
{
	float fTimeWidth = 0.0f;					// タイム全体の横幅
	int nEndNumIdx = CTimeUI::MAX_DIGIT - 1;	// 終端数字のインデックス

	// 全ての空白を加算 (最後の文字は含まない)
	fTimeWidth += m_spaceValue.x * nEndNumIdx;
	fTimeWidth += m_spacePart.x * CTimeUI::MAX_PART;

	// 先頭数字の横幅を加算
	assert(m_apValue[0] != nullptr);
	fTimeWidth += m_apValue[0]->GetVec3Size().x * 0.5f;

	// 終端数字の横幅を加算
	assert(m_apValue[nEndNumIdx] != nullptr);
	fTimeWidth += m_apValue[nEndNumIdx]->GetVec3Size().x * 0.5f;

	// タイム全体の横幅を返す
	return fTimeWidth;
}

//============================================================
//	タイム全体の縦幅取得処理
//============================================================
float CTimeUI::GetTimeHeight() const
{
	float fTimeHeight = 0.0f;					// タイム全体の縦幅
	int nEndNumIdx = CTimeUI::MAX_DIGIT - 1;	// 終端数字のインデックス

	// 全ての空白を加算 (最後の文字は含まない)
	fTimeHeight += m_spaceValue.y * nEndNumIdx;
	fTimeHeight += m_spacePart.y * CTimeUI::MAX_PART;

	// 先頭数字の縦幅を加算
	assert(m_apValue[0] != nullptr);
	fTimeHeight += m_apValue[0]->GetVec3Size().y * 0.5f;

	// 終端数字の縦幅を加算
	assert(m_apValue[nEndNumIdx] != nullptr);
	fTimeHeight += m_apValue[nEndNumIdx]->GetVec3Size().y * 0.5f;

	// タイム全体の縦幅を返す
	return fTimeHeight;
}

//============================================================
//	数字全体の大きさ取得処理
//============================================================
VECTOR3 CTimeUI::GetTimeSize() const
{
	// 数字全体の大きさを返す
	return VECTOR3(GetTimeWidth(), GetTimeHeight(), 0.0f);
}

//============================================================
//	相対位置の設定処理
//============================================================
void CTimeUI::SetPositionRelative()
{
	int nValueIdx = 0;	// 数字の生成数
	int nPartIdx = 0;	// 区切りの生成数
	VECTOR3 spaceValue = m_spaceValue * 0.5f;	// 数字の空白
	VECTOR3 spacePart = m_spacePart * 0.5f;		// 区切りの空白
	VECTOR3 sizeTime = GetTimeSize() * 0.5f;	// タイム全体の大きさ
	VECTOR3 sizeHead = m_apValue[0]->GetVec3Size() * 0.5f;	// 先頭数字の大きさ
	VECTOR3 rotStart = VECTOR3(m_rot.z + HALF_PI, m_rot.z, 0.0f);	// 文字の開始向き

	VECTOR3 posOffset = VEC3_ZERO;	// 文字の開始オフセット
	posOffset.x = -sizeTime.x - spaceValue.x + sizeHead.x - (sizeTime.x * (m_alignX - 1));	// 開始オフセットX
	posOffset.y = -sizeTime.y - spaceValue.y + sizeHead.y - (sizeTime.y * (m_alignY - 1));	// 開始オフセットY

	VECTOR3 posStart = VEC3_ZERO;	// 文字の開始位置
	posStart.x = m_pos.x + sinf(rotStart.x) * posOffset.x + sinf(rotStart.y) * posOffset.y;	// 開始位置X
	posStart.y = m_pos.y + cosf(rotStart.x) * posOffset.x + cosf(rotStart.y) * posOffset.y;	// 開始位置Y

	for (int nCntTimer = 0; nCntTimer < CTimeUI::MAX_DIGIT + CTimeUI::MAX_PART; nCntTimer++)
	{ // 数字の数 + 区切りの数分繰り返す

		if (nCntTimer == CTimeUI::MAX_MIN || nCntTimer == CTimeUI::MAX_MIN + CTimeUI::MAX_SEC + 1)
		{ // 区切りタイミングの場合

			assert(m_apPart[nValueIdx] != nullptr);

			// ポリゴン生成位置をずらす
			posStart.x += sinf(rotStart.x) * spacePart.x + sinf(rotStart.y) * spacePart.y;
			posStart.y += cosf(rotStart.x) * spacePart.x + cosf(rotStart.y) * spacePart.y;

			// 区切りの位置を設定
			m_apPart[nValueIdx]->SetVec3Position(posStart);

			// ポリゴン生成位置をずらす
			posStart.x += sinf(rotStart.x) * spacePart.x + sinf(rotStart.y) * spacePart.y;
			posStart.y += cosf(rotStart.x) * spacePart.x + cosf(rotStart.y) * spacePart.y;

			// 区切りの生成数を加算
			nValueIdx++;
		}
		else
		{ // 数字タイミングの場合

			assert(m_apValue[nPartIdx] != nullptr);

			// ポリゴン生成位置をずらす
			posStart.x += sinf(rotStart.x) * spaceValue.x + sinf(rotStart.y) * spaceValue.y;
			posStart.y += cosf(rotStart.x) * spaceValue.x + cosf(rotStart.y) * spaceValue.y;

			// 数字の位置を設定
			m_apValue[nPartIdx]->SetVec3Position(posStart);

			// ポリゴン生成位置をずらす
			posStart.x += sinf(rotStart.x) * spaceValue.x + sinf(rotStart.y) * spaceValue.y;
			posStart.y += cosf(rotStart.x) * spaceValue.x + cosf(rotStart.y) * spaceValue.y;

			// 数字の生成数を加算
			nPartIdx++;
		}
	}
}

//============================================================
//	数字のテクスチャ座標の設定処理
//============================================================
void CTimeUI::SetTexNum()
{
	int aNumDivide[CTimeUI::MAX_DIGIT];	// 数値分解用

	// 分を桁数ごとに分解
	useful::DivideDigitNum(&aNumDivide[0], GetMin(), CTimeUI::MAX_MIN);

	// 秒を桁数ごとに分解
	useful::DivideDigitNum(&aNumDivide[CTimeUI::MAX_MIN], GetSec(), CTimeUI::MAX_SEC);

	// ミリ秒を桁数ごとに分解
	useful::DivideDigitNum(&aNumDivide[CTimeUI::MAX_MIN + CTimeUI::MAX_SEC], GetMSec(), CTimeUI::MAX_MSEC);

	for (int nCntValue = 0; nCntValue < CTimeUI::MAX_DIGIT; nCntValue++)
	{ // 数字の数分繰り返す

		// 数字の設定
		m_apValue[nCntValue]->SetNumber(aNumDivide[nCntValue]);
	}
}
