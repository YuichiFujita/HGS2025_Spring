//============================================================
//
//	ステージ処理 [stage.cpp]
//	Author：藤田勇一
//
//============================================================
//************************************************************
//	インクルードファイル
//************************************************************
#include "stage.h"
#include "manager.h"
#include "texture.h"
#include "collision.h"
#include "retention.h"
#include "field.h"
#include "wall.h"
#include "scenery.h"
#include "sky.h"
#include "liquid.h"

//************************************************************
//	定数宣言
//************************************************************
//	親クラス [CStage] のメンバ関数
//************************************************************
//============================================================
//	コンストラクタ
//============================================================
CStage::CStage() :
	m_sCurPath	({})	// 現在のステージパス
{
	// メンバ変数をクリア
	memset(&m_limit, 0, sizeof(m_limit));	// 範囲情報
}

//============================================================
//	デストラクタ
//============================================================
CStage::~CStage()
{

}

//============================================================
//	初期化処理
//============================================================
HRESULT CStage::Init()
{
	// メンバ変数を初期化
	memset(&m_limit, 0, sizeof(m_limit));	// 範囲情報
	m_sCurPath = {};	// 現在のステージパス

	// 成功を返す
	return S_OK;
}

//============================================================
//	終了処理
//============================================================
void CStage::Uninit()
{

}

//============================================================
//	ステージ割当処理
//============================================================
HRESULT CStage::BindStage(const char* pPath)
{
	char aString[128];	// テキストの文字列の代入用
	int nEnd = 0;		// テキスト読み込み終了の確認用

	// ステージの削除
	DeleteStage();

	// 読み込むステージパスを保存
	m_sCurPath = pPath;

	// ファイルを読み込み形式で開く
	FILE* pFile = fopen(pPath, "r");

	if (pFile != nullptr)
	{ // ファイルが開けた場合

		while (1)
		{ // EOFまで無限ループ

			// ファイルから文字列を読み込む
			nEnd = fscanf(pFile, "%s", &aString[0]);	// 終端の場合 EOFを返す
			if (nEnd == EOF)
			{ // ファイルが終端の場合

				// 処理を抜ける
				break;
			}

			// 範囲情報の読込
			if (FAILED(LoadLimit(&aString[0], pFile)))
			{ // 読み込みに失敗した場合

				// 失敗を返す
				assert(false);
				return E_FAIL;
			}

			// 地面の読込
			else if (FAILED(LoadField(&aString[0], pFile)))
			{ // 読み込みに失敗した場合

				// 失敗を返す
				assert(false);
				return E_FAIL;
			}

			// 壁の読込
			else if (FAILED(LoadWall(&aString[0], pFile)))
			{ // 読み込みに失敗した場合

				// 失敗を返す
				assert(false);
				return E_FAIL;
			}

			// 景色の読込
			else if (FAILED(LoadScenery(&aString[0], pFile)))
			{ // 読み込みに失敗した場合

				// 失敗を返す
				assert(false);
				return E_FAIL;
			}

			// 空の読込
			else if (FAILED(LoadSky(&aString[0], pFile)))
			{ // 読み込みに失敗した場合

				// 失敗を返す
				assert(false);
				return E_FAIL;
			}

			// 液体の読込
			else if (FAILED(LoadLiquid(&aString[0], pFile)))
			{ // 読み込みに失敗した場合

				// 失敗を返す
				assert(false);
				return E_FAIL;
			}
		}
		
		// ファイルを閉じる
		fclose(pFile);

		// 成功を返す
		return S_OK;
	}
	else
	{ // ファイルが開けなかった場合

		// エラーメッセージボックス
		MessageBox(nullptr, "ステージセットアップの読み込みに失敗！", "警告！", MB_ICONWARNING);

		// 失敗を返す
		return E_FAIL;
	}
}

//============================================================
//	ステージ削除処理
//============================================================
void CStage::DeleteStage()
{
	// 現在のステージパスをクリア
	m_sCurPath.clear();

	// 範囲情報のクリア
	memset(&m_limit, 0, sizeof(m_limit));

	// 地面の全破棄
	CListManager<CField>* pListField = CField::GetList();
	if (pListField != nullptr)
	{ // 一つでも地面がある場合

		std::list<CField*> list = pListField->GetList();
		for (auto& rField : list)
		{ // 全要素数分繰り返す

			// 地面の終了
			SAFE_UNINIT(rField);
		}
	}

	// 壁の全破棄
	CListManager<CWall>* pListWall = CWall::GetList();
	if (pListWall != nullptr)
	{ // 一つでも壁がある場合

		std::list<CWall*> list = pListWall->GetList();
		for (auto& rWall : list)
		{ // 全要素数分繰り返す

			// 壁の終了
			SAFE_UNINIT(rWall);
		}
	}

	// 景色の全破棄
	CListManager<CScenery>* pListScenery = CScenery::GetList();
	if (pListScenery != nullptr)
	{ // 一つでも景色がある場合

		std::list<CScenery*> list = pListScenery->GetList();
		for (auto& rScenery : list)
		{ // 全要素数分繰り返す

			// 景色の終了
			SAFE_UNINIT(rScenery);
		}
	}

	// 空の全破棄
	CListManager<CSky>* pListSky = CSky::GetList();
	if (pListSky != nullptr)
	{ // 一つでも空がある場合

		std::list<CSky*> list = pListSky->GetList();
		for (auto& rSky : list)
		{ // 全要素数分繰り返す

			// 空の終了
			SAFE_UNINIT(rSky);
		}
	}

	// 液体の全破棄
	CListManager<CLiquid>* pListLiquid = CLiquid::GetList();
	if (pListLiquid != nullptr)
	{ // 一つでも液体がある場合

		std::list<CLiquid*> list = pListLiquid->GetList();
		for (auto& rLiquid : list)
		{ // 全要素数分繰り返す

			// 液体の終了
			SAFE_UNINIT(rLiquid);
		}
	}
}

//============================================================
//	範囲外の着地処理
//============================================================
bool CStage::LandLimitPosition(VECTOR3& rPos, VECTOR3& rMove, const float fHeight)
{
	if (rPos.y - fHeight < m_limit.fField)
	{ // 位置が地面より下の場合

		// 位置を補正
		rPos.y = m_limit.fField + fHeight;

		// 移動量を初期化
		rMove.y = 0.0f;

		// 着地している状況を返す
		return true;
	}

	// 着地していない状況を返す
	return false;
}

//============================================================
//	地面の着地処理
//============================================================
bool CStage::LandFieldPosition(VECTOR3& rPos, VECTOR3& rOldPos, VECTOR3& rMove, CField** ppCollField)
{
	CListManager<CField> *pListManager = CField::GetList();	// フィールドリストマネージャー
	if (pListManager == nullptr) { return false; }			// リスト未使用の場合抜ける
	std::list<CField*> listField = pListManager->GetList();	// フィールドリスト情報
	CField *pCurField = nullptr;	// 着地予定の地面
	float fCurPos = m_limit.fField;	// 着地予定のY座標
	bool bLand = false;				// 地面の着地判定

	for (auto& rList : listField)
	{ // 地面の総数分繰り返す

		assert(rList != nullptr);
		if (rList->IsPositionRange(rPos))
		{ // 地面の範囲内の場合

			float fPosHeight = rList->GetPositionHeight(rPos);	// 着地Y座標
			if (fPosHeight <= rOldPos.y && fPosHeight > rPos.y)
			{
				if (fCurPos <= fPosHeight)
				{ // 現在の着地予定Y座標より高い位置にある場合

					// 着地予定の地面を更新
					pCurField = rList;

					// 着地予定のY座標を更新
					fCurPos = fPosHeight;
				}
			}
		}
	}

	if (pCurField == nullptr)
	{ // 着地予定の地面が存在しない場合

		if (ppCollField != nullptr)
		{ // 地面の保存アドレスがある場合

			// 地面無しを保存
			*ppCollField = nullptr;
		}
	}
	else
	{ // 着地予定の地面が存在する場合

		// メッシュの着地状況を保存
		bLand = pCurField->LandPosition(rPos, rMove);
		if (ppCollField != nullptr)
		{ // 地面の保存アドレスがある場合

			if (bLand)
			{ // 着地している場合

				// 着地した地面を保存
				*ppCollField = pCurField;
			}
		}
	}

	// 着地判定を返す
	return bLand;
}

//============================================================
//	一番上の地面着地処理
//============================================================
bool CStage::LandFieldPositionTop(VECTOR3& rPos, VECTOR3& rMove, CField** ppCollField)
{
	CListManager<CField> *pListManager = CField::GetList();	// フィールドリストマネージャー
	if (pListManager == nullptr) { return false; }			// リスト未使用の場合抜ける
	std::list<CField*> listField = pListManager->GetList();	// フィールドリスト情報
	CField *pCurField = nullptr;	// 着地予定の地面
	float fCurPos = m_limit.fField;	// 着地予定のY座標
	bool bLand = false;				// 地面の着地判定

	for (auto& rList : listField)
	{ // 地面の総数分繰り返す

		assert(rList != nullptr);
		if (rList->IsPositionRange(rPos))
		{ // 地面の範囲内の場合

			float fPosHeight = rList->GetPositionHeight(rPos);	// 着地Y座標
			if (fCurPos <= fPosHeight)
			{ // 現在の着地予定Y座標より高い位置にある場合

				// 着地予定の地面を更新
				pCurField = rList;

				// 着地予定のY座標を更新
				fCurPos = fPosHeight;
			}
		}
	}

	if (pCurField == nullptr)
	{ // 着地予定の地面が存在しない場合抜ける

		if (ppCollField != nullptr)
		{ // 地面の保存アドレスがある場合

			// 地面無しを保存
			*ppCollField = pCurField;
		}
	}
	else
	{ // 着地予定の地面が存在しない場合抜ける

		// メッシュの着地状況を保存
		bLand = pCurField->LandPosition(rPos, rMove);
		if (ppCollField != nullptr)
		{ // 地面の保存アドレスがある場合

			if (bLand)
			{ // 着地している場合

				// 着地した地面を保存
				*ppCollField = pCurField;
			}
		}
	}

	// 着地判定を返す
	return bLand;
}

//============================================================
//	位置の補正処理
//============================================================
void CStage::LimitPosition(VECTOR3& rPos, const float fRadius)
{
	switch (m_limit.mode)
	{ // 制限モードごとの処理
	case LIMIT_NONE:	// 制限なし

		// 制限がない場合関数を呼び出す必要がない
		assert(false);

		break;

	case LIMIT_BOX:		// 矩形範囲

		// 角柱の内側制限
		collision::InBoxPillarXZ
		( // 引数
			rPos,				// 判定位置
			VEC3_ZERO,			// 判定原点位置
			VECTOR3(fRadius),	// 判定サイズ(右・上・後)
			VECTOR3(fRadius),	// 判定サイズ(左・下・前)
			VECTOR3(fabsf(m_limit.fRight), 0.0f, fabsf(m_limit.fFar)),	// 判定原点サイズ(右・上・後)
			VECTOR3(fabsf(m_limit.fLeft), 0.0f, fabsf(m_limit.fNear))	// 判定原点サイズ(左・下・前)
		);

		break;

	case LIMIT_CIRCLE:	// 円範囲

		// 円柱の内側制限
		collision::InCirclePillar
		( // 引数
			rPos,			// 判定位置
			VEC3_ZERO,		// 判定原点位置
			fRadius,		// 判定半径
			m_limit.fRadius	// 判定原点半径
		);

		break;

	default:	// 例外処理
		assert(false);
		break;
	}
}

//============================================================
//	キルY座標との当たり判定
//============================================================
bool CStage::CollisionKillY(const VECTOR3& rPos)
{
	if (rPos.y < m_limit.fKillY)
	{ // 位置が削除の制限位置より下の場合

		// 死亡している状況を返す
		return true;
	}

	// 死亡していない状況を返す
	return false;
}

//============================================================
//	地面の範囲内の取得処理
//============================================================
bool CStage::IsFieldPositionRange(const VECTOR3& rPos)
{
	CListManager<CField> *pListManager = CField::GetList();	// フィールドリストマネージャー
	if (pListManager == nullptr) { return false; }			// リスト未使用の場合抜ける
	std::list<CField*> listField = pListManager->GetList();	// フィールドリスト情報

	for (auto& rList : listField)
	{ // 地面の総数分繰り返す

		assert(rList != nullptr);
		if (rList->IsPositionRange(rPos))
		{ // 地面の範囲内の場合

			// 範囲内にいる状態を返す
			return true;
		}
	}

	// 範囲内にいない状態を返す
	return false;
}

//============================================================
//	地面の着地位置の取得処理
//============================================================
float CStage::GetFieldPositionHeight(const VECTOR3& rPos)
{
	CListManager<CField> *pListManager = CField::GetList();	// フィールドリストマネージャー
	if (pListManager == nullptr) { return false; }			// リスト未使用の場合抜ける
	std::list<CField*> listField = pListManager->GetList();	// フィールドリスト情報
	CField *pCurField = nullptr;	// 着地予定の地面
	float fCurPos = m_limit.fField;	// 着地予定のY座標

	for (auto& rList : listField)
	{ // 地面の総数分繰り返す

		assert(rList != nullptr);
		if (rList->IsPositionRange(rPos))
		{ // 地面の範囲内の場合

			float fPosHeight = rList->GetPositionHeight(rPos);	// 着地Y座標
			if (fCurPos <= fPosHeight)
			{ // 現在の着地予定Y座標より高い位置にある場合

				// 着地予定の地面を更新
				pCurField = rList;

				// 着地予定のY座標を更新
				fCurPos = fPosHeight;
			}
		}
	}

	if (pCurField != nullptr)
	{ // 着地予定の地面が存在する場合

		// 着地位置を返す
		return fCurPos;
	}
	else
	{ // 着地予定の地面が存在しない場合

		// 引数位置を返す
		return rPos.y;
	}
}

//============================================================
//	メッシュの着地位置の取得処理 (現在位置よりは下)
//============================================================
float CStage::GetFieldDownPositionHeight(const VECTOR3& rPos)
{
	CListManager<CField> *pListManager = CField::GetList();	// フィールドリストマネージャー
	if (pListManager == nullptr) { return false; }			// リスト未使用の場合抜ける
	std::list<CField*> listField = pListManager->GetList();	// フィールドリスト情報
	CField *pCurField = nullptr;	// 着地予定の地面
	float fCurPos = m_limit.fField;	// 着地予定のY座標

	for (auto& rList : listField)
	{ // 地面の総数分繰り返す

		assert(rList != nullptr);
		if (rList->IsPositionRange(rPos))
		{ // 地面の範囲内の場合

			float fPosHeight = rList->GetPositionHeight(rPos);	// 着地Y座標
			if (fCurPos <= fPosHeight && rPos.y >= fPosHeight)
			{ // 現在の着地予定Y座標より高い位置にある場合

				// 着地予定の地面を更新
				pCurField = rList;

				// 着地予定のY座標を更新
				fCurPos = fPosHeight;
			}
		}
	}

	if (pCurField != nullptr)
	{ // 着地予定の地面が存在する場合

		// 着地位置を返す
		return fCurPos;
	}
	else
	{ // 着地予定の地面が存在しない場合

		// 引数位置を返す
		return rPos.y;
	}
}

//============================================================
//	生成処理
//============================================================
CStage* CStage::Create()
{
	// ステージの生成
	CStage* pStage = new CStage;
	if (pStage == nullptr)
	{ // 生成に失敗した場合

		return nullptr;
	}
	else
	{ // 生成に成功した場合

		// ステージの初期化
		if (FAILED(pStage->Init()))
		{ // 初期化に失敗した場合

			// ステージの破棄
			SAFE_DELETE(pStage);
			return nullptr;
		}

		// 確保したアドレスを返す
		return pStage;
	}
}

//============================================================
//	破棄処理
//============================================================
void CStage::Release(CStage*& prStage)
{
	// ステージの終了
	assert(prStage != nullptr);
	prStage->Uninit();

	// メモリ開放
	SAFE_DELETE(prStage);
}

//============================================================
//	範囲情報の読込処理
//============================================================
HRESULT CStage::LoadLimit(const char* pString, FILE *pFile)
{
	char aString[128];	// テキストの文字列の代入用
	SLimit limit;		// ステージ範囲の代入用
	if (pString == nullptr || pFile == nullptr)
	{ // 文字列・ファイルが存在しない場合

		// 失敗を返す
		assert(false);
		return E_FAIL;
	}

	// ステージ範囲の設定
	if (strcmp(pString, "LIMITSET") == 0)
	{ // 読み込んだ文字列が LIMITSET の場合

		// 制限モードを無しに設定
		limit.mode = LIMIT_NONE;

		do
		{ // 読み込んだ文字列が END_LIMITSET ではない場合ループ

			// ファイルから文字列を読み込む
			fscanf(pFile, "%s", &aString[0]);

			if (strcmp(&aString[0], "CENTER") == 0)
			{ // 読み込んだ文字列が CENTER の場合

				fscanf(pFile, "%s", &aString[0]);		// = を読み込む (不要)
				fscanf(pFile, "%f", &limit.center.x);	// 中心座標Xを読み込む
				fscanf(pFile, "%f", &limit.center.y);	// 中心座標Yを読み込む
				fscanf(pFile, "%f", &limit.center.z);	// 中心座標Zを読み込む
			}
			else if (strcmp(&aString[0], "NEAR") == 0)
			{ // 読み込んだ文字列が NEAR の場合

				fscanf(pFile, "%s", &aString[0]);		// = を読み込む (不要)
				fscanf(pFile, "%f", &limit.fNear);		// 前位置を読み込む

				// 制限モードを矩形範囲に設定
				limit.mode = LIMIT_BOX;
			}
			else if (strcmp(&aString[0], "FAR") == 0)
			{ // 読み込んだ文字列が FAR の場合

				fscanf(pFile, "%s", &aString[0]);		// = を読み込む (不要)
				fscanf(pFile, "%f", &limit.fFar);		// 後位置を読み込む

				// 制限モードを矩形範囲に設定
				limit.mode = LIMIT_BOX;
			}
			else if (strcmp(&aString[0], "RIGHT") == 0)
			{ // 読み込んだ文字列が RIGHT の場合

				fscanf(pFile, "%s", &aString[0]);		// = を読み込む (不要)
				fscanf(pFile, "%f", &limit.fRight);		// 右位置を読み込む

				// 制限モードを矩形範囲に設定
				limit.mode = LIMIT_BOX;
			}
			else if (strcmp(&aString[0], "LEFT") == 0)
			{ // 読み込んだ文字列が LEFT の場合

				fscanf(pFile, "%s", &aString[0]);		// = を読み込む (不要)
				fscanf(pFile, "%f", &limit.fLeft);		// 左位置を読み込む

				// 制限モードを矩形範囲に設定
				limit.mode = LIMIT_BOX;
			}
			else if (strcmp(&aString[0], "RADIUS") == 0)
			{ // 読み込んだ文字列が RADIUS の場合

				fscanf(pFile, "%s", &aString[0]);		// = を読み込む (不要)
				fscanf(pFile, "%f", &limit.fRadius);	// 半径を読み込む

				// 制限モードを円範囲に設定
				limit.mode = LIMIT_CIRCLE;
			}
			else if (strcmp(&aString[0], "FIELD") == 0)
			{ // 読み込んだ文字列が FIELD の場合

				fscanf(pFile, "%s", &aString[0]);		// = を読み込む (不要)
				fscanf(pFile, "%f", &limit.fField);		// 地面位置を読み込む
			}
			else if (strcmp(&aString[0], "KILLY") == 0)
			{ // 読み込んだ文字列が KILLY の場合

				fscanf(pFile, "%s", &aString[0]);		// = を読み込む (不要)
				fscanf(pFile, "%f", &limit.fKillY);		// 削除位置を読み込む
			}
		} while (strcmp(&aString[0], "END_LIMITSET") != 0);	// 読み込んだ文字列が END_LIMITSET ではない場合ループ

		// ステージ範囲の設定
		SetLimit(limit);
	}

	// 成功を返す
	return S_OK;
}

//============================================================
//	地面情報の読込処理
//============================================================
HRESULT CStage::LoadField(const char* pString, FILE *pFile)
{
	char aString[128];	// テキストの文字列の代入用
	int nType = 0;		// 種類の代入用
	VECTOR3 pos = VEC3_ZERO;	// 位置の代入用
	VECTOR3 rot = VEC3_ZERO;	// 向きの代入用
	VECTOR2 size = VEC2_ZERO;	// 大きさの代入用
	COLOR col = color::White();	// 色の代入用
	POSGRID2 part = GRID2_ZERO;		// 分割数の代入用
	POSGRID2 texPart = GRID2_ZERO;	// テクスチャ分割数の代入用

	if (pString == nullptr || pFile == nullptr)
	{ // 文字列・ファイルが存在しない場合

		// 失敗を返す
		assert(false);
		return E_FAIL;
	}

	// 地面の設定
	if (strcmp(pString, "STAGE_FIELDSET") == 0)
	{ // 読み込んだ文字列が STAGE_FIELDSET の場合

		do
		{ // 読み込んだ文字列が END_STAGE_FIELDSET ではない場合ループ

			// ファイルから文字列を読み込む
			fscanf(pFile, "%s", &aString[0]);

			if (strcmp(&aString[0], "FIELDSET") == 0)
			{ // 読み込んだ文字列が FIELDSET の場合
	
				do
				{ // 読み込んだ文字列が END_FIELDSET ではない場合ループ
	
					// ファイルから文字列を読み込む
					fscanf(pFile, "%s", &aString[0]);
	
					if (strcmp(&aString[0], "TYPE") == 0)
					{ // 読み込んだ文字列が TYPE の場合
	
						fscanf(pFile, "%s", &aString[0]);	// = を読み込む (不要)
						fscanf(pFile, "%d", &nType);		// 種類を読み込む
					}
					else if (strcmp(&aString[0], "POS") == 0)
					{ // 読み込んだ文字列が POS の場合

						fscanf(pFile, "%s", &aString[0]);	// = を読み込む (不要)
						fscanf(pFile, "%f", &pos.x);		// 位置Xを読み込む
						fscanf(pFile, "%f", &pos.y);		// 位置Yを読み込む
						fscanf(pFile, "%f", &pos.z);		// 位置Zを読み込む
					}
					else if (strcmp(&aString[0], "ROT") == 0)
					{ // 読み込んだ文字列が ROT の場合

						fscanf(pFile, "%s", &aString[0]);	// = を読み込む (不要)
						fscanf(pFile, "%f", &rot.x);		// 向きXを読み込む
						fscanf(pFile, "%f", &rot.y);		// 向きYを読み込む
						fscanf(pFile, "%f", &rot.z);		// 向きZを読み込む
					}
					else if (strcmp(&aString[0], "SIZE") == 0)
					{ // 読み込んだ文字列が SIZE の場合

						fscanf(pFile, "%s", &aString[0]);	// = を読み込む (不要)
						fscanf(pFile, "%f", &size.x);		// 大きさXを読み込む
						fscanf(pFile, "%f", &size.y);		// 大きさYを読み込む
					}
					else if (strcmp(&aString[0], "COL") == 0)
					{ // 読み込んだ文字列が COL の場合

						fscanf(pFile, "%s", &aString[0]);	// = を読み込む (不要)
						fscanf(pFile, "%f", &col.r);		// 色Rを読み込む
						fscanf(pFile, "%f", &col.g);		// 色Gを読み込む
						fscanf(pFile, "%f", &col.b);		// 色Bを読み込む
						fscanf(pFile, "%f", &col.a);		// 色Aを読み込む
					}
					else if (strcmp(&aString[0], "PART") == 0)
					{ // 読み込んだ文字列が PART の場合

						fscanf(pFile, "%s", &aString[0]);	// = を読み込む (不要)
						fscanf(pFile, "%d", &part.x);		// 横分割数を読み込む
						fscanf(pFile, "%d", &part.y);		// 縦分割数を読み込む
					}
					else if (strcmp(&aString[0], "TEXPART") == 0)
					{ // 読み込んだ文字列が TEXPART の場合

						fscanf(pFile, "%s", &aString[0]);	// = を読み込む (不要)
						fscanf(pFile, "%d", &texPart.x);	// テクスチャ横分割数を読み込む
						fscanf(pFile, "%d", &texPart.y);	// テクスチャ縦分割数を読み込む
					}
				} while (strcmp(&aString[0], "END_FIELDSET") != 0);	// 読み込んだ文字列が END_FIELDSET ではない場合ループ

				// 地面オブジェクトの生成
				if (CField::Create((CField::EType)nType, pos, D3DXToRadian(rot), size, col, part, texPart) == nullptr)
				{ // 確保に失敗した場合
	
					// 失敗を返す
					assert(false);
					return E_FAIL;
				}
			}
		} while (strcmp(&aString[0], "END_STAGE_FIELDSET") != 0);	// 読み込んだ文字列が END_STAGE_FIELDSET ではない場合ループ
	}

	// 成功を返す
	return S_OK;
}

//============================================================
//	壁情報の読込処理
//============================================================
HRESULT CStage::LoadWall(const char* pString, FILE *pFile)
{
	char aString[128];	// テキストの文字列の代入用
	int nType = 0;		// 種類の代入用
	VECTOR3 pos = VEC3_ZERO;	// 位置の代入用
	VECTOR3 rot = VEC3_ZERO;	// 向きの代入用
	VECTOR2 size = VEC2_ZERO;	// 大きさの代入用
	COLOR col = color::White();	// 色の代入用
	POSGRID2 part = GRID2_ZERO;		// 分割数の代入用
	POSGRID2 texPart = GRID2_ZERO;	// テクスチャ分割数の代入用

	if (pString == nullptr || pFile == nullptr)
	{ // 文字列・ファイルが存在しない場合

		// 失敗を返す
		assert(false);
		return E_FAIL;
	}

	// 壁の設定
	if (strcmp(pString, "STAGE_WALLSET") == 0)
	{ // 読み込んだ文字列が STAGE_WALLSET の場合

		do
		{ // 読み込んだ文字列が END_STAGE_SCENERYSET ではない場合ループ

			// ファイルから文字列を読み込む
			fscanf(pFile, "%s", &aString[0]);

			if (strcmp(&aString[0], "WALLSET") == 0)
			{ // 読み込んだ文字列が WALLSET の場合
	
				do
				{ // 読み込んだ文字列が END_WALLSET ではない場合ループ
	
					// ファイルから文字列を読み込む
					fscanf(pFile, "%s", &aString[0]);
	
					if (strcmp(&aString[0], "TYPE") == 0)
					{ // 読み込んだ文字列が TYPE の場合
	
						fscanf(pFile, "%s", &aString[0]);	// = を読み込む (不要)
						fscanf(pFile, "%d", &nType);		// 種類を読み込む
					}
					else if (strcmp(&aString[0], "POS") == 0)
					{ // 読み込んだ文字列が POS の場合

						fscanf(pFile, "%s", &aString[0]);	// = を読み込む (不要)
						fscanf(pFile, "%f", &pos.x);		// 位置Xを読み込む
						fscanf(pFile, "%f", &pos.y);		// 位置Yを読み込む
						fscanf(pFile, "%f", &pos.z);		// 位置Zを読み込む
					}
					else if (strcmp(&aString[0], "ROT") == 0)
					{ // 読み込んだ文字列が ROT の場合

						fscanf(pFile, "%s", &aString[0]);	// = を読み込む (不要)
						fscanf(pFile, "%f", &rot.x);		// 向きXを読み込む
						fscanf(pFile, "%f", &rot.y);		// 向きYを読み込む
						fscanf(pFile, "%f", &rot.z);		// 向きZを読み込む
					}
					else if (strcmp(&aString[0], "SIZE") == 0)
					{ // 読み込んだ文字列が SIZE の場合

						fscanf(pFile, "%s", &aString[0]);	// = を読み込む (不要)
						fscanf(pFile, "%f", &size.x);		// 大きさXを読み込む
						fscanf(pFile, "%f", &size.y);		// 大きさYを読み込む
					}
					else if (strcmp(&aString[0], "COL") == 0)
					{ // 読み込んだ文字列が COL の場合

						fscanf(pFile, "%s", &aString[0]);	// = を読み込む (不要)
						fscanf(pFile, "%f", &col.r);		// 色Rを読み込む
						fscanf(pFile, "%f", &col.g);		// 色Gを読み込む
						fscanf(pFile, "%f", &col.b);		// 色Bを読み込む
						fscanf(pFile, "%f", &col.a);		// 色Aを読み込む
					}
					else if (strcmp(&aString[0], "PART") == 0)
					{ // 読み込んだ文字列が PART の場合

						fscanf(pFile, "%s", &aString[0]);	// = を読み込む (不要)
						fscanf(pFile, "%d", &part.x);		// 横分割数を読み込む
						fscanf(pFile, "%d", &part.y);		// 縦分割数を読み込む
					}
					else if (strcmp(&aString[0], "TEXPART") == 0)
					{ // 読み込んだ文字列が TEXPART の場合

						fscanf(pFile, "%s", &aString[0]);	// = を読み込む (不要)
						fscanf(pFile, "%d", &texPart.x);	// テクスチャ横分割数を読み込む
						fscanf(pFile, "%d", &texPart.y);	// テクスチャ縦分割数を読み込む
					}
				} while (strcmp(&aString[0], "END_WALLSET") != 0);	// 読み込んだ文字列が END_WALLSET ではない場合ループ

				// 壁オブジェクトの生成
				if (CWall::Create((CWall::EType)nType, pos, D3DXToRadian(rot), size, col, part, texPart) == nullptr)
				{ // 確保に失敗した場合

					// 失敗を返す
					assert(false);
					return E_FAIL;
				}
			}
		} while (strcmp(&aString[0], "END_STAGE_WALLSET") != 0);	// 読み込んだ文字列が END_STAGE_WALLSET ではない場合ループ
	}

	// 成功を返す
	return S_OK;
}

//============================================================
//	景色情報の読込処理
//============================================================
HRESULT CStage::LoadScenery(const char* pString, FILE *pFile)
{
	VECTOR3 pos = VEC3_ZERO;	// 位置の代入用
	VECTOR3 rot = VEC3_ZERO;	// 向きの代入用
	COLOR col = color::White();	// 色の代入用
	POSGRID2 part = GRID2_ZERO;		// 分割数の代入用
	POSGRID2 texPart = GRID2_ZERO;	// テクスチャ分割数の代入用
	char aString[128];		// テキストの文字列の代入用
	int nType = 0;			// 種類の代入用
	float fRadius = 0.0f;	// 半径の代入用
	float fHeight = 0.0f;	// 縦幅の代入用

	if (pString == nullptr || pFile == nullptr)
	{ // 文字列・ファイルが存在しない場合

		// 失敗を返す
		assert(false);
		return E_FAIL;
	}

	// 景色の設定
	if (strcmp(pString, "STAGE_SCENERYSET") == 0)
	{ // 読み込んだ文字列が STAGE_SCENERYSET の場合

		do
		{ // 読み込んだ文字列が END_STAGE_SCENERYSET ではない場合ループ

			// ファイルから文字列を読み込む
			fscanf(pFile, "%s", &aString[0]);

			if (strcmp(&aString[0], "SCENERYSET") == 0)
			{ // 読み込んだ文字列が SCENERYSET の場合
	
				do
				{ // 読み込んだ文字列が END_SCENERYSET ではない場合ループ
	
					// ファイルから文字列を読み込む
					fscanf(pFile, "%s", &aString[0]);
	
					if (strcmp(&aString[0], "TYPE") == 0)
					{ // 読み込んだ文字列が TYPE の場合
	
						fscanf(pFile, "%s", &aString[0]);	// = を読み込む (不要)
						fscanf(pFile, "%d", &nType);		// 種類を読み込む
					}
					else if (strcmp(&aString[0], "POS") == 0)
					{ // 読み込んだ文字列が POS の場合

						fscanf(pFile, "%s", &aString[0]);	// = を読み込む (不要)
						fscanf(pFile, "%f", &pos.x);		// 位置Xを読み込む
						fscanf(pFile, "%f", &pos.y);		// 位置Yを読み込む
						fscanf(pFile, "%f", &pos.z);		// 位置Zを読み込む
					}
					else if (strcmp(&aString[0], "ROT") == 0)
					{ // 読み込んだ文字列が ROT の場合

						fscanf(pFile, "%s", &aString[0]);	// = を読み込む (不要)
						fscanf(pFile, "%f", &rot.x);		// 向きXを読み込む
						fscanf(pFile, "%f", &rot.y);		// 向きYを読み込む
						fscanf(pFile, "%f", &rot.z);		// 向きZを読み込む
					}
					else if (strcmp(&aString[0], "COL") == 0)
					{ // 読み込んだ文字列が COL の場合

						fscanf(pFile, "%s", &aString[0]);	// = を読み込む (不要)
						fscanf(pFile, "%f", &col.r);		// 色Rを読み込む
						fscanf(pFile, "%f", &col.g);		// 色Gを読み込む
						fscanf(pFile, "%f", &col.b);		// 色Bを読み込む
						fscanf(pFile, "%f", &col.a);		// 色Aを読み込む
					}
					else if (strcmp(&aString[0], "PART") == 0)
					{ // 読み込んだ文字列が PART の場合

						fscanf(pFile, "%s", &aString[0]);	// = を読み込む (不要)
						fscanf(pFile, "%d", &part.x);		// 横分割数を読み込む
						fscanf(pFile, "%d", &part.y);		// 縦分割数を読み込む
					}
					else if (strcmp(&aString[0], "TEXPART") == 0)
					{ // 読み込んだ文字列が TEXPART の場合

						fscanf(pFile, "%s", &aString[0]);	// = を読み込む (不要)
						fscanf(pFile, "%d", &texPart.x);	// テクスチャ横分割数を読み込む
						fscanf(pFile, "%d", &texPart.y);	// テクスチャ縦分割数を読み込む
					}
					else if (strcmp(&aString[0], "RADIUS") == 0)
					{ // 読み込んだ文字列が RADIUS の場合

						fscanf(pFile, "%s", &aString[0]);	// = を読み込む (不要)
						fscanf(pFile, "%f", &fRadius);		// 半径を読み込む
					}
					else if (strcmp(&aString[0], "HEIGHT") == 0)
					{ // 読み込んだ文字列が HEIGHT の場合

						fscanf(pFile, "%s", &aString[0]);	// = を読み込む (不要)
						fscanf(pFile, "%f", &fHeight);		// 縦幅を読み込む
					}
				} while (strcmp(&aString[0], "END_SCENERYSET") != 0);	// 読み込んだ文字列が END_SCENERYSET ではない場合ループ

				// 景色オブジェクトの生成
				if (CScenery::Create((CScenery::EType)nType, pos, D3DXToRadian(rot), col, part, texPart, fRadius, fHeight) == nullptr)
				{ // 確保に失敗した場合

					// 失敗を返す
					assert(false);
					return E_FAIL;
				}
			}
		} while (strcmp(&aString[0], "END_STAGE_SCENERYSET") != 0);	// 読み込んだ文字列が END_STAGE_SCENERYSET ではない場合ループ
	}

	// 成功を返す
	return S_OK;
}

//============================================================
//	空情報の読込処理
//============================================================
HRESULT CStage::LoadSky(const char* pString, FILE *pFile)
{
	VECTOR3 pos = VEC3_ZERO;	// 位置の代入用
	VECTOR3 rot = VEC3_ZERO;	// 向きの代入用
	COLOR col = color::White();	// 色の代入用
	POSGRID2 part = GRID2_ZERO;		// 分割数の代入用
	POSGRID2 texPart = GRID2_ZERO;	// テクスチャ分割数の代入用
	char aString[128];		// テキストの文字列の代入用
	int nType = 0;			// 種類の代入用
	float fRadius = 0.0f;	// 半径の代入用

	if (pString == nullptr || pFile == nullptr)
	{ // 文字列・ファイルが存在しない場合

		// 失敗を返す
		assert(false);
		return E_FAIL;
	}

	// 空の設定
	if (strcmp(pString, "STAGE_SKYSET") == 0)
	{ // 読み込んだ文字列が STAGE_SKYSET の場合

		do
		{ // 読み込んだ文字列が END_STAGE_SKYSET ではない場合ループ

			// ファイルから文字列を読み込む
			fscanf(pFile, "%s", &aString[0]);

			if (strcmp(&aString[0], "SKYSET") == 0)
			{ // 読み込んだ文字列が SKYSET の場合

				do
				{ // 読み込んだ文字列が END_SKYSET ではない場合ループ

					// ファイルから文字列を読み込む
					fscanf(pFile, "%s", &aString[0]);

					if (strcmp(&aString[0], "TYPE") == 0)
					{ // 読み込んだ文字列が TYPE の場合

						fscanf(pFile, "%s", &aString[0]);	// = を読み込む (不要)
						fscanf(pFile, "%d", &nType);		// 種類を読み込む
					}
					else if (strcmp(&aString[0], "POS") == 0)
					{ // 読み込んだ文字列が POS の場合

						fscanf(pFile, "%s", &aString[0]);	// = を読み込む (不要)
						fscanf(pFile, "%f", &pos.x);		// 位置Xを読み込む
						fscanf(pFile, "%f", &pos.y);		// 位置Yを読み込む
						fscanf(pFile, "%f", &pos.z);		// 位置Zを読み込む
					}
					else if (strcmp(&aString[0], "ROT") == 0)
					{ // 読み込んだ文字列が ROT の場合

						fscanf(pFile, "%s", &aString[0]);	// = を読み込む (不要)
						fscanf(pFile, "%f", &rot.x);		// 向きXを読み込む
						fscanf(pFile, "%f", &rot.y);		// 向きYを読み込む
						fscanf(pFile, "%f", &rot.z);		// 向きZを読み込む
					}
					else if (strcmp(&aString[0], "COL") == 0)
					{ // 読み込んだ文字列が COL の場合

						fscanf(pFile, "%s", &aString[0]);	// = を読み込む (不要)
						fscanf(pFile, "%f", &col.r);		// 色Rを読み込む
						fscanf(pFile, "%f", &col.g);		// 色Gを読み込む
						fscanf(pFile, "%f", &col.b);		// 色Bを読み込む
						fscanf(pFile, "%f", &col.a);		// 色Aを読み込む
					}
					else if (strcmp(&aString[0], "PART") == 0)
					{ // 読み込んだ文字列が PART の場合

						fscanf(pFile, "%s", &aString[0]);	// = を読み込む (不要)
						fscanf(pFile, "%d", &part.x);		// 横分割数を読み込む
						fscanf(pFile, "%d", &part.y);		// 縦分割数を読み込む
					}
					else if (strcmp(&aString[0], "TEXPART") == 0)
					{ // 読み込んだ文字列が TEXPART の場合

						fscanf(pFile, "%s", &aString[0]);	// = を読み込む (不要)
						fscanf(pFile, "%d", &texPart.x);	// テクスチャ横分割数を読み込む
						fscanf(pFile, "%d", &texPart.y);	// テクスチャ縦分割数を読み込む
					}
					else if (strcmp(&aString[0], "RADIUS") == 0)
					{ // 読み込んだ文字列が RADIUS の場合

						fscanf(pFile, "%s", &aString[0]);	// = を読み込む (不要)
						fscanf(pFile, "%f", &fRadius);		// 半径を読み込む
					}
				} while (strcmp(&aString[0], "END_SKYSET") != 0);	// 読み込んだ文字列が END_SKYSET ではない場合ループ

				// 空オブジェクトの生成
				if (CSky::Create((CSky::EType)nType, pos, D3DXToRadian(rot), col, part, texPart, fRadius) == nullptr)
				{ // 確保に失敗した場合

					// 失敗を返す
					assert(false);
					return E_FAIL;
				}
			}
		} while (strcmp(&aString[0], "END_STAGE_SKYSET") != 0);	// 読み込んだ文字列が END_STAGE_SKYSET ではない場合ループ
	}

	// 成功を返す
	return S_OK;
}

//============================================================
//	液体情報の読込処理
//============================================================
HRESULT CStage::LoadLiquid(const char* pString, FILE *pFile)
{
	VECTOR3 pos = VEC3_ZERO;	// 位置の代入用
	VECTOR3 rot = VEC3_ZERO;	// 向きの代入用
	VECTOR2 size = VEC2_ZERO;	// 大きさの代入用
	COLOR col = color::White();	// 色の代入用
	POSGRID2 part = GRID2_ZERO;		// 分割数の代入用
	CLiquid::STexMove texMove;		// テクスチャ移動量の代入用
	char aString[128];			// テキストの文字列の代入用
	int nType = 0;				// 種類の代入用
	float fMaxUp = 0.0f;		// 波の最高上昇量
	float fAddSinRot = 0.0f;	// 波打ち向き加算量
	float fAddVtxRot = 0.0f;	// 隣波の向き加算量

	if (pString == nullptr || pFile == nullptr)
	{ // 文字列・ファイルが存在しない場合

		// 失敗を返す
		assert(false);
		return E_FAIL;
	}

	// 液体の設定
	if (strcmp(pString, "STAGE_LIQUIDSET") == 0)
	{ // 読み込んだ文字列が STAGE_LIQUIDSET の場合

		do
		{ // 読み込んだ文字列が END_STAGE_LIQUIDSET ではない場合ループ

			// ファイルから文字列を読み込む
			fscanf(pFile, "%s", &aString[0]);

			if (strcmp(&aString[0], "LIQUIDSET") == 0)
			{ // 読み込んだ文字列が LIQUIDSET の場合

				do
				{ // 読み込んだ文字列が END_LIQUIDSET ではない場合ループ

					// ファイルから文字列を読み込む
					fscanf(pFile, "%s", &aString[0]);

					if (strcmp(&aString[0], "TYPE") == 0)
					{ // 読み込んだ文字列が TYPE の場合

						fscanf(pFile, "%s", &aString[0]);	// = を読み込む (不要)
						fscanf(pFile, "%d", &nType);		// 種類を読み込む
					}
					else if (strcmp(&aString[0], "POS") == 0)
					{ // 読み込んだ文字列が POS の場合

						fscanf(pFile, "%s", &aString[0]);	// = を読み込む (不要)
						fscanf(pFile, "%f", &pos.x);		// 位置Xを読み込む
						fscanf(pFile, "%f", &pos.y);		// 位置Yを読み込む
						fscanf(pFile, "%f", &pos.z);		// 位置Zを読み込む
					}
					else if (strcmp(&aString[0], "ROT") == 0)
					{ // 読み込んだ文字列が ROT の場合

						fscanf(pFile, "%s", &aString[0]);	// = を読み込む (不要)
						fscanf(pFile, "%f", &rot.x);		// 向きXを読み込む
						fscanf(pFile, "%f", &rot.y);		// 向きYを読み込む
						fscanf(pFile, "%f", &rot.z);		// 向きZを読み込む
					}
					else if (strcmp(&aString[0], "SIZE") == 0)
					{ // 読み込んだ文字列が SIZE の場合

						fscanf(pFile, "%s", &aString[0]);	// = を読み込む (不要)
						fscanf(pFile, "%f", &size.x);		// 大きさXを読み込む
						fscanf(pFile, "%f", &size.y);		// 大きさYを読み込む
					}
					else if (strcmp(&aString[0], "COL") == 0)
					{ // 読み込んだ文字列が COL の場合

						fscanf(pFile, "%s", &aString[0]);	// = を読み込む (不要)
						fscanf(pFile, "%f", &col.r);		// 色Rを読み込む
						fscanf(pFile, "%f", &col.g);		// 色Gを読み込む
						fscanf(pFile, "%f", &col.b);		// 色Bを読み込む
						fscanf(pFile, "%f", &col.a);		// 色Aを読み込む
					}
					else if (strcmp(&aString[0], "PART") == 0)
					{ // 読み込んだ文字列が PART の場合

						fscanf(pFile, "%s", &aString[0]);	// = を読み込む (不要)
						fscanf(pFile, "%d", &part.x);		// 横分割数を読み込む
						fscanf(pFile, "%d", &part.y);		// 縦分割数を読み込む
					}
					else if (strcmp(&aString[0], "TEXMOVE_LOW") == 0)
					{ // 読み込んだ文字列が TEXMOVE_LOW の場合

						fscanf(pFile, "%s", &aString[0]);			// = を読み込む (不要)
						fscanf(pFile, "%f", &texMove.texMoveLow.x);	// 下液体のテクスチャ移動量を読み込む
						fscanf(pFile, "%f", &texMove.texMoveLow.y);	// 下液体のテクスチャ移動量を読み込む
					}
					else if (strcmp(&aString[0], "TEXMOVE_HIGH") == 0)
					{ // 読み込んだ文字列が TEXMOVE_HIGH の場合

						fscanf(pFile, "%s", &aString[0]);				// = を読み込む (不要)
						fscanf(pFile, "%f", &texMove.texMoveHigh.x);	// 上液体のテクスチャ移動量を読み込む
						fscanf(pFile, "%f", &texMove.texMoveHigh.y);	// 上液体のテクスチャ移動量を読み込む
					}
					else if (strcmp(&aString[0], "MAX_UP") == 0)
					{ // 読み込んだ文字列が MAX_UP の場合

						fscanf(pFile, "%s", &aString[0]);	// = を読み込む (不要)
						fscanf(pFile, "%f", &fMaxUp);		// 波の最高上昇量を読み込む
					}
					else if (strcmp(&aString[0], "ADD_SINROT") == 0)
					{ // 読み込んだ文字列が ADD_SINROT の場合

						fscanf(pFile, "%s", &aString[0]);	// = を読み込む (不要)
						fscanf(pFile, "%f", &fAddSinRot);	// 波打ち向き加算量を読み込む
					}
					else if (strcmp(&aString[0], "ADD_VTXROT") == 0)
					{ // 読み込んだ文字列が ADD_VTXROT の場合

						fscanf(pFile, "%s", &aString[0]);	// = を読み込む (不要)
						fscanf(pFile, "%f", &fAddVtxRot);	// 隣波の向き加算量を読み込む
					}
				} while (strcmp(&aString[0], "END_LIQUIDSET") != 0);	// 読み込んだ文字列が END_LIQUIDSET ではない場合ループ

				// 液体オブジェクトの生成
				CLiquid *pLiquid = CLiquid::Create
				( // 引数
					(CLiquid::EType)nType,		// 種類
					pos,						// 位置
					D3DXToRadian(rot),			// 向き
					size,						// 大きさ
					col,						// 色
					part,						// 分割数
					texMove,					// テクスチャ移動量
					fMaxUp,						// 波の最高上昇量
					D3DXToRadian(fAddSinRot),	// 波打ち向き加算量
					D3DXToRadian(fAddVtxRot)	// 隣波の向き加算量
				);
				if (pLiquid == nullptr)
				{ // 確保に失敗した場合

					// 失敗を返す
					assert(false);
					return E_FAIL;
				}
			}
		} while (strcmp(&aString[0], "END_STAGE_LIQUIDSET") != 0);	// 読み込んだ文字列が END_STAGE_LIQUIDSET ではない場合ループ
	}

	// 成功を返す
	return S_OK;
}
