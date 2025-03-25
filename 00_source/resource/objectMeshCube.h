//============================================================
//
//	オブジェクトメッシュキューブヘッダー [objectMeshCube.h]
//	Author：藤田勇一
//
//============================================================
//************************************************************
//	二重インクルード防止
//************************************************************
#ifndef _OBJECT_MESHCUBE_H_
#define _OBJECT_MESHCUBE_H_

//************************************************************
//	インクルードファイル
//************************************************************
#include "object.h"
#include "renderState.h"

//************************************************************
//	クラス定義
//************************************************************
// オブジェクトメッシュキューブクラス
class CObjectMeshCube : public CObject
{
public:
	// コンストラクタ
	explicit CObjectMeshCube(const CObject::ELabel label = LABEL_NONE, const CObject::EDim dimension = DIM_3D, const int nPriority = DEFAULT_PRIO);

	// デストラクタ
	~CObjectMeshCube() override;

	// 原点列挙
	enum EOrigin
	{
		ORIGIN_CENTER = 0,	// 中心原点
		ORIGIN_DOWN,		// 下原点
		ORIGIN_UP,			// 上原点
		ORIGIN_MAX,			// この列挙型の総数
	};

	// 縁取り列挙
	enum EBorder
	{
		BORDER_OFF = 0,		// 縁取りなし
		BORDER_ON,			// 縁取りあり
		BORDER_MAX,			// この列挙型の総数
	};

	// テクスチャ使用列挙
	enum ETexState
	{
		TEXSTATE_ONE = 0,	// 同一テクスチャ
		TEXSTATE_SELECT,	// 全選択テクスチャ
		TEXSTATE_MAX,		// この列挙型の総数
	};

	// キューブ色列挙
	enum ECubeCol
	{
		CUBECOL_CUBE = 0,	// キューブ色
		CUBECOL_BORDER,		// 縁取り色
		CUBECOL_MAX			// この列挙型の総数
	};

	// キューブ分割数列挙
	enum ECubePart
	{
		CUBEPART_X = 0,		// テクスチャ分割数X
		CUBEPART_Y,			// テクスチャ分割数Y
		CUBEPART_Z,			// テクスチャ分割数Z
		CUBEPART_MAX		// この列挙型の総数
	};

	// テクスチャ構造体
	struct SFaceTex
	{
	public:
		// デフォルトコンストラクタ
		SFaceTex() :
			All		(0),	// 全面のテクスチャインデックス
			Left	(0),	// 左のテクスチャインデックス
			Right	(0),	// 右のテクスチャインデックス
			Bottom	(0),	// 下のテクスチャインデックス
			Top		(0),	// 上のテクスチャインデックス
			Near	(0),	// 前のテクスチャインデックス
			Far		(0)		// 後のテクスチャインデックス
		{}

		// 引数付きコンストラクタ
		SFaceTex(const int in_nAll) :
			All		(in_nAll),		// 全面のテクスチャインデックス
			Left	(0),			// 左のテクスチャインデックス
			Right	(0),			// 右のテクスチャインデックス
			Bottom	(0),			// 下のテクスチャインデックス
			Top		(0),			// 上のテクスチャインデックス
			Near	(0),			// 前のテクスチャインデックス
			Far		(0)				// 後のテクスチャインデックス
		{}
		SFaceTex(const int in_nLeft, const int in_nRight, const int in_nBottom, const int in_nTop, const int in_nNear, const int in_nFar) :
			All		(0),			// 全面のテクスチャインデックス
			Left	(in_nLeft),		// 左のテクスチャインデックス
			Right	(in_nRight),	// 右のテクスチャインデックス
			Bottom	(in_nBottom),	// 下のテクスチャインデックス
			Top		(in_nTop),		// 上のテクスチャインデックス
			Near	(in_nNear),		// 前のテクスチャインデックス
			Far		(in_nFar)		// 後のテクスチャインデックス
		{}
		SFaceTex(const int in_nAll, const int in_nLeft, const int in_nRight, const int in_nBottom, const int in_nTop, const int in_nNear, const int in_nFar) :
			All		(in_nAll),		// 全面のテクスチャインデックス
			Left	(in_nLeft),		// 左のテクスチャインデックス
			Right	(in_nRight),	// 右のテクスチャインデックス
			Bottom	(in_nBottom),	// 下のテクスチャインデックス
			Top		(in_nTop),		// 上のテクスチャインデックス
			Near	(in_nNear),		// 前のテクスチャインデックス
			Far		(in_nFar)		// 後のテクスチャインデックス
		{}

		// デストラクタ
		~SFaceTex() {}

		// メンバ変数
		int All;	// 全面のテクスチャインデックス
		int Left;	// 左のテクスチャインデックス
		int Right;	// 右のテクスチャインデックス
		int Bottom;	// 下のテクスチャインデックス
		int Top;	// 上のテクスチャインデックス
		int Near;	// 前のテクスチャインデックス
		int Far;	// 後のテクスチャインデックス
	};

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
	static CObjectMeshCube* Create	// 生成
	( // 引数
		const VECTOR3&	rPos,		// 位置
		const VECTOR3&	rRot,		// 向き
		const VECTOR3&	rSize,		// 大きさ
		const COLOR&	rCubeCol,	// キューブ色
		const COLOR&	rBorderCol	= color::Black(),		// 縁取り色
		const EBorder	bordState	= BORDER_OFF,			// 縁取り状態
		const float		fBordThick	= 0.0f,					// 縁取り太さ
		const ETexState	texState	= TEXSTATE_ONE,			// テクスチャ状態
		const SFaceTex&	rTexIdx		= SFaceTex(NONE_IDX),	// テクスチャ種類
		const EOrigin	origin		= ORIGIN_CENTER,		// 原点
		const VECTOR2&	rTexPartX	= VEC2_ONE,				// テクスチャ分割数X
		const VECTOR2&	rTexPartY	= VEC2_ONE,				// テクスチャ分割数Y
		const VECTOR2&	rTexPartZ	= VEC2_ONE				// テクスチャ分割数Z
	);

	// メンバ関数
	CRenderState* GetRenderState();						// レンダーステート情報取得
	void BindTexture(const SFaceTex textureIdx);		// テクスチャ割当
	void SetCubeAlpha(const float fAlpha);				// キューブ透明度設定
	void SetCubeColor(const COLOR& rCol);				// キューブ色設定
	void SetBorderAlpha(const float fAlpha);			// 縁取り透明度設定
	void SetBorderColor(const COLOR& rCol);				// 縁取り色設定
	HRESULT SetBorderState(const EBorder bordState);	// 縁取り状態設定
	void SetBorderThick(const float fBordThick);		// 縁取り太さ設定
	void SetTextureState(const ETexState texState);		// テクスチャ状態設定
	void SetTexturePatternX(const VECTOR2& rTexPart);	// テクスチャ分割数X設定
	void SetTexturePatternY(const VECTOR2& rTexPart);	// テクスチャ分割数Y設定
	void SetTexturePatternZ(const VECTOR2& rTexPart);	// テクスチャ分割数Z設定
	void SetOrigin(const EOrigin origin);				// 原点設定
	inline SFaceTex GetTextureIndex() const		{ return m_texIdx; }					// テクスチャインデックス取得
	inline float GetCubeAlpha() const			{ return m_aCol[CUBECOL_CUBE].a; }		// 透明度取得
	inline COLOR GetCubeColor() const			{ return m_aCol[CUBECOL_CUBE]; }		// キューブ色取得
	inline float GetBorderAlpha() const			{ return m_aCol[CUBECOL_BORDER].a; }	// 透明度取得
	inline COLOR GetBorderColor() const			{ return m_aCol[CUBECOL_BORDER]; }		// 縁取り色取得
	inline EBorder GetBorderState() const		{ return m_bordState; }					// 縁取り状態取得
	inline float GetBorderThick() const			{ return m_fBordThick; }				// 縁取り太さ取得
	inline ETexState GetTextureState() const	{ return m_texState; }					// テクスチャ状態取得
	inline VECTOR2 GetTexturePatternX() const	{ return m_aTexPart[CUBEPART_X]; }		// テクスチャ分割数X取得
	inline VECTOR2 GetTexturePatternY() const	{ return m_aTexPart[CUBEPART_Y]; }		// テクスチャ分割数Y取得
	inline VECTOR2 GetTexturePatternZ() const	{ return m_aTexPart[CUBEPART_Z]; }		// テクスチャ分割数Z取得
	inline EOrigin GetOrigin() const			{ return m_origin; }					// 原点取得

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
	COLOR m_aCol[CUBECOL_MAX];			// 色
	VECTOR2 m_aTexPart[CUBEPART_MAX];	// テクスチャ分割数
	MATRIX m_mtxWorld;		// ワールドマトリックス
	VECTOR3 m_pos;			// 位置
	VECTOR3 m_rot;			// 向き
	VECTOR3 m_size;			// 大きさ
	EBorder m_bordState;	// 縁取り使用状態
	float m_fBordThick;		// 縁取り太さ
	ETexState m_texState;	// テクスチャ使用状態
	SFaceTex m_texIdx;		// テクスチャ種類
	EOrigin m_origin;		// 原点
	int m_nNumVtx;			// 必要頂点数
	int m_nNumIdx;			// 必要インデックス数
};

#endif	// _OBJECT_MESHCUBE_H_
