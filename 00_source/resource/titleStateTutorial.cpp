//============================================================
//
//	�`���[�g���A���\����ԏ��� [titleStateTutorial.cpp]
//	Author�F���c�E��
//
//============================================================
//************************************************************
//	�C���N���[�h�t�@�C��
//************************************************************
#include "titleStateTutorial.h"
#include "titleManager.h"
#include "manager.h"
#include "fade.h"
#include "object2D.h"

//************************************************************
//	�萔�錾
//************************************************************
namespace
{
	const char* TEXTURE = "data\\TEXTURE\\tutorial000.png";	// �e�N�X�`���p�X
}

//************************************************************
//	�q�N���X [CTitleStateTutorial] �̃����o�֐�
//************************************************************
//============================================================
//	�R���X�g���N�^
//============================================================
CTitleStateTutorial::CTitleStateTutorial() :
	m_pTutorial	(nullptr)	// �`���[�g���A�����
{

}

//============================================================
//	�f�X�g���N�^
//============================================================
CTitleStateTutorial::~CTitleStateTutorial()
{

}

//============================================================
//	����������
//============================================================
HRESULT CTitleStateTutorial::Init()
{
	// �����o�ϐ���������
	m_pTutorial = nullptr;	// �`���[�g���A�����

	// �`���[�g���A���̐���
	m_pTutorial = CObject2D::Create(SCREEN_CENT, SCREEN_SIZE);
	if (m_pTutorial == nullptr)
	{ // �����Ɏ��s�����ꍇ

		assert(false);
		return E_FAIL;
	}

	// �`���[�g���A���e�N�X�`���̊���
	m_pTutorial->BindTexture(TEXTURE);

	return S_OK;
}

//============================================================
//	�I������
//============================================================
void CTitleStateTutorial::Uninit()
{
	// �`���[�g���A���̏I��
	SAFE_UNINIT(m_pTutorial);

	// ���g�̔j��
	delete this;
}

//============================================================
//	�X�V����
//============================================================
void CTitleStateTutorial::Update(const float fDeltaTime)
{
	CFade* pFade = GET_MANAGER->GetFade();	// �t�F�[�h���
	if (!pFade->IsFade() && input::Decide())
	{ // �J�ڑ��삪�s��ꂽ�ꍇ

		// �����_���̍쐬
		auto pFunc = [this]()->HRESULT
		{
			// �J�ڐ�I����Ԃɂ���
			return m_pContext->ChangeState(new CTitleStateSelect);
		};

		// �J�ڐ�I����ԂɑJ�ڂ���
		pFade->SetFade(CFade::DEF_LEVEL, CFade::DEF_LEVEL, pFunc);
	}

	// �`���[�g���A���̍X�V
	if (m_pTutorial != nullptr) { m_pTutorial->Update(fDeltaTime); }
}
