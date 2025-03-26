//============================================================
//
//	�u���b�N�t�@�N�g���[���� [blockFactory.cpp]
//	Author�F��������
//
//============================================================
//************************************************************
//	�C���N���[�h�t�@�C��
//************************************************************
#include "blockFactory.h"
#include "block.h"

#include "timer.h"
#include "useful.h"

//************************************************************
// �萔��`
//************************************************************
namespace
{
	const int PRIORITY = 0;			// �D�揇��
	const float ADD_RATE = 0.0001f;	// �����̉��Z��
	const float MAX_RATE = 1.0f;	// �����̍ő�l
	const float MIN_RATE = 0.1f;	// �����̍ŏ��l
	const float CREATE_RATE = 1.0f;	// �����̊���

	const float RIGHT_POS = -740.0f;	// �E���̈ʒu
	const float LEFT_POS = 740.0f;		// �����̈ʒu
}

//************************************************************
//	�e�N���X [CBlockFactory] �̃����o�֐�
//************************************************************
//============================================================
//	�R���X�g���N�^
//============================================================
CBlockFactory::CBlockFactory() : CObject(CObject::LABEL_BLOCKFACTORY, CObject::DIM_3D, PRIORITY),
m_fRate(MIN_RATE),		// ����
m_fCreateRate(0.0f)		// �������̊���
{

}

//============================================================
//	�f�X�g���N�^
//============================================================
CBlockFactory::~CBlockFactory()
{

}

//============================================================
//	����������
//============================================================
HRESULT CBlockFactory::Init()
{
	return S_OK;
}

//============================================================
//	�I������
//============================================================
void CBlockFactory::Uninit()
{
	// �j��
	Release();
}

//============================================================
//	�X�V����
//============================================================
void CBlockFactory::Update(const float fDeltaTime)
{
	if (m_fRate < MAX_RATE)
	{ // �ő�l�ȉ��̏ꍇ

		// ���������Z����
		m_fRate += ADD_RATE;

		if (m_fRate >= MAX_RATE)
		{ // �������ő�l�ɒB�����ꍇ

			m_fRate = MAX_RATE;
		}
	}

	// ���������Z����
	m_fCreateRate += sinf(m_fRate);
	int nNum = (int)(m_fCreateRate / CREATE_RATE);

	for (int nCnt = 0; nCnt < nNum; nCnt++)
	{
		// �ݒ���
		VECTOR3 pos = VEC3_ZERO;						// �ʒu
		float fSpeed = useful::Random(2.0f, 8.0f, 1);	// ���x
		bool bRight = (bool)(rand() % 2);				// �E����

		// �ʒu��ݒ肷��
		if (bRight) { pos.x = RIGHT_POS; }
		else { pos.x = LEFT_POS; }
		pos.y = useful::Random(100.0f, 720.0f, 1);
		pos.z = 0.0f;

		// ���ɐi�ޏꍇ�A�ړ��ʂ��t�]������
		if (!bRight) { fSpeed *= -1; }

		// �u���b�N�𐶐�����
		CBlock::Create(pos, CBlock::TYPE_BREAK, fSpeed, bRight);

		// �������������Z�b�g����
		m_fCreateRate = 0.0f;
	}
}

//============================================================
//	�`�揈��
//============================================================
void CBlockFactory::Draw(CShader* pShader)
{

}

//============================================================
//	��������
//============================================================
CBlockFactory* CBlockFactory::Create()
{
	// �u���b�N�t�@�N�g���[�̐���
	CBlockFactory* pFactory = new CBlockFactory;
	if (pFactory == nullptr)
	{ // �����Ɏ��s�����ꍇ

		return nullptr;
	}
	else
	{ // �����ɐ��������ꍇ

		// �u���b�N�t�@�N�g���[�̏�����
		if (FAILED(pFactory->Init()))
		{ // �������Ɏ��s�����ꍇ

			// �u���b�N�t�@�N�g���[�̔j��
			SAFE_DELETE(pFactory);
			return nullptr;
		}

		// �m�ۂ����A�h���X��Ԃ�
		return pFactory;
	}
}
