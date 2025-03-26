//============================================================
//
//	銃弾ヘッダー [bullet.h]
//	Author：小原立暉
// 
//============================================================
//************************************************************
//	二重インクルード防止
//************************************************************
#ifndef _BULLET_H_
#define _BULLET_H_

//************************************************************
//	インクルードファイル
//************************************************************
#include "object3D.h"

//************************************************************
//	クラス定義
//************************************************************
// 銃弾クラス
class CBullet : public CObject3D
{
public:

	// コンストラクタ
	CBullet();

	// デストラクタ
	~CBullet() override;

	// オーバーライド関数
	HRESULT Init() override;	// 初期化
	void Uninit() override;		// 終了
	void Update(const float fDeltaTime) override;		// 更新
	void Draw(CShader* pShader = nullptr) override;		// 描画

	// 静的メンバ関数
	static CBullet* Create	// 生成
	( // 引数
		const VECTOR3& rPos,	// 位置
		const bool bRight		// 右側
	);
	static CListManager<CBullet>* GetList();	// リスト取得

private:

	// メンバ関数
	void SetMove(const bool bRight);	// 移動量の設定処理
	void BlockCollision(void);			// ブロックとの当たり判定

	// メンバ関数
	void Move(void);			// 移動処理

	// 静的メンバ変数
	static CListManager<CBullet>* m_pList;	// オブジェクトリスト

	// メンバ変数
	CListManager<CBullet>::AIterator m_iterator;	// イテレーター
	VECTOR3 m_move;		// 移動量
};

#endif	// _BLOCK_H_
