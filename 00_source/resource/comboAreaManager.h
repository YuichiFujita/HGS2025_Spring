//============================================================
//
//	コンボエリアマネージャーヘッダー [comboAreaManager.h]
//	Author：小原立暉
// 
//============================================================
//************************************************************
//	二重インクルード防止
//************************************************************
#ifndef _COMBO_AREA_MANAGER_H_
#define _COMBO_AREA_MANAGER_H_

//************************************************************
//	インクルードファイル
//************************************************************
#include "object.h"

//************************************************************
//	クラス定義
//************************************************************
// コンボエリアマネージャークラス
class CComboAreaManager : public CObject
{
public:

	// コンストラクタ
	CComboAreaManager();

	// デストラクタ
	~CComboAreaManager() override;

	// オーバーライド関数
	HRESULT Init() override;	// 初期化
	void Uninit() override;		// 終了
	void Update(const float fDeltaTime) override;		// 更新
	void Draw(CShader* pShader = nullptr) override;		// 描画

	// 静的メンバ関数
	static CComboAreaManager* Create();		// 生成

private:

	// オーバーライド関数
	inline void Release() override { CObject::Release(); }	// 破棄
};

#endif	// _COMBO_AREA_MANAGER_H_
