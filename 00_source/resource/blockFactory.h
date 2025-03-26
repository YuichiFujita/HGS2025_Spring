//============================================================
//
//	ブロックファクトリーヘッダー [blockFactory.h]
//	Author：小原立暉
//
//============================================================
//************************************************************
//	二重インクルード防止
//************************************************************
#ifndef _BLOCK_FACTORY_H_
#define _BLOCK_FACTORY_H_

//************************************************************
//	インクルードファイル
//************************************************************
#include "object.h"

//************************************************************
//	クラス定義
//************************************************************
// ブロックファクトリークラス
class CBlockFactory : public CObject
{
public:

	// コンストラクタ
	CBlockFactory();

	// デストラクタ
	~CBlockFactory() override;

	// オーバーライド関数
	HRESULT Init() override;	// 初期化
	void Uninit() override;		// 終了
	void Update(const float fDeltaTime) override;		// 更新
	void Draw(CShader* pShader = nullptr) override;		// 描画

	// 静的メンバ関数
	static CBlockFactory* Create();	// 生成

private:

	// オーバーライド関数
	inline void Release() override { CObject::Release(); }	// 破棄

	// メンバ変数
	float m_fRate;			// 割合
	float m_fCreateRate;	// 生成割合
};

#endif	// _TIME_UI_H_
