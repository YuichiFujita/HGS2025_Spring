//============================================================
//
//	�i�F���� [scenery.cpp]
//	Author�F���c�E��
//
//============================================================
//************************************************************
//	�C���N���[�h�t�@�C��
//************************************************************
#include "scenery.h"
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
		"data\\TEXTURE\\scenery000.png",	// �R�e�N�X�`��
		"data\\TEXTURE\\scenery001.png",	// �R�e�N�X�`��
		"data\\TEXTURE\\scenery002.png",	// �R�e�N�X�`��
	};

	const int PRIORITY		= 0;	// �i�F�̗D�揇��
	const int ALPHA_NUMREF	= 180;	// ���e�X�g�̎Q�ƒl
}

//************************************************************
//	�X�^�e�B�b�N�A�T�[�g
//************************************************************
static_assert(NUM_ARRAY(TEXTURE_FILE) == CScenery::TYPE_MAX, "ERROR : Type Count Mismatch");

//************************************************************
//	�ÓI�����o�ϐ��錾
//************************************************************
CListManager<CScenery>* CScenery::m_pList = nullptr;	// �I�u�W�F�N�g���X�g

//************************************************************
//	�q�N���X [CScenery] �̃����o�֐�
//************************************************************
//============================================================
//	�R���X�g���N�^
//============================================================
CScenery::CScenery() : CObjectMeshCylinder(CObject::LABEL_SCENERY, CObject::DIM_3D, PRIORITY),
	m_type	(TYPE_MOUNTAIN_SMALL_00)	// ���
{

}

//============================================================
//	�f�X�g���N�^
//============================================================
CScenery::~CScenery()
{

}

//============================================================
//	����������
//============================================================
HRESULT CScenery::Init()
{
	// �����o�ϐ���������
	m_type = TYPE_MOUNTAIN_SMALL_00;	// ���

	// �I�u�W�F�N�g���b�V���V�����_�[�̏�����
	if (FAILED(CObjectMeshCylinder::Init()))
	{ // �������Ɏ��s�����ꍇ

		// ���s��Ԃ�
		assert(false);
		return E_FAIL;
	}

	// �e�N�X�`��������ݒ�
	SetTexDir(CObjectMeshCylinder::TEXDIR_INSIDE);

	// �����_�[�X�e�[�g�̏����擾
	CRenderState *pRenderState = GetRenderState();

	// ���e�X�g�̐ݒ�
	pRenderState->SetAlphaTest(true);			// ���e�X�g�̗L�� / �����̐ݒ�
	pRenderState->SetAlphaFunc(D3DCMP_GREATER);	// ���e�X�g�̐ݒ�
	pRenderState->SetAlphaNumRef(ALPHA_NUMREF);	// ���e�X�g�̎Q�ƒl�ݒ�

	// �|���S���̗��ʂ݂̂�\����Ԃɂ���
	pRenderState->SetCulling(D3DCULL_CW);

	// ���C�e�B���O��OFF�ɂ���
	pRenderState->SetLighting(false);

	if (m_pList == nullptr)
	{ // ���X�g�}�l�[�W���[�����݂��Ȃ��ꍇ

		// ���X�g�}�l�[�W���[�̐���
		m_pList = CListManager<CScenery>::Create();
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
void CScenery::Uninit()
{
	// ���X�g���玩�g�̃I�u�W�F�N�g���폜
	m_pList->DelList(m_iterator);

	if (m_pList->GetNumAll() == 0)
	{ // �I�u�W�F�N�g������Ȃ��ꍇ

		// ���X�g�}�l�[�W���[�̔j��
		m_pList->Release(m_pList);
	}

	// �I�u�W�F�N�g���b�V���V�����_�[�̏I��
	CObjectMeshCylinder::Uninit();
}

//============================================================
//	�X�V����
//============================================================
void CScenery::Update(const float fDeltaTime)
{
	// �I�u�W�F�N�g���b�V���V�����_�[�̍X�V
	CObjectMeshCylinder::Update(fDeltaTime);
}

//============================================================
//	�`�揈��
//============================================================
void CScenery::Draw(CShader* pShader)
{
	// �I�u�W�F�N�g���b�V���V�����_�[�̕`��
	CObjectMeshCylinder::Draw(pShader);
}

//============================================================
//	��������
//============================================================
CScenery* CScenery::Create
(
	const EType type,			// ���
	const VECTOR3& rPos,	// �ʒu
	const VECTOR3& rRot,	// ����
	const COLOR& rCol,		// �F
	const POSGRID2& rPart,		// ������
	const POSGRID2& rTexPart,	// �e�N�X�`��������
	const float fRadius,		// ���a
	const float fHeight			// �c��
)
{
	// �i�F�̐���
	CScenery* pScenery = new CScenery;
	if (pScenery == nullptr)
	{ // �����Ɏ��s�����ꍇ

		return nullptr;
	}
	else
	{ // �����ɐ��������ꍇ

		// �i�F�̏�����
		if (FAILED(pScenery->Init()))
		{ // �������Ɏ��s�����ꍇ

			// �i�F�̔j��
			SAFE_DELETE(pScenery);
			return nullptr;
		}

		// ��ނ�ݒ�
		pScenery->SetType(type);

		// �ʒu��ݒ�
		pScenery->SetVec3Position(rPos);

		// ������ݒ�
		pScenery->SetVec3Rotation(rRot);

		// �F��ݒ�
		pScenery->SetColor(rCol);

		// ���a��ݒ�
		pScenery->SetRadius(fRadius);

		// �c����ݒ�
		pScenery->SetHeight(fHeight);

		// ��������ݒ�
		if (FAILED(pScenery->SetPattern(rPart)))
		{ // �������̐ݒ�Ɏ��s�����ꍇ

			// �i�F�̔j��
			SAFE_DELETE(pScenery);
			return nullptr;
		}

		// �e�N�X�`����������ݒ�
		pScenery->SetTexPattern(rTexPart);

		// �m�ۂ����A�h���X��Ԃ�
		return pScenery;
	}
}

//============================================================
//	���X�g�擾����
//============================================================
CListManager<CScenery>* CScenery::GetList()
{
	// �I�u�W�F�N�g���X�g��Ԃ�
	return m_pList;
}

//============================================================
//	��ނ̐ݒ菈��
//============================================================
void CScenery::SetType(const EType type)
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
