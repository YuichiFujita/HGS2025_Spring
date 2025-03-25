//============================================================
//
//	ランキング画面処理 [sceneRanking.cpp]
//	Author：藤田勇一
//
//============================================================
//************************************************************
//	インクルードファイル
//************************************************************
#include "sceneRanking.h"
#include "manager.h"
#include "sound.h"
#include "camera.h"
#include "rankingManager.h"

//************************************************************
//	静的メンバ変数宣言
//************************************************************
CRankingManager* CSceneRanking::m_pRankingManager = nullptr;	// ランキングマネージャー

//************************************************************
//	子クラス [CSceneRanking] のメンバ関数
//************************************************************
//============================================================
//	コンストラクタ
//============================================================
CSceneRanking::CSceneRanking(const EMode mode) : CScene(mode)
{

}

//============================================================
//	デストラクタ
//============================================================
CSceneRanking::~CSceneRanking()
{

}

//============================================================
//	初期化処理
//============================================================
HRESULT CSceneRanking::Init()
{
	// シーンの初期化
	if (FAILED(CScene::Init()))
	{ // 初期化に失敗した場合

		assert(false);
		return E_FAIL;
	}

	// ランキングマネージャーの生成
	m_pRankingManager = CRankingManager::Create();
	if (m_pRankingManager == nullptr)
	{ // 生成に失敗した場合

		assert(false);
		return E_FAIL;
	}

	// TODO：カメラ設定
#if 0
	// 固定カメラにする
	CCamera* pCamera = GET_MANAGER->GetCamera();	// カメラ情報
	pCamera->SetState(CCamera::STATE_NONE);			// 固定状態を設定
#endif

	// BGMの再生
	PLAY_SOUND(CSound::LABEL_BGM_GENERAL);

	return S_OK;
}

//============================================================
//	終了処理
//============================================================
void CSceneRanking::Uninit()
{
	// ランキングマネージャーの破棄
	SAFE_REF_RELEASE(m_pRankingManager);

	// シーンの終了
	CScene::Uninit();
}

//============================================================
//	更新処理
//============================================================
void CSceneRanking::Update(const float fDeltaTime)
{
	// ランキングマネージャーの更新
	assert(m_pRankingManager != nullptr);
	m_pRankingManager->Update(fDeltaTime);
}

//============================================================
//	ランキングマネージャー取得処理
//============================================================
CRankingManager* CSceneRanking::GetRankingManager()
{
	// インスタンス未使用
	assert(m_pRankingManager != nullptr);

	// ランキングマネージャーのポインタを返す
	return m_pRankingManager;
}
