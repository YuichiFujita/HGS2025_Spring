//============================================================
//
//	サウンドヘッダー [sound.h]
//	Author：藤田勇一
//
//============================================================
//************************************************************
//	二重インクルード防止
//************************************************************
#ifndef _SOUND_H_
#define _SOUND_H_

//************************************************************
//	クラス定義
//************************************************************
// サウンドクラス
class CSound
{
public:
	// コンストラクタ
	CSound();

	// デストラクタ
	~CSound();

	// サウンド列挙
	enum ELabel
	{
		LABEL_NONE = NONE_IDX,	// ラベル無し
		LABEL_BGM_GENERAL,		// BGM (汎用)
		LABEL_BGM_TITLE,		// タイトル
		LABEL_BGM_GAME,			// ゲーム
		LABEL_BGM_RESULT,		// リザルト
		LABEL_SE_JUMP,			// ジャンプ
		LABEL_SE_HOVER,			// ホバー
		LABEL_SE_SHOT,			// 射撃
		LABEL_SE_EXPLOSION,		// 爆発
		LABEL_SE_DEATH,			// 死亡
		LABEL_SE_TITLE,			// タイトル
		LABEL_SE_SELECT,		// 選択音
		LABEL_SE_DESITION,		// 決定音
		LABEL_MAX				// この列挙型の総数
	};

	// メンバ関数
	HRESULT Init(HWND hWnd);	// 初期化
	void Uninit();				// 終了
	HRESULT LoadAll(HWND hWnd);	// 全読込
	HRESULT Play(ELabel label);	// 再生
	void Stop(ELabel label);	// 停止
	void Stop();				// 全停止
	void SetVolume(const ELabel label, float fVolume);	// 音量設定
	float GetVolume(const ELabel label) const;			// 音量取得
	void SetFrequency(const ELabel label, float fFreq);	// 周波数設定
	float GetFrequency(const ELabel label) const;		// 周波数取得

	// 静的メンバ関数
	static CSound* Create(HWND hWnd);		// 生成
	static void Release(CSound*& prSound);	// 破棄

private:
	// メンバ関数
	HRESULT CheckChunk(HANDLE hFile, DWORD format, DWORD* pChunkSize, DWORD* pChunkDataPosition);	// チャンクチェック
	HRESULT ReadChunkData(HANDLE hFile, void* pBuffer, DWORD dwBuffersize, DWORD dwBufferoffset);	// チャンクデータ読込

	// メンバ変数
	IXAudio2* m_pXAudio2;								// XAudio2オブジェクトへのインターフェイス
	IXAudio2MasteringVoice* m_pMasteringVoice;			// マスターボイス
	IXAudio2SourceVoice* m_apSourceVoice[LABEL_MAX];	// ソースボイス
	BYTE* m_apDataAudio[LABEL_MAX];						// オーディオデータ
	DWORD m_aSizeAudio[LABEL_MAX];						// オーディオデータサイズ
};

#endif	// _SOUND_H_
