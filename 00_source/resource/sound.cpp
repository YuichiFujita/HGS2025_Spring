//============================================================
//
//	サウンド処理 [sound.cpp]
//	Author：藤田勇一
//
//============================================================
//************************************************************
//	インクルードファイル
//************************************************************
#include "sound.h"

//************************************************************
//	定数宣言
//************************************************************
namespace
{
	// サウンド情報構造体
	struct SSoundInfo
	{
		// デフォルトコンストラクタ
		SSoundInfo() :
			pFilePath	(nullptr),	// ファイルパス
			nCntLoop	(0)			// ループカウント
		{}

		// 引数付きコンストラクタ
		SSoundInfo(const char* in_pFilePath, const int in_nCntLoop) :
			pFilePath	(in_pFilePath),	// ファイルパス
			nCntLoop	(in_nCntLoop)	// ループカウント
		{}

		// デストラクタ
		~SSoundInfo() {}

		// メンバ変数
		const char* pFilePath;	// ファイルパス
		int nCntLoop;			// ループカウント
	};
	const SSoundInfo SOUND_FILE[] =	// サウンドファイル
	{
		SSoundInfo("data\\BGM\\bgm_general.wav", -1),	// BGM
		SSoundInfo("data\\SE\\snd_ballchime.wav", 0),	// BALL入力効果音
	};
}

//************************************************************
//	スタティックアサート
//************************************************************
static_assert(NUM_ARRAY(SOUND_FILE) == CSound::LABEL_MAX, "ERROR : Sound Count Mismatch");

//************************************************************
//	親クラス [CSound] のメンバ関数
//************************************************************
//============================================================
//	コンストラクタ
//============================================================
CSound::CSound() :
	m_pXAudio2			(nullptr),	// XAudio2オブジェクトへのインターフェイス
	m_pMasteringVoice	(nullptr)	// マスターボイス
{
	// メンバ変数をクリア
	memset(&m_apSourceVoice[0],	0, sizeof(m_apSourceVoice));	// ソースボイス
	memset(&m_apDataAudio[0],	0, sizeof(m_apDataAudio));		// オーディオデータ
	memset(&m_aSizeAudio[0],	0, sizeof(m_aSizeAudio));		// オーディオデータサイズ
}

//============================================================
//	デストラクタ
//============================================================
CSound::~CSound()
{

}

//============================================================
//	初期化処理
//============================================================
HRESULT CSound::Init(HWND hWnd)
{
	HRESULT hr;	// 終了確認用

	// メンバ変数を初期化
	m_pXAudio2			= nullptr;	// XAudio2オブジェクトへのインターフェイス
	m_pMasteringVoice	= nullptr;	// マスターボイス
	memset(&m_apSourceVoice[0],	0, sizeof(m_apSourceVoice));	// ソースボイス
	memset(&m_apDataAudio[0],	0, sizeof(m_apDataAudio));		// オーディオデータ
	memset(&m_aSizeAudio[0],	0, sizeof(m_aSizeAudio));		// オーディオデータサイズ

	// COMライブラリの初期化
	hr = CoInitializeEx(nullptr, COINIT_MULTITHREADED);
	if (FAILED(hr))
	{
		MessageBox(hWnd, "COMライブラリの初期化に失敗！", "警告！", MB_ICONWARNING);

		// COMライブラリの終了処理
		CoUninitialize();

		assert(false);
		return E_FAIL;
	}

	// XAudio2オブジェクトの作成
	hr = XAudio2Create(&m_pXAudio2, 0);
	if (FAILED(hr))
	{
		MessageBox(hWnd, "XAudio2オブジェクトの作成に失敗！", "警告！", MB_ICONWARNING);

		// COMライブラリの終了処理
		CoUninitialize();

		assert(false);
		return E_FAIL;
	}
	
	// マスターボイスの生成
	hr = m_pXAudio2->CreateMasteringVoice(&m_pMasteringVoice);
	if (FAILED(hr))
	{
		MessageBox(hWnd, "マスターボイスの生成に失敗！", "警告！", MB_ICONWARNING);

		if(m_pXAudio2 != nullptr)
		{
			// XAudio2オブジェクトの開放
			m_pXAudio2->Release();
			m_pXAudio2 = nullptr;
		}

		// COMライブラリの終了処理
		CoUninitialize();

		assert(false);
		return E_FAIL;
	}

	return S_OK;
}

//============================================================
//	終了処理
//============================================================
void CSound::Uninit()
{
	for (int nCntSound = 0; nCntSound < LABEL_MAX; nCntSound++)
	{
		if (m_apSourceVoice[nCntSound] != nullptr)
		{
			// 一時停止
			m_apSourceVoice[nCntSound]->Stop(0);
	
			// ソースボイスの破棄
			m_apSourceVoice[nCntSound]->DestroyVoice();
			m_apSourceVoice[nCntSound] = nullptr;
		}

		// オーディオデータの開放
		SAFE_FREE(m_apDataAudio[nCntSound]);
	}
	
	// マスターボイスの破棄
	m_pMasteringVoice->DestroyVoice();
	m_pMasteringVoice = nullptr;

	// XAudio2オブジェクトの開放
	SAFE_RELEASE(m_pXAudio2);

	// COMライブラリの終了処理
	CoUninitialize();
}

//============================================================
//	全読込処理
//============================================================
HRESULT CSound::LoadAll(HWND hWnd)
{
	// サウンドデータの初期化
	for (int nCntSound = 0; nCntSound < LABEL_MAX; nCntSound++)
	{
		HRESULT hr;
		HANDLE hFile;
		DWORD dwChunkSize = 0;
		DWORD dwChunkPosition = 0;
		DWORD dwFiletype;
		WAVEFORMATEXTENSIBLE wfx;
		XAUDIO2_BUFFER buffer;

		// バッファのクリア
		memset(&wfx, 0, sizeof(WAVEFORMATEXTENSIBLE));
		memset(&buffer, 0, sizeof(XAUDIO2_BUFFER));

		// サウンドデータファイルの生成
		hFile = CreateFile(SOUND_FILE[nCntSound].pFilePath, GENERIC_READ, FILE_SHARE_READ, nullptr, OPEN_EXISTING, 0, nullptr);
		if (hFile == INVALID_HANDLE_VALUE)
		{
			MessageBox(hWnd, "サウンドデータファイルの生成に失敗！(1)", "警告！", MB_ICONWARNING);
			return HRESULT_FROM_WIN32(GetLastError());
		}
		if (SetFilePointer(hFile, 0, nullptr, FILE_BEGIN) == INVALID_SET_FILE_POINTER)
		{ // ファイルポインタを先頭に移動

			MessageBox(hWnd, "サウンドデータファイルの生成に失敗！(2)", "警告！", MB_ICONWARNING);
			return HRESULT_FROM_WIN32(GetLastError());
		}

		// WAVEファイルのチェック
		hr = CheckChunk(hFile, 'FFIR', &dwChunkSize, &dwChunkPosition);
		if (FAILED(hr))
		{
			MessageBox(hWnd, "WAVEファイルのチェックに失敗！(1)", "警告！", MB_ICONWARNING);
			return S_FALSE;
		}
		hr = ReadChunkData(hFile, &dwFiletype, sizeof(DWORD), dwChunkPosition);
		if (FAILED(hr))
		{
			MessageBox(hWnd, "WAVEファイルのチェックに失敗！(2)", "警告！", MB_ICONWARNING);
			return S_FALSE;
		}
		if (dwFiletype != 'EVAW')
		{
			MessageBox(hWnd, "WAVEファイルのチェックに失敗！(3)", "警告！", MB_ICONWARNING);
			return S_FALSE;
		}

		// フォーマットチェック
		hr = CheckChunk(hFile, ' tmf', &dwChunkSize, &dwChunkPosition);
		if (FAILED(hr))
		{
			MessageBox(hWnd, "フォーマットチェックに失敗！(1)", "警告！", MB_ICONWARNING);
			return S_FALSE;
		}
		hr = ReadChunkData(hFile, &wfx, dwChunkSize, dwChunkPosition);
		if (FAILED(hr))
		{
			MessageBox(hWnd, "フォーマットチェックに失敗！(2)", "警告！", MB_ICONWARNING);
			return S_FALSE;
		}

		// オーディオデータ読み込み
		hr = CheckChunk(hFile, 'atad', &m_aSizeAudio[nCntSound], &dwChunkPosition);
		if (FAILED(hr))
		{
			MessageBox(hWnd, "オーディオデータ読み込みに失敗！(1)", "警告！", MB_ICONWARNING);
			return S_FALSE;
		}
		m_apDataAudio[nCntSound] = (BYTE*)malloc(m_aSizeAudio[nCntSound]);
		hr = ReadChunkData(hFile, m_apDataAudio[nCntSound], m_aSizeAudio[nCntSound], dwChunkPosition);
		if (FAILED(hr))
		{
			MessageBox(hWnd, "オーディオデータ読み込みに失敗！(2)", "警告！", MB_ICONWARNING);
			return S_FALSE;
		}

		// ソースボイスの生成
		hr = m_pXAudio2->CreateSourceVoice(&m_apSourceVoice[nCntSound], &(wfx.Format));
		if (FAILED(hr))
		{
			MessageBox(hWnd, "ソースボイスの生成に失敗！", "警告！", MB_ICONWARNING);
			return S_FALSE;
		}

		// バッファの値設定
		memset(&buffer, 0, sizeof(XAUDIO2_BUFFER));
		buffer.AudioBytes = m_aSizeAudio[nCntSound];
		buffer.pAudioData = m_apDataAudio[nCntSound];
		buffer.Flags = XAUDIO2_END_OF_STREAM;
		buffer.LoopCount = SOUND_FILE[nCntSound].nCntLoop;

		// オーディオバッファの登録
		m_apSourceVoice[nCntSound]->SubmitSourceBuffer(&buffer);

		// ファイルをクローズ
		CloseHandle(hFile);
	}

	return S_OK;
}

//============================================================
//	セグメント再生 (再生中なら停止)
//============================================================
HRESULT CSound::Play(ELabel label)
{
	XAUDIO2_VOICE_STATE xa2state;
	XAUDIO2_BUFFER buffer;

	// バッファの値設定
	memset(&buffer, 0, sizeof(XAUDIO2_BUFFER));
	buffer.AudioBytes	= m_aSizeAudio[label];
	buffer.pAudioData	= m_apDataAudio[label];
	buffer.Flags		= XAUDIO2_END_OF_STREAM;
	buffer.LoopCount	= SOUND_FILE[label].nCntLoop;

	// 状態取得
	m_apSourceVoice[label]->GetState(&xa2state);
	if (xa2state.BuffersQueued != 0)
	{ // 再生中

		// 一時停止
		m_apSourceVoice[label]->Stop(0);

		// オーディオバッファの削除
		m_apSourceVoice[label]->FlushSourceBuffers();
	}

	// オーディオバッファの登録
	m_apSourceVoice[label]->SubmitSourceBuffer(&buffer);

	// 再生
	m_apSourceVoice[label]->Start(0);

	return S_OK;
}

//============================================================
//	セグメント停止 (ラベル指定)
//============================================================
void CSound::Stop(ELabel label)
{
	XAUDIO2_VOICE_STATE xa2state;

	// 状態取得
	m_apSourceVoice[label]->GetState(&xa2state);
	if (xa2state.BuffersQueued != 0)
	{ // 再生中

		// 一時停止
		m_apSourceVoice[label]->Stop(0);

		// 音量を初期化
		m_apSourceVoice[label]->SetVolume(1.0f);

		// 周波数を初期化
		m_apSourceVoice[label]->SetFrequencyRatio(1.0f);

		// オーディオバッファの削除
		m_apSourceVoice[label]->FlushSourceBuffers();
	}
}

//============================================================
//	セグメント停止 (全て)
//============================================================
void CSound::Stop()
{
	// 一時停止
	for (int nCntSound = 0; nCntSound < LABEL_MAX; nCntSound++)
	{
		if (m_apSourceVoice[nCntSound] != nullptr)
		{
			// 一時停止
			m_apSourceVoice[nCntSound]->Stop(0);

			// 音量を初期化
			m_apSourceVoice[nCntSound]->SetVolume(1.0f);

			// 周波数を初期化
			m_apSourceVoice[nCntSound]->SetFrequencyRatio(1.0f);
		}
	}
}

//============================================================
//	音量の設定処理
//============================================================
void CSound::SetVolume(const ELabel label, float fVolume)
{
	XAUDIO2_VOICE_STATE xa2state;

	// 状態取得
	m_apSourceVoice[label]->GetState(&xa2state);
	if (xa2state.BuffersQueued != 0)
	{ // 再生中の場合

		// 音量を設定
		useful::LimitNum(fVolume, 0.0f, 1.0f);
		m_apSourceVoice[label]->SetVolume(fVolume);
	}
}

//============================================================
//	音量の取得処理
//============================================================
float CSound::GetVolume(const ELabel label) const
{
	float fVolume = 0.0f;	// 音量

	// 音量を取得
	m_apSourceVoice[label]->GetVolume(&fVolume);

	// 音量を返す
	return fVolume;
}

//============================================================
//	周波数の設定処理
//============================================================
void CSound::SetFrequency(const ELabel label, float fFreq)
{
	XAUDIO2_VOICE_STATE xa2state;

	// 状態取得
	m_apSourceVoice[label]->GetState(&xa2state);
	if (xa2state.BuffersQueued != 0)
	{ // 再生中の場合

		// 周波数を設定
		m_apSourceVoice[label]->SetFrequencyRatio(fFreq);
	}
}

//============================================================
//	周波数の取得処理
//============================================================
float CSound::GetFrequency(const ELabel label) const
{
	float fFreq = 0.0f;	// 周波数

	// 周波数を取得
	m_apSourceVoice[label]->GetFrequencyRatio(&fFreq);

	// 周波数を返す
	return fFreq;
}

//============================================================
//	生成処理
//============================================================
CSound* CSound::Create(HWND hWnd)
{
	// サウンドの生成
	CSound* pSound = new CSound;
	if (pSound == nullptr)
	{ // 生成に失敗した場合

		return nullptr;
	}
	else
	{ // 生成に成功した場合

		// サウンドの初期化
		if (FAILED(pSound->Init(hWnd)))
		{ // 初期化に失敗した場合

			// サウンドの破棄
			SAFE_DELETE(pSound);
			return nullptr;
		}

		// 確保したアドレスを返す
		return pSound;
	}
}

//============================================================
//	破棄処理
//============================================================
void CSound::Release(CSound*& prSound)
{
	// サウンドの終了
	assert(prSound != nullptr);
	prSound->Uninit();

	// メモリ開放
	SAFE_DELETE(prSound);
}

//============================================================
//	チャンクのチェック
//============================================================
HRESULT CSound::CheckChunk(HANDLE hFile, DWORD format, DWORD* pChunkSize, DWORD* pChunkDataPosition)
{
	HRESULT hr = S_OK;	// 終了確認用
	DWORD dwRead;
	DWORD dwChunkType;
	DWORD dwChunkDataSize;
	DWORD dwRIFFDataSize = 0;
	DWORD dwFileType;
	DWORD dwBytesRead = 0;
	DWORD dwOffset = 0;
	if (SetFilePointer(hFile, 0, nullptr, FILE_BEGIN) == INVALID_SET_FILE_POINTER)
	{ // ファイルポインタを先頭に移動

		return HRESULT_FROM_WIN32(GetLastError());
	}
	
	while (hr == S_OK)
	{
		if (ReadFile(hFile, &dwChunkType, sizeof(DWORD), &dwRead, nullptr) == 0)
		{ // チャンクの読み込み

			hr = HRESULT_FROM_WIN32(GetLastError());
		}

		if (ReadFile(hFile, &dwChunkDataSize, sizeof(DWORD), &dwRead, nullptr) == 0)
		{ // チャンクデータの読み込み

			hr = HRESULT_FROM_WIN32(GetLastError());
		}

		switch(dwChunkType)
		{
		case 'FFIR':

			dwRIFFDataSize  = dwChunkDataSize;
			dwChunkDataSize = 4;

			if (ReadFile(hFile, &dwFileType, sizeof(DWORD), &dwRead, nullptr) == 0)
			{ // ファイルタイプの読み込み

				hr = HRESULT_FROM_WIN32(GetLastError());
			}

			break;

		default:

			if (SetFilePointer(hFile, dwChunkDataSize, nullptr, FILE_CURRENT) == INVALID_SET_FILE_POINTER)
			{ // ファイルポインタをチャンクデータ分移動

				return HRESULT_FROM_WIN32(GetLastError());
			}
		}

		dwOffset += sizeof(DWORD) * 2;
		if (dwChunkType == format)
		{
			*pChunkSize         = dwChunkDataSize;
			*pChunkDataPosition = dwOffset;

			return S_OK;
		}

		dwOffset += dwChunkDataSize;
		if (dwBytesRead >= dwRIFFDataSize)
		{
			return S_FALSE;
		}
	}
	
	return S_OK;
}

//============================================================
//	チャンクデータの読み込み
//============================================================
HRESULT CSound::ReadChunkData(HANDLE hFile, void* pBuffer, DWORD dwBuffersize, DWORD dwBufferoffset)
{
	DWORD dwRead;
	if (SetFilePointer(hFile, dwBufferoffset, nullptr, FILE_BEGIN) == INVALID_SET_FILE_POINTER)
	{ // ファイルポインタを指定位置まで移動

		return HRESULT_FROM_WIN32(GetLastError());
	}

	if (ReadFile(hFile, pBuffer, dwBuffersize, &dwRead, nullptr) == 0)
	{ // データの読み込み

		return HRESULT_FROM_WIN32(GetLastError());
	}
	
	return S_OK;
}
