//============================================================
//
//	�Ǐ��� [wall.cpp]
//	Author�F���c�E��
//
//============================================================
//************************************************************
//	�C���N���[�h�t�@�C��
//************************************************************
#include "wall.h"
#include "manager.h"
#include "collision.h"
#include "renderer.h"
#include "texture.h"

//************************************************************
//	�萔�錾
//************************************************************
namespace
{
	const char* TEXTURE_FILE[] =	// �e�N�X�`���t�@�C��
	{
		"data\\TEXTURE\\soil000.png",	// �y�e�N�X�`��
	};

	const int PRIORITY = 2;	// �ǂ̗D�揇��
}

//************************************************************
//	�X�^�e�B�b�N�A�T�[�g
//************************************************************
static_assert(NUM_ARRAY(TEXTURE_FILE) == CWall::TYPE_MAX, "ERROR : Type Count Mismatch");

//************************************************************
//	�ÓI�����o�ϐ��錾
//************************************************************
CListManager<CWall>* CWall::m_pList = nullptr;	// �I�u�W�F�N�g���X�g

//************************************************************
//	�q�N���X [CWall] �̃����o�֐�
//************************************************************
//============================================================
//	�R���X�g���N�^
//============================================================
CWall::CWall() : CObjectMeshWall(CObject::LABEL_WALL, CObject::DIM_3D, PRIORITY),
	m_type	(TYPE_SOIL)	// ���
{

}

//============================================================
//	�f�X�g���N�^
//============================================================
CWall::~CWall()
{

}

//============================================================
//	����������
//============================================================
HRESULT CWall::Init()
{
	// �����o�ϐ���������
	m_type = TYPE_SOIL;	// ���

	// �I�u�W�F�N�g���b�V���E�H�[���̏�����
	if (FAILED(CObjectMeshWall::Init()))
	{ // �������Ɏ��s�����ꍇ

		// ���s��Ԃ�
		assert(false);
		return E_FAIL;
	}

	if (m_pList == nullptr)
	{ // ���X�g�}�l�[�W���[�����݂��Ȃ��ꍇ

		// ���X�g�}�l�[�W���[�̐���
		m_pList = CListManager<CWall>::Create();
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
void CWall::Uninit()
{
	// ���X�g���玩�g�̃I�u�W�F�N�g���폜
	m_pList->DelList(m_iterator);

	if (m_pList->GetNumAll() == 0)
	{ // �I�u�W�F�N�g������Ȃ��ꍇ

		// ���X�g�}�l�[�W���[�̔j��
		m_pList->Release(m_pList);
	}

	// �I�u�W�F�N�g���b�V���E�H�[���̏I��
	CObjectMeshWall::Uninit();
}

//============================================================
//	�X�V����
//============================================================
void CWall::Update(const float fDeltaTime)
{
	// �I�u�W�F�N�g���b�V���E�H�[���̍X�V
	CObjectMeshWall::Update(fDeltaTime);
}

//============================================================
//	�`�揈��
//============================================================
void CWall::Draw(CShader* pShader)
{
	// �I�u�W�F�N�g���b�V���E�H�[���̕`��
	CObjectMeshWall::Draw(pShader);
}

//============================================================
//	��������
//============================================================
CWall* CWall::Create
(
	const EType type,			// ���
	const VECTOR3& rPos,	// �ʒu
	const VECTOR3& rRot,	// ����
	const VECTOR2& rSize,	// �傫��
	const COLOR& rCol,		// �F
	const POSGRID2& rPart,		// ������
	const POSGRID2& rTexPart	// �e�N�X�`��������
)
{
	// �ǂ̐���
	CWall* pWall = new CWall;
	if (pWall == nullptr)
	{ // �����Ɏ��s�����ꍇ

		return nullptr;
	}
	else
	{ // �����ɐ��������ꍇ

		// �ǂ̏�����
		if (FAILED(pWall->Init()))
		{ // �������Ɏ��s�����ꍇ

			// �ǂ̔j��
			SAFE_DELETE(pWall);
			return nullptr;
		}

		// ��ނ�ݒ�
		pWall->SetType(type);

		// �ʒu��ݒ�
		pWall->SetVec3Position(rPos);

		// ������ݒ�
		pWall->SetVec3Rotation(rRot);

		// �傫����ݒ�
		pWall->SetVec2Size(rSize);

		// �F��ݒ�
		pWall->SetColor(rCol);

		// ��������ݒ�
		if (FAILED(pWall->SetPattern(rPart)))
		{ // �������̐ݒ�Ɏ��s�����ꍇ

			// �ǂ̔j��
			SAFE_DELETE(pWall);
			return nullptr;
		}

		// �e�N�X�`����������ݒ�
		pWall->SetTexPattern(rTexPart);

		// �m�ۂ����A�h���X��Ԃ�
		return pWall;
	}
}

//============================================================
//	���X�g�擾����
//============================================================
CListManager<CWall>* CWall::GetList()
{
	// �I�u�W�F�N�g���X�g��Ԃ�
	return m_pList;
}

//============================================================
//	��ނ̐ݒ菈��
//============================================================
void CWall::SetType(const EType type)
{
	if (type > NONE_IDX && type < TYPE_MAX)
	{ // �C���f�b�N�X���͈͓��̏ꍇ

		// ��ނ�ۑ�
		m_type = type;

		// �e�N�X�`����o�^�E����
		BindTexture(GET_MANAGER->GetTexture()->Regist(TEXTURE_FILE[type]));
	}
	else { assert(false); }	// �͈͊O
}
