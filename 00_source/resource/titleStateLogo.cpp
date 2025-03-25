//============================================================
//
//	���S�\����ԏ��� [titleStateLogo.cpp]
//	Author�F���c�E��
//
//============================================================
//************************************************************
//	�C���N���[�h�t�@�C��
//************************************************************
#include "titleStateLogo.h"
#include "titleManager.h"
#include "manager.h"
#include "fade.h"
#include "object2D.h"

//************************************************************
//	�萔�錾
//************************************************************
namespace
{
	const char* TEXTURE = "data\\TEXTURE\\logo000.png";	// �e�N�X�`���p�X
	const float TRANS_TIME = 6.0f;	// �����J�ڎ���
}

//************************************************************
//	�q�N���X [CTitleStateLogo] �̃����o�֐�
//************************************************************
//============================================================
//	�R���X�g���N�^
//============================================================
CTitleStateLogo::CTitleStateLogo() :
	m_pLogo		(nullptr),	// ���S���
	m_fCurTime	(0.0f)		// ���݂̌o�ߎ���
{

}

//============================================================
//	�f�X�g���N�^
//============================================================
CTitleStateLogo::~CTitleStateLogo()
{

}

//============================================================
//	����������
//============================================================
HRESULT CTitleStateLogo::Init()
{
	// �����o�ϐ���������
	m_pLogo		= nullptr;	// ���S���
	m_fCurTime	= 0.0f;		// ���݂̌o�ߎ���

	// ���S�̐���
	m_pLogo = CObject2D::Create(SCREEN_CENT, SCREEN_SIZE);
	if (m_pLogo == nullptr)
	{ // �����Ɏ��s�����ꍇ

		assert(false);
		return E_FAIL;
	}

	// ���S�e�N�X�`���̊���
	m_pLogo->BindTexture(TEXTURE);

	return S_OK;
}

//============================================================
//	�I������
//============================================================
void CTitleStateLogo::Uninit()
{
	// ���S�̏I��
	SAFE_UNINIT(m_pLogo);

	// ���g�̔j��
	delete this;
}

//============================================================
//	�X�V����
//============================================================
void CTitleStateLogo::Update(const float fDeltaTime)
{
	CFade* pFade = GET_MANAGER->GetFade();	// �t�F�[�h���
	if (!pFade->IsFade())
	{ // �t�F�[�h���Ă��Ȃ��ꍇ

		// ����̍X�V
		UpdateDecide();

		// �^�C�g���J�ڂ̍X�V
		UpdateTransSelect(fDeltaTime);
	}

	// ���S�̍X�V
	if (m_pLogo != nullptr) { m_pLogo->Update(fDeltaTime); }
}

//============================================================
//	����̍X�V����
//============================================================
void CTitleStateLogo::UpdateDecide()
{
	if (input::Decide())
	{
		// �����_���̍쐬
		auto pFunc = [this]()->HRESULT
		{
			// �J�ڐ�I����Ԃɂ���
			return m_pContext->ChangeState(new CTitleStateSelect);
		};

		// �J�ڐ�I����ԂɑJ�ڂ���
		CFade* pFade = GET_MANAGER->GetFade();	// �t�F�[�h���
		pFade->SetFade(CFade::DEF_LEVEL, CFade::DEF_LEVEL, pFunc);
	}
}

//============================================================
//	�^�C�g���J�ڂ̍X�V����
//============================================================
void CTitleStateLogo::UpdateTransSelect(const float fDeltaTime)
{
	// ���݂̌o�ߎ��Ԃ����Z
	m_fCurTime += fDeltaTime;
	if (m_fCurTime >= TRANS_TIME)
	{ // �o�߂����ꍇ

		// �����_���̍쐬
		auto pFunc = [this]()->HRESULT
		{
			// �J�ڐ�I����Ԃɂ���
			return m_pContext->ChangeState(new CTitleStateSelect);
		};

		// �J�ڐ�I����ԂɑJ�ڂ���
		CFade* pFade = GET_MANAGER->GetFade();	// �t�F�[�h���
		pFade->SetFade(CFade::DEF_LEVEL, CFade::DEF_LEVEL, pFunc);
	}
}
