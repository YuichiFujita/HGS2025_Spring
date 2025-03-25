//============================================================
//
//	軌跡ヘッダー [orbit.h]
//	Author：藤田勇一
//
//============================================================
//************************************************************
//	二重インクルード防止
//************************************************************
#ifndef _ORBIT_H_
#define _ORBIT_H_

//************************************************************
//	インクルードファイル
//************************************************************
#include "object.h"
#include "renderState.h"

//************************************************************
//	定数宣言
//************************************************************
namespace orbit
{
	const int MAX_OFFSET		= 2;	// オフセットの数
	const int DEFAULT_PART		= 10;	// デフォルトの分割数
	const int DEFAULT_TEXPART	= 1;	// デフォルトのテクスチャ分割数
}

//************************************************************
//	クラス定義
//************************************************************
// 軌跡クラス
class COrbit : public CObject
{
public:
	// 状態列挙
	enum EState
	{
		STATE_NONE = 0,	// 何もしない状態
		STATE_NORMAL,	// 通常状態
		STATE_VANISH,	// 消失状態
		STATE_MAX		// この列挙型の総数
	};

	// コンストラクタ
	COrbit();

	// デストラクタ
	~COrbit() override;

	// オフセット構造体
	struct SOffset
	{
	public:
		// コンストラクタ
		SOffset() {}

		SOffset(const VECTOR3& rOffset0, const VECTOR3& rOffset1, const COLOR& rCol)
		{ aOffset[0] = rOffset0; aOffset[1] = rOffset1; aCol[0] = aCol[1] = rCol; }

		SOffset(const VECTOR3& rOffset0, const VECTOR3& rOffset1, const COLOR& rCol0, const COLOR& rCol1)
		{ aOffset[0] = rOffset0; aOffset[1] = rOffset1; aCol[0] = rCol0; aCol[1] = rCol1; }

		// デストラクタ
		~SOffset() {}

		// メンバ変数
		VECTOR3	aOffset[orbit::MAX_OFFSET];	// 両端のオフセット
		COLOR	aCol[orbit::MAX_OFFSET];	// 両端の基準色
	};

	// 軌跡構造体
	struct SOrbit
	{
		MATRIX		aMtxWorldPoint[orbit::MAX_OFFSET];	// 両端のワールドマトリックス
		SOffset			offset;		// オフセット情報
		VECTOR3*	pPosPoint;	// 各頂点座標
		COLOR*		pColPoint;	// 各頂点カラー
		MATRIX*		pMtxParent;	// 親のマトリックス
		MATRIX		mtxVanish;	// 消失開始時の親のマトリックス
		int nPart;		// 分割数
		int nTexPart;	// テクスチャ分割数
		bool bAlpha;	// 透明化状況
		bool bInit;		// 初期化状況
	};

	// オーバーライド関数
	HRESULT Init() override;	// 初期化
	void Uninit() override;		// 終了
	void Update(const float fDeltaTime) override;	// 更新
	void Draw(CShader* pShader = nullptr) override;	// 描画

	// 静的メンバ関数
	static COrbit *Create	// 生成
	( // 引数
		MATRIX *pMtxParent,	// 親マトリックス
		const SOffset offset,	// オフセット情報
		const int nPart		= orbit::DEFAULT_PART,		// 分割数
		const int nTexPart	= orbit::DEFAULT_TEXPART,	// テクスチャ分割数
		const bool bAlpha	= true,						// 透明化状況
		const CRenderState::EBlend blend = CRenderState::BLEND_ADD	// αブレンド状況
	);

	// メンバ関数
	CRenderState* GetRenderState();				// レンダーステート情報取得
	void BindTexture(const int nTextureID);			// テクスチャ割当 (インデックス)
	void BindTexture(const char* pTexturePath);		// テクスチャ割当 (パス)
	void DeleteMatrixParent();					// 親マトリックス削除
	void SetState(const EState state);				// 状態設定
	void SetMatrixParent(MATRIX* pMtxParent);	// 親のマトリックス設定
	void SetOffset(const SOffset offset);			// オフセット情報設定
	void SetTexPart(const int nTexPart);			// テクスチャ分割数設定
	void SetEnableAlpha(const bool bAlpha);			// 透明化状況設定
	void SetEnableInit(const bool bInit);			// 初期化状況設定
	HRESULT SetLength(const int nPart);				// 長さ設定
	inline int GetTextureIndex() const	{ return m_nTextureID; }	// テクスチャインデックス取得
	inline EState GetState() const		{ return m_state; }			// 状態取得

protected:
	// メンバ関数
	void SetVtx();	// 頂点情報の設定

private:
	// オーバーライド関数
	void Release() override { CObject::Release(); }	// 破棄

	// メンバ関数
	void DrawNormal();	// 通常描画
	void DrawShader(CShader* pShader);	// シェーダー描画

	// メンバ変数
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;	// 頂点バッファへのポインタ
	CRenderState* m_pRenderState;		// レンダーステートの情報
	SOrbit m_orbit;			// 軌跡の情報
	EState m_state;			// 状態
	int m_nCounterState;	// 状態管理カウンター
	int m_nNumVtx;			// 必要頂点数
	int m_nTextureID;		// テクスチャインデックス
};

#endif	// _ORBIT_H_
