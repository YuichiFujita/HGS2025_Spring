//============================================================
//
//	スクロール2Dヘッダー [scroll2D.h]
//	Author：藤田勇一
//
//============================================================
//************************************************************
//	二重インクルード防止
//************************************************************
#ifndef _SCROLL2D_H_
#define _SCROLL2D_H_

//************************************************************
//	インクルードファイル
//************************************************************
#include "main.h"
#include "object2D.h"

//************************************************************
//	クラス定義
//************************************************************
// スクロール2Dクラス
class CScroll2D : public CObject2D
{
public:
	// コンストラクタ
	explicit CScroll2D(const CObject::ELabel label = LABEL_NONE, const CObject::EDim dimension = DIM_2D, const int nPriority = DEFAULT_PRIO);

	// デストラクタ
	~CScroll2D() override;

	// オーバーライド関数
	HRESULT Init() override;	// 初期化
	void Uninit() override;		// 終了
	void Update(const float fDeltaTime) override;		// 更新
	void Draw(CShader* pShader = nullptr) override;		// 描画
	void SetVec3Position(const VECTOR3& rPos) override;	// 位置設定
	void SetVec3Rotation(const VECTOR3& rRot) override;	// 向き設定
	void SetVec3Size(const VECTOR3& rSize) override;	// 大きさ設定

	// 静的メンバ関数
	static CScroll2D* Create	// 生成
	( // 引数
		const float fMoveU,		// 横座標の移動量
		const float fMoveV,		// 縦座標の移動量
		const VECTOR3& rPos,	// 位置
		const VECTOR3& rSize = VEC3_ONE,	// 大きさ
		const VECTOR3& rRot = VEC3_ZERO,	// 向き
		const COLOR& rCol = color::White()	// 色
	);

	// メンバ関数
	void SetAlpha(const float fAlpha);		// 透明度設定
	void SetColor(const COLOR& rCol);		// 色設定
	void SetTexU(const float fTexU);		// 横座標の開始位置設定
	void SetTexV(const float fTexV);		// 縦座標の開始位置設定
	void SetOffsetU(const float fOffsetU);	// 横座標のオフセット位置設定
	void SetOffsetV(const float fOffsetV);	// 縦座標のオフセット位置設定
	void SetMoveU(const float fMoveU);		// 横座標の移動量設定
	void SetMoveV(const float fMoveV);		// 縦座標の移動量設定
	inline float GetTexU()		{ return m_fTexU; }		// 横座標の開始位置取得
	inline float GetTexV()		{ return m_fTexV; }		// 縦座標の開始位置取得
	inline float GetOffsetU()	{ return m_fOffsetU; }	// 横座標のオフセット位置取得
	inline float GetOffsetV()	{ return m_fOffsetV; }	// 縦座標のオフセット位置取得
	inline float GetMoveU()		{ return m_fMoveU; }	// 横座標の移動量取得
	inline float GetMoveV()		{ return m_fMoveV; }	// 縦座標の移動量取得
	inline int GetNumLoopU()	{ return m_nNumLoopU; }	// 横パターン繰り返し数取得
	inline int GetNumLoopV()	{ return m_nNumLoopV; }	// 縦パターン繰り返し数取得

private:
	// メンバ関数
	bool NormalizeTexPos(float* pTexPos);	// テクスチャ座標正規化

	// メンバ変数
	float m_fTexU;		// テクスチャ横座標の開始位置
	float m_fTexV;		// テクスチャ縦座標の開始位置
	float m_fOffsetU;	// テクスチャ横座標のオフセット位置
	float m_fOffsetV;	// テクスチャ縦座標のオフセット位置
	float m_fMoveU;		// テクスチャ横座標の移動量
	float m_fMoveV;		// テクスチャ縦座標の移動量
	int m_nNumLoopU;	// 横パターン繰り返し数
	int m_nNumLoopV;	// 縦パターン繰り返し数
};

#endif	// _SCROLL2D_H_
