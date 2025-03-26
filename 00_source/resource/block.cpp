//============================================================
//
//	�u���b�N���� [block.cpp]
//	Author�F��������
//
//============================================================
//************************************************************
//	�C���N���[�h�t�@�C��
//************************************************************
#include "block.h"

#include "blockBreak.h"

//************************************************************
//	�萔�錾
//************************************************************
namespace
{
	const D3DXVECTOR3 RADIUS = D3DXVECTOR3(50.0f, 25.0f, 0.0f);		// ���a
}

//************************************************************
//	�ÓI�����o�ϐ��錾
//************************************************************
CListManager<CBlock>* CBlock::m_pList = nullptr;	// �I�u�W�F�N�g���X�g

//************************************************************
//	�q�N���X [CBlock] �̃����o�֐�
//************************************************************
//============================================================
//	�R���X�g���N�^
//============================================================
CBlock::CBlock() : CObject3D(CObject::LABEL_BLOCK, CObject::DIM_3D, 4),
m_type(CBlock::TYPE_BREAK),		// ���
m_bRight(true),					// �E����
m_fSpeed(0.0f)					// ���x
{

}

//============================================================
//	�f�X�g���N�^
//============================================================
CBlock::~CBlock()
{

}

//============================================================
//	����������
//============================================================
HRESULT CBlock::Init()
{
	// �I�u�W�F�N�g���b�V���L���[�u�̏�����
	if (FAILED(CObject3D::Init()))
	{ // �������Ɏ��s�����ꍇ

		// ���s��Ԃ�
		assert(false);
		return E_FAIL;
	}

	if (m_pList == nullptr)
	{ // ���X�g�}�l�[�W���[�����݂��Ȃ��ꍇ

		// ���X�g�}�l�[�W���[�̐���
		m_pList = CListManager<CBlock>::Create();
		if (m_pList == nullptr)
		{ // �����Ɏ��s�����ꍇ

			// ���s��Ԃ�
			assert(false);
			return E_FAIL;
		}
	}

	// ���X�g�Ɏ��g�̃I�u�W�F�N�g��ǉ��E�C�e���[�^�[���擾
	m_iterator = m_pList->AddList(this);

	// ������Ԃ�
	return S_OK;
}

//============================================================
//	�I������
//============================================================
void CBlock::Uninit()
{
	// ���X�g���玩�g�̃I�u�W�F�N�g���폜
	m_pList->DelList(m_iterator);

	if (m_pList->GetNumAll() == 0)
	{ // �I�u�W�F�N�g������Ȃ��ꍇ

		// ���X�g�}�l�[�W���[�̔j��
		m_pList->Release(m_pList);
	}

	// �I�u�W�F�N�g���b�V���L���[�u�̏I��
	CObject3D::Uninit();
}

//============================================================
//	�X�V����
//============================================================
void CBlock::Update(const float fDeltaTime)
{
	// �ړ�����
	Move();

	// ���_���W�̐ݒ菈��
	SetVtx();
}

//============================================================
//	�`�揈��
//============================================================
void CBlock::Draw(CShader* pShader)
{
	// �I�u�W�F�N�g���b�V���L���[�u�̕`��
	CObject3D::Draw(pShader);
}

//============================================================
//	��������
//============================================================
CBlock* CBlock::Create
(
	const VECTOR3& rPos,	// �ʒu
	const EType type,		// ���
	const float fSpeed,		// ���x
	const bool bRight		// �E��
)
{
	// �u���b�N�̐���
	CBlock* pBlock = nullptr;

	switch (type)
	{
	case CBlock::TYPE_BREAK:

		// �j��\�u���b�N
		pBlock = new CBlockBreak;

		break;

	default:

		// ��~
		assert(false);

		break;
	}

	if (pBlock == nullptr)
	{ // �����Ɏ��s�����ꍇ

		return nullptr;
	}
	else
	{ // �����ɐ��������ꍇ

		// �v���C���[�̏�����
		if (FAILED(pBlock->Init()))
		{ // �������Ɏ��s�����ꍇ

			// �v���C���[�̔j��
			SAFE_DELETE(pBlock);
			return nullptr;
		}

		// �ʒu��ݒ�
		pBlock->SetVec3Position(rPos);

		// ������ݒ�
		pBlock->SetVec3Rotation(VEC3_ZERO);

		// �T�C�Y��ݒ�
		pBlock->SetVec3Size(RADIUS);

		// ��ނ�ݒ�
		pBlock->m_type = type;

		// ���x��ݒ�
		pBlock->m_fSpeed = fSpeed;

		// �E������ݒ�
		pBlock->m_bRight = bRight;

		// �m�ۂ����A�h���X��Ԃ�
		return pBlock;
	}
}

//============================================================
//	���X�g�擾����
//============================================================
CListManager<CBlock>* CBlock::GetList()
{
	// �I�u�W�F�N�g���X�g��Ԃ�
	return m_pList;
}

//============================================================
// �ړ�����
//============================================================
void CBlock::Move(void)
{
	// �ʒu�擾
	VECTOR3 pos = GetVec3Position();

	// �ʒu��i�߂�
	if (m_bRight)
	{ // �E���ړ��̏ꍇ
		pos.x += m_fSpeed;
	}
	else
	{ // �����ړ��̏ꍇ
		pos.x -= m_fSpeed;
	}

	// �ʒu�ݒ�
	SetVec3Position(pos);
}
