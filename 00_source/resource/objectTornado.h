//============================================================
//
//	オブジェクト竜巻ヘッダー [objectTornado.h]
//	Author：藤田勇一
//
//============================================================
//************************************************************
//	二重インクルード防止
//************************************************************
#ifndef _OBJECT_TORNADO_H_
#define _OBJECT_TORNADO_H_

//************************************************************
//	インクルードファイル
//************************************************************
#include "object.h"
#include "renderState.h"

//************************************************************
//	クラス定義
//************************************************************
// オブジェクト竜巻クラス
class CObjectTornado : public CObject
{
public:
	// コンストラクタ
	CObjectTornado();

	// デストラクタ
	~CObjectTornado() override;

	// オーバーライド関数
	HRESULT Init() override;	// 初期化
	void Uninit() override;		// 終了
	void Update(const float fDeltaTime) override;		// 更新
	void Draw(CShader* pShader = nullptr) override;		// 描画
	void SetVec3Position(const VECTOR3& rPos) override;	// 位置設定
	inline VECTOR3 GetVec3Position() const override	{ return m_pos; }		// 位置取得
	inline MATRIX* GetPtrMtxWorld() override		{ return &m_mtxWorld; }	// マトリックスポインタ取得
	inline MATRIX GetMtxWorld() const override		{ return m_mtxWorld; }	// マトリックス取得

	// 静的メンバ関数
	static CObjectTornado* Create	// 生成
	( // 引数
		const VECTOR3& rPos,		// 位置
		const VECTOR3& rGrowRot,	// 成長向き
		const COLOR& rCol,			// 色
		MATRIX* pMtxParent		= nullptr,	// 親のマトリックス
		const int	nNumAround	= 3,		// 渦の周回数
		const int	nPattern	= 16,		// 渦の分割数
		const float	fMoveRot	= 0.3f,		// 向きの変更量
		const float	fThickness	= 25.0f,	// ポリゴンの太さ
		const float	fOuterPlusY	= 35.0f,	// ポリゴン外周のY座標加算量
		const float	fSetWidth	= 80.0f,	// 生成時の横ずれ量
		const float	fSetAlpha	= 0.85f,	// 生成時の透明度
		const float	fAddWidth	= 7.0f,		// 横ずれの加算量
		const float	fAddHeight	= 7.0f,		// 縦ずれの加算量
		const float	fSubAlpha	= 0.015f,	// 透明度の減算量
		const float	fGrowWidth	= 0.0f,		// 横ずれの成長量
		const float	fGrowHeight	= 0.0f,		// 縦ずれの成長量
		const float	fGrowAlpha	= 0.0f		// 透明度の成長量
	);

	// メンバ関数
	void SetGrow	// 成長設定
	( // 引数
		const float fMoveRot,		// 向きの変更量
		const float fGrowWidth,		// 横ずれの成長量
		const float fGrowHeight,	// 縦ずれの成長量
		const float fGrowAlpha		// 透明度の成長量
	);
	HRESULT SetVortex	// 渦設定
	( // 引数
		const int nNumAround,	// 渦の周回数
		const int nPattern		// 渦の分割数
	);

	CRenderState* GetRenderState();					// レンダーステート情報取得
	void BindTexture(const int nTextureIdx);		// テクスチャ割当 (インデックス)
	void BindTexture(const char* pTexturePath);		// テクスチャ割当 (パス)
	void DeleteMatrixParent();						// 親マトリックス削除
	void SetMatrixParent(MATRIX* pMtxParent);		// 親マトリックス設定
	void SetAlpha(const float fAlpha);				// 透明度設定
	void SetColor(const COLOR& rCol);				// 色設定
	void SetRotationGrow(const VECTOR3& rRot);		// 成長向き設定
	void SetThickness(const float fThickness);		// ポリゴンの太さ設定
	void SetOuterPlusY(const float fOuterPlusY);	// ポリゴン外周のY座標加算量設定
	void SetCreateWidth(const float fSetWidth);		// 生成時の横ずれ量設定
	void SetCreateAlpha(const float fSetAlpha);		// 生成時の透明度設定
	void SetAddWidth(const float fAddWidth);		// 横ずれの加算量設定
	void SetAddHeight(const float fAddHeight);		// 縦ずれの加算量設定
	void SetSubAlpha(const float fSubAlpha);		// 透明度の減算量設定
	inline int GetTextureIndex() const		{ return m_nTextureIdx; }	// テクスチャインデックス取得
	inline float GetAlpha() const			{ return m_col.a; }			// 透明度取得
	inline COLOR GetColor() const			{ return m_col; }			// 色取得
	inline VECTOR3 GetRotationGrow() const	{ return m_growRot; }		// 成長向き取得
	inline float GetThickness() const		{ return m_fThickness; }	// ポリゴンの太さ取得
	inline float GetOuterPlusY() const		{ return m_fOuterPlusY; }	// ポリゴン外周のY座標加算量取得
	inline float GetCreateWidth() const		{ return m_fSetWidth; }		// 生成時の横ずれ量取得
	inline float GetCreateAlpha() const		{ return m_fSetAlpha; }		// 生成時の透明度取得
	inline float GetAddWidth() const		{ return m_fAddWidth; }		// 横ずれの加算量取得
	inline float GetAddHeight() const		{ return m_fAddHeight; }	// 縦ずれの加算量取得
	inline float GetSubAlpha() const		{ return m_fSubAlpha; }		// 透明度の減算量取得

protected:
	// 仮想関数
	virtual void CalcDrawMatrix();	// 描画マトリックス計算

	// メンバ関数
	void SetVtx();	// 頂点情報の設定

private:
	// オーバーライド関数
	inline void Release() override { CObject::Release(); }	// 破棄

	// メンバ関数
	void DrawNormal();	// 通常描画
	void DrawShader(CShader* pShader);	// シェーダー描画

	// メンバ変数
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;	// 頂点バッファへのポインタ
	CRenderState* m_pRenderState;		// レンダーステートの情報
	MATRIX* m_pMtxParent;	// 親のマトリックス
	MATRIX m_mtxWorld;		// ワールドマトリックス
	VECTOR3 m_pos;			// 位置
	VECTOR3 m_rot;			// 向き
	VECTOR3 m_growRot;		// 成長向き
	COLOR m_col;			// 色
	float m_fMoveRot;		// 向きの変更量
	float m_fThickness;		// ポリゴンの太さ
	float m_fOuterPlusY;	// ポリゴン外周のY座標加算量
	float m_fSetWidth;		// 生成時の横ずれ量
	float m_fSetAlpha;		// 生成時の透明度
	float m_fAddWidth;		// 横ずれの加算量
	float m_fAddHeight;		// 縦ずれの加算量
	float m_fSubAlpha;		// 透明度の減算量
	float m_fGrowWidth;		// 横ずれの成長量
	float m_fGrowHeight;	// 縦ずれの成長量
	float m_fGrowAlpha;		// 透明度の成長量
	int m_nNumVtx;			// 必要頂点数
	int m_nNumAround;		// 渦の周回数
	int m_nPattern;			// 渦の分割数
	int m_nTextureIdx;		// テクスチャインデックス
};

#endif	// _OBJECT_TORNADO_H_
