//============================================================
//
//	ステージヘッダー [stage.h]
//	Author：藤田勇一
//
//============================================================
//************************************************************
//	二重インクルード防止
//************************************************************
#ifndef _STAGE_H_
#define _STAGE_H_

//************************************************************
//	インクルードファイル
//************************************************************
#include "scene.h"

//************************************************************
//	前方宣言
//************************************************************
class CField;	// 地面クラス

//************************************************************
//	クラス定義
//************************************************************
// ステージクラス
class CStage
{
public:
	// 制限列挙
	enum ELimit
	{
		LIMIT_NONE = 0,	// 範囲制限なし
		LIMIT_BOX,		// 矩形範囲
		LIMIT_CIRCLE,	// 円範囲
		LIMIT_MAX		// この列挙型の総数
	};

	// コンストラクタ
	CStage();

	// デストラクタ
	~CStage();

	// 範囲構造体
	struct SLimit
	{
		ELimit mode;	// 制限モード
		VECTOR3 center;	// 中心座標
		float fNear;	// 制限位置 (前)
		float fFar;		// 制限位置 (後)
		float fRight;	// 制限位置 (右)
		float fLeft;	// 制限位置 (左)
		float fRadius;	// 制限位置 (半径)
		float fField;	// 制限位置 (地面)
		float fKillY;	// 制限位置 (削除)
	};

	// メンバ関数
	HRESULT Init();	// 初期化
	void Uninit();	// 終了
	HRESULT BindStage(const char* pPath);		// ステージ割当
	void DeleteStage();							// ステージ削除
	inline void SetLimit(const SLimit& rLimit)	{ m_limit = rLimit; }	// 範囲設定
	inline SLimit GetLimit() const				{ return m_limit; }		// 範囲取得

	bool LandLimitPosition(VECTOR3& rPos, VECTOR3& rMove, const float fHeight);	// 範囲外の着地判定
	bool LandFieldPosition(VECTOR3& rPos, VECTOR3& rOldPos, VECTOR3& rMove, CField** ppCollField = nullptr);	// 地面着地
	bool LandFieldPositionTop(VECTOR3& rPos, VECTOR3& rMove, CField** ppCollField = nullptr);					// 一番上の地面着地
	void LimitPosition(VECTOR3& rPos, const float fRadius);	// 位置補正
	bool CollisionKillY(const VECTOR3& rPos);				// キルY座標との当たり判定
	bool IsFieldPositionRange(const VECTOR3& rPos);			// メッシュの範囲内取得
	float GetFieldPositionHeight(const VECTOR3& rPos);		// メッシュの着地位置取得
	float GetFieldDownPositionHeight(const VECTOR3& rPos);	// メッシュの着地位置取得 (現在位置よりは下)

	// 静的メンバ関数
	static CStage* Create();				// 生成
	static void Release(CStage*& prStage);	// 破棄

private:
	// メンバ関数
	HRESULT LoadLimit(const char* pString, FILE* pFile);	// 範囲情報の読込
	HRESULT LoadField(const char* pString, FILE* pFile);	// 地面情報の読込
	HRESULT LoadWall(const char* pString, FILE* pFile);		// 壁情報の読込
	HRESULT LoadScenery(const char* pString, FILE* pFile);	// 景色情報の読込
	HRESULT LoadSky(const char* pString, FILE* pFile);		// 空情報の読込
	HRESULT LoadLiquid(const char* pString, FILE* pFile);	// 液体情報の読込

	// メンバ変数
	std::string m_sCurPath;	// 現在のステージパス
	SLimit m_limit;			// 範囲情報
};

#endif	// _STAGE_H_
