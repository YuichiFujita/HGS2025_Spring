//============================================================
//
//	�n�ʏ��� [field.cpp]
//	Author�F���c�E��
//
//============================================================
//************************************************************
//	�C���N���[�h�t�@�C��
//************************************************************
#include "field.h"
#include "manager.h"
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
	const int PRIORITY = 0;	// �n�ʂ̗D�揇��
}

//************************************************************
//	�ÓI�����o�ϐ��錾
//************************************************************
CListManager<CField>* CField::m_pList = nullptr;	// �I�u�W�F�N�g���X�g

//************************************************************
//	�X�^�e�B�b�N�A�T�[�g
//************************************************************
static_assert(NUM_ARRAY(TEXTURE_FILE) == CField::TYPE_MAX, "ERROR : Type Count Mismatch");

//************************************************************
//	�q�N���X [CField] �̃����o�֐�
//************************************************************
//============================================================
//	�R���X�g���N�^
//============================================================
CField::CField() : CObjectMeshField(CObject::LABEL_FIELD, CObject::DIM_3D, PRIORITY),
	m_type	(TYPE_SOIL)	// ���
{

}

//============================================================
//	�f�X�g���N�^
//============================================================
CField::~CField()
{

}

//============================================================
//	����������
//============================================================
HRESULT CField::Init()
{
	// �����o�ϐ���������
	m_type = TYPE_SOIL;	// ���

	// �I�u�W�F�N�g���b�V���t�B�[���h�̏�����
	if (FAILED(CObjectMeshField::Init()))
	{ // �������Ɏ��s�����ꍇ

		// ���s��Ԃ�
		assert(false);
		return E_FAIL;
	}

	if (m_pList == nullptr)
	{ // ���X�g�}�l�[�W���[�����݂��Ȃ��ꍇ

		// ���X�g�}�l�[�W���[�̐���
		m_pList = CListManager<CField>::Create();
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
void CField::Uninit()
{
	// ���X�g���玩�g�̃I�u�W�F�N�g���폜
	m_pList->DelList(m_iterator);

	if (m_pList->GetNumAll() == 0)
	{ // �I�u�W�F�N�g������Ȃ��ꍇ

		// ���X�g�}�l�[�W���[�̔j��
		m_pList->Release(m_pList);
	}

	// �I�u�W�F�N�g���b�V���t�B�[���h�̏I��
	CObjectMeshField::Uninit();
}

//============================================================
//	�X�V����
//============================================================
void CField::Update(const float fDeltaTime)
{
	// �I�u�W�F�N�g���b�V���t�B�[���h�̍X�V
	CObjectMeshField::Update(fDeltaTime);
}

//============================================================
//	�`�揈��
//============================================================
void CField::Draw(CShader* pShader)
{
	// �I�u�W�F�N�g���b�V���t�B�[���h�̕`��
	CObjectMeshField::Draw(pShader);
}

//============================================================
//	��������
//============================================================
CField* CField::Create
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
	// �n�ʂ̐���
	CField* pField = new CField;
	if (pField == nullptr)
	{ // �����Ɏ��s�����ꍇ

		return nullptr;
	}
	else
	{ // �����ɐ��������ꍇ

		// �n�ʂ̏�����
		if (FAILED(pField->Init()))
		{ // �������Ɏ��s�����ꍇ

			// �n�ʂ̔j��
			SAFE_DELETE(pField);
			return nullptr;
		}

		// ��ނ�ݒ�
		pField->SetType(type);

		// �ʒu��ݒ�
		pField->SetVec3Position(rPos);

		// ������ݒ�
		pField->SetVec3Rotation(rRot);

		// �傫����ݒ�
		pField->SetVec2Size(rSize);

		// �F��ݒ�
		pField->SetColor(rCol);

		// ��������ݒ�
		if (FAILED(pField->SetPattern(rPart)))
		{ // �������̐ݒ�Ɏ��s�����ꍇ

			// �n�ʂ̔j��
			SAFE_DELETE(pField);
			return nullptr;
		}

		// �e�N�X�`����������ݒ�
		pField->SetTexPattern(rTexPart);

		// �m�ۂ����A�h���X��Ԃ�
		return pField;
	}
}

//============================================================
//	���X�g�擾����
//============================================================
CListManager<CField>* CField::GetList()
{
	// �I�u�W�F�N�g���X�g��Ԃ�
	return m_pList;
}

//============================================================
//	��ނ̐ݒ菈��
//============================================================
void CField::SetType(const EType type)
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
