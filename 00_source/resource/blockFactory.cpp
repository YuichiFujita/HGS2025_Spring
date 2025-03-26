//============================================================
//
//	ブロックファクトリー処理 [blockFactory.cpp]
//	Author：小原立暉
//
//============================================================
//************************************************************
//	インクルードファイル
//************************************************************
#include "blockFactory.h"
#include "block.h"

#include "timer.h"
#include "useful.h"

//************************************************************
// 定数定義
//************************************************************
namespace
{
	const int PRIORITY = 0;			// 優先順位
	const float ADD_RATE = 0.0001f;	// 割合の加算数
	const float MAX_RATE = 1.0f;	// 割合の最大値
	const float MIN_RATE = 0.1f;	// 割合の最小値
	const float CREATE_RATE = 1.0f;	// 生成の割合

	const float RIGHT_POS = -740.0f;	// 右側の位置
	const float LEFT_POS = 740.0f;		// 左側の位置
}

//************************************************************
//	親クラス [CBlockFactory] のメンバ関数
//************************************************************
//============================================================
//	コンストラクタ
//============================================================
CBlockFactory::CBlockFactory() : CObject(CObject::LABEL_BLOCKFACTORY, CObject::DIM_3D, PRIORITY),
m_fRate(MIN_RATE),		// 割合
m_fCreateRate(0.0f)		// 生成時の割合
{

}

//============================================================
//	デストラクタ
//============================================================
CBlockFactory::~CBlockFactory()
{

}

//============================================================
//	初期化処理
//============================================================
HRESULT CBlockFactory::Init()
{
	return S_OK;
}

//============================================================
//	終了処理
//============================================================
void CBlockFactory::Uninit()
{
	// 破棄
	Release();
}

//============================================================
//	更新処理
//============================================================
void CBlockFactory::Update(const float fDeltaTime)
{
	if (m_fRate < MAX_RATE)
	{ // 最大値以下の場合

		// 割合を加算する
		m_fRate += ADD_RATE;

		if (m_fRate >= MAX_RATE)
		{ // 割合が最大値に達した場合

			m_fRate = MAX_RATE;
		}
	}

	// 割合を加算する
	m_fCreateRate += sinf(m_fRate);
	int nNum = (int)(m_fCreateRate / CREATE_RATE);

	for (int nCnt = 0; nCnt < nNum; nCnt++)
	{
		// 設定情報
		VECTOR3 pos = VEC3_ZERO;						// 位置
		float fSpeed = useful::Random(2.0f, 8.0f, 1);	// 速度
		bool bRight = (bool)(rand() % 2);				// 右側状況

		// 位置を設定する
		if (bRight) { pos.x = RIGHT_POS; }
		else { pos.x = LEFT_POS; }
		pos.y = useful::Random(100.0f, 720.0f, 1);
		pos.z = 0.0f;

		// 左に進む場合、移動量を逆転させる
		if (!bRight) { fSpeed *= -1; }

		// ブロックを生成する
		CBlock::Create(pos, CBlock::TYPE_BREAK, fSpeed, bRight);

		// 生成割合をリセットする
		m_fCreateRate = 0.0f;
	}
}

//============================================================
//	描画処理
//============================================================
void CBlockFactory::Draw(CShader* pShader)
{

}

//============================================================
//	生成処理
//============================================================
CBlockFactory* CBlockFactory::Create()
{
	// ブロックファクトリーの生成
	CBlockFactory* pFactory = new CBlockFactory;
	if (pFactory == nullptr)
	{ // 生成に失敗した場合

		return nullptr;
	}
	else
	{ // 生成に成功した場合

		// ブロックファクトリーの初期化
		if (FAILED(pFactory->Init()))
		{ // 初期化に失敗した場合

			// ブロックファクトリーの破棄
			SAFE_DELETE(pFactory);
			return nullptr;
		}

		// 確保したアドレスを返す
		return pFactory;
	}
}
