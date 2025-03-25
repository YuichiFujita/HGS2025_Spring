//============================================================
//
//	アニメーション2Dヘッダー [anim2D.h]
//	Author：藤田勇一
//
//============================================================
//************************************************************
//	二重インクルード防止
//************************************************************
#ifndef _ANIM2D_H_
#define _ANIM2D_H_

//************************************************************
//	インクルードファイル
//************************************************************
#include "object2D.h"

//************************************************************
//	クラス定義
//************************************************************
// アニメーション2Dクラス
class CAnim2D : public CObject2D
{
public:
	// コンストラクタ
	explicit CAnim2D(const CObject::ELabel label = LABEL_NONE, const EDim dimension = DIM_2D, const int nPriority = DEFAULT_PRIO);

	// デストラクタ
	~CAnim2D() override;

	// オーバーライド関数
	HRESULT Init() override;	// 初期化
	void Uninit() override;		// 終了
	void Update(const float fDeltaTime) override;		// 更新
	void Draw(CShader* pShader = nullptr) override;		// 描画
	void SetVec3Position(const VECTOR3& rPos) override;	// 位置設定
	void SetVec3Rotation(const VECTOR3& rRot) override;	// 向き設定
	void SetVec3Size(const VECTOR3& rSize) override;	// 大きさ設定

	// 静的メンバ関数
	static CAnim2D* Create	// 生成
	( // 引数
		const int nWidthPtrn,	// テクスチャの横の分割数
		const int nHeightPtrn,	// テクスチャの縦の分割数
		const VECTOR3& rPos,	// 位置
		const VECTOR3& rSize = VEC3_ONE,	// 大きさ
		const VECTOR3& rRot = VEC3_ZERO,	// 向き
		const COLOR& rCol = color::White()	// 色
	);

	// メンバ関数
	void SetAlpha(const float fAlpha);				// 透明度設定
	void SetColor(const COLOR& rCol);				// 色設定
	void SetPattern(const int nPattern);			// パターンの設定
	void SetMaxPattern(const int nMaxPtrn);			// パターンの総数の設定
	void SetWidthPattern(const int nWidthPtrn);		// テクスチャの横分割数の設定
	void SetHeightPattern(const int nHeightPtrn);	// テクスチャの縦分割数の設定
	void SetCounter(const int nCntChange);			// カウンターの設定
	void SetEnableStop(const bool bStop);			// 停止状況の設定
	void SetEnablePlayBack(const bool bPlayBack);	// 逆再生状況の設定
	inline int GetPattern() const		{ return m_nPattern; }	// パターンの取得
	inline int GetLoopAnimation() const	{ return m_nNumLoop; }	// パターン繰り返し数の取得

private:
	// メンバ変数
	int m_nCounter;		// アニメーションカウンター
	int m_nCntChange;	// パターン変更カウント
	int m_nPattern;		// アニメーションパターン
	int m_nMaxPtrn;		// パターンの総数
	int m_nWidthPtrn;	// テクスチャの横の分割数
	int m_nHeightPtrn;	// テクスチャの縦の分割数
	int m_nNumLoop;		// パターン繰り返し数
	bool m_bStop;		// 停止状況
	bool m_bPlayBack;	// 逆再生状況
};

#endif	// _ANIM2D_H_
