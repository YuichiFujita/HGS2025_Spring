//============================================================
//
//	コンボエリアヘッダー [comboArea.h]
//	Author：小原立暉
// 
//============================================================
//************************************************************
//	二重インクルード防止
//************************************************************
#ifndef _COMBO_AREA_H_
#define _COMBO_AREA_H_

//************************************************************
//	インクルードファイル
//************************************************************
#include "object3D.h"

//************************************************************
//	クラス定義
//************************************************************
// コンボエリアクラス
class CComboArea : public CObject3D
{
public:

	// コンストラクタ
	CComboArea();

	// デストラクタ
	~CComboArea() override;

	// オーバーライド関数
	HRESULT Init() override;	// 初期化
	void Uninit() override;		// 終了
	void Update(const float fDeltaTime) override;		// 更新
	void Draw(CShader* pShader = nullptr) override;		// 描画

	// セット・ゲット関係
	float GetMulti(void) const;	// 倍率の取得処理

	// 静的メンバ関数
	static CComboArea* Create	// 生成
	( // 引数
		const VECTOR3& rPos,	// 位置
		const int nMul			// コンボ倍率
	);
	static CListManager<CComboArea>* GetList();	// リスト取得

private:

	// 静的メンバ変数
	static CListManager<CComboArea>* m_pList;	// オブジェクトリスト

	// メンバ変数
	CListManager<CComboArea>::AIterator m_iterator;	// イテレーター
	int m_nIdx;		// 番号
};

#endif	// _COMBO_AREA_H_
