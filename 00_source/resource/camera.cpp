//============================================================
//
//	カメラ処理 [camera.cpp]
//	Author：藤田勇一
//
//============================================================
//************************************************************
//	インクルードファイル
//************************************************************
#include "camera.h"
#include "manager.h"
#include "renderer.h"
#include "sceneGame.h"

//************************************************************
//	定数宣言
//************************************************************
namespace
{
	// カメラ基本情報
	namespace basic
	{
		const VECTOR3 INIT_VECU	= VECTOR3(0.0f, 1.0f, 0.0f);	// 上方向ベクトルの初期値
		const float VIEW_NEAR	= 10.0f;		// モデルが見えるZ軸の最小値
		const float VIEW_FAR	= 100000.0f;	// モデルが見えるZ軸の最大値
		const float MIN_DIS		= 1.0f;			// カメラの視点から注視点への距離の最小
		const float MAX_DIS		= 10000.0f;		// カメラの視点から注視点への距離の最大
		const float REV_DIS		= 0.001f;		// カメラ揺れ計算時のカメラ距離の補正係数

		const float VIEW_ASPECT		= (float)SCREEN_WIDTH / (float)SCREEN_HEIGHT;	// アスペクト比
		const float VIEW_ANGLE		= D3DXToRadian(45.0f);	// 視野角
		const float LIMIT_ROT_HIGH	= D3DX_PI - 0.04f;		// X上回転の制限値
		const float LIMIT_ROT_LOW	= 0.04f;				// X下回転の制限値
	}

	// 固定カメラ情報
	namespace none
	{
		const VECTOR3 POSR		= VECTOR3(0.0f, 0.0f, 0.0f);	// 注視点位置
		const VECTOR3 ROT		= VECTOR3(HALF_PI, 0.0f, 0.0f);	// 向き
		const float DISTANCE	= 100.0f;	// 追従カメラの距離
		const float REV_POS		= 1.0f;		// カメラ位置の補正係数
		const float REV_ROT		= 1.0f;		// カメラ向きの補正係数
	}

	// ゲームカメラ情報
	namespace game
	{
		const VECTOR3 POSR		= VECTOR3(0.0f, 300.0f, 0.0f);	// 注視点位置
		const VECTOR3 ROT		= VECTOR3(HALF_PI, 0.0f, 0.0f);	// 向き
		const float DISTANCE	= 100.0f;	// 追従カメラの距離
		const float REV_POS		= 1.0f;		// カメラ位置の補正係数
		const float REV_ROT		= 1.0f;		// カメラ向きの補正係数
	}

	// 操作カメラ情報
	namespace control
	{
		const float REV_MOVE_MOUSE	= 1.0f;		// マウス操作でのカメラの移動の補正係数
		const float REV_DIS_MOUSE	= 0.2f;		// マウス操作でのカメラの距離の補正係数
		const float REV_ROT_MOUSE	= 0.0045f;	// マウス操作でのカメラの回転量の補正係数
	}
}

//************************************************************
//	静的メンバ変数宣言
//************************************************************
CCamera::AFuncState CCamera::m_aFuncState[] =	// 状態更新関数リスト
{
	&CCamera::UpdateNone,		// 固定状態の更新
	&CCamera::UpdateGame,		// ゲーム状態の更新
	&CCamera::UpdateControl,	// 操作状態の更新
};
CCamera::AFuncInit CCamera::m_aFuncInit[] =	// 状態初期化関数リスト
{
	&CCamera::InitNone,		// 固定状態の初期化
	&CCamera::InitGame,		// ゲーム状態の初期化
	&CCamera::InitControl,	// 操作状態の初期化
};

//************************************************************
//	親クラス [CCamera] のメンバ関数
//************************************************************
//============================================================
//	コンストラクタ
//============================================================
CCamera::CCamera() :
	m_camera	({}),			// カメラ情報
	m_state		(STATE_NONE),	// 状態
	m_bUpdate	(false)			// 更新状況
{
	// スタティックアサート
	static_assert(NUM_ARRAY(m_aFuncState) == CCamera::STATE_MAX, "ERROR : State Count Mismatch");
	static_assert(NUM_ARRAY(m_aFuncInit)  == CCamera::STATE_MAX, "ERROR : State Count Mismatch");
}

//============================================================
//	デストラクタ
//============================================================
CCamera::~CCamera()
{

}

//============================================================
//	初期化処理
//============================================================
HRESULT CCamera::Init()
{
	//--------------------------------------------------------
	//	メンバ変数を初期化
	//--------------------------------------------------------
	m_camera	= {};			// カメラ情報
	m_state		= STATE_NONE;	// 状態
	m_bUpdate	= true;			// 更新状況

	// カメラ情報を初期化
	m_camera.posV		= VEC3_ZERO;		// 現在の視点
	m_camera.posR		= VEC3_ZERO;		// 現在の注視点
	m_camera.destPosV	= VEC3_ZERO;		// 目標の視点
	m_camera.destPosR	= VEC3_ZERO;		// 目標の注視点
	m_camera.vecU		= basic::INIT_VECU;	// 上方向ベクトル
	m_camera.rot		= VEC3_ZERO;		// 現在の向き
	m_camera.destRot	= VEC3_ZERO;		// 目標の向き
	m_camera.fDis		= 0.0f;				// 現在の視点と注視点の距離
	m_camera.fDestDis	= 0.0f;				// 目標の視点と注視点の距離

	// カメラ揺れ情報を初期化
	m_camera.swing.shiftPos		= VEC3_ZERO;	// 位置ずれ量
	m_camera.swing.fShiftAngle	= 0.0f;			// 位置をずらす角度
	m_camera.swing.fShiftLength	= 0.0f;			// 位置をずらす距離
	m_camera.swing.fSubAngle	= 0.0f;			// ずらす角度の減算量
	m_camera.swing.fSubLength	= 0.0f;			// ずらす距離の減算量

	// ビューポート情報を初期化
	m_camera.viewport.X		 = 0;				// 左上隅のピクセル座標X
	m_camera.viewport.Y		 = 0;				// 左上隅のピクセル座標Y
	m_camera.viewport.Width	 = SCREEN_WIDTH;	// 描画する画面の横幅
	m_camera.viewport.Height = SCREEN_HEIGHT;	// 描画する画面の縦幅
	m_camera.viewport.MinZ	 = 0.0f;
	m_camera.viewport.MaxZ	 = 1.0f;

	// 固定カメラにする
	SetState(CCamera::STATE_NONE);

	return S_OK;
}

//============================================================
//	終了処理
//============================================================
void CCamera::Uninit()
{

}

//============================================================
//	更新処理
//============================================================
void CCamera::Update(const float fDeltaTime)
{
	// 更新を止めている場合抜ける
	if (!m_bUpdate) { return; }

	// 状態の更新
	(this->*(m_aFuncState[m_state]))(fDeltaTime);

	// カメラ揺れの更新
	UpdateSwing();
}

//============================================================
//	カメラ設定処理
//============================================================
void CCamera::SetCamera()
{
	LPDIRECT3DDEVICE9 pDevice = GET_DEVICE;	// デバイスのポインタ

	// ビューポートの設定
	pDevice->SetViewport(&m_camera.viewport);

	// プロジェクションマトリックスの初期化
	m_camera.mtxProj.Identity();

	if (m_state == STATE_CONTROL)
	{ // 操作カメラの場合

		// 透視投影でプロジェクションマトリックスを作成
		D3DXMatrixPerspectiveFovLH
		( // 引数
			&m_camera.mtxProj,	// プロジェクションマトリックス
			basic::VIEW_ANGLE,	// 視野角
			basic::VIEW_ASPECT,	// 画面のアスペクト比
			basic::VIEW_NEAR,	// Z軸の最小値
			basic::VIEW_FAR		// Z軸の最大値
		);
	}
	else
	{ // 別カメラの場合

		// 平行投影でプロジェクションマトリックスを作成
		D3DXMatrixOrthoLH
		( // 引数
			&m_camera.mtxProj,		// プロジェクションマトリックス
			(float)SCREEN_WIDTH,	// 投影する横幅
			(float)SCREEN_HEIGHT,	// 投影する縦幅
			basic::VIEW_NEAR,		// Z軸の最小値
			basic::VIEW_FAR			// Z軸の最大値
		);
	}

	// プロジェクションマトリックスの設定
	pDevice->SetTransform(D3DTS_PROJECTION, &m_camera.mtxProj);

	// ビューマトリックスの初期化
	m_camera.mtxView.Identity();

	// ビューマトリックスの作成
	D3DXMatrixLookAtLH
	( // 引数
		&m_camera.mtxView,	// ビューマトリックス
		&m_camera.posV,		// 視点
		&m_camera.posR,		// 注視点
		&m_camera.vecU		// 上方向ベクトル
	);

	// ビューマトリックスの設定
	pDevice->SetTransform(D3DTS_VIEW, &m_camera.mtxView);
}

//============================================================
//	視点の設定処理
//============================================================
void CCamera::SetPositionV(const VECTOR3& rPosV)
{
	SCamera* pCamera = &m_camera;	// メインカメラ情報

	// 視点・目標視点を設定
	pCamera->posV = pCamera->destPosV = rPosV;

	// 注視点・目標注視点の更新
	pCamera->posR.x = pCamera->destPosR.x = pCamera->posV.x + ((pCamera->fDis * sinf(pCamera->rot.x)) * sinf(pCamera->rot.y));
	pCamera->posR.y = pCamera->destPosR.y = pCamera->posV.y + ((pCamera->fDis * cosf(pCamera->rot.x)));
	pCamera->posR.z = pCamera->destPosR.z = pCamera->posV.z + ((pCamera->fDis * sinf(pCamera->rot.x)) * cosf(pCamera->rot.y));
}

//============================================================
//	注視点の設定処理
//============================================================
void CCamera::SetPositionR(const VECTOR3& rPosR)
{
	SCamera* pCamera = &m_camera;	// メインカメラ情報

	// 注視点・目標注視点を設定
	pCamera->posR = pCamera->destPosR = rPosR;

	// 視点・目標視点の更新
	pCamera->posV.x = pCamera->destPosV.x = pCamera->posR.x + ((-pCamera->fDis * sinf(pCamera->rot.x)) * sinf(pCamera->rot.y));
	pCamera->posV.y = pCamera->destPosV.y = pCamera->posR.y + ((-pCamera->fDis * cosf(pCamera->rot.x)));
	pCamera->posV.z = pCamera->destPosV.z = pCamera->posR.z + ((-pCamera->fDis * sinf(pCamera->rot.x)) * cosf(pCamera->rot.y));
}

//============================================================
//	向きの設定処理
//============================================================
void CCamera::SetRotation(const VECTOR3& rRot)
{
	SCamera* pCamera = &m_camera;	// メインカメラ情報

	// 向き・目標向きを設定
	pCamera->rot = pCamera->destRot = rRot;

	// 視点・目標視点の更新
	pCamera->posV.x = pCamera->destPosV.x = pCamera->posR.x + ((-pCamera->fDis * sinf(pCamera->rot.x)) * sinf(pCamera->rot.y));
	pCamera->posV.y = pCamera->destPosV.y = pCamera->posR.y + ((-pCamera->fDis * cosf(pCamera->rot.x)));
	pCamera->posV.z = pCamera->destPosV.z = pCamera->posR.z + ((-pCamera->fDis * sinf(pCamera->rot.x)) * cosf(pCamera->rot.y));
}

//============================================================
//	距離の設定処理
//============================================================
void CCamera::SetDistance(const float fDis)
{
	SCamera* pCamera = &m_camera;	// メインカメラ情報

	// 距離・目標距離を設定
	pCamera->fDis = pCamera->fDestDis = fDis;

	// 視点・目標視点の更新
	pCamera->posV.x = pCamera->destPosV.x = pCamera->posR.x + ((-pCamera->fDis * sinf(pCamera->rot.x)) * sinf(pCamera->rot.y));
	pCamera->posV.y = pCamera->destPosV.y = pCamera->posR.y + ((-pCamera->fDis * cosf(pCamera->rot.x)));
	pCamera->posV.z = pCamera->destPosV.z = pCamera->posR.z + ((-pCamera->fDis * sinf(pCamera->rot.x)) * cosf(pCamera->rot.y));
}

//============================================================
//	カメラ取得処理
//============================================================
CCamera::SCamera CCamera::GetCamera()
{
	// カメラの情報を返す
	return m_camera;
}

//============================================================
//	カメラ揺れの初期化処理
//============================================================
void CCamera::ResetSwing()
{
	// カメラ揺れ情報を初期化
	SSwing* pSwing = &m_camera.swing;
	pSwing->shiftPos	 = VEC3_ZERO;	// 位置ずれ量
	pSwing->fShiftAngle	 = 0.0f;		// 位置をずらす角度
	pSwing->fShiftLength = 0.0f;		// 位置をずらす距離
	pSwing->fSubAngle	 = 0.0f;		// ずらす角度の減算量
	pSwing->fSubLength	 = 0.0f;		// ずらす距離の減算量
}

//============================================================
//	カメラ状態の設定処理
//============================================================
void CCamera::SetState(const EState state, const bool bInit)
{
	// 状態の設定
	m_state = state;

	// カメラ揺れの初期化
	ResetSwing();

	if (bInit)
	{ // カメラ初期化がONの場合

		// 状態の初期化
		(this->*(m_aFuncInit[m_state]))();
	}
}

//============================================================
//	カメラ状態取得処理
//============================================================
CCamera::EState CCamera::GetState() const
{
	// 状態を返す
	return m_state;
}

//============================================================
//	カメラ揺れの設定処理
//============================================================
void CCamera::SetSwing(const SSwing swing)
{
	// カメラ操作状態の場合抜ける
	if (m_state == STATE_CONTROL) { return; }

	// 引数のカメラ揺れ情報を設定
	m_camera.swing = swing;
}

//============================================================
//	更新状況の設定処理
//============================================================
void CCamera::SetEnableUpdate(const bool bUpdate)
{
	// 引数の更新状況を設定
	m_bUpdate = bUpdate;
}

//============================================================
//	生成処理
//============================================================
CCamera* CCamera::Create()
{
	// カメラの生成
	CCamera* pCamera = new CCamera;
	if (pCamera == nullptr)
	{ // 生成に失敗した場合

		return nullptr;
	}
	else
	{ // 生成に成功した場合

		// カメラの初期化
		if (FAILED(pCamera->Init()))
		{ // 初期化に失敗した場合

			// カメラの破棄
			SAFE_DELETE(pCamera);
			return nullptr;
		}

		// 確保したアドレスを返す
		return pCamera;
	}
}

//============================================================
//	破棄処理
//============================================================
void CCamera::Release(CCamera*& prCamera)
{
	// カメラの終了
	assert(prCamera != nullptr);
	prCamera->Uninit();

	// メモリ開放
	SAFE_DELETE(prCamera);
}

//============================================================
//	固定カメラ初期化処理
//============================================================
void CCamera::InitNone()
{
	// カメラ固定状態ではない場合抜ける
	if (m_state != STATE_NONE) { return; }

	//----------------------------------------------------
	//	向きの更新
	//----------------------------------------------------
	// 向きの設定
	m_camera.rot = m_camera.destRot = none::ROT;
	useful::NormalizeRot(m_camera.rot);		// 現在向きを正規化
	useful::NormalizeRot(m_camera.destRot);	// 目標向きを正規化

	//----------------------------------------------------
	//	距離の更新
	//----------------------------------------------------
	// 距離の設定
	m_camera.fDis = m_camera.fDestDis = none::DISTANCE;

	//----------------------------------------------------
	//	位置の更新
	//----------------------------------------------------
	// 注視点の設定
	m_camera.posR = m_camera.destPosR = none::POSR;

	// 視点の設定
	m_camera.posV.x = m_camera.destPosV.x = m_camera.destPosR.x + ((-m_camera.fDis * sinf(m_camera.rot.x)) * sinf(m_camera.rot.y));
	m_camera.posV.y = m_camera.destPosV.y = m_camera.destPosR.y + ((-m_camera.fDis * cosf(m_camera.rot.x)));
	m_camera.posV.z = m_camera.destPosV.z = m_camera.destPosR.z + ((-m_camera.fDis * sinf(m_camera.rot.x)) * cosf(m_camera.rot.y));
}

//============================================================
//	ゲームカメラ初期化処理
//============================================================
void CCamera::InitGame()
{
	// カメラゲーム状態ではない場合抜ける
	if (m_state != STATE_GAME) { return; }

	//----------------------------------------------------
	//	向きの更新
	//----------------------------------------------------
	// 向きの設定
	m_camera.rot = m_camera.destRot = game::ROT;
	useful::NormalizeRot(m_camera.rot);		// 現在向きを正規化
	useful::NormalizeRot(m_camera.destRot);	// 目標向きを正規化

	//----------------------------------------------------
	//	距離の更新
	//----------------------------------------------------
	// 距離の設定
	m_camera.fDis = m_camera.fDestDis = game::DISTANCE;

	//----------------------------------------------------
	//	位置の更新
	//----------------------------------------------------
	// 注視点の設定
	m_camera.posR = m_camera.destPosR = game::POSR;

	// 視点の設定
	m_camera.posV.x = m_camera.destPosV.x = m_camera.destPosR.x + ((-m_camera.fDis * sinf(m_camera.rot.x)) * sinf(m_camera.rot.y));
	m_camera.posV.y = m_camera.destPosV.y = m_camera.destPosR.y + ((-m_camera.fDis * cosf(m_camera.rot.x)));
	m_camera.posV.z = m_camera.destPosV.z = m_camera.destPosR.z + ((-m_camera.fDis * sinf(m_camera.rot.x)) * cosf(m_camera.rot.y));
}

//============================================================
//	操作カメラ初期化処理
//============================================================
void CCamera::InitControl()
{

}

//============================================================
//	固定カメラの更新処理
//============================================================
void CCamera::UpdateNone(const float fDeltaTime)
{
	// カメラ固定状態ではない場合抜ける
	if (m_state != STATE_NONE) { return; }

	//----------------------------------------------------
	//	向きの更新
	//----------------------------------------------------
	VECTOR3 diffRot = VEC3_ZERO;	// 差分向き

	// 目標向きの設定
	m_camera.destRot = none::ROT;
	useful::NormalizeRot(m_camera.destRot);	// 目標向きを正規化

	// 差分向きの計算
	diffRot = m_camera.destRot - m_camera.rot;
	useful::NormalizeRot(diffRot);			// 差分向きを正規化

	// 現在向きの更新
	m_camera.rot += diffRot * none::REV_ROT;
	useful::NormalizeRot(m_camera.rot);		// 現在向きを正規化

	//----------------------------------------------------
	//	距離の更新
	//----------------------------------------------------
	// 距離の設定
	m_camera.fDis = m_camera.fDestDis = none::DISTANCE;

	//----------------------------------------------------
	//	位置の更新
	//----------------------------------------------------
	VECTOR3 diffPosV = VEC3_ZERO;	// 視点の差分位置
	VECTOR3 diffPosR = VEC3_ZERO;	// 注視点の差分位置

	// 注視点の更新
	m_camera.destPosR = none::POSR;

	// 視点の更新
	m_camera.destPosV.x = m_camera.destPosR.x + ((-m_camera.fDis * sinf(m_camera.rot.x)) * sinf(m_camera.rot.y));
	m_camera.destPosV.y = m_camera.destPosR.y + ((-m_camera.fDis * cosf(m_camera.rot.x)));
	m_camera.destPosV.z = m_camera.destPosR.z + ((-m_camera.fDis * sinf(m_camera.rot.x)) * cosf(m_camera.rot.y));

	// 差分位置を計算
	diffPosR = m_camera.destPosR - m_camera.posR;	// 注視点
	diffPosV = m_camera.destPosV - m_camera.posV;	// 視点

	// 現在位置を更新
	m_camera.posR += diffPosR * none::REV_POS;	// 注視点
	m_camera.posV += diffPosV * none::REV_POS;	// 視点
}

//============================================================
//	ゲームカメラの更新処理
//============================================================
void CCamera::UpdateGame(const float fDeltaTime)
{
	// カメラゲーム状態ではない場合抜ける
	if (m_state != STATE_GAME) { return; }

	//----------------------------------------------------
	//	向きの更新
	//----------------------------------------------------
	VECTOR3 diffRot = VEC3_ZERO;	// 差分向き

	// 目標向きの設定
	m_camera.destRot = game::ROT;
	useful::NormalizeRot(m_camera.destRot);	// 目標向きを正規化

	// 差分向きの計算
	diffRot = m_camera.destRot - m_camera.rot;
	useful::NormalizeRot(diffRot);			// 差分向きを正規化

	// 現在向きの更新
	m_camera.rot += diffRot * game::REV_ROT;
	useful::NormalizeRot(m_camera.rot);		// 現在向きを正規化

	//----------------------------------------------------
	//	距離の更新
	//----------------------------------------------------
	// 距離の設定
	m_camera.fDis = m_camera.fDestDis = game::DISTANCE;

	//----------------------------------------------------
	//	位置の更新
	//----------------------------------------------------
	VECTOR3 diffPosV = VEC3_ZERO;	// 視点の差分位置
	VECTOR3 diffPosR = VEC3_ZERO;	// 注視点の差分位置

	// 注視点の更新
	m_camera.destPosR = game::POSR;

	// 視点の更新
	m_camera.destPosV.x = m_camera.destPosR.x + ((-m_camera.fDis * sinf(m_camera.rot.x)) * sinf(m_camera.rot.y));
	m_camera.destPosV.y = m_camera.destPosR.y + ((-m_camera.fDis * cosf(m_camera.rot.x)));
	m_camera.destPosV.z = m_camera.destPosR.z + ((-m_camera.fDis * sinf(m_camera.rot.x)) * cosf(m_camera.rot.y));

	// 差分位置を計算
	diffPosR = m_camera.destPosR - m_camera.posR;	// 注視点
	diffPosV = m_camera.destPosV - m_camera.posV;	// 視点

	// 現在位置を更新
	m_camera.posR += diffPosR * game::REV_POS;	// 注視点
	m_camera.posV += diffPosV * game::REV_POS;	// 視点
}

//============================================================
//	操作カメラの更新処理
//============================================================
void CCamera::UpdateControl(const float fDeltaTime)
{
	// 位置の更新
	UpdateMove();

	// 距離の更新
	UpdateDistance();

	// 向きの更新
	UpdateRotation();
}

//============================================================
//	位置の更新処理
//============================================================
void CCamera::UpdateMove()
{
	CInputMouse* pMouse = GET_INPUTMOUSE;	// マウス情報
	VECTOR3 mouseMove = pMouse->GetMove();	// マウス移動量

	// マウス操作の更新
	if (pMouse->IsPress(CInputMouse::KEY_LEFT) && pMouse->IsPress(CInputMouse::KEY_RIGHT))
	{ // 右クリックと左クリックが押されている場合

		// 視点を移動
		m_camera.posV.x -= sinf(m_camera.rot.y + (D3DX_PI * 0.5f)) * mouseMove.x * control::REV_MOVE_MOUSE;
		m_camera.posV.z -= cosf(m_camera.rot.y + (D3DX_PI * 0.5f)) * mouseMove.x * control::REV_MOVE_MOUSE;
		m_camera.posV.x += sinf(m_camera.rot.y) * mouseMove.y * control::REV_MOVE_MOUSE;
		m_camera.posV.z += cosf(m_camera.rot.y) * mouseMove.y * control::REV_MOVE_MOUSE;

		// 注視点を移動
		m_camera.posR.x -= sinf(m_camera.rot.y + (D3DX_PI * 0.5f)) * mouseMove.x * control::REV_MOVE_MOUSE;
		m_camera.posR.z -= cosf(m_camera.rot.y + (D3DX_PI * 0.5f)) * mouseMove.x * control::REV_MOVE_MOUSE;
		m_camera.posR.x += sinf(m_camera.rot.y) * mouseMove.y * control::REV_MOVE_MOUSE;
		m_camera.posR.z += cosf(m_camera.rot.y) * mouseMove.y * control::REV_MOVE_MOUSE;
	}
}

//============================================================
//	距離の更新処理
//============================================================
void CCamera::UpdateDistance()
{
	CInputMouse* pMouse = GET_INPUTMOUSE;	// マウス情報
	VECTOR3 mouseMove = pMouse->GetMove();	// マウス移動量

	// マウス操作の更新
	if (mouseMove.z != 0.0f)
	{ // マウスホイールが操作された場合

		// 距離を加算
		m_camera.fDis += mouseMove.z * control::REV_DIS_MOUSE;
	}

	// 距離の補正
	useful::LimitNum(m_camera.fDis, basic::MIN_DIS, basic::MAX_DIS);
}

//============================================================
//	向きの更新処理
//============================================================
void CCamera::UpdateRotation()
{
	CInputMouse* pMouse = GET_INPUTMOUSE;	// マウス情報
	VECTOR3 mouseMove = pMouse->GetMove();	// マウス移動量

	// マウス操作の更新
	if (pMouse->IsPress(CInputMouse::KEY_LEFT) && !pMouse->IsPress(CInputMouse::KEY_RIGHT))
	{ // 左クリックだけが押されている場合

		// カメラのY軸を回転
		m_camera.rot.y += mouseMove.x * control::REV_ROT_MOUSE;

		// カメラのX軸を回転
		m_camera.rot.x += mouseMove.y * control::REV_ROT_MOUSE;
	}

	// 向きの補正
	useful::LimitNum(m_camera.rot.x, basic::LIMIT_ROT_LOW, basic::LIMIT_ROT_HIGH);
	useful::NormalizeRot(m_camera.rot.y);

	// 視点の更新
	m_camera.posV.x = m_camera.posR.x + ((-m_camera.fDis * sinf(m_camera.rot.x)) * sinf(m_camera.rot.y));
	m_camera.posV.y = m_camera.posR.y + ((-m_camera.fDis * cosf(m_camera.rot.x)));
	m_camera.posV.z = m_camera.posR.z + ((-m_camera.fDis * sinf(m_camera.rot.x)) * cosf(m_camera.rot.y));

	// マウス操作の更新
	if (pMouse->IsPress(CInputMouse::KEY_RIGHT) && !pMouse->IsPress(CInputMouse::KEY_LEFT))
	{ // 右クリックだけが押されている場合

		// カメラのY軸を回転
		m_camera.rot.y += mouseMove.x * control::REV_ROT_MOUSE;

		// カメラのX軸を回転
		m_camera.rot.x += mouseMove.y * control::REV_ROT_MOUSE;
	}

	// 向きの補正
	useful::LimitNum(m_camera.rot.x, basic::LIMIT_ROT_LOW, basic::LIMIT_ROT_HIGH);
	useful::NormalizeRot(m_camera.rot.y);

	// 注視点の更新
	m_camera.posR.x = m_camera.posV.x + ((m_camera.fDis * sinf(m_camera.rot.x)) * sinf(m_camera.rot.y));
	m_camera.posR.y = m_camera.posV.y + ((m_camera.fDis * cosf(m_camera.rot.x)));
	m_camera.posR.z = m_camera.posV.z + ((m_camera.fDis * sinf(m_camera.rot.x)) * cosf(m_camera.rot.y));
}

//============================================================
//	カメラ揺れの更新処理
//============================================================
void CCamera::UpdateSwing()
{
	// カメラ操作状態の場合抜ける
	if (m_state == STATE_CONTROL) { return; }

	SCamera* pCamera = &m_camera;		// カメラ情報
	SSwing*  pSwing  = &m_camera.swing;	// カメラ揺れ情報

	if (pSwing->fShiftLength > 0.0f)
	{ // 注視点のずらし量が設定されている場合

		D3DXQUATERNION quat;	// クォータニオン
		MATRIX mtxRot;			// 回転マトリックス
		VECTOR3 offset;			// 位置ずれオフセット
		float fRotY;			// 位置ずれ向き
		VECTOR3 vecAxis = pCamera->posR - pCamera->posV;	// 回転軸ベクトル

		// クォータニオンを作成
		D3DXVec3Normalize(&vecAxis, &vecAxis);	// 回転軸を正規化
		D3DXQuaternionRotationAxis(&quat, &vecAxis, pSwing->fShiftAngle);

		// 回転マトリックスを作成
		D3DXMatrixRotationQuaternion(&mtxRot, &quat);

		// 位置をずらす向きを求める
		fRotY = atan2f(-vecAxis.z, vecAxis.x);

		// 位置ずれオフセットを設定
		float fCalcTemp = pSwing->fShiftLength * (fabsf(pCamera->fDis) * basic::REV_DIS);
		offset = VECTOR3(sinf(fRotY) * fCalcTemp, 0.0f, cosf(fRotY) * fCalcTemp);

		// オフセットを反映した回転マトリックスを座標変換
		D3DXVec3TransformCoord(&pSwing->shiftPos, &offset, &mtxRot);

		// 注視点に位置のずれを加算
		pCamera->posR += pSwing->shiftPos;

		// 位置ずれ量を減算
		pSwing->fShiftAngle  -= pSwing->fSubAngle;
		pSwing->fShiftLength -= pSwing->fSubLength;

		// 角度を補正
		useful::NormalizeRot(pSwing->fShiftAngle);

		// 距離を補正
		useful::LimitMinNum(pSwing->fShiftLength, 0.0f);
	}
}
