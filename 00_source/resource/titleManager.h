//============================================================
//
//	タイトルマネージャーヘッダー [titleManager.h]
//	Author：藤田勇一
//
//============================================================
//************************************************************
//	二重インクルード防止
//************************************************************
#ifndef _TITLEMANAGER_H_
#define _TITLEMANAGER_H_

//************************************************************
//	前方宣言
//************************************************************
class CTitleState;	// タイトル状態クラス

//************************************************************
//	クラス定義
//************************************************************
// タイトルマネージャークラス
class CTitleManager
{
public:
	// コンストラクタ
	CTitleManager();

	// デストラクタ
	~CTitleManager();

	// メンバ関数
	HRESULT Init();	// 初期化
	void Uninit();	// 終了
	void Update(const float fDeltaTime);		// 更新
	HRESULT ChangeState(CTitleState* pState);	// 状態変更
	void TransGame();	// ゲーム画面遷移

	// 静的メンバ関数
	static CTitleManager* Create();	// 生成
	static void Release(CTitleManager*& prTitleManager);	// 破棄

private:
	// メンバ変数
	CTitleState* m_pState;	// 状態
};

#endif	// _TITLEMANAGER_H_
