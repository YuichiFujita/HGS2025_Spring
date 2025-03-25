//============================================================
//
//	キャラクターアニメーション3Dヘッダー [characterAnim3D.h]
//	Author：藤田勇一
//
//============================================================
//************************************************************
//	二重インクルード防止
//************************************************************
#ifndef _CHARACTER_ANIM3D_H_
#define _CHARACTER_ANIM3D_H_

//************************************************************
//	インクルードファイル
//************************************************************
#include "objectCharaAnim3D.h"
#include "extension.h"

//************************************************************
//	クラス定義
//************************************************************
// キャラクターアニメーション3Dクラス
class CCharacterAnim3D
{
public:
	// コンストラクタ
	CCharacterAnim3D();

	// デストラクタ
	~CCharacterAnim3D();

	// モーション情報型エイリアス定義
	using AMotion = CObjectCharaAnim3D::SInfo;

	// メンバ関数
	HRESULT Init();		// キャラクター初期化
	void Uninit();		// キャラクター終了
	HRESULT LoadAll();	// キャラクター全読込
	AMotion Regist(const char* pCharaPath);	// キャラクター登録

	// 静的メンバ関数
	static CCharacterAnim3D* Create();	// 生成
	static void Release(CCharacterAnim3D*& prCharacterAnim3D);	// 破棄

private:
	// メンバ関数
	HRESULT SearchFolderAll(std::string sFolderPath);	// フォルダ全検索

	// メンバ関数
	HRESULT LoadSetup(AMotion* pInfoChara, const char* pCharaPath);			// キャラクター情報セットアップ
	HRESULT LoadMotionSetup(AMotion* pInfoChara, const char* pMotionPath);	// モーション情報セットアップ

	// メンバ変数
	std::map<std::string, AMotion> m_mapCharacter;	// キャラクターアニメーション3D連想配列
	extension::ALoad m_load;	// 読み込み可能拡張子
};

#endif	// _CHARACTER_ANIM3D_H_
