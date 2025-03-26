//============================================================
//
//	�������� [explosion.cpp]
//	Author�F���c�E��
//
//============================================================
//************************************************************
//	�C���N���[�h�t�@�C��
//************************************************************
#include "explosion.h"

//************************************************************
//	�萔�錾
//************************************************************
namespace
{
	const char* TEXTURE_FILE[] =	// �e�N�X�`���t�@�C��
	{
		"data\\TEXTURE\\explosion000.png",	// �΃e�N�X�`��
		"data\\TEXTURE\\explosion001.png",	// ���e�N�X�`��
	};
	const POSGRID2 PTRN[] =		// ������
	{
		POSGRID2(10, 1),	// �Ε�����
		POSGRID2(8, 1),		// ��������
	};
	const float NEXT_TIME[] =	// �J�ڎ���
	{
		0.06f,	// �ΑJ�ڎ���
		0.08f,	// ���J�ڎ���
	};

	const int PRIORITY = 6;	// �����̗D�揇��
}

//************************************************************
//	�X�^�e�B�b�N�A�T�[�g
//************************************************************
static_assert(NUM_ARRAY(TEXTURE_FILE) == CExplosion::TYPE_MAX, "ERROR : Type Count Mismatch");

//************************************************************
//	�q�N���X [CExplosion] �̃����o�֐�
//************************************************************
//============================================================
//	�R���X�g���N�^
//============================================================
CExplosion::CExplosion() : CAnim3D(CObject::LABEL_EXPLOSION, CObject::DIM_3D, PRIORITY),
	m_type	(TYPE_FIRE)	// ���
{

}

//============================================================
//	�f�X�g���N�^
//============================================================
CExplosion::~CExplosion()
{

}

//============================================================
//	����������
//============================================================
HRESULT CExplosion::Init()
{
	// �����o�ϐ���������
	m_type = TYPE_FIRE;	// ���

	// �A�j���[�V����3D�̏�����
	if (FAILED(CAnim3D::Init()))
	{ // �������Ɏ��s�����ꍇ

		// ���s��Ԃ�
		assert(false);
		return E_FAIL;
	}

	// �e�N�X�`��������
	BindTexture(TEXTURE_FILE[m_type]);

	// �e�N�X�`���������ݒ�
	SetTexPtrn(PTRN[m_type]);

	// �p�^�[���ύX���Ԑݒ�
	SetNextTime(NEXT_TIME[m_type]);

	// �Đ��J�n
	SetEnablePlay(true);

	// ������Ԃ�
	return S_OK;
}

//============================================================
//	�I������
//============================================================
void CExplosion::Uninit()
{
	// �A�j���[�V����3D�̏I��
	CAnim3D::Uninit();
}

//============================================================
//	�X�V����
//============================================================
void CExplosion::Update(const float fDeltaTime)
{
	// �A�j���[�V����3D�̍X�V
	CAnim3D::Update(fDeltaTime);
	if (GetLoopAnim() > 0)
	{ // ��x���[�v���������ꍇ

		// ���g�̏I��
		Uninit();
		return;
	}
}

//============================================================
//	�`�揈��
//============================================================
void CExplosion::Draw(CShader* pShader)
{
	// �A�j���[�V����3D�̕`��
	CAnim3D::Draw(pShader);
}

//============================================================
//	��������
//============================================================
CExplosion* CExplosion::Create
(
	const EType type,		// ���
	const VECTOR3& rPos,	// �ʒu
	const VECTOR3& rSize	// �傫��
)
{
	// �����̐���
	CExplosion* pExplosion = new CExplosion;
	if (pExplosion == nullptr)
	{ // �����Ɏ��s�����ꍇ

		return nullptr;
	}
	else
	{ // �����ɐ��������ꍇ

		// �����̏�����
		if (FAILED(pExplosion->Init()))
		{ // �������Ɏ��s�����ꍇ

			// �����̔j��
			SAFE_DELETE(pExplosion);
			return nullptr;
		}

		// ��ނ�ݒ�
		pExplosion->SetType(type);

		// �ʒu��ݒ�
		pExplosion->SetVec3Position(rPos);

		// �傫����ݒ�
		pExplosion->SetVec3Size(rSize);

		// �m�ۂ����A�h���X��Ԃ�
		return pExplosion;
	}
}

//============================================================
//	��ނ̐ݒ菈��
//============================================================
void CExplosion::SetType(const EType type)
{
	if (type > NONE_IDX && type < TYPE_MAX)
	{ // �C���f�b�N�X���͈͓��̏ꍇ

		// ��ނ�ۑ�
		m_type = type;

		// �e�N�X�`��������
		BindTexture(TEXTURE_FILE[m_type]);

		// �e�N�X�`���������ݒ�
		SetTexPtrn(PTRN[m_type]);

		// �p�^�[���ύX���Ԑݒ�
		SetNextTime(NEXT_TIME[m_type]);
	}
	else { assert(false); }	// �͈͊O
}
