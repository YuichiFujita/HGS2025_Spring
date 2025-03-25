//============================================================
//
//	オブジェクトキャラクターアニメーション3Dヘッダー [objectCharaAnim3D.h]
//	Author：藤田勇一
//
//============================================================
//************************************************************
//	二重インクルード防止
//************************************************************
#ifndef _OBJECTCHARA_ANIM3D_H_
#define _OBJECTCHARA_ANIM3D_H_

//************************************************************
//	インクルードファイル
//************************************************************
#include "anim3D.h"

//************************************************************
//	クラス定義
//************************************************************
// オブジェクトキャラクターアニメーション3Dクラス
class CObjectCharaAnim3D : public CAnim3D
{
public:
	// コンストラクタ
	explicit CObjectCharaAnim3D(const CObject::ELabel label = LABEL_NONE, const CObject::EDim dimension = DIM_3D, const int nPriority = DEFAULT_PRIO);

	// デストラクタ
	~CObjectCharaAnim3D() override;

	// 当たり判定管理構造体
	struct SColl
	{
		// デフォルトコンストラクタ
		SColl() :
			offset	(VEC3_ZERO),	// 判定原点オフセット
			size	(VEC3_ZERO)		// 判定大きさ
		{}

		// デストラクタ
		~SColl() {}

		// メンバ変数
		VECTOR3 offset;	// 判定原点オフセット
		VECTOR3 size;	// 判定大きさ
	};

	// キャラクター管理構造体
	struct SChara
	{
		// デフォルトコンストラクタ
		SChara() :
			vecNextTime	 ({}),			// パターン変更時間配列
			sPathTexture (""),			// テクスチャパスをクリア
			ptrnTexture	 (GRID2_ZERO),	// テクスチャ分割数
			nMaxPtrn	 (0),			// 最大パターン数
			offset		 (VEC3_ZERO),	// 原点オフセット
			size		 (VEC3_ZERO),	// キャラクター大きさ
			bLoop		 (false)		// ループON/OFF
		{}

		// デストラクタ
		~SChara() {}

		// テクスチャ分割数・パターン総数の設定
		inline HRESULT SetTexPtrn(const POSGRID2& rPtrn)
		{
			// テクスチャ分割数・パターン総数を設定
			ptrnTexture = rPtrn;
			nMaxPtrn = rPtrn.x * rPtrn.y;

			// パターン変更時間の破棄
			vecNextTime.clear();

			// パターン変更時間の生成
			vecNextTime.resize(nMaxPtrn);

			// パターン変更時間を初期化
			if (FAILED(SetNextTime(DEF_NEXT))) { return E_FAIL; }

			return S_OK;
		}

		// パターン変更時間の設定
		inline HRESULT SetNextTime(const float fNextTime)
		{
			if (fNextTime <= 0.0f)					 { return E_FAIL; }	// 変更時間がプラスではない場合失敗
			if (nMaxPtrn != (int)vecNextTime.size()) { return E_FAIL; }	// パターン最大数と同じサイズではない場合失敗

			for (int i = 0; i < nMaxPtrn; i++)
			{
				// 引数のパターン変更時間を設定
				vecNextTime[i] = fNextTime;
			}

			return S_OK;
		}

		// メンバ変数
		std::vector<float> vecNextTime;	// パターン変更時間配列
		std::string sPathTexture;		// テクスチャパス
		POSGRID2 ptrnTexture;			// テクスチャ分割数
		int nMaxPtrn;	// 最大パターン数
		VECTOR3 offset;	// 原点オフセット
		VECTOR3 size;	// キャラクター大きさ
		bool bLoop;		// ループON/OFF
	};

	// モーション管理構造体
	struct SMotion
	{
		// デフォルトコンストラクタ
		SMotion() :
			infoChara	({}),		// キャラクター情報
			infoColl	({}),		// 当たり判定情報
			fCancelTime	(-1.0f),	// キャンセル可能時間
			fComboTime	(-1.0f)		// コンボ可能時間
		{}

		// デストラクタ
		~SMotion() {}

		// メンバ変数
		SChara infoChara;	// キャラクター情報
		SColl infoColl;		// 当たり判定情報
		float fCancelTime;	// キャンセル可能時間
		float fComboTime;	// コンボ可能時間
	};

	// モーション情報構造体
	struct SInfo
	{
		// デフォルトコンストラクタ
		SInfo() :
			vecMotion	({}),	// モーション情報
			nType		(0)		// モーション種類
		{}

		// デストラクタ
		~SInfo() {}

		// メンバ関数
		inline int GetNumMotion() { return (int)vecMotion.size(); }	// モーション情報の総数取得

		// メンバ変数
		std::vector<SMotion> vecMotion;	// モーション情報
		int nType;	// モーション種類
	};

	// オーバーライド関数
	HRESULT Init() override;	// 初期化
	void Uninit() override;		// 終了
	void Update(const float fDeltaTime) override;	// 更新
	void Draw(CShader* pShader = nullptr) override;	// 描画

	// 静的メンバ関数
	static CObjectCharaAnim3D* Create(const VECTOR3& rPos, const VECTOR3& rRot = VEC3_ZERO);	// 生成

	// メンバ関数
	void BindCharaData(const char* pCharaPath);	// キャラクター情報割当
	void SetMotion(const int nType);			// モーション設定
	void AddInfo(const SMotion& rMotion);		// モーション情報追加
	void SetAllInfo(const SInfo& rInfo);		// モーション情報全設定

	bool IsCancel() const;	// キャンセル取得
	bool IsCombo() const;	// コンボ取得
	inline int GetMotion() const	{ return m_info.nType; }			// モーション取得
	inline int GetNumType()			{ return m_info.GetNumMotion(); }	// 種類総数取得
	inline float GetCancelTime(const int nType) const	{ return m_info.vecMotion[nType].fCancelTime; }				// キャンセル時間取得
	inline float GetComboTime(const int nType) const	{ return m_info.vecMotion[nType].fComboTime; }				// コンボ時間取得
	inline VECTOR3 GetCollSize() const					{ return m_info.vecMotion[m_info.nType].infoColl.size; }	// 判定大きさ取得

	VECTOR3 CalcOriginOffsetPosition(const VECTOR3& rPos, const VECTOR3& rRot) const;	// 原点オフセット反映位置計算
	VECTOR3 CalcCollOffsetPosition(const VECTOR3& rPos, const VECTOR3& rRot) const;		// 判定原点オフセット反映位置計算
	VECTOR3 GetOriginOffsetPosition() const;	// 原点オフセット反映位置取得
	VECTOR3 GetCollOffsetPosition() const;		// 判定原点オフセット反映位置取得

private:
	// オーバーライド関数
	void CalcDrawMatrix() override;	// 描画マトリックス計算

	// メンバ関数
	VECTOR3 CalcOffsetPosition	// オフセット反映位置計算
	( // 引数
		const VECTOR3& rPos,	// 位置
		const VECTOR3& rRot,	// 向き
		const VECTOR3& rOffset	// オフセット
	) const;

	// メンバ変数
	SInfo m_info;	// モーション情報
};

#endif	// _OBJECTCHARA_ANIM3D_H_
