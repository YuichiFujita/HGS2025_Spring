//============================================================
//
//	タイトル画面処理 [sceneTitle.cpp]
//	Author：藤田勇一
//
//============================================================
//************************************************************
//	インクルードファイル
//************************************************************
#include "sceneTitle.h"
#include "manager.h"
#include "texture.h"
#include "sound.h"
#include "camera.h"
#include "titleManager.h"

//************************************************************
//	静的メンバ変数宣言
//************************************************************
CTitleManager* CSceneTitle::m_pTitleManager = nullptr;	// タイトルマネージャー

//************************************************************
//	子クラス [CSceneTitle] のメンバ関数
//************************************************************
//============================================================
//	コンストラクタ
//============================================================
CSceneTitle::CSceneTitle(const EMode mode) : CScene(mode)
{

}

//============================================================
//	デストラクタ
//============================================================
CSceneTitle::~CSceneTitle()
{

}

//============================================================
//	初期化処理
//============================================================
HRESULT CSceneTitle::Init()
{
	// シーンの初期化
	if (FAILED(CScene::Init()))
	{ // 初期化に失敗した場合

		assert(false);
		return E_FAIL;
	}

	// タイトルマネージャーの生成
	m_pTitleManager = CTitleManager::Create();
	if (m_pTitleManager == nullptr)
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
	PLAY_SOUND(CSound::LABEL_BGM_TITLE);

	return S_OK;
}

//============================================================
//	終了処理
//============================================================
void CSceneTitle::Uninit()
{
	// タイトルマネージャーの破棄
	SAFE_REF_RELEASE(m_pTitleManager);

	// シーンの終了
	CScene::Uninit();
}

//============================================================
//	更新処理
//============================================================
void CSceneTitle::Update(const float fDeltaTime)
{
	// タイトルマネージャーの更新
	assert(m_pTitleManager != nullptr);
	m_pTitleManager->Update(fDeltaTime);

	// シーンの更新
	CScene::Update(fDeltaTime);
}

//============================================================
//	タイトルマネージャー取得処理
//============================================================
CTitleManager* CSceneTitle::GetTitleManager()
{
	// インスタンス未使用
	assert(m_pTitleManager != nullptr);

	// タイトルマネージャーを返す
	return m_pTitleManager;
}
