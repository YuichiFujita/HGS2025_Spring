//============================================================
//
//	コンボUIヘッダー [comboUI.h]
//	Author：小原立暉
//
//============================================================
//************************************************************
//	二重インクルード防止
//************************************************************
#ifndef _COMBO_UI_H_
#define _COMBO_UI_H_

//************************************************************
//	インクルードファイル
//************************************************************
#include "object.h"

//************************************************************
//	前方宣言
//************************************************************
class CMultiValue;	// マルチ数字クラス

//************************************************************
//	クラス定義
//************************************************************
// コンボUIクラス
class CComboUI : public CObject
{
public:

	// コンストラクタ
	CComboUI();

	// デストラクタ
	~CComboUI() override;

	// オーバーライド関数
	HRESULT Init() override;	// 初期化
	void Uninit() override;		// 終了
	void Update(const float fDeltaTime) override;		// 更新
	void Draw(CShader* pShader = nullptr) override;		// 描画
	
	void SetData();				// 情報の設定処理
	void ResetCombo();			// コンボのリセット処理
	void SetMultiValue(const float fMulti);		// 倍率数値の設定処理
	void AddScoreValue(const int nScore);		// スコア数値の追加処理

	// 静的メンバ関数
	static CComboUI* Create();	// 生成

private:

	// オーバーライド関数
	inline void Release() override { CObject::Release(); }	// 破棄

	// メンバ変数
	CMultiValue* m_pMulti;		// 倍率の情報
	CMultiValue* m_pScore;		// スコアの情報
};

#endif	// _TIME_UI_H_
