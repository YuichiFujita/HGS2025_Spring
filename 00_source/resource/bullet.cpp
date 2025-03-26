//============================================================
//
//	�e�e���� [bullet.cpp]
//	Author�F��������
//
//============================================================
//************************************************************
//	�C���N���[�h�t�@�C��
//************************************************************
#include "bullet.h"

#include "block.h"

//************************************************************
//	�萔�錾
//************************************************************
namespace
{
	const D3DXVECTOR3 RADIUS = D3DXVECTOR3(10.0f, 10.0f, 0.0f);		// ���a
	const float SPEED = 4.0f;		// ���x
}

//************************************************************
//	�ÓI�����o�ϐ��錾
//************************************************************
CListManager<CBullet>* CBullet::m_pList = nullptr;	// �I�u�W�F�N�g���X�g

//************************************************************
//	�q�N���X [CBullet] �̃����o�֐�
//************************************************************
//============================================================
//	�R���X�g���N�^
//============================================================
CBullet::CBullet() : CObject3D(CObject::LABEL_BLOCK, CObject::DIM_3D, 5),
m_move(VEC3_ZERO)	// �ړ���
{

}

//============================================================
//	�f�X�g���N�^
//============================================================
CBullet::~CBullet()
{

}

//============================================================
//	����������
//============================================================
HRESULT CBullet::Init()
{
	// �I�u�W�F�N�g3D�̏�����
	if (FAILED(CObject3D::Init()))
	{ // �������Ɏ��s�����ꍇ

		// ���s��Ԃ�
		assert(false);
		return E_FAIL;
	}

	if (m_pList == nullptr)
	{ // ���X�g�}�l�[�W���[�����݂��Ȃ��ꍇ

		// ���X�g�}�l�[�W���[�̐���
		m_pList = CListManager<CBullet>::Create();
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
void CBullet::Uninit()
{
	// ���X�g���玩�g�̃I�u�W�F�N�g���폜
	m_pList->DelList(m_iterator);

	if (m_pList->GetNumAll() == 0)
	{ // �I�u�W�F�N�g������Ȃ��ꍇ

		// ���X�g�}�l�[�W���[�̔j��
		m_pList->Release(m_pList);
	}

	// �I�u�W�F�N�g3D�̏I��
	CObject3D::Uninit();
}

//============================================================
//	�X�V����
//============================================================
void CBullet::Update(const float fDeltaTime)
{
	if (useful::ScreenOut(GetVec3Position(), RADIUS.x))
	{ // ��ʊO�ɏo���ꍇ

		// �I������
		Uninit();
		return;
	}

	// �ړ�����
	Move();

	// ���_���W�̐ݒ菈��
	SetVtx();
}

//============================================================
//	�`�揈��
//============================================================
void CBullet::Draw(CShader* pShader)
{
	// �I�u�W�F�N�g3D�̕`��
	CObject3D::Draw(pShader);
}

//============================================================
//	��������
//============================================================
CBullet* CBullet::Create
(
	const VECTOR3& rPos,	// �ʒu
	const bool bRight		// �E��
)
{
	// �e�e�̐���
	CBullet* pBlock = new CBullet;

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

		// �ړ��ʂ�ݒ�
		pBlock->SetMove(bRight);

		// �m�ۂ����A�h���X��Ԃ�
		return pBlock;
	}
}

//============================================================
//	���X�g�擾����
//============================================================
CListManager<CBullet>* CBullet::GetList()
{
	// �I�u�W�F�N�g���X�g��Ԃ�
	return m_pList;
}

//============================================================
// �ړ��ʂ̐ݒ菈��
//============================================================
void CBullet::SetMove(const bool bRight)
{
	if (bRight)
	{ // �E���ړ��̏ꍇ

		m_move.x = SPEED;
	}
	else
	{ // �����ړ��̏ꍇ

		m_move.x = -SPEED;
	}
}

//============================================================
// �u���b�N�Ƃ̓����蔻��
//============================================================
void CBullet::BlockCollision(void)
{
	VECTOR3 pos = GetVec3Position();
	VECTOR3 size = GetVec3Size();

	// �u���b�N�̃��X�g���擾
	std::list<CBlock*> list = CBlock::GetList()->GetList();

	for (auto pBlock : list)
	{
		if (pBlock->Collision(pos, size))
		{ // �u���b�N�ɓ��������ꍇ

			// �q�b�g����
			pBlock->Hit();
		}
	}
}

//============================================================
// �ړ�����
//============================================================
void CBullet::Move(void)
{
	// �ʒu�ړ�
	VECTOR3 pos = GetVec3Position();
	pos.x += m_move.x;
	SetVec3Position(pos);
}
