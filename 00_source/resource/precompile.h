//============================================================
//
//	プレコンパイルヘッダー [precompile.h]
//	Author：藤田勇一
//
//============================================================
//************************************************************
//	二重インクルード防止
//************************************************************
#ifndef _PRECOMPILE_H_
#define _PRECOMPILE_H_

//************************************************************
//	警告メッセージ無効化
//************************************************************
#pragma warning(disable : 26812)	// enumclass警告の削除

//************************************************************
//	マクロ定義
//************************************************************
#define _CRT_SECURE_NO_WARNINGS	// ビルド時の警告対処用マクロ
#define _CRTDBG_MAP_ALLOC		// メモリリーク出力用マクロ
#define  DIRECTINPUT_VERSION	(0x0800)	// ビルド時の警告対処用マクロ

//************************************************************
//	インクルードファイル
//************************************************************
#include <windows.h>
#include <stdio.h>
#include <time.h>
#include <assert.h>
#include <d3dx9.h>
#include <dinput.h>		// 入力
#include <Xinput.h>		// パッド
#include <xaudio2.h>	// サウンド
#include <crtdbg.h>		// メモリリーク検出に必要
#include <iostream>		// 入出力ストリーム
#include <fstream>		// ファイル入出力ストリーム
#include <sstream>		// 文字列入出力ストリーム
#include <iomanip>		// 小数点出力
#include <filesystem>	// ファイル操作
#include <functional>	// 関数ポインタ
#include <string>		// 文字列
#include <thread>		// スレッド
#include <future>		// スレッド返り値取得
#include <vector>		// 動的配列
#include <list>			// リスト構造
#include <map>			// 連想配列

#include "useful.h"			// 便利関数
#include "listManager.h"	// リスト管理
#include "input.h"			// 入力検知
#include "inputManager.h"	// 入力マネージャー
#include "debugproc.h"		// デバッグ表示

//************************************************************
//	メモリリークのソース表示 ON/OFF
//************************************************************
#if 1
#ifdef _DEBUG
#define new  ::new(_NORMAL_BLOCK, __FILE__, __LINE__)
#endif	// _DEBUG
#endif

//************************************************************
//	ゲーム記録 スコア/タイム
//************************************************************
#if 0
#define SCORE
#else
#define TIMER
#endif

#endif	// _PRECOMPILE_H_
