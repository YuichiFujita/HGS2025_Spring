//============================================================
//
//	振動文字列2Dヘッダー [shakeString2D.h]
//	Author：藤田勇一
//
//============================================================
//************************************************************
//	二重インクルード防止
//************************************************************
#ifndef _SHAKE_STRING2D_H_
#define _SHAKE_STRING2D_H_

//************************************************************
//	インクルードファイル
//************************************************************
#include "string2D.h"

//************************************************************
//	クラス定義
//************************************************************
// 振動文字列2Dクラス
class CShakeString2D : public CString2D
{
public:
	// コンストラクタ
	CShakeString2D();

	// デストラクタ
	~CShakeString2D() override;

	// オーバーライド関数
	HRESULT Init() override;	// 初期化
	void Uninit() override;		// 終了
	void Update(const float fDeltaTime) override;		// 更新
	void Draw(CShader* pShader = nullptr) override;		// 描画
	void SetVec3Position(const VECTOR3& rPos) override;	// 位置設定

	// 静的メンバ関数
	static CShakeString2D* Create	// 生成 (マルチバイト文字列)
	( // 引数
		const std::string& rFilePath,	// フォントパス
		const bool bItalic,				// イタリック
		const std::string& rStr,		// 指定文字列
		const VECTOR3& rPos,			// 原点位置
		const float fNextTime = 0.1f,			// 文字振動の待機時間
		const float fMove = 1.0f,				// 振動移動量
		const float fHeight = 100.0f,			// 文字縦幅
		const EAlignX alignX = XALIGN_CENTER,	// 横配置
		const VECTOR3& rRot = VEC3_ZERO,		// 原点向き
		const COLOR& rCol = color::White()		// 色
	);
	static CShakeString2D* Create	// 生成 (ワイド文字列)
	( // 引数
		const std::string& rFilePath,	// フォントパス
		const bool bItalic,				// イタリック
		const std::wstring& rStr,		// 指定文字列
		const VECTOR3& rPos,			// 原点位置
		const float fNextTime = 0.1f,			// 文字振動の待機時間
		const float fMove = 1.0f,				// 振動移動量
		const float fHeight = 100.0f,			// 文字縦幅
		const EAlignX alignX = XALIGN_CENTER,	// 横配置
		const VECTOR3& rRot = VEC3_ZERO,		// 原点向き
		const COLOR& rCol = color::White()		// 色
	);

	// メンバ関数
	inline void SetNextTime(const float fNextTime)		{ m_fNextTime = fNextTime; }	// 文字振動の待機時間設定
	inline float GetNextTime() const					{ return m_fNextTime; }			// 文字振動の待機時間取得
	inline void SetShakeMove(const float fShakeMove)	{ m_fShakeMove = fShakeMove; }	// 振動の移動量設定
	inline float GetShakeMove() const					{ return m_fShakeMove; }		// 振動の移動量取得

private:
	// メンバ関数
	void UpdateShake(const float fDeltaTime);	// 振動更新

	// メンバ変数
	VECTOR3 m_posSave;	// 保存位置
	float m_fShakeMove;	// 振動の移動量
	float m_fNextTime;	// 次振動までの待機時間
	float m_fCurTime;	// 現在の待機時間
};

#endif	// _SHAKE_STRING2D_H_
