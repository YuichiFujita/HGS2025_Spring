//============================================================
//
//	�󏈗� [sky.cpp]
//	Author�F���c�E��
//
//============================================================
//************************************************************
//	�C���N���[�h�t�@�C��
//************************************************************
#include "sky.h"
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
		"data\\TEXTURE\\sky000.png",	// �ʏ�e�N�X�`��
	};

	const int	PRIORITY	= 0;		// ��̗D�揇��
	const float	SKY_ROTMOVE	= 0.001f;	// �����ύX��
}

//************************************************************
//	�X�^�e�B�b�N�A�T�[�g
//************************************************************
static_assert(NUM_ARRAY(TEXTURE_FILE) == CSky::TYPE_MAX, "ERROR : Type Count Mismatch");

//************************************************************
//	�ÓI�����o�ϐ��錾
//************************************************************
CListManager<CSky>* CSky::m_pList = nullptr;	// �I�u�W�F�N�g���X�g

//************************************************************
//	�q�N���X [CSky] �̃����o�֐�
//************************************************************
//============================================================
//	�R���X�g���N�^
//============================================================
CSky::CSky() : CObjectMeshDome(CObject::LABEL_SKY, CObject::DIM_3D, PRIORITY),
	m_type	(TYPE_NORMAL)	// ���
{

}

//============================================================
//	�f�X�g���N�^
//============================================================
CSky::~CSky()
{

}

//============================================================
//	����������
//============================================================
HRESULT CSky::Init()
{
	// �����o�ϐ���������
	m_type = TYPE_NORMAL;	// ���

	// �I�u�W�F�N�g���b�V���h�[���̏�����
	if (FAILED(CObjectMeshDome::Init()))
	{ // �������Ɏ��s�����ꍇ

		// ���s��Ԃ�
		assert(false);
		return E_FAIL;
	}

	// �e�N�X�`��������ݒ�
	SetTexDir(CObjectMeshDome::TEXDIR_INSIDE);

	// �����_�[�X�e�[�g�̏����擾
	CRenderState *pRenderState = GetRenderState();

	// �|���S���̗��ʂ݂̂�\����Ԃɂ���
	pRenderState->SetCulling(D3DCULL_CW);

	// ���C�e�B���O��OFF�ɂ���
	pRenderState->SetLighting(false);

	if (m_pList == nullptr)
	{ // ���X�g�}�l�[�W���[�����݂��Ȃ��ꍇ

		// ���X�g�}�l�[�W���[�̐���
		m_pList = CListManager<CSky>::Create();
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
void CSky::Uninit()
{
	// ���X�g���玩�g�̃I�u�W�F�N�g���폜
	m_pList->DelList(m_iterator);

	if (m_pList->GetNumAll() == 0)
	{ // �I�u�W�F�N�g������Ȃ��ꍇ

		// ���X�g�}�l�[�W���[�̔j��
		m_pList->Release(m_pList);
	}

	// �I�u�W�F�N�g���b�V���h�[���̏I��
	CObjectMeshDome::Uninit();
}

//============================================================
//	�X�V����
//============================================================
void CSky::Update(const float fDeltaTime)
{
	VECTOR3 rot = GetVec3Rotation();	// ����

	// ������ύX
	rot.y -= SKY_ROTMOVE;

	// ������ݒ�
	SetVec3Rotation(rot);

	// �I�u�W�F�N�g���b�V���h�[���̍X�V
	CObjectMeshDome::Update(fDeltaTime);
}

//============================================================
//	�`�揈��
//============================================================
void CSky::Draw(CShader* pShader)
{
	// �I�u�W�F�N�g���b�V���h�[���̕`��
	CObjectMeshDome::Draw(pShader);
}

//============================================================
//	��������
//============================================================
CSky* CSky::Create
(
	const EType type,			// ���
	const VECTOR3& rPos,	// �ʒu
	const VECTOR3& rRot,	// ����
	const COLOR& rCol,		// �F
	const POSGRID2& rPart,		// ������
	const POSGRID2& rTexPart,	// �e�N�X�`��������
	const float fRadius			// ���a
)
{
	// ��̐���
	CSky* pSky = new CSky;
	if (pSky == nullptr)
	{ // �����Ɏ��s�����ꍇ

		return nullptr;
	}
	else
	{ // �����ɐ��������ꍇ

		// ��̏�����
		if (FAILED(pSky->Init()))
		{ // �������Ɏ��s�����ꍇ

			// ��̔j��
			SAFE_DELETE(pSky);
			return nullptr;
		}

		// ��ނ�ݒ�
		pSky->SetType(type);

		// �ʒu��ݒ�
		pSky->SetVec3Position(rPos);

		// ������ݒ�
		pSky->SetVec3Rotation(rRot);

		// �F��ݒ�
		pSky->SetColor(rCol);

		// ���a��ݒ�
		pSky->SetRadius(fRadius);

		// ��������ݒ�
		if (FAILED(pSky->SetPattern(rPart)))
		{ // �������̐ݒ�Ɏ��s�����ꍇ

			// ��̔j��
			SAFE_DELETE(pSky);
			return nullptr;
		}

		// �e�N�X�`����������ݒ�
		pSky->SetTexPattern(rTexPart);

		// �m�ۂ����A�h���X��Ԃ�
		return pSky;
	}
}

//============================================================
//	���X�g�擾����
//============================================================
CListManager<CSky>* CSky::GetList()
{
	// �I�u�W�F�N�g���X�g��Ԃ�
	return m_pList;
}

//============================================================
//	��ނ̐ݒ菈��
//============================================================
void CSky::SetType(const EType type)
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
