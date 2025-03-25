//============================================================
//
//	シーン処理 [scene.cpp]
//	Author：藤田勇一
//
//============================================================
//************************************************************
//	インクルードファイル
//************************************************************
#include "scene.h"
#include "sceneTitle.h"
#include "sceneGame.h"
#include "sceneResult.h"
#include "sceneRanking.h"
#include "manager.h"
#include "renderer.h"
#include "lightManager.h"
#include "camera.h"
#include "stage.h"

//************************************************************
//	親クラス [CScene] のメンバ関数
//************************************************************
//============================================================
//	コンストラクタ
//============================================================
CScene::CScene(const EMode mode) : m_mode(mode)
{

}

//============================================================
//	デストラクタ
//============================================================
CScene::~CScene()
{

}

//============================================================
//	初期化処理
//============================================================
HRESULT CScene::Init()
{
	// ステージの割当
	GET_MANAGER->GetStage()->BindStage("data\\TXT\\stage.txt");

	return S_OK;
}

//============================================================
//	終了処理
//============================================================
void CScene::Uninit()
{

}

//============================================================
//	更新処理
//============================================================
void CScene::Update(const float fDeltaTime)
{
	CManager*		pManager	= GET_MANAGER;				// マネージャー
	CLightManager*	pLight		= pManager->GetLight();		// ライト
	CCamera*		pCamera		= pManager->GetCamera();	// カメラ
	CRenderer*		pRenderer	= pManager->GetRenderer();	// レンダラー

	// ライトの更新
	assert(pLight != nullptr);
	pLight->Update(fDeltaTime);

	// カメラの更新
	assert(pCamera != nullptr);
	pCamera->Update(fDeltaTime);

	// レンダラーの更新
	assert(pRenderer != nullptr);
	pRenderer->Update(fDeltaTime);
}

//============================================================
//	生成処理
//============================================================
CScene* CScene::Create(const EMode mode)
{
	// シーンの生成
	CScene* pScene = nullptr;	// シーン情報
	switch (mode)
	{ // モードごとの処理
	case MODE_TITLE:
		pScene = new CSceneTitle(mode);
		break;

	case MODE_GAME:
		pScene = new CSceneGame(mode);
		break;

	case MODE_RESULT:
		pScene = new CSceneResult(mode);
		break;

	case MODE_RANKING:
		pScene = new CSceneRanking(mode);
		break;

	default:
		assert(false);
		break;
	}

	// 確保したアドレスを返す
	assert(pScene != nullptr);
	return pScene;
}

//============================================================
//	破棄処理
//============================================================
void CScene::Release(CScene*& prScene)
{
	// シーンの終了
	assert(prScene != nullptr);
	prScene->Uninit();

	// メモリ開放
	SAFE_DELETE(prScene);
}

//============================================================
//	プレイヤー取得処理
//============================================================
CPlayer* CScene::GetPlayer()
{
	// TODO：プレイヤー追加時に修正
#if 0
	CListManager<CPlayer>* pListManager = CPlayer::GetList();	// プレイヤーリストマネージャー
	if (pListManager == nullptr)		{ return nullptr; }		// リスト未使用の場合抜ける
	if (pListManager->GetNumAll() != 1)	{ return nullptr; }		// プレイヤーが1人ではない場合抜ける
	CPlayer* pPlayer = pListManager->GetList().front();			// プレイヤー情報

	// インスタンス未使用
	assert(pPlayer != nullptr);

	// プレイヤーのポインタを返す
	return pPlayer;
#else
	return nullptr;
#endif
}
