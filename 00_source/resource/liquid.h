//============================================================
//
//	液体ヘッダー [liquid.h]
//	Author：藤田勇一
//
//============================================================
//************************************************************
//	二重インクルード防止
//************************************************************
#ifndef _LIQUID_H_
#define _LIQUID_H_

//************************************************************
//	インクルードファイル
//************************************************************
#include "object.h"
#include "scrollMeshField.h"

//************************************************************
//	クラス定義
//************************************************************
// 液体クラス
class CLiquid : public CObject
{
public:
	// 種類列挙
	enum EType
	{
		TYPE_WATER = 0,	// 水
		TYPE_MAX		// この列挙型の総数
	};

	// 上下列挙
	enum ELiquid
	{
		LIQUID_LOW = 0,	// 下
		LIQUID_HIGH,	// 上
		LIQUID_MAX		// この列挙型の総数
	};

	// コンストラクタ
	CLiquid();

	// デストラクタ
	~CLiquid() override;

	// テクスチャ移動量構造体
	struct STexMove
	{
	public:
		// コンストラクタ
		STexMove() {}
		STexMove(const VECTOR2 TexMoveLow, const VECTOR2 TexMoveHigh)
		{ texMoveLow = TexMoveLow; texMoveHigh = TexMoveHigh; }

		// デストラクタ
		~STexMove() {}

		// メンバ変数
		VECTOR2 texMoveLow;		// 下液体のテクスチャ移動量
		VECTOR2 texMoveHigh;	// 上液体のテクスチャ移動量
	};

	HRESULT Init() override;	// 初期化
	void Uninit() override;		// 終了
	void Update(const float fDeltaTime) override;		// 更新
	void Draw(CShader* pShader = nullptr) override;		// 描画
	void SetVec3Position(const VECTOR3& rPos) override;	// 位置設定
	void SetVec3Rotation(const VECTOR3& rRot) override;	// 向き設定
	void SetVec2Size(const VECTOR2& rSize) override;	// 大きさ設定
	VECTOR3 GetVec3Position() const override	{ return m_apLiquid[LIQUID_LOW]->GetVec3Position(); }	// 位置取得
	VECTOR3 GetVec3Rotation() const override	{ return m_apLiquid[LIQUID_LOW]->GetVec3Rotation(); }	// 向き取得
	VECTOR2 GetVec2Size() const override		{ return m_apLiquid[LIQUID_LOW]->GetVec2Size(); }		// 大きさ取得
	MATRIX* GetPtrMtxWorld() override			{ return m_apLiquid[LIQUID_LOW]->GetPtrMtxWorld(); }	// マトリックスポインタ取得
	MATRIX GetMtxWorld() const override			{ return m_apLiquid[LIQUID_LOW]->GetMtxWorld(); }		// マトリックス取得

	// 静的メンバ関数
	static CLiquid* Create	// 生成
	( // 引数
		const EType type,				// 種類
		const VECTOR3& rPos,			// 位置
		const VECTOR3& rRot,			// 向き
		const VECTOR2& rSize,			// 大きさ
		const COLOR& rCol,				// 色
		const POSGRID2& rPart,			// 分割数
		const STexMove& rTexMove,		// テクスチャ移動量
		const float fMaxUp = 0.0f,		// 波の最高上昇量
		const float fAddSinRot = 0.0f,	// 波打ち向き加算量
		const float fAddVtxRot = 0.0f	// 隣波の向き加算量
	);
	static CListManager<CLiquid>* GetList();	// リスト取得

	// メンバ関数
	void SetColor(const COLOR& rCol);			// 色設定
	HRESULT SetPattern(const POSGRID2& rPart);	// 分割数設定
	void SetType(const EType type);				// 種類設定
	void SetTexMove(const STexMove texMove);	// テクスチャ移動量設定
	void SetMaxUp(const float fMaxUp);			// 波の最高上昇量設定
	void SetAddSinRot(const float fAddSinRot);	// 波打ち向き加算量設定
	void SetAddVtxRot(const float fAddVtxRot);	// 隣波の向き加算量設定
	inline COLOR GetColor() const		{ return m_apLiquid[LIQUID_LOW]->GetColor(); }		// 色取得
	inline POSGRID2 GetPattern() const	{ return m_apLiquid[LIQUID_LOW]->GetPattern(); }	// 分割数取得
	inline EType GetType() const		{ return m_type; }			// 種類取得
	inline float GetMaxUp() const		{ return m_fMaxUp; }		// 波の最高上昇量取得
	inline float GetAddSinRot() const	{ return m_fAddSinRot; }	// 波打ち向き加算量取得
	inline float GetAddVtxRot() const	{ return m_fAddVtxRot; }	// 隣波の向き加算量取得

	STexMove GetTexMove() const;	// テクスチャ移動量取得
	CScrollMeshField* GetMeshField(const int nID) const;	// メッシュフィールド取得

private:
	// オーバーライド関数
	void Release() override { CObject::Release(); }	// 破棄

	// 静的メンバ変数
	static CListManager<CLiquid>* m_pList;	// オブジェクトリスト

	// メンバ変数
	CListManager<CLiquid>::AIterator m_iterator;	// イテレーター
	CScrollMeshField* m_apLiquid[LIQUID_MAX];		// 液体の情報
	EType m_type;		// 種類
	float m_fMaxUp;		// 波の最高上昇量
	float m_fSinRot;	// 波打ち向き
	float m_fAddSinRot;	// 波打ち向き加算量
	float m_fAddVtxRot;	// 隣波の向き加算量
};

#endif	// _LIQUID_H_
