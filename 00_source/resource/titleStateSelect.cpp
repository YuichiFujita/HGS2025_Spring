//============================================================
//
//	�J�ڐ�I����ԏ��� [titleStateSelect.cpp]
//	Author�F���c�E��
//
//============================================================
//************************************************************
//	�C���N���[�h�t�@�C��
//************************************************************
#include "titleStateSelect.h"
#include "titleManager.h"
#include "manager.h"
#include "fade.h"
#include "object2D.h"

//************************************************************
//	�萔�錾
//************************************************************
namespace
{
	const float TRANS_TIME = 6.0f;	// �����J�ڎ���

	namespace title
	{
		const char*		TEXTURE	= "data\\TEXTURE\\title000.png";		// �e�N�X�`���p�X
		const VECTOR3	POS		= VECTOR3(SCREEN_CENT.x, 235.0f, 0.0f);	// �ʒu
		const float		WIDTH	= 720.0f;	// ����
	}

	namespace select
	{
		const char* TEXTURE[] =	// �e�N�X�`���p�X
		{
			"data\\TEXTURE\\titleSelect000.png",	// �J�n
			"data\\TEXTURE\\titleSelect001.png",	// �`���[�g���A��
		};
		const VECTOR3	POS		= VECTOR3(SCREEN_CENT.x - 260.0f, 535.0f, 0.0f);	// �ʒu
		const VECTOR3	OFFSET	= VECTOR3(260.0f * 2.0f, 0.0f, 0.0f);	// �I�t�Z�b�g
		const float		WIDTH	= 480.0f;				// ����
		const COLOR		COL_DEFAULT	= color::White();	// �ʏ�F
		const COLOR		COL_CHOICE	= color::Yellow();	// �I��F
	}
}

//************************************************************
//	�q�N���X [CTitleStateSelect] �̃����o�֐�
//************************************************************
//============================================================
//	�R���X�g���N�^
//============================================================
CTitleStateSelect::CTitleStateSelect() :
	m_pTitle	 (nullptr),	// �^�C�g�����
	m_nCurSelect (0),		// ���݂̑I����
	m_nOldSelect (0),		// �O��̑I����
	m_fCurTime	 (0.0f)		// ���݂̌o�ߎ���
{
	// �����o�ϐ����N���A
	memset(&m_apSelect[0], 0, sizeof(m_apSelect));	// �I�������
}

//============================================================
//	�f�X�g���N�^
//============================================================
CTitleStateSelect::~CTitleStateSelect()
{

}

//============================================================
//	����������
//============================================================
HRESULT CTitleStateSelect::Init()
{
	// �����o�ϐ���������
	memset(&m_apSelect[0], 0, sizeof(m_apSelect));	// �I�������
	m_pTitle	 = nullptr;	// �^�C�g�����
	m_nCurSelect = 0;		// ���݂̑I����
	m_nOldSelect = 0;		// �O��̑I����
	m_fCurTime	 = 0.0f;	// ���݂̌o�ߎ���

	// �^�C�g���̐���
	m_pTitle = CObject2D::Create(title::POS);
	if (m_pTitle == nullptr)
	{ // �����Ɏ��s�����ꍇ

		assert(false);
		return E_FAIL;
	}

	// �^�C�g���e�N�X�`���̊���
	m_pTitle->BindTexture(title::TEXTURE);

	// �^�C�g���傫���̐ݒ�
	float fTitleHeight = useful::GetTexHeightFromAspect(title::WIDTH, title::TEXTURE);	// �e�N�X�`����̏c��
	m_pTitle->SetVec3Size(VECTOR3(title::WIDTH, fTitleHeight, 0.0f));

	for (int i = 0; i < SELECT_MAX; i++)
	{ // �I�����̑������J��Ԃ�

		// �I�����̐���
		m_apSelect[i] = CObject2D::Create(select::POS + (select::OFFSET * (float)i));
		if (m_apSelect[i] == nullptr)
		{ // �����Ɏ��s�����ꍇ

			assert(false);
			return E_FAIL;
		}

		// �I�����e�N�X�`���̊���
		m_apSelect[i]->BindTexture(select::TEXTURE[i]);

		// �I�����傫���̐ݒ�
		float fSelectHeight = useful::GetTexHeightFromAspect(select::WIDTH, select::TEXTURE[i]);	// �e�N�X�`����̏c��
		m_apSelect[i]->SetVec3Size(VECTOR3(select::WIDTH, fSelectHeight, 0.0f));
	}

	return S_OK;
}

//============================================================
//	�I������
//============================================================
void CTitleStateSelect::Uninit()
{
	// �^�C�g���̏I��
	SAFE_UNINIT(m_pTitle);

	for (int i = 0; i < SELECT_MAX; i++)
	{ // �I�����̑������J��Ԃ�

		// �I�����̏I��
		SAFE_UNINIT(m_apSelect[i]);
	}

	// ���g�̔j��
	delete this;
}

//============================================================
//	�X�V����
//============================================================
void CTitleStateSelect::Update(const float fDeltaTime)
{
	CFade* pFade = GET_MANAGER->GetFade();	// �t�F�[�h���
	if (!pFade->IsFade())
	{ // �t�F�[�h���Ă��Ȃ��ꍇ

		// �I���̍X�V
		UpdateSelect();

		// ����̍X�V
		UpdateDecide();

		// ���S�J�ڂ̍X�V
		UpdateTransLogo(fDeltaTime);
	}

	// �^�C�g���̍X�V
	if (m_pTitle != nullptr) { m_pTitle->Update(fDeltaTime); }

	for (int i = 0; i < SELECT_MAX; i++)
	{ // �I�����̑������J��Ԃ�

		// �I�����̍X�V
		if (m_apSelect[i] != nullptr) { m_apSelect[i]->Update(fDeltaTime); }
	}
}

//============================================================
//	�I���̍X�V����
//============================================================
void CTitleStateSelect::UpdateSelect()
{
	// �O��̑I������ۑ�
	m_nOldSelect = m_nCurSelect;

	// �I��������
	CInputKeyboard* pKey = GET_INPUTKEY;	// �L�[�{�[�h���
	if (pKey->IsTrigger(DIK_LEFT))
	{
		// ���ɑI�������炷
		m_nCurSelect = (m_nCurSelect + 1) % SELECT_MAX;

		// �o�ߎ��Ԃ�������
		m_fCurTime = 0.0f;
	}
	if (pKey->IsTrigger(DIK_RIGHT))
	{
		// �E�ɑI�������炷
		m_nCurSelect = (m_nCurSelect + (SELECT_MAX - 1)) % SELECT_MAX;

		// �o�ߎ��Ԃ�������
		m_fCurTime = 0.0f;
	}

	// �O��̑I��v�f�̐F�𔒐F�ɐݒ�
	m_apSelect[m_nOldSelect]->SetColor(select::COL_DEFAULT);

	// ���݂̑I��v�f�̐F�����F�ɐݒ�
	m_apSelect[m_nCurSelect]->SetColor(select::COL_CHOICE);
}

//============================================================
//	����̍X�V����
//============================================================
void CTitleStateSelect::UpdateDecide()
{
	if (input::Decide())
	{
		// �o�ߎ��Ԃ�������
		m_fCurTime = 0.0f;

		// �I�����ɉ����đJ�ڐ��ύX
		switch (m_nCurSelect)
		{ // ���݂̑I�������Ƃ̏���
		case SELECT_START:
		{
			// �Q�[����ʂɑJ�ڂ���
			m_pContext->TransGame();
			break;
		}
		case SELECT_TUTORIAL:
		{
			// �����_���̍쐬
			auto pFunc = [this]()->HRESULT
			{
				// �`���[�g���A���\����Ԃɂ���
				return m_pContext->ChangeState(new CTitleStateTutorial);
			};

			// �J�ڐ�I����ԂɑJ�ڂ���
			CFade* pFade = GET_MANAGER->GetFade();	// �t�F�[�h���
			pFade->SetFade(CFade::DEF_LEVEL, CFade::DEF_LEVEL, pFunc);
			break;
		}
		default:
			assert(false);
			break;
		}
	}
}

//============================================================
//	���S�J�ڂ̍X�V����
//============================================================
void CTitleStateSelect::UpdateTransLogo(const float fDeltaTime)
{
	// ���݂̌o�ߎ��Ԃ����Z
	m_fCurTime += fDeltaTime;
	if (m_fCurTime >= TRANS_TIME)
	{ // �o�߂����ꍇ

		// �����_���̍쐬
		auto pFunc = [this]()->HRESULT
		{
			// ���S�\����Ԃɂ���
			return m_pContext->ChangeState(new CTitleStateLogo);
		};

		// �J�ڐ�I����ԂɑJ�ڂ���
		CFade* pFade = GET_MANAGER->GetFade();	// �t�F�[�h���
		pFade->SetFade(CFade::DEF_LEVEL, CFade::DEF_LEVEL, pFunc);
	}
}
