//============================================================
//
//	マルチ数字ヘッダー [multiValue.h]
//	Author：藤田勇一
//
//============================================================
//************************************************************
//	二重インクルード防止
//************************************************************
#ifndef _MULTIVALUE_H_
#define _MULTIVALUE_H_

//************************************************************
//	インクルードファイル
//************************************************************
#include "object.h"
#include "value.h"

//************************************************************
//	クラス定義
//************************************************************
// マルチ数字クラス
class CMultiValue : public CObject
{
public:
	// コンストラクタ
	CMultiValue();

	// デストラクタ
	~CMultiValue() override;

	// オーバーライド関数
	HRESULT Init() override;	// 初期化
	void Uninit() override;		// 終了
	void Update(const float fDeltaTime) override;		// 更新
	void Draw(CShader* pShader = nullptr) override;		// 描画
	void SetPriority(const int nPriority) override;		// 優先順位設定
	void SetVec3Position(const VECTOR3& rPos) override;	// 位置設定
	void SetVec3Rotation(const VECTOR3& rRot) override;	// 向き設定
	void SetVec3Size(const VECTOR3& rSize) override;	// 大きさ設定
	inline VECTOR3 GetVec3Position() const override	{ return m_pos; }	// 位置取得
	inline VECTOR3 GetVec3Rotation() const override	{ return m_rot; }	// 向き取得
	inline VECTOR3 GetVec3Size() const override		{ return m_size; }	// 大きさ取得

	// 静的メンバ関数
	static CMultiValue* Create	// 生成
	( // 引数
		const CValue::EType type,	// 数字種類
		const int nNum,				// 数字
		const int nDigit,			// 桁数
		const VECTOR3& rPos,		// 位置
		const VECTOR3& rSize,		// 大きさ
		const VECTOR3& rSpace,		// 空白
		const EAlignX alignX = XALIGN_CENTER,	// 横配置
		const EAlignY alignY = YALIGN_CENTER,	// 縦配置
		const VECTOR3& rRot = VEC3_ZERO,		// 向き
		const COLOR& rCol = color::White()		// 色
	);

	// メンバ関数
	void AddNum(const int nNum);			// 数値加算
	void SetNum(const int nNum);			// 数値設定
	void SetMin(const int nMin);			// 最小値設定
	void SetMax(const int nMax);			// 最大値設定
	HRESULT SetDigit(const int nDigit);		// 桁数設定
	void SetAlignX(const EAlignX align);	// 横配置設定
	void SetAlignY(const EAlignY align);	// 縦配置設定
	void SetType(const CValue::EType type);	// 種類設定
	void SetAlpha(const float fAlpha);		// 透明度設定
	void SetColor(const COLOR& rCol);		// 色設定
	void SetSpace(const VECTOR3& rSpace);	// 空白設定
	float GetValueWidth() const;			// 数字全体の横幅取得
	float GetValueHeight() const;			// 数字全体の縦幅取得
	VECTOR3 GetValueSize() const;			// 数字全体の大きさ取得

	inline int GetNum() const	{ return m_nNum; }	// 数値取得
	inline int GetMin() const	{ return m_nMin; }	// 最小値取得
	inline int GetMax() const	{ return m_nMax; }	// 最大値取得

	inline int GetDigit() const			{ return (int)m_listValue.size(); }	// 桁数取得
	inline EAlignX GetAlignX() const	{ return m_alignX; }	// 横配置取得
	inline EAlignY GetAlignY() const	{ return m_alignY; }	// 縦配置取得
	inline float GetAlpha() const		{ return m_col.a; }		// 透明度取得
	inline COLOR GetColor() const		{ return m_col; }		// 色取得
	inline VECTOR3 GetSpace() const		{ return m_space; }		// 空白取得

private:
	// オーバーライド関数
	inline void Release() override { CObject::Release(); }	// 破棄

	// メンバ関数
	void SetPositionRelative();	// 相対位置設定
	void SetTexNum();			// 数字のテクスチャ座標設定

	// メンバ変数
	std::list<CValue*> m_listValue;	// 数字リスト
	VECTOR3 m_pos;		// 原点位置
	VECTOR3 m_rot;		// 原点向き
	VECTOR3 m_size;		// 大きさ
	VECTOR3 m_space;	// 空白
	COLOR m_col;		// 色
	EAlignX m_alignX;	// 横配置
	EAlignY m_alignY;	// 縦配置
	int m_nNum;			// 数字
	int m_nMin;			// 最小値
	int m_nMax;			// 最大値
};

#endif	// _MULTIVALUE_H_
