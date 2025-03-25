//============================================================
//
//	オブジェクトメッシュサークルヘッダー [objectMeshCircle.h]
//	Author：藤田勇一
//
//============================================================
//************************************************************
//	二重インクルード防止
//************************************************************
#ifndef _OBJECT_MESHCIRCLE_H_
#define _OBJECT_MESHCIRCLE_H_

//************************************************************
//	インクルードファイル
//************************************************************
#include "object.h"
#include "renderState.h"

//************************************************************
//	クラス定義
//************************************************************
// オブジェクトメッシュサークルクラス
class CObjectMeshCircle : public CObject
{
public:
	// コンストラクタ
	explicit CObjectMeshCircle(const CObject::ELabel label = LABEL_NONE, const CObject::EDim dimension = DIM_3D, const int nPriority = DEFAULT_PRIO);

	// デストラクタ
	~CObjectMeshCircle() override;

	// オーバーライド関数
	HRESULT Init() override;	// 初期化
	void Uninit() override;		// 終了
	void Update(const float fDeltaTime) override;		// 更新
	void Draw(CShader* pShader = nullptr) override;		// 描画
	void SetVec3Position(const VECTOR3& rPos) override;	// 位置設定
	void SetVec3Rotation(const VECTOR3& rRot) override;	// 向き設定
	inline VECTOR3 GetVec3Position() const override	{ return m_pos; }		// 位置取得
	inline VECTOR3 GetVec3Rotation() const override	{ return m_rot; }		// 向き取得
	inline MATRIX* GetPtrMtxWorld() override		{ return &m_mtxWorld; }	// マトリックスポインタ取得
	inline MATRIX GetMtxWorld() const override		{ return m_mtxWorld; }	// マトリックス取得

	// 静的メンバ関数
	static CObjectMeshCircle* Create	// 生成
	( // 引数
		const VECTOR3& rPos,	// 位置
		const VECTOR3& rRot,	// 向き
		const COLOR& rCol,		// 色
		const POSGRID2& rPart,	// 分割数
		const float fRadius		// 半径
	);

	// メンバ関数
	CRenderState* GetRenderState();				// レンダーステート情報取得
	void BindTexture(const int nTextureIdx);	// テクスチャ割当 (インデックス)
	void BindTexture(const char* pTexturePath);	// テクスチャ割当 (パス)
	void SetAlpha(const float fAlpha);			// 透明度設定
	void SetColor(const COLOR& rCol);			// 色設定
	void SetRadius(const float fRadius);		// 半径設定
	HRESULT SetPattern(const POSGRID2& rPart);	// 分割数設定
	inline int GetTextureIndex() const	{ return m_nTextureIdx; }	// テクスチャインデックス取得
	inline float GetAlpha() const		{ return m_col.a; }			// 透明度取得
	inline COLOR GetColor() const		{ return m_col; }			// 色取得
	inline float GetRadius() const		{ return m_fRadius; }		// 半径取得
	inline POSGRID2 GetPattern() const	{ return m_part; }			// 分割数取得

protected:
	// 仮想関数
	virtual void CalcDrawMatrix();	// 描画マトリックス計算

	// メンバ関数
	void SetVtx();	// 頂点情報の設定
	void SetIdx();	// インデックス情報の設定

private:
	// オーバーライド関数
	inline void Release() override { CObject::Release(); }	// 破棄

	// メンバ関数
	void DrawNormal();	// 通常描画
	void DrawShader(CShader* pShader);	// シェーダー描画

	// メンバ変数
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;	// 頂点バッファへのポインタ
	LPDIRECT3DINDEXBUFFER9 m_pIdxBuff;	// インデックスバッファへのポインタ
	CRenderState* m_pRenderState;		// レンダーステートの情報
	MATRIX m_mtxWorld;	// ワールドマトリックス
	VECTOR3 m_pos;		// 位置
	VECTOR3 m_rot;		// 向き
	COLOR m_col;		// 色
	POSGRID2 m_part;	// 分割数
	float m_fRadius;	// 半径
	int m_nNumVtx;		// 必要頂点数
	int m_nNumIdx;		// 必要インデックス数
	int m_nTextureIdx;	// テクスチャインデックス
};

#endif	// _OBJECT_MESHCIRCLE_H_
