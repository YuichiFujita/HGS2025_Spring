//============================================================
//
//	オブジェクトビルボードヘッダー [objectBillboard.h]
//	Author：藤田勇一
//
//============================================================
//************************************************************
//	二重インクルード防止
//************************************************************
#ifndef _OBJECTBILLBOARD_H_
#define _OBJECTBILLBOARD_H_

//************************************************************
//	インクルードファイル
//************************************************************
#include "object.h"
#include "renderState.h"

//************************************************************
//	クラス定義
//************************************************************
// オブジェクトビルボードクラス
class CObjectBillboard : public CObject
{
public:
	// 原点列挙
	enum EOrigin
	{
		ORIGIN_CENTER = 0,	// 中心原点
		ORIGIN_DOWN,		// 下原点
		ORIGIN_MAX,			// この列挙型の総数
	};

	// 回転列挙
	enum ERotate
	{
		ROTATE_NORMAL = 0,	// 通常回転
		ROTATE_LATERAL,		// 横回転
		ROTATE_MAX,			// この列挙型の総数
	};

	// コンストラクタ
	explicit CObjectBillboard(const CObject::ELabel label = LABEL_NONE, const CObject::EDim dimension = DIM_3D, const int nPriority = DEFAULT_PRIO);

	// デストラクタ
	~CObjectBillboard() override;

	// オーバーライド関数
	HRESULT Init() override;	// 初期化
	void Uninit() override;		// 終了
	void Update(const float fDeltaTime) override;		// 更新
	void Draw(CShader* pShader = nullptr) override;		// 描画
	void SetVec3Position(const VECTOR3& rPos) override;	// 位置設定
	void SetVec3Rotation(const VECTOR3& rRot) override;	// 向き設定
	void SetVec3Size(const VECTOR3& rSize) override;	// 大きさ設定
	inline VECTOR3 GetVec3Position() const override	{ return m_pos; }		// 位置取得
	inline VECTOR3 GetVec3Rotation() const override	{ return m_rot; }		// 向き取得
	inline VECTOR3 GetVec3Size() const override		{ return m_size; }		// 大きさ取得
	inline MATRIX* GetPtrMtxWorld() override		{ return &m_mtxWorld; }	// マトリックスポインタ取得
	inline MATRIX GetMtxWorld() const override		{ return m_mtxWorld; }	// マトリックス取得

	// 静的メンバ関数
	static CObjectBillboard* Create	// 生成
	( // 引数
		const VECTOR3& rPos,	// 位置
		const VECTOR3& rSize,	// 大きさ
		const VECTOR3& rRot = VEC3_ZERO,		// 向き
		const COLOR& rCol = color::White(),		// 色
		const EOrigin origin = ORIGIN_CENTER,	// 原点
		const ERotate rotate = ROTATE_NORMAL	// 回転
	);

	// メンバ関数
	CRenderState* GetRenderState();				// レンダーステート情報取得
	void BindTexture(const int nTextureIdx);	// テクスチャ割当 (インデックス)
	void BindTexture(const char* pTexturePath);	// テクスチャ割当 (パス)
	void SetAlpha(const float fAlpha);			// 透明度設定
	void SetColor(const COLOR& rCol);			// 色設定
	void SetOrigin(const EOrigin origin);		// 原点設定
	void SetRotate(const ERotate rotate);		// 回転設定
	inline int GetTextureIndex() const	{ return m_nTextureIdx; }	// テクスチャインデックス取得
	inline float GetAlpha() const		{ return m_col.a; }			// 透明度取得
	inline COLOR GetColor() const		{ return m_col; }			// 色取得
	inline EOrigin GetOrigin() const	{ return m_origin; }		// 原点取得
	inline ERotate GetRotate() const	{ return m_rotate; }		// 回転取得

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
	MATRIX	m_mtxWorld;		// ワールドマトリックス
	VECTOR3	m_pos;			// 位置
	VECTOR3	m_rot;			// 向き
	VECTOR3	m_size;			// 大きさ
	COLOR	m_col;			// 色
	EOrigin	m_origin;		// 原点
	ERotate	m_rotate;		// 回転
	float	m_fAngle;		// 対角線の角度
	float	m_fLength;		// 対角線の長さ
	int		m_nTextureIdx;	// テクスチャインデックス
};

#endif	// _OBJECTBILLBOARD_H_
