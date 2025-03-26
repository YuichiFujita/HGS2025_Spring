//============================================================
//
//	ブロックヘッダー [block.h]
//	Author：小原立暉
// 
//============================================================
//************************************************************
//	二重インクルード防止
//************************************************************
#ifndef _BLOCK_H_
#define _BLOCK_H_

//************************************************************
//	インクルードファイル
//************************************************************
#include "objectMeshCube.h"

//************************************************************
//	クラス定義
//************************************************************
// ブロッククラス
class CBlock : public CObjectMeshCube
{
public:

	// 種類の列挙型
	enum EType
	{
		TYPE_BREAK = 0,		// 破壊ブロック
		TYPE_MAX			// この列挙型の総数
	};

	// コンストラクタ
	CBlock();

	// デストラクタ
	~CBlock() override;

	// オーバーライド関数
	HRESULT Init() override;	// 初期化
	void Uninit() override;		// 終了
	void Update(const float fDeltaTime) override;		// 更新
	void Draw(CShader* pShader = nullptr) override;		// 描画

	// 静的メンバ関数
	static CBlock* Create	// 生成
	( // 引数
		const VECTOR3& rPos,	// 位置
		const EType type,		// 種類
		const bool bRight		// 右側
	);
	static CListManager<CBlock>* GetList();	// リスト取得

	// メンバ関数
	virtual bool Hit() = 0;		// ヒット処理

private:

	// 静的メンバ変数
	static CListManager<CBlock>* m_pList;	// オブジェクトリスト

	// メンバ変数
	CListManager<CBlock>::AIterator m_iterator;	// イテレーター
	EType m_type;		// 種類
	bool m_bRight;		// 右側状況
};

#endif	// _BLOCK_H_
