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
#include "object3D.h"

//************************************************************
//	クラス定義
//************************************************************
// プレイヤークラス
class CPlayer : public CObject3D
{
public:
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
	inline void SetMove(const VECTOR3& rMove)	{ m_move = rMove; }		// 移動量設定
	inline VECTOR3 GetMove() const				{ return m_move; }		// 移動量取得
	inline void SetEnableJump(const bool bJump)	{ m_bJump = bJump; }	// ジャンプ状況設定
	inline bool IsJump() const					{ return m_bJump; }		// ジャンプ状況設定
	inline float GetCurMulti() const			{ return m_fMaxMulti; }	// 現在のスコア倍率取得

private:
	// エイリアス定義
	typedef void(CPlayer::*AFuncState)(const float);	// 状態更新関数ポインタ

	// 関数配列
	static AFuncState m_aFuncState[];	// 状態更新関数リスト

	// メンバ関数
	bool CollisionBlock(const VECTOR3& rPos);		// ブロックとの当たり判定
	void CollisionMulti(const VECTOR3& rPos);		// 倍率エリアとの当たり判定
	void UpdateNone(const float fDeltaTime);		// 何もしない状態時の更新
	void UpdateNormal(const float fDeltaTime);		// 通常状態時の更新
	void UpdateDeath(const float fDeltaTime);		// 死亡状態時の更新
	void UpdateMove(const float fDeltaTime);		// 移動量/目標向きの更新
	void UpdateOldPosition();						// 過去位置の更新
	void UpdateDirection();							// 方向の更新
	void UpdateGravity(const float fDeltaTime);		// 重力の更新
	void UpdateJump(const VECTOR3& rPos, const float fDeltaTime);	// ジャンプの更新
	void UpdateShot(const VECTOR3& rPos, const float fDeltaTime);	// 攻撃の更新
	bool UpdateLanding(VECTOR3* pPos, const float fDeltaTime);		// 着地状況の更新
	void UpdatePosition(VECTOR3* pPos, const float fDeltaTime);		// 位置の更新

	// 静的メンバ変数
	static CListManager<CPlayer>* m_pList;	// オブジェクトリスト

	// メンバ変数
	CListManager<CPlayer>::AIterator m_iterator;	// イテレーター
	VECTOR3	m_oldPos;		// 過去位置
	VECTOR3	m_move;			// 移動量
	EState	m_state;		// 状態
	bool	m_bRight;		// 左右フラグ
	bool	m_bJump;		// 現在ジャンプ状況
	bool	m_bOldJump;		// 過去ジャンプ状況
	bool	m_bReleaseJump;	// ジャンプ操作解除フラグ
	bool	m_bJumpPress;	// ジャンプ操作フラグ
	float	m_fJumpTimer;	// ジャンプ操作時間
	float	m_fShotTimer;	// 攻撃インターバル
	float	m_fHoverTimer;	// ホバー時間
	float	m_fMaxMulti;	// ジャンプ中の最高倍率
};

#endif	// _PLAYER_H_
