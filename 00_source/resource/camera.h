//============================================================
//
//	カメラヘッダー [camera.h]
//	Author：藤田勇一
//
//============================================================
//************************************************************
//	二重インクルード防止
//************************************************************
#ifndef _CAMERA_H_
#define _CAMERA_H_

//************************************************************
//	インクルードファイル
//************************************************************
#include "object.h"

//************************************************************
//	クラス定義
//************************************************************
// カメラクラス
class CCamera
{
public:
	// 状態列挙
	enum EState
	{
		STATE_NONE = 0,	// 固定状態
		STATE_FOLLOW,	// 追従状態
		STATE_CONTROL,	// 操作状態
		STATE_MAX		// この列挙型の総数
	};

	// コンストラクタ
	CCamera();

	// デストラクタ
	~CCamera();

	// カメラ揺れ構造体
	struct SSwing
	{
	public:
		// コンストラクタ
		SSwing() :
			shiftPos	 (VEC3_ZERO),	// 位置ずれ量
			fShiftAngle	 (0.0f),		// 位置をずらす角度
			fShiftLength (0.0f),		// 位置をずらす距離
			fSubAngle	 (0.0f),		// ずらす角度の減算量
			fSubLength	 (0.0f)			// ずらす距離の減算量
		{}

		// 引数付きコンストラクタ
		SSwing(const float in_fShiftLength, const float in_fSubAngle, const float in_fSubLength) :
			shiftPos	 (VEC3_ZERO),		// 位置ずれ量
			fShiftAngle	 (0.0f),			// 位置をずらす角度
			fShiftLength (in_fShiftLength),	// 位置をずらす距離
			fSubAngle	 (in_fSubAngle),	// ずらす角度の減算量
			fSubLength	 (in_fSubLength)	// ずらす距離の減算量
		{}

		// デストラクタ
		~SSwing() {}

		// メンバ変数
		VECTOR3	shiftPos;		// 位置ずれ量
		float	fShiftAngle;	// 位置をずらす角度
		float	fShiftLength;	// 位置をずらす距離
		float	fSubAngle;		// ずらす角度の減算量
		float	fSubLength;		// ずらす距離の減算量
	};

	// カメラ構造体
	struct SCamera
	{
		// デフォルトコンストラクタ
		SCamera() :
			posV		(VEC3_ZERO),	// 現在の視点
			posR		(VEC3_ZERO),	// 現在の注視点
			destPosV	(VEC3_ZERO),	// 目標の視点
			destPosR	(VEC3_ZERO),	// 目標の注視点
			vecU		(VEC3_ZERO),	// 上方向ベクトル
			rot			(VEC3_ZERO),	// 現在の向き
			destRot		(VEC3_ZERO),	// 目標の向き
			fDis		(0.0f),			// 現在の視点と注視点の距離
			fDestDis	(0.0f),			// 目標の視点と注視点の距離
			swing		({}),			// カメラ揺れ情報
			viewport	({}),			// ビューポート情報
			mtxProj		(MTX_IDENT),	// プロジェクションマトリックス
			mtxView		(MTX_IDENT)		// ビューマトリックス
		{}

		// デストラクタ
		~SCamera() {}

		// メンバ変数
		VECTOR3 posV;			// 現在の視点
		VECTOR3 posR;			// 現在の注視点
		VECTOR3 destPosV;		// 目標の視点
		VECTOR3 destPosR;		// 目標の注視点
		VECTOR3 vecU;			// 上方向ベクトル
		VECTOR3 rot;			// 現在の向き
		VECTOR3 destRot;		// 目標の向き
		float fDis;				// 現在の視点と注視点の距離
		float fDestDis;			// 目標の視点と注視点の距離
		SSwing swing;			// カメラ揺れ情報
		D3DVIEWPORT9 viewport;	// ビューポート情報
		MATRIX mtxProj;			// プロジェクションマトリックス
		MATRIX mtxView;			// ビューマトリックス
	};

	// メンバ関数
	HRESULT Init();	// 初期化
	void Uninit();	// 終了
	void Update(const float fDeltaTime);	// 更新
	void SetCamera();		// カメラ設定
	SCamera GetCamera();	// カメラ取得
	void ResetSwing();		// カメラ揺れ初期化
	void SetState(const EState state, const bool bInit = true);	// カメラ状態設定
	EState GetState() const;					// カメラ状態取得
	void SetSwing(const SSwing swing);			// カメラ揺れ設定
	void SetEnableUpdate(const bool bUpdate);	// 更新状況設定
	void SetPositionV(const VECTOR3& rPos);		// 視点設定
	void SetPositionR(const VECTOR3& rPos);		// 注視点設定
	void SetRotation(const VECTOR3& rRot);		// 向き設定
	void SetDistance(const float fDis);			// 距離設定
	inline VECTOR3 GetPositionV() const		{ return m_camera.posV; }		// 現在の視点取得
	inline VECTOR3 GetDestPositionV() const	{ return m_camera.destPosV; }	// 目標の視点取得
	inline VECTOR3 GetPositionR() const		{ return m_camera.posR; }		// 現在の注視点取得
	inline VECTOR3 GetDestPositionR() const	{ return m_camera.destPosR; }	// 目標の注視点取得
	inline VECTOR3 GetRotation() const		{ return m_camera.rot; }		// 現在の向き取得
	inline VECTOR3 GetDestRotation() const	{ return m_camera.destRot; }	// 目標の向き取得
	inline float GetDistance() const		{ return m_camera.fDis; }		// 現在の距離取得
	inline float GetDestDistance() const	{ return m_camera.fDestDis; }	// 目標の距離取得

	// 静的メンバ関数
	static CCamera* Create();					// 生成
	static void Release(CCamera*& prCamera);	// 破棄

private:
	// エイリアス定義
	typedef void(CCamera::*AFuncState)(const float);	// 状態更新関数ポインタ
	typedef void(CCamera::*AFuncInit)();				// 状態初期化関数ポインタ

	// 関数配列
	static AFuncState m_aFuncState[];	// 状態更新関数リスト
	static AFuncInit m_aFuncInit[];		// 状態初期化関数リスト

	// メンバ関数
	void InitNone();	// 固定カメラ初期化
	void InitFollow();	// 追従カメラ初期化
	void InitControl();	// 操作カメラ初期化
	void UpdateNone(const float fDeltaTime);	// 固定カメラ更新
	void UpdateFollow(const float fDeltaTime);	// 追従カメラ更新
	void UpdateControl(const float fDeltaTime);	// 操作カメラ更新
	void UpdateMove();		// 位置更新
	void UpdateDistance();	// 距離更新
	void UpdateRotation();	// 向き更新
	void UpdateSwing();		// カメラ揺れ更新

	// メンバ変数
	SCamera	m_camera;	// カメラ情報
	EState	m_state;	// 状態
	bool	m_bUpdate;	// 更新状況
};

#endif	// _CAMERA_H_
