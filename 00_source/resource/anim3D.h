//============================================================
//
//	アニメーション3Dヘッダー [anim3D.h]
//	Author：藤田勇一
//
//============================================================
//************************************************************
//	二重インクルード防止
//************************************************************
#ifndef _ANIM3D_H_
#define _ANIM3D_H_

//************************************************************
//	インクルードファイル
//************************************************************
#include "object3D.h"

//************************************************************
//	クラス定義
//************************************************************
// アニメーション3Dクラス
class CAnim3D : public CObject3D
{
public:
	// 定数
	static constexpr float DEF_NEXT = 1.0f;	// パターン変更時間 (デフォルト)

	// コンストラクタ
	explicit CAnim3D(const CObject::ELabel label = LABEL_NONE, const EDim dimension = DIM_3D, const int nPriority = DEFAULT_PRIO);

	// デストラクタ
	~CAnim3D() override;

	// オーバーライド関数
	HRESULT Init() override;	// 初期化
	void Uninit() override;		// 終了
	void Update(const float fDeltaTime) override;		// 更新
	void Draw(CShader* pShader = nullptr) override;		// 描画
	void SetVec3Position(const VECTOR3& rPos) override;	// 位置設定
	void SetVec3Rotation(const VECTOR3& rRot) override;	// 向き設定
	void SetVec3Size(const VECTOR3& rSize) override;	// 大きさ設定

	// 静的メンバ関数
	static CAnim3D* Create	// 生成
	( // 引数
		const POSGRID2& rPtrn,	// テクスチャ分割数
		const VECTOR3& rPos,	// 位置
		const float fNextTime = DEF_NEXT,	// パターン変更時間
		const VECTOR3& rSize = VEC3_ONE,	// 大きさ
		const VECTOR3& rRot = VEC3_ZERO,	// 向き
		const COLOR& rCol = color::White()	// 色
	);

	// メンバ関数
	void SetAlpha(const float fAlpha);				// 透明度設定
	void SetColor(const COLOR& rCol);				// 色設定
	void SetCurPtrn(const int nPtrn);				// 現在パターン設定
	void SetTexPtrn(const POSGRID2& rPtrn);			// テクスチャ分割数設定
	void SetTexPtrnWidth(const int nTexPtrnW);		// テクスチャ横分割数設定
	void SetTexPtrnHeight(const int nTexPtrnH);		// テクスチャ縦分割数設定
	void SetEnablePlay(const bool bPlay);			// 再生フラグ設定
	void SetEnablePlayBack(const bool bPlayBack);	// 逆再生フラグ設定
	void SetEnableLoop(const bool bLoop);			// ループフラグ設定
	void ResetCurPtrn();							// 現在パターン初期化
	void SetNextTime(const int nPtrnIdx, const float fNextTime);	// パターン変更時間設定 (パターン指定)
	void SetNextTime(const float fNextTime);						// パターン変更時間設定 (全パターン)

	inline int GetCurPtrn() const	{ return m_nCurPtrn; }	// 現在パターン取得
	inline void ResetNumLoop()		{ m_nNumLoop = 0; }		// パターン繰り返し数初期化
	inline int GetLoopAnim() const	{ return m_nNumLoop; }	// パターン繰り返し数取得
	inline bool IsPlay() const		{ return m_bPlay; }		// 再生フラグ取得
	inline bool IsPlayBack() const	{ return m_bPlayBack; }	// 逆再生フラグ取得
	inline bool IsLoop() const		{ return m_bLoop; }		// ループフラグ取得
	inline bool IsFinish() const	{ return m_bFinish; }	// 終了フラグ取得
	inline float GetCurTime() const			{ return m_fCurTime; }		// 現在の待機時間
	inline float GetCurWholeTime() const	{ return m_fCurWholeTime; }	// 現在の全体時間
	inline float GetMaxWholeTime() const	{ return m_fMaxWholeTime; }	// 総全体時間

private:
	// メンバ関数
	HRESULT SetMaxPtrn(const int nMaxPtrn);	// パターン総数設定
	void NextPtrn(const float fDeltaTime);	// パターン加算
	void BackPtrn(const float fDeltaTime);	// パターン減算

	// メンバ変数
	std::function<void(float)> m_funcPattern;	// パターン変更関数ポインタ
	POSGRID2 m_ptrn;		// テクスチャ分割数
	float* m_pNextTime;		// パターン変更時間
	float m_fCurTime;		// 現在の待機時間
	float m_fCurWholeTime;	// 現在の全体時間
	float m_fMaxWholeTime;	// 総全体時間
	int m_nCurPtrn;			// 現在のパターン
	int m_nMaxPtrn;			// パターン総数
	int m_nNumLoop;			// パターン繰り返し数
	bool m_bPlay;			// 再生フラグ
	bool m_bPlayBack;		// 逆再生フラグ
	bool m_bLoop;			// ループフラグ
	bool m_bFinish;			// 終了フラグ
};

#endif	// _ANIM3D_H_
