//============================================================
//
//	パーティクル3Dヘッダー [particle3D.h]
//	Author：藤田勇一
//
//============================================================
//************************************************************
//	二重インクルード防止
//************************************************************
#ifndef _PARTICLE3D_H_
#define _PARTICLE3D_H_

//************************************************************
//	インクルードファイル
//************************************************************
#include "object.h"

//************************************************************
//	クラス定義
//************************************************************
// パーティクル3Dクラス
class CParticle3D : public CObject
{
public:
	// コンストラクタ
	CParticle3D();

	// デストラクタ
	~CParticle3D() override;

	// 種類列挙
	enum EType
	{
		TYPE_NONE = 0,			// なし
		TYPE_DAMAGE,			// ダメージ
		TYPE_HEAL,				// 回復
		TYPE_FIRE,				// 炎
		TYPE_SMALL_EXPLOSION,	// 小爆発
		TYPE_BIG_EXPLOSION,		// 大爆発
		TYPE_PLAYER_DAMAGE,		// プレイヤーダメージ
		TYPE_MAX				// この列挙型の総数
	};

	// オーバーライド関数
	HRESULT Init() override;	// 初期化
	void Uninit() override;		// 終了
	void Update(const float fDeltaTime) override;		// 更新
	void Draw(CShader* pShader = nullptr) override;		// 描画
	void SetVec3Position(const VECTOR3& rPos) override;	// 位置設定
	inline VECTOR3 GetVec3Position() const override { return m_pos; }	// 位置取得

	// 静的メンバ関数
	static CParticle3D* Create	// 生成
	( // 引数
		const EType type,		// 種類
		const VECTOR3& rPos,	// 位置
		const COLOR& rCol = color::White()	// 色
	);

	// メンバ関数
	void SetAlpha(const float fAlpha);	// 透明度設定
	void SetColor(const COLOR& rCol);	// 色設定
	void SetType(const EType type);		// 種類設定
	inline float GetAlpha() const	{ return m_col.a; }	// 透明度取得
	inline COLOR GetColor() const	{ return m_col; }	// 色取得
	inline EType GetType() const	{ return m_type; }	// 種類取得

private:
	// オーバーライド関数
	inline void Release() override { CObject::Release(); }	// 破棄

	// メンバ関数
	void Damage(const VECTOR3& rPos, const COLOR& rCol);	// ダメージ
	void Heal(const VECTOR3& rPos, const COLOR& rCol);		// 回復

	void Fire(const VECTOR3& rPos);				// 炎
	void SmallExplosion(const VECTOR3& rPos);	// 小爆発
	void BigExplosion(const VECTOR3& rPos);		// 大爆発
	void PlayerDamage(const VECTOR3& rPos);		// プレイヤーダメージ

	// メンバ変数
	VECTOR3 m_pos;	// 位置
	COLOR m_col;	// 色
	EType m_type;	// 種類
	int m_nLife;	// 寿命
};

#endif	// _PARTICLE3D_H_
