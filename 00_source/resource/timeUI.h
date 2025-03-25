//============================================================
//
//	タイムUIヘッダー [timeUI.h]
//	Author：藤田勇一
//
//============================================================
//************************************************************
//	二重インクルード防止
//************************************************************
#ifndef _TIME_UI_H_
#define _TIME_UI_H_

//************************************************************
//	インクルードファイル
//************************************************************
#include "object.h"
#include "value.h"

//************************************************************
//	前方宣言
//************************************************************
class CObject2D;	// オブジェクト2Dクラス

//************************************************************
//	クラス定義
//************************************************************
// タイムUIクラス
class CTimeUI : public CObject
{
public:
	// 定数
	static constexpr int MAX_MIN	= 2;	// タイマーの分の桁数
	static constexpr int MAX_SEC	= 2;	// タイマーの秒の桁数
	static constexpr int MAX_MSEC	= 3;	// タイマーのミリ秒の桁数
	static constexpr int MAX_PART	= 2;	// 区切りの最大数
	static constexpr int MAX_DIGIT	= (MAX_MIN + MAX_SEC + MAX_MSEC);	// タイマーの桁数 (分・秒・ミリ秒)

	// コンストラクタ
	CTimeUI();

	// デストラクタ
	~CTimeUI() override;

	// オーバーライド関数
	HRESULT Init() override;	// 初期化
	void Uninit() override;		// 終了
	void Update(const float fDeltaTime) override;		// 更新
	void Draw(CShader* pShader = nullptr) override;		// 描画
	void SetPriority(const int nPriority) override;		// 優先順位設定
	void SetEnableUpdate(const bool bUpdate) override;	// 更新状況設定
	void SetEnableDraw(const bool bDraw) override;		// 描画状況設定
	void SetVec3Position(const VECTOR3& rPos) override;	// 位置設定
	void SetVec3Rotation(const VECTOR3& rRot) override;	// 向き設定
	inline VECTOR3 GetVec3Position() const override { return m_pos; }	// 位置取得
	inline VECTOR3 GetVec3Rotation() const override { return m_rot; }	// 向き取得

	// 静的メンバ関数
	static CTimeUI* Create	// 生成
	( // 引数
		const float fTime,			// 表示時間
		const VECTOR3& rPos,		// 位置
		const VECTOR3& rSizeValue,	// 数字の大きさ
		const VECTOR3& rSizePart,	// 区切りの大きさ
		const VECTOR3& rSpaceValue,	// 数字の空白
		const VECTOR3& rSpacePart,	// 区切りの空白
		const CValue::EType type = CValue::TYPE_NORMAL,	// 数字種類
		const EAlignX alignX = XALIGN_CENTER,			// 横配置
		const EAlignY alignY = YALIGN_CENTER,			// 縦配置
		const VECTOR3& rRot = VEC3_ZERO,				// 向き
		const COLOR& rCol = color::White()				// 色
	);

	// 仮想関数
	virtual inline void SetTime(const float fTime);				// 表示時間設定
	virtual inline float GetTime() const	{ return m_fTime; }	// 表示時間取得
	virtual inline int GetMin() const		{ return (int)((DWORD)(m_fTime * 1000.0f) / 60000); }		// 分取得
	virtual inline int GetSec() const		{ return (int)((DWORD)(m_fTime * 1000.0f) / 1000 % 60); }	// 秒取得
	virtual inline int GetMSec() const		{ return (int)((DWORD)(m_fTime * 1000.0f) % 1000); }		// ミリ秒取得

	// メンバ関数
	void SetValueType(const CValue::EType type);	// 数字の種類設定
	void SetSizeValue(const VECTOR3& rSize);		// 区切りの大きさ設定
	void SetSizePart(const VECTOR3& rSize);			// 数字の大きさ設定
	void SetSpaceValue(const VECTOR3& rSpace);		// 区切りの空白設定
	void SetSpacePart(const VECTOR3& rSpace);		// 数字の空白設定
	void SetAlpha(const float fAlpha);				// 透明度の設定
	void SetColor(const COLOR& rCol);				// 色の設定
	void SetAlignX(const EAlignX align);			// 横配置設定
	void SetAlignY(const EAlignY align);			// 縦配置設定
	float GetTimeWidth() const;						// タイム全体の横幅取得
	float GetTimeHeight() const;					// タイム全体の縦幅取得
	VECTOR3 GetTimeSize() const;					// タイム全体の大きさ取得
	inline CValue::EType GetValueType() const	{ return m_type; }			// 数字の種類取得
	inline VECTOR3 GetSizeValue() const			{ return m_sizeValue; }		// 区切りの大きさ取得
	inline VECTOR3 GetSizePart() const			{ return m_sizePart; }		// 数字の大きさ取得
	inline VECTOR3 GetSpaceValue() const		{ return m_spaceValue; }	// 区切りの空白取得
	inline VECTOR3 GetSpacePart() const			{ return m_spacePart; }		// 数字の空白取得
	inline float GetAlpha() const				{ return m_col.a; }			// 透明度の設定
	inline COLOR GetColor() const				{ return m_col;}			// 色の設定
	inline EAlignX GetAlignX() const			{ return m_alignX; }		// 横配置取得
	inline EAlignY GetAlignY() const			{ return m_alignY; }		// 縦配置取得

private:
	// オーバーライド関数
	inline void Release() override { CObject::Release(); }	// 破棄

	// メンバ関数
	void SetPositionRelative();	// 相対位置設定
	void SetTexNum();			// 数字のテクスチャ座標設定

	// メンバ変数
	CValue* m_apValue[MAX_DIGIT];	// 数値の情報
	CObject2D* m_apPart[MAX_PART];	// 区切りの情報
	CValue::EType m_type;	// 数字種類
	VECTOR3 m_pos;			// 原点位置
	VECTOR3 m_rot;			// 原点向き
	VECTOR3 m_sizeValue;	// 数字の大きさ
	VECTOR3 m_sizePart;		// 区切りの大きさ
	VECTOR3 m_spaceValue;	// 数字の空白
	VECTOR3 m_spacePart;	// 区切りの空白
	COLOR m_col;			// 色
	EAlignX m_alignX;		// 横配置
	EAlignY m_alignY;		// 縦配置
	float m_fTime;			// 表示時間
};

#endif	// _TIME_UI_H_
