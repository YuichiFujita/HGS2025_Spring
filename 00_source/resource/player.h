//============================================================
//
//	プレイヤーヘッダー [player.h]
//	Author：藤田勇一
// 
//============================================================
//************************************************************
//	二重インクルード防止
//************************************************************
#ifndef _PLAYER_H_
#define _PLAYER_H_

//************************************************************
//	インクルードファイル
//************************************************************
#include "objectChara.h"

//************************************************************
//	クラス定義
//************************************************************
// プレイヤークラス
class CPlayer : public CObjectChara
{
public:
	// モデル列挙
	enum EModel
	{
		MODEL_WAIST = 0,	// 腰
		MODEL_BODY,			// 体
		MODEL_HEAD,			// 頭
		MODEL_ARMUL,		// 左上腕
		MODEL_ARMUR,		// 右上腕
		MODEL_ARMDL,		// 左下腕
		MODEL_ARMDR,		// 右下腕
		MODEL_HANDL,		// 左手
		MODEL_HANDR,		// 右手
		MODEL_LEGUL,		// 左太もも
		MODEL_LEGUR,		// 右太もも
		MODEL_LEGDL,		// 左脛
		MODEL_LEGDR,		// 右脛
		MODEL_FOOTL,		// 左足
		MODEL_FOOTR,		// 右足
		MODEL_MAX			// この列挙型の総数
	};

	// モーション列挙
	enum EMotion
	{
		MOTION_IDOL = 0,	// 待機モーション
		MOTION_WALK,		// 歩行モーション
		MOTION_MAX			// この列挙型の総数
	};

	// 状態列挙
	enum EState
	{
		STATE_NONE = 0,	// 何もしない状態
		STATE_NORMAL,	// 通常状態
		STATE_DEATH,	// 死亡状態
		STATE_MAX		// この列挙型の総数
	};

	// コンストラクタ
	CPlayer();

	// デストラクタ
	~CPlayer() override;

	// オーバーライド関数
	HRESULT Init() override;	// 初期化
	void Uninit() override;		// 終了
	void Update(const float fDeltaTime) override;		// 更新
	void Draw(CShader* pShader = nullptr) override;		// 描画
	void SetEnableUpdate(const bool bUpdate) override;	// 更新状況設定
	void SetEnableDraw(const bool bDraw) override;		// 描画状況設定

	// 静的メンバ関数
	static CPlayer* Create	// 生成
	( // 引数
		const VECTOR3& rPos,	// 位置
		const VECTOR3& rRot		// 向き
	);
	static CListManager<CPlayer>* GetList();	// リスト取得

	// メンバ関数
	bool Hit();	// ヒット
	void SetState(const EState state);	// 状態設定
	EState GetState() const;			// 状態取得
	float GetRadius() const;			// 半径取得
	float GetHeight() const;			// 縦幅取得
	inline void SetDestRotation(const VECTOR3& rRot)	{ m_destRot = rRot; }	// 目標向き設定
	inline VECTOR3 GetDestRotation() const				{ return m_destRot; }	// 目標向き取得
	inline void SetMove(const VECTOR3& rMove)			{ m_move = rMove; }		// 移動量設定
	inline VECTOR3 GetMove() const						{ return m_move; }		// 移動量取得
	inline void SetEnableJump(const bool bJump)			{ m_bJump = bJump; }	// ジャンプ状況設定
	inline bool IsJump() const							{ return m_bJump; }		// ジャンプ状況設定

private:
	// エイリアス定義
	typedef EMotion(CPlayer::*AFuncState)(const float);	// 状態更新関数ポインタ

	// 関数配列
	static AFuncState m_aFuncState[];	// 状態更新関数リスト

	// メンバ関数
	EMotion UpdateNone(const float fDeltaTime);		// 何もしない状態時の更新
	EMotion UpdateNormal(const float fDeltaTime);	// 通常状態時の更新
	EMotion UpdateDeath(const float fDeltaTime);	// 死亡状態時の更新
	EMotion UpdateMove(const float fDeltaTime);		// 移動量/目標向きの更新
	void UpdateOldPosition();						// 過去位置の更新
	void UpdateGravity(const float fDeltaTime);		// 重力の更新
	bool UpdateLanding(VECTOR3* pPos, const float fDeltaTime);	// 着地状況の更新
	void UpdatePosition(VECTOR3* pPos, const float fDeltaTime);	// 位置の更新
	void UpdateRotation(VECTOR3* pRot, const float fDeltaTime);	// 向きの更新
	void UpdateMotion(int nMotion, const float fDeltaTime);		// モーション/キャラクターの更新

	// 静的メンバ変数
	static CListManager<CPlayer>* m_pList;	// オブジェクトリスト

	// メンバ変数
	CListManager<CPlayer>::AIterator m_iterator;	// イテレーター
	VECTOR3	m_oldPos;	// 過去位置
	VECTOR3	m_move;		// 移動量
	VECTOR3	m_destRot;	// 目標向き
	EState	m_state;	// 状態
	bool	m_bJump;	// ジャンプ状況
};

#endif	// _PLAYER_H_
