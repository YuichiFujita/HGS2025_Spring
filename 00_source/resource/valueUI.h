//============================================================
//
//	数字UIヘッダー [valueUI.h]
//	Author：藤田勇一
//
//============================================================
//************************************************************
//	二重インクルード防止
//************************************************************
#ifndef _VALUE_UI_H_
#define _VALUE_UI_H_

//************************************************************
//	インクルードファイル
//************************************************************
#include "object.h"
#include "value.h"

//************************************************************
//	前方宣言
//************************************************************
class CObject2D;	// オブジェクト2Dクラス
class CMultiValue;	// マルチ数字クラス

//************************************************************
//	クラス定義
//************************************************************
// 数字UIクラス
class CValueUI : public CObject
{
public:
	// コンストラクタ
	CValueUI();

	// デストラクタ
	~CValueUI() override;

	// オーバーライド関数
	HRESULT Init() override;	// 初期化
	void Uninit() override;		// 終了
	void Update(const float fDeltaTime) override;	// 更新
	void Draw(CShader* pShader = nullptr) override;	// 描画
	void SetPriority(const int nPriority) override;	// 優先順位設定
	void SetEnableDraw(const bool bDraw) override;	// 描画状況設定
	void SetVec3Position(const VECTOR3& rPos) override;					// 位置設定
	inline VECTOR3 GetVec3Position() const override { return m_pos; }	// 位置取得

	// 静的メンバ関数
	static CValueUI* Create	// 生成 (テクスチャ指定なし)
	( // 引数
		const CValue::EType type,	// 数字種類
		const int nDigit,			// 桁数
		const VECTOR3& rPos,		// 位置
		const VECTOR3& rSpace,		// 行間
		const VECTOR3& rSpaceValue,	// 数字行間
		const VECTOR3& rSizeTitle,	// タイトル大きさ
		const VECTOR3& rSizeValue,	// 数字大きさ
		const VECTOR3& rRotTitle = VEC3_ZERO,		// タイトル向き
		const VECTOR3& rRotValue = VEC3_ZERO,		// 数字向き
		const COLOR& rColTitle = color::White(),	// タイトル色
		const COLOR& rColValue = color::White()		// 数字色
	);
	static CValueUI* Create	// 生成 (テクスチャインデックス指定)
	( // 引数
		const int nTextureIdx,		// タイトルテクスチャインデックス
		const CValue::EType type,	// 数字種類
		const int nDigit,			// 桁数
		const VECTOR3& rPos,		// 位置
		const VECTOR3& rSpace,		// 行間
		const VECTOR3& rSpaceValue,	// 数字行間
		const VECTOR3& rSizeTitle,	// タイトル大きさ
		const VECTOR3& rSizeValue,	// 数字大きさ
		const VECTOR3& rRotTitle = VEC3_ZERO,		// タイトル向き
		const VECTOR3& rRotValue = VEC3_ZERO,		// 数字向き
		const COLOR& rColTitle = color::White(),	// タイトル色
		const COLOR& rColValue = color::White()		// 数字色
	);
	static CValueUI* Create	// 生成 (テクスチャパス指定)
	( // 引数
		const char* pTexturePath,	// タイトルテクスチャパス
		const CValue::EType type,	// 数字種類
		const int nDigit,			// 桁数
		const VECTOR3& rPos,		// 位置
		const VECTOR3& rSpace,		// 行間
		const VECTOR3& rSpaceValue,	// 数字行間
		const VECTOR3& rSizeTitle,	// タイトル大きさ
		const VECTOR3& rSizeValue,	// 数字大きさ
		const VECTOR3& rRotTitle = VEC3_ZERO,		// タイトル向き
		const VECTOR3& rRotValue = VEC3_ZERO,		// 数字向き
		const COLOR& rColTitle = color::White(),	// タイトル色
		const COLOR& rColValue = color::White()		// 数字色
	);

	// メンバ関数
	void SetSpace(const VECTOR3& rSpace);				// 行間設定
	void BindTextureTitle(const int nTextureIdx);		// タイトルテクスチャ割当 (インデックス)
	void BindTextureTitle(const char* pTexturePath);	// タイトルテクスチャ割当 (パス)
	void SetRotationTitle(const VECTOR3& rRot);			// タイトル向き設定
	void SetSizeTitle(const VECTOR3& rSize);			// タイトル大きさ設定
	void SetAlphaTitle(const float fAlpha);				// タイトル透明度設定
	void SetColorTitle(const COLOR& rCol);				// タイトル色設定
	inline VECTOR3 GetSpace() const				{ return m_space; }						// 行間取得
	inline VECTOR3 GetRotationTitle() const		{ return m_pTitle->GetVec3Rotation(); }	// タイトル向き取得
	inline VECTOR3 GetSizeTitle() const			{ return m_pTitle->GetVec3Size(); }		// タイトル大きさ取得
	inline float GetAlphaTitle() const			{ return m_pTitle->GetAlpha(); }		// タイトル透明度取得
	inline COLOR GetColorTitle() const			{ return m_pTitle->GetColor(); }		// タイトル色取得
	inline CMultiValue* GetMultiValue() const	{ return m_pValue; }					// 数字情報取得

private:
	// オーバーライド関数
	inline void Release() override { CObject::Release(); }	// 破棄

	// メンバ関数
	void SetPositionRelative();	// 相対位置設定

	// メンバ変数
	CObject2D* m_pTitle;	// タイトル情報
	CMultiValue* m_pValue;	// 数字情報
	VECTOR3 m_pos;			// 位置
	VECTOR3 m_space;		// 行間
};

#endif	// _VALUE_UI_H_
