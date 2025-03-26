//============================================================
//
//	�J�n���o���� [start.cpp]
//	Author�F���c�E��
//
//============================================================
//************************************************************
//	�C���N���[�h�t�@�C��
//************************************************************
#include "start.h"

#include "manager.h"
#include "sound.h"

//************************************************************
//	�萔�錾
//************************************************************
namespace
{
	const char*		TEXTURE = "data\\TEXTURE\\start000.png";	// �e�N�X�`���p�X
	const VECTOR3	POS = SCREEN_CENT;	// �ʒu
	const float		WIDTH = 1020.0f;	// ����
	const float		SCALE_TIME = 0.8f;	// ���o����
	const float		ALPHA_TIME = 2.5f;	// ���o����
}

//************************************************************
//	�q�N���X [CStart] �̃����o�֐�
//************************************************************
//============================================================
//	�R���X�g���N�^
//============================================================
CStart::CStart() :
	m_fStateTime	(0.0f)	// �o�ߎ���
{

}

//============================================================
//	�f�X�g���N�^
//============================================================
CStart::~CStart()
{

}

//============================================================
//	����������
//============================================================
HRESULT CStart::Init()
{
	// �����o�ϐ���������
	m_fStateTime = 0.0f;	// �o�ߎ���

	// �I�u�W�F�N�g2D�̏�����
	if (FAILED(CObject2D::Init()))
	{ // �������Ɏ��s�����ꍇ

		assert(false);
		return E_FAIL;
	}

	// �e�N�X�`���̊���
	BindTexture(TEXTURE);

	// �ʒu�𒆐S�ɂ���
	SetVec3Position(SCREEN_CENT);

	// �傫���̐ݒ�
	SetVec3Size(VECTOR3(0.0f, 0.0f, 0.0f));

	return S_OK;
}

//============================================================
//	�I������
//============================================================
void CStart::Uninit()
{
	// �J�n�̉�
	//PLAY_SOUND(CSound::LABEL_SE_START);

	// �I�u�W�F�N�g2D�̏I��
	CObject2D::Uninit();
}

//============================================================
//	�X�V����
//============================================================
void CStart::Update(const float fDeltaTime)
{
	// �o�ߎ��Ԃ����Z
	m_fStateTime += fDeltaTime;

	// ���`�ۊǂő傫������
	float fWidth = EaseOutBack(0.0f, WIDTH, 0.0f, SCALE_TIME, m_fStateTime);

	// �傫���̐ݒ�
	float fTitleHeight = useful::GetTexHeightFromAspect(fWidth, TEXTURE);	// �e�N�X�`����̏c��
	SetVec3Size(VECTOR3(fWidth, fTitleHeight, 0.0f));

	// ���`�ۊǂœ����x����
	float fAlpha = EasingCubicInOut(1.0f, 0.0f, SCALE_TIME, ALPHA_TIME, m_fStateTime);

	// �����x�̐ݒ�
	SetAlpha(fAlpha);

	// �I�u�W�F�N�g2D�̍X�V
	CObject2D::Update(fDeltaTime);
}

//============================================================
//	�`�揈��
//============================================================
void CStart::Draw(CShader* pShader)
{
	// �I�u�W�F�N�g2D�̕`��
	CObject2D::Draw(pShader);
}

//============================================================
//	��������
//============================================================
CStart* CStart::Create()
{
	// �J�n���o�̐���
	CStart* pStart = new CStart;
	if (pStart == nullptr)
	{ // �����Ɏ��s�����ꍇ

		return nullptr;
	}
	else
	{ // �����ɐ��������ꍇ

		// �J�n���o�̏�����
		if (FAILED(pStart->Init()))
		{ // �������Ɏ��s�����ꍇ

			// �J�n���o�̔j��
			SAFE_DELETE(pStart);
			return nullptr;
		}

		// �m�ۂ����A�h���X��Ԃ�
		return pStart;
	}
}

//============================================================
//	���o�I���t���O�擾����
//============================================================
bool CStart::IsStagEnd() const
{
	// ���Ԃ��o�߂������Ă���ꍇ���o�I��
	return (m_fStateTime >= ALPHA_TIME);
}
