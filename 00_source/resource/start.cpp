//============================================================
//
//	開始演出処理 [start.cpp]
//	Author：藤田勇一
//
//============================================================
//************************************************************
//	インクルードファイル
//************************************************************
#include "start.h"

#include "manager.h"
#include "sound.h"

//************************************************************
//	定数宣言
//************************************************************
namespace
{
	const char*		TEXTURE = "data\\TEXTURE\\start000.png";	// テクスチャパス
	const VECTOR3	POS = SCREEN_CENT;	// 位置
	const float		WIDTH = 1020.0f;	// 横幅
	const float		SCALE_TIME = 0.8f;	// 演出時間
	const float		ALPHA_TIME = 2.5f;	// 演出時間
}

//************************************************************
//	子クラス [CStart] のメンバ関数
//************************************************************
//============================================================
//	コンストラクタ
//============================================================
CStart::CStart() :
	m_fStateTime	(0.0f)	// 経過時間
{

}

//============================================================
//	デストラクタ
//============================================================
CStart::~CStart()
{

}

//============================================================
//	初期化処理
//============================================================
HRESULT CStart::Init()
{
	// メンバ変数を初期化
	m_fStateTime = 0.0f;	// 経過時間

	// オブジェクト2Dの初期化
	if (FAILED(CObject2D::Init()))
	{ // 初期化に失敗した場合

		assert(false);
		return E_FAIL;
	}

	// テクスチャの割当
	BindTexture(TEXTURE);

	// 位置を中心にする
	SetVec3Position(SCREEN_CENT);

	// 大きさの設定
	SetVec3Size(VECTOR3(0.0f, 0.0f, 0.0f));

	return S_OK;
}

//============================================================
//	終了処理
//============================================================
void CStart::Uninit()
{
	// 開始の音
	//PLAY_SOUND(CSound::LABEL_SE_START);

	// オブジェクト2Dの終了
	CObject2D::Uninit();
}

//============================================================
//	更新処理
//============================================================
void CStart::Update(const float fDeltaTime)
{
	// 経過時間を加算
	m_fStateTime += fDeltaTime;

	// 線形保管で大きさ調整
	float fWidth = EaseOutBack(0.0f, WIDTH, 0.0f, SCALE_TIME, m_fStateTime);

	// 大きさの設定
	float fTitleHeight = useful::GetTexHeightFromAspect(fWidth, TEXTURE);	// テクスチャ基準の縦幅
	SetVec3Size(VECTOR3(fWidth, fTitleHeight, 0.0f));

	// 線形保管で透明度調整
	float fAlpha = EasingCubicInOut(1.0f, 0.0f, SCALE_TIME, ALPHA_TIME, m_fStateTime);

	// 透明度の設定
	SetAlpha(fAlpha);

	// オブジェクト2Dの更新
	CObject2D::Update(fDeltaTime);
}

//============================================================
//	描画処理
//============================================================
void CStart::Draw(CShader* pShader)
{
	// オブジェクト2Dの描画
	CObject2D::Draw(pShader);
}

//============================================================
//	生成処理
//============================================================
CStart* CStart::Create()
{
	// 開始演出の生成
	CStart* pStart = new CStart;
	if (pStart == nullptr)
	{ // 生成に失敗した場合

		return nullptr;
	}
	else
	{ // 生成に成功した場合

		// 開始演出の初期化
		if (FAILED(pStart->Init()))
		{ // 初期化に失敗した場合

			// 開始演出の破棄
			SAFE_DELETE(pStart);
			return nullptr;
		}

		// 確保したアドレスを返す
		return pStart;
	}
}

//============================================================
//	演出終了フラグ取得処理
//============================================================
bool CStart::IsStagEnd() const
{
	// 時間が経過しきっている場合演出終了
	return (m_fStateTime >= ALPHA_TIME);
}
