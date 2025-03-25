//============================================================
//
//	地面ヘッダー [field.h]
//	Author：藤田勇一
//
//============================================================
//************************************************************
//	二重インクルード防止
//************************************************************
#ifndef _FIELD_H_
#define _FIELD_H_

//************************************************************
//	インクルードファイル
//************************************************************
#include "objectMeshField.h"

//************************************************************
//	クラス定義
//************************************************************
// 地面クラス
class CField : public CObjectMeshField
{
public:
	// 種類列挙
	enum EType
	{
		TYPE_SOIL = 0,	// 土テクスチャ
		TYPE_MAX,		// この列挙型の総数
	};

	// コンストラクタ
	CField();

	// デストラクタ
	~CField() override;

	// オーバーライド関数
	HRESULT Init() override;	// 初期化
	void Uninit() override;		// 終了
	void Update(const float fDeltaTime) override;	// 更新
	void Draw(CShader* pShader = nullptr) override;	// 描画

	// 静的メンバ関数
	static CField* Create	// 生成
	( // 引数
		const EType type,			// 種類
		const VECTOR3& rPos,	// 位置
		const VECTOR3& rRot,	// 向き
		const VECTOR2& rSize,	// 大きさ
		const COLOR& rCol,		// 色
		const POSGRID2& rPart,		// 分割数
		const POSGRID2& rTexPart	// テクスチャ分割数
	);
	static CListManager<CField>* GetList();	// リスト取得

	// メンバ関数
	void SetType(const EType type);	// 種類設定
	inline EType GetType() const { return m_type; }	// 種類取得

private:

	// 静的メンバ変数
	static CListManager<CField>* m_pList;	// オブジェクトリスト

	// メンバ変数
	CListManager<CField>::AIterator m_iterator;	// イテレーター
	EType m_type;	// 種類
};

#endif	// _FIELD_H_
