//============================================================
//
//	オブジェクトキャラクターヘッダー [objectChara.h]
//	Author：藤田勇一
//
//============================================================
//************************************************************
//	二重インクルード防止
//************************************************************
#ifndef _OBJECTCHARA_H_
#define _OBJECTCHARA_H_

//************************************************************
//	インクルードファイル
//************************************************************
#include "object.h"
#include "motion.h"
#include "character.h"

//************************************************************
//	前方宣言
//************************************************************
class CMultiModel;	// マルチモデルクラス

//************************************************************
//	クラス定義
//************************************************************
// オブジェクトキャラクタークラス
class CObjectChara : public CObject
{
public:
	// コンストラクタ
	explicit CObjectChara(const CObject::ELabel label = LABEL_NONE, const CObject::EDim dimension = DIM_3D, const int nPriority = DEFAULT_PRIO);

	// デストラクタ
	~CObjectChara() override;

	// オーバーライド関数
	HRESULT Init() override;	// 初期化
	void Uninit() override;		// 終了
	void Update(const float fDeltaTime) override;		// 更新
	void Draw(CShader* pShader = nullptr) override;		// 描画
	void SetEnableUpdate(const bool bUpdate) override;	// 更新状況設定
	void SetEnableDraw(const bool bDraw) override;		// 描画状況設定
	void SetVec3Position(const VECTOR3& rPos) override;	// 位置設定
	void SetVec3Rotation(const VECTOR3& rRot) override;	// 向き設定
	void SetVec3Scale(const VECTOR3& rScale) override;	// 拡大率設定
	inline VECTOR3 GetVec3Position() const override	{ return m_pos; }		// 位置取得
	inline VECTOR3 GetVec3Rotation() const override	{ return m_rot; }		// 向き取得
	inline MATRIX* GetPtrMtxWorld() override		{ return &m_mtxWorld; }	// マトリックスポインタ取得
	inline MATRIX GetMtxWorld() const override		{ return m_mtxWorld; }	// マトリックス取得

	// 静的メンバ関数
	static CObjectChara* Create(const VECTOR3& rPos, const VECTOR3& rRot = VEC3_ZERO);	// 生成

	// 仮想関数
	virtual void SetMotion(const int nType, const int nBlendFrame = 0);	// モーション設定

	// メンバ関数
	void SetPartsInfo	// パーツ情報設定
	( // 引数
		const int nPartsIdx,	// パーツインデックス
		const int nParentIdx,	// 親インデックス
		const VECTOR3& rPos,	// 位置
		const VECTOR3& rRot,	// 向き
		const char* pFileName	// ファイル名
	);

	void BindCharaData(const char* pCharaPath);	// キャラクター情報割当
	void SetNumParts(const int nNumParts);		// パーツ数設定
	void SetPartsInfoAll(CCharacter::SPartsInfo& rInfo);					// パーツ情報全設定
	void SetPartsPosition(const int nPartsIdx, const VECTOR3& rPos);		// パーツ位置設定
	void SetPartsRotation(const int nPartsIdx, const VECTOR3& rRot);		// パーツ向き設定
	VECTOR3 GetPartsPosition(const int nPartsIdx) const;					// パーツ位置取得
	VECTOR3 GetPartsRotation(const int nPartsIdx) const;					// パーツ向き取得
	CMultiModel* GetParts(const int nPartsIdx) const;						// パーツ取得
	inline CMotion* GetMotion() const	{ return m_pMotion; }				// モーション取得
	inline int GetNumParts() const		{ return (int)m_vecParts.size(); }	// パーツ数取得

	void SetMaterial(const D3DXMATERIAL& rMat, const int nPartsIdx, const int nMatIdx);	// マテリアル設定
	void SetAllMaterial(const D3DXMATERIAL& rMat);	// マテリアル全設定
	void ResetMaterial();				// マテリアル再設定
	void SetAlpha(const float fAlpha);	// 透明度設定
	float GetAlpha() const;				// 透明度取得
	float GetMaxAlpha() const;			// 最大透明度取得

	inline void AddMotionInfo(const CMotion::SMotion& rInfo)	{ m_pMotion->AddInfo(rInfo); }				// モーション情報追加
	inline void SetEnableMotionUpdate(const bool bUpdate)		{ m_pMotion->SetEnableUpdate(bUpdate); }	// 更新状況設定
	inline int GetMotionType() const			{ return m_pMotion->GetType(); }							// モーション種類取得
	inline int GetMotionNumType() const			{ return m_pMotion->GetNumType(); }							// モーション種類総数取得
	inline int GetMotionKey() const				{ return m_pMotion->GetKey();}								// モーションキー番号取得
	inline int GetMotionNumKey() const			{ return m_pMotion->GetNumKey(m_pMotion->GetType()); }		// モーションキー総数取得
	inline int GetMotionKeyCounter() const		{ return m_pMotion->GetKeyCounter(); }						// モーションキーカウンター取得
	inline int GetMotionWholeCounter() const	{ return m_pMotion->GetWholeCounter(); }					// モーション全体カウンター取得
	inline int GetMotionWholeFrame() const		{ return m_pMotion->GetWholeFrame(m_pMotion->GetType()); }	// モーション全体フレーム数取得
	inline int GetMotionCancelFrame() const		{ return m_pMotion->GetCancelFrame(m_pMotion->GetType()); }	// モーションキャンセルフレーム取得
	inline int GetMotionComboFrame() const		{ return m_pMotion->GetComboFrame(m_pMotion->GetType()); }	// モーションコンボフレーム取得
	inline bool IsMotionFinish() const			{ return m_pMotion->IsFinish(); }							// モーション終了取得
	inline bool IsMotionLoop() const			{ return m_pMotion->IsLoop(m_pMotion->GetType()); }			// モーションループ取得
	inline bool IsMotionCancel() const			{ return m_pMotion->IsCancel(m_pMotion->GetType()); }		// モーションキャンセル取得
	inline bool IsMotionCombo() const			{ return m_pMotion->IsCombo(m_pMotion->GetType()); }		// モーションコンボ取得
	inline bool IsWeaponDisp() const			{ return m_pMotion->IsWeaponDisp(m_pMotion->GetType()); }	// モーション武器表示取得
	inline bool IsLeftWeaponCollision()			{ return m_pMotion->IsLeftWeaponCollision(); }				// 左の攻撃判定フラグ取得
	inline bool IsRightWeaponCollision()		{ return m_pMotion->IsRightWeaponCollision(); }				// 右の攻撃判定フラグ取得

protected:
	// 仮想関数
	virtual void CalcDrawMatrix();	// 描画マトリックス計算

private:
	// オーバーライド関数
	inline void Release() override { CObject::Release(); }	// 破棄

	// メンバ変数
	std::vector<CMultiModel*> m_vecParts;	// パーツ情報
	CMotion* m_pMotion;		// モーションの情報
	MATRIX	 m_mtxWorld;	// ワールドマトリックス
	VECTOR3	 m_pos;			// 位置
	VECTOR3	 m_rot;			// 向き
};

#endif	// _OBJECTCHARA_H_
