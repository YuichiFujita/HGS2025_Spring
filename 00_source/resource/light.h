//============================================================
//
//	ライトヘッダー [light.h]
//	Author：藤田勇一
//
//============================================================
//************************************************************
//	二重インクルード防止
//************************************************************
#ifndef _LIGHT_H_
#define _LIGHT_H_

//************************************************************
//	インクルードファイル
//************************************************************
#include "object.h"

//************************************************************
//	クラス定義
//************************************************************
// ライトクラス
class CLight : public CObject
{
public:
	// コンストラクタ
	CLight();

	// デストラクタ
	~CLight() override;

	// オーバーライド関数
	HRESULT Init() override;	// 初期化
	void Uninit() override;		// 終了
	void Update(const float fDeltaTime) override;	// 更新
	void Draw(CShader* pShader = nullptr) override;	// 描画

	// メンバ関数
	void SetType(const D3DLIGHTTYPE type);	// 種類設定
	void SetDiffuse(const COLOR& rCol);		// 拡散光設定
	void SetDirection(const VECTOR3& rDir);	// 方向設定
	void SetEnableLight(const bool bLight);	// 有効設定
	inline D3DLIGHT9 GetLight() { return m_light; }	// ライト取得

private:
	// オーバーライド関数
	inline void Release() override { CObject::Release(); }	// 破棄

	// 静的メンバ変数
	static int m_nNumLight;	// ライトの総数

	// メンバ変数
	D3DLIGHT9 m_light;		// ライト情報
	const int m_nLightIdx;	// 自身のライトインデックス
};

#endif	// _LIGHT_H_
