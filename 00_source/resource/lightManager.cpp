//============================================================
//
//	ライトマネージャー処理 [lightManager.cpp]
//	Author：藤田勇一
//
//============================================================
//************************************************************
//	インクルードファイル
//************************************************************
#include "lightManager.h"

//************************************************************
//	定数宣言
//************************************************************
namespace
{
	const COLOR DIFFUSE[] =	// 設定用拡散光カラー
	{
		COLOR(1.0f, 1.0f, 1.0f, 1.0f),
		COLOR(0.7f, 0.7f, 0.7f, 1.0f),
		COLOR(0.6f, 0.6f, 0.6f, 1.0f),
		COLOR(0.4f, 0.4f, 0.4f, 1.0f),
	};

	const VECTOR3 DIRECTION[] =	// 設定用方向ベクトル
	{
		VECTOR3( 0.22f, -0.97f,  0.54f),
		VECTOR3(-0.38f,  0.78f, -0.54f),
		VECTOR3( 0.89f, -0.21f,  0.44f),
		VECTOR3(-0.96f,  0.15f, -0.44f),
	};
}

//************************************************************
//	スタティックアサート
//************************************************************
static_assert(NUM_ARRAY(DIFFUSE)   == CLightManager::MAX_NUM, "ERROR : Light Count Mismatch");
static_assert(NUM_ARRAY(DIRECTION) == CLightManager::MAX_NUM, "ERROR : Light Count Mismatch");

//************************************************************
//	親クラス [CLightManager] のメンバ関数
//************************************************************
//============================================================
//	コンストラクタ
//============================================================
CLightManager::CLightManager()
{
	// メンバ変数をクリア
	memset(&m_apLight[0], 0, sizeof(m_apLight));	// ライト情報
}

//============================================================
//	デストラクタ
//============================================================
CLightManager::~CLightManager()
{

}

//============================================================
//	初期化処理
//============================================================
HRESULT CLightManager::Init()
{
	for (int i = 0; i < MAX_NUM; i++)
	{ // 使用するライト数分繰り返す

		// ライトの生成
		m_apLight[i] = CLightDir::Create();
		if (m_apLight[i] == nullptr)
		{ // 生成に失敗した場合

			assert(false);
			return E_FAIL;
		}

		// ライトのラベルを変更
		m_apLight[i]->SetLabel(CObject::LABEL_NONE);	// 自動破棄/自動更新を停止

		// ライトの拡散光を設定
		m_apLight[i]->SetDiffuse(DIFFUSE[i]);

		// ライトの方向を設定
		m_apLight[i]->SetDirection(DIRECTION[i]);
	}

	return S_OK;
}

//============================================================
//	終了処理
//============================================================
void CLightManager::Uninit()
{
	for (int i = 0; i < MAX_NUM; i++)
	{ // 使用するライト数分繰り返す

		// ライトの終了
		SAFE_UNINIT(m_apLight[i]);
	}
}

//============================================================
//	更新処理
//============================================================
void CLightManager::Update(const float fDeltaTime)
{
	for (int i = 0; i < MAX_NUM; i++)
	{ // 使用するライト数分繰り返す

		// ライトの更新
		m_apLight[i]->Update(fDeltaTime);
	}
}

//============================================================
//	生成処理
//============================================================
CLightManager* CLightManager::Create()
{
	// ライトマネージャーの生成
	CLightManager* pLightManager = new CLightManager;
	if (pLightManager == nullptr)
	{ // 生成に失敗した場合

		return nullptr;
	}
	else
	{ // 生成に成功した場合

		// ライトマネージャーの初期化
		if (FAILED(pLightManager->Init()))
		{ // 初期化に失敗した場合

			// ライトマネージャーの破棄
			SAFE_DELETE(pLightManager);
			return nullptr;
		}

		// 確保したアドレスを返す
		return pLightManager;
	}
}

//============================================================
//	破棄処理
//============================================================
void CLightManager::Release(CLightManager*& prLightManager)
{
	// ライトマネージャーの終了
	assert(prLightManager != nullptr);
	prLightManager->Uninit();

	// メモリ開放
	SAFE_DELETE(prLightManager);
}
