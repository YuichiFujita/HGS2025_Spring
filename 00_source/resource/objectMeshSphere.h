//============================================================
//
//	オブジェクトメッシュスフィアヘッダー [objectMeshSphere.h]
//	Author：藤田勇一
//
//============================================================
//************************************************************
//	二重インクルード防止
//************************************************************
#ifndef _OBJECT_MESHSPHERE_H_
#define _OBJECT_MESHSPHERE_H_

//************************************************************
//	インクルードファイル
//************************************************************
#include "object.h"
#include "objectMeshDome.h"

//************************************************************
//	クラス定義
//************************************************************
// オブジェクトメッシュスフィアクラス
class CObjectMeshSphere : public CObject
{
public:
	// 半球列挙
	enum EDome
	{
		DOME_TOP = 0,	// 上半球
		DOME_BOTTOM,	// 下半球
		DOME_MAX		// この列挙型の総数
	};

	// コンストラクタ
	explicit CObjectMeshSphere(const CObject::ELabel label = LABEL_NONE, const CObject::EDim dimension = DIM_3D, const int nPriority = DEFAULT_PRIO);

	// デストラクタ
	~CObjectMeshSphere() override;

	// オーバーライド関数
	HRESULT Init() override;	// 初期化
	void Uninit() override;		// 終了
	void Update(const float fDeltaTime) override;		// 更新
	void Draw(CShader* pShader = nullptr) override;		// 描画
	void SetPriority(const int nPriority) override;		// 優先順位設定
	void SetVec3Position(const VECTOR3& rPos) override;	// 位置設定
	void SetVec3Rotation(const VECTOR3& rRot) override;	// 向き設定
	inline VECTOR3 GetVec3Position() const override	{ return m_apDome[DOME_TOP]->GetVec3Position(); }	// 位置取得
	inline VECTOR3 GetVec3Rotation() const override	{ return m_apDome[DOME_TOP]->GetVec3Rotation(); }	// 向き取得
	inline MATRIX* GetPtrMtxWorld() override		{ return m_apDome[DOME_TOP]->GetPtrMtxWorld(); }	// マトリックスポインタ取得
	inline MATRIX GetMtxWorld() const override		{ return m_apDome[DOME_TOP]->GetMtxWorld(); }		// マトリックス取得

	// 静的メンバ関数
	static CObjectMeshSphere* Create	// 生成
	( // 引数
		const VECTOR3& rPos,		// 位置
		const VECTOR3& rRot,		// 向き
		const COLOR& rCol,			// 色
		const POSGRID2& rPart,		// 分割数
		const POSGRID2& rTexPart,	// テクスチャ分割数
		const float fRadius			// 半径
	);

	// メンバ関数
	void SetRenderState(CRenderState renderState);	// レンダーステート情報設定
	void BindTexture(const int nTextureIdx);		// テクスチャ割当 (インデックス)
	void BindTexture(const char* pTexturePath);		// テクスチャ割当 (パス)
	void SetAlpha(const float fAlpha);				// 透明度設定
	void SetColor(const COLOR& rCol);				// 色設定
	void SetRadius(const float fRadius);			// 半径設定
	HRESULT SetPattern(const POSGRID2& rPart);		// 分割数設定
	void SetTexPattern(const POSGRID2& rTexPart);	// テクスチャ分割数設定
	inline int GetTextureIndex() const		{ return m_apDome[DOME_TOP]->GetTextureIndex(); }	// テクスチャインデックス取得
	inline float GetAlpha() const			{ return m_apDome[DOME_TOP]->GetAlpha(); }			// 透明度取得
	inline COLOR GetColor() const			{ return m_apDome[DOME_TOP]->GetColor(); }			// 色取得
	inline float GetRadius() const			{ return m_apDome[DOME_TOP]->GetRadius(); }			// 半径取得
	inline POSGRID2 GetPattern() const		{ return m_apDome[DOME_TOP]->GetPattern(); }		// 分割数取得
	inline POSGRID2 GetTexPattern() const	{ return m_apDome[DOME_TOP]->GetTexPattern(); }		// テクスチャ分割数取得

private:
	// オーバーライド関数
	inline void Release() override { CObject::Release(); }	// 破棄

	// メンバ変数
	CObjectMeshDome* m_apDome[DOME_MAX];	// 半球の情報
	POSGRID2 m_part;	// 分割数
	POSGRID2 m_texPart;	// テクスチャ分割数
};

#endif	// _OBJECT_MESHSPHERE_H_
