//============================================================
//
//	爆発ヘッダー [explosion.h]
//	Author：藤田勇一
//
//============================================================
//************************************************************
//	二重インクルード防止
//************************************************************
#ifndef _EXPLOSION_H_
#define _EXPLOSION_H_

//************************************************************
//	インクルードファイル
//************************************************************
#include "anim3D.h"

//************************************************************
//	クラス定義
//************************************************************
// 爆発クラス
class CExplosion : public CAnim3D
{
public:
	// 種類列挙
	enum EType
	{
		TYPE_FIRE = 0,	// 火テクスチャ
		TYPE_SMOKE,		// 煙テクスチャ
		TYPE_MAX		// この列挙型の総数
	};

	// コンストラクタ
	CExplosion();

	// デストラクタ
	~CExplosion() override;

	// オーバーライド関数
	HRESULT Init() override;	// 初期化
	void Uninit() override;		// 終了
	void Update(const float fDeltaTime) override;	// 更新
	void Draw(CShader* pShader = nullptr) override;	// 描画

	// 静的メンバ関数
	static CExplosion* Create	// 生成
	( // 引数
		const EType type,		// 種類
		const VECTOR3& rPos,	// 位置
		const VECTOR3& rSize	// 大きさ
	);

	// メンバ関数
	void SetType(const EType type);	// 種類設定
	inline EType GetType() const { return m_type; }	// 種類取得

private:
	// メンバ変数
	EType m_type;	// 種類
};

#endif	// _EXPLOSION_H_
