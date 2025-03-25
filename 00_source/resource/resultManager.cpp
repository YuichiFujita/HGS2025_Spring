//============================================================
//
//	���U���g�}�l�[�W���[���� [resultManager.cpp]
//	Author�F���c�E��
//
//============================================================
//************************************************************
//	�C���N���[�h�t�@�C��
//************************************************************
#include "resultManager.h"
#include "resultState.h"
#include "manager.h"
#include "retention.h"
#include "object2D.h"
#include "scrollString2D.h"

#ifdef SCORE
#include "multiValue.h"
#else TIMER
#include "timeUI.h"
#endif

//************************************************************
//	�萔�錾
//************************************************************
namespace
{
	namespace title
	{
		const char*		TEXTURE	= "data\\TEXTURE\\timer_title000.png";	// �e�N�X�`���p�X
		const VECTOR3	POS		= VECTOR3(SCREEN_CENT.x, 120.0f, 0.0f);	// �ʒu
		const float		HEIGHT	= 120.0f;	// �c��
	}

	namespace rank
	{
		const char*	FONT = "data\\FONT\\JF�h�b�g���_�S�V�b�N14.ttf";	// �t�H���g�p�X
		const int	PRIORITY	= 6;			// �e�L�X�g�̗D�揇��
		const bool	ITALIC		= false;		// �C�^���b�N
		const float	HEIGHT		= 82.0f;		// �����c��
		const float	WAIT_TIME	= 0.15f;		// �����\���̑ҋ@����
		const EAlignX ALIGN_X	= XALIGN_LEFT;	// ���z�u
		const EAlignY ALIGN_Y	= YALIGN_TOP;	// �c�z�u
		const VECTOR3 POS		= VECTOR3(500.0f, 305.0f, 0.0f);	// �ʒu
	}

	namespace cont
	{
		const char*		TEXTURE	= "data\\TEXTURE\\continue000.png";		// �e�N�X�`���p�X
		const VECTOR3	POS		= VECTOR3(SCREEN_CENT.x, 445.0f, 0.0f);	// �ʒu
		const float		WIDTH	= 700.0f;	// ����
	}

	namespace select
	{
		const char* TEXTURE[] =	// �e�N�X�`���p�X
		{
			"data\\TEXTURE\\continueSelect000.png",	// YES
			"data\\TEXTURE\\continueSelect001.png",	// NO
		};
		const VECTOR3	POS		= VECTOR3(SCREEN_CENT.x - 160.0f, 585.0f, 0.0f);	// �ʒu
		const VECTOR3	OFFSET	= VECTOR3(160.0f * 2.0f, 0.0f, 0.0f);	// �I�t�Z�b�g
		const float		WIDTH	= 340.0f;				// ����
		const COLOR		COL_DEFAULT	= color::White();	// �ʏ�F
		const COLOR		COL_CHOICE	= color::Yellow();	// �I��F
	}

#ifdef SCORE
	namespace score
	{
		const CValue::EType TYPE = CValue::TYPE_NORMAL;	// �������
		const float DIGIT	= 8;						// �X�R�A����
		const VECTOR3 POS	= VECTOR3(SCREEN_CENT.x, 240.0f, 0.0f);	// �X�R�A�ʒu
		const VECTOR3 SIZE	= VECTOR3(52.8f, 62.4f, 0.0f) * 1.4f;	// �X�R�A�����傫��
		const VECTOR3 SPACE	= VECTOR3(SIZE.x * 0.85f, 0.0f, 0.0f);	// �X�R�A������
	}
#else TIMER
	namespace time
	{
		const CValue::EType TYPE = CValue::TYPE_NORMAL;	// �������
		const VECTOR3 POS		 = VECTOR3(SCREEN_CENT.x, 240.0f, 0.0f);		// �^�C�}�[�ʒu
		const VECTOR3 VAL_SIZE	 = VECTOR3(52.8f, 62.4f, 0.0f) * 1.4f * 1.6f;			// �^�C�}�[�����傫��
		const VECTOR3 PART_SIZE	 = VECTOR3(27.3f, 62.4f, 0.0f) * 1.2f * 1.6f;			// �^�C�}�[��؂�傫��
		const VECTOR3 VAL_SPACE	 = VECTOR3(VAL_SIZE.x * 0.85f, 0.0f, 0.0f);		// �^�C�}�[������
		const VECTOR3 PART_SPACE = VECTOR3(PART_SIZE.x * 0.85f, 0.0f, 0.0f);	// �^�C�}�[��؂��
	}
#endif
}

//************************************************************
//	�e�N���X [CResultManager] �̃����o�֐�
//************************************************************
//============================================================
//	�R���X�g���N�^
//============================================================
CResultManager::CResultManager() :
	m_pUpdateRank	(nullptr),	// �����L���O�X�V���
	m_pContinue		(nullptr),	// �R���e�j���[���
#ifdef SCORE
	m_pScore		(nullptr),	// �X�R�A���
#else TIMER
	m_pTime			(nullptr),	// �^�C�����
#endif
	m_pTitle		(nullptr),	// �^�C�g�����
	m_pState		(nullptr),	// ���
	m_nCurSelect	(0),		// ���݂̑I����
	m_nOldSelect	(0)			// �O��̑I����
{
	// �����o�ϐ����N���A
	memset(&m_apSelect[0], 0, sizeof(m_apSelect));	// �I�������
}

//============================================================
//	�f�X�g���N�^
//============================================================
CResultManager::~CResultManager()
{

}

//============================================================
//	����������
//============================================================
HRESULT CResultManager::Init()
{
	CRetention* pRetention = GET_MANAGER->GetRetention();	// �f�[�^�ۑ��}�l�[�W���[

	// �����o�ϐ���������
	memset(&m_apSelect[0], 0, sizeof(m_apSelect));	// �I�������
	m_pUpdateRank	= nullptr;	// �����L���O�X�V���
	m_pContinue		= nullptr;	// �R���e�j���[���
#ifdef SCORE
	m_pScore		= nullptr;	// �X�R�A���
#else TIMER
	m_pTime			= nullptr;	// �^�C�����
#endif
	m_pTitle		= nullptr;	// �^�C�g�����
	m_pState		= nullptr;	// ���
	m_nCurSelect	= 0;		// ���݂̑I����
	m_nOldSelect	= 0;		// �O��̑I����

	// �ʏ��Ԃɂ���
	ChangeState(new CResultStateNormal);

	// �^�C�g���̐���
	m_pTitle = CObject2D::Create(title::POS);
	if (m_pTitle == nullptr)
	{ // �����Ɏ��s�����ꍇ

		assert(false);
		return E_FAIL;
	}

	// �������x����ݒ�
	m_pTitle->SetLabel(CObject::LABEL_UI);

	// �^�C�g���e�N�X�`���̊���
	m_pTitle->BindTexture(title::TEXTURE);

	// �^�C�g���傫���̐ݒ�
	float fTitleWidth = useful::GetTexWidthFromAspect(title::HEIGHT, title::TEXTURE);	// �e�N�X�`����̉���
	m_pTitle->SetVec3Size(VECTOR3(fTitleWidth, title::HEIGHT, 0.0f));

#ifdef SCORE
	// �X�R�A�̐���
	m_pScore = CMultiValue::Create
	( // ����
		score::TYPE,			// �������
		pRetention->GetScore(),	// ����
		score::DIGIT,			// ����
		score::POS,				// �ʒu
		score::SIZE,			// �傫��
		score::SPACE			// ��
	);
	if (m_pScore == nullptr)
	{ // �����Ɏ��s�����ꍇ

		assert(false);
		return E_FAIL;
	}
#else TIMER
	// �^�C���̐���
	m_pTime = CTimeUI::Create
	( // ����
		pRetention->GetTime(),	// ����
		time::POS,				// �ʒu
		time::VAL_SIZE,			// �����̑傫��
		time::PART_SIZE,		// ��؂�̑傫��
		time::VAL_SPACE,		// �����̋�
		time::PART_SPACE,		// ��؂�̋�
		time::TYPE				// �������
	);
	if (m_pTime == nullptr)
	{ // �����Ɏ��s�����ꍇ

		assert(false);
		return E_FAIL;
	}
#endif

	const int nUpdateIdx = pRetention->GetUpdateRankIdx();	// �����L���O�X�V�C���f�b�N�X
	if (nUpdateIdx > NONE_IDX)
	{ // �����L���O���X�V����Ă���ꍇ

		// �X�V��������쐬
		const std::string sRank = std::to_string(nUpdateIdx + 1);		// �X�V���ʕ�����
		const std::string sUpdate = "�����L���O" + sRank + "�ʍX�V";	// �X�V������

		// �����L���O�X�V�̐���
		m_pUpdateRank = CScrollString2D::Create
		( // ����
			rank::FONT,			// �t�H���g�p�X
			rank::ITALIC,		// �C�^���b�N
			sUpdate,			// �\��������
			rank::POS,			// ���_�ʒu
			rank::WAIT_TIME,	// �����\���̑ҋ@����
			rank::HEIGHT,		// �����c��
			rank::ALIGN_X,		// ���z�u
			rank::ALIGN_Y		// �c�z�u
		);
		if (m_pUpdateRank == nullptr)
		{ // �����Ɏ��s�����ꍇ

			assert(false);
			return E_FAIL;
		}

		// ����������J�n����
		m_pUpdateRank->SetEnableScroll(true);

		// �������莞�̍Đ�SE��ݒ�
		//m_pUpdateRank->SetScrollSE(CSound::LABEL_SE_TEXT01);

		// �F�����F�ɂ���
		m_pUpdateRank->SetColor(color::Yellow());
	}

	// �R���e�j���[�̐���
	m_pContinue = CObject2D::Create(cont::POS);
	if (m_pContinue == nullptr)
	{ // �����Ɏ��s�����ꍇ

		assert(false);
		return E_FAIL;
	}

	// �R���e�j���[�e�N�X�`���̊���
	m_pContinue->BindTexture(cont::TEXTURE);

	// �R���e�j���[�傫���̐ݒ�
	float fContinueHeight = useful::GetTexHeightFromAspect(cont::WIDTH, cont::TEXTURE);	// �e�N�X�`����̏c��
	m_pContinue->SetVec3Size(VECTOR3(cont::WIDTH, fContinueHeight, 0.0f));

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
void CResultManager::Uninit()
{
	// ��Ԃ̏I��
	SAFE_UNINIT(m_pState);

#ifdef SCORE
	// �X�R�A�̏I��
	//SAFE_UNINIT(m_pScore);
#else TIMER
	// �^�C���̏I��
	//SAFE_UNINIT(m_pTimer);
#endif

	// �����L���O�X�V�̏I��
	SAFE_UNINIT(m_pUpdateRank);

	// �R���e�j���[�̏I��
	SAFE_UNINIT(m_pContinue);

	for (int i = 0; i < SELECT_MAX; i++)
	{ // �I�����̑������J��Ԃ�

		// �I�����̏I��
		SAFE_UNINIT(m_apSelect[i]);
	}
}

//============================================================
//	�X�V����
//============================================================
void CResultManager::Update(const float fDeltaTime)
{
	// ��Ԃ��Ƃ̍X�V
	assert(m_pState != nullptr);
	m_pState->Update(fDeltaTime);

	CFade* pFade = GET_MANAGER->GetFade();	// �t�F�[�h���
	if (!pFade->IsFade())
	{ // �t�F�[�h���Ă��Ȃ��ꍇ

		if (IsEndScroll())
		{ // �������肪�I����Ă���ꍇ

			// �I���̍X�V
			UpdateSelect();

			// ����̍X�V
			UpdateDecide();
		}
	}

	// �����L���O�X�V�̍X�V
	if (m_pUpdateRank != nullptr) { m_pUpdateRank->Update(fDeltaTime); }

	// �R���e�j���[�̍X�V
	if (m_pContinue != nullptr) { m_pContinue->Update(fDeltaTime); }

	for (int i = 0; i < SELECT_MAX; i++)
	{ // �I�����̑������J��Ԃ�

		// �I�����̍X�V
		if (m_apSelect[i] != nullptr) { m_apSelect[i]->Update(fDeltaTime); }
	}

#ifdef SCORE
	// �X�R�A�̍X�V
	//m_pScore->Update(fDeltaTime);
#else TIMER
	// �^�C���̍X�V
	//m_pTime->Update(fDeltaTime);
#endif
}

//============================================================
//	��Ԃ̕ύX����
//============================================================
HRESULT CResultManager::ChangeState(CResultState* pState)
{
	// ��Ԃ̐����Ɏ��s���Ă���ꍇ������
	if (pState == nullptr) { assert(false); return E_FAIL; }

	// ��ԃC���X�^���X���I��
	SAFE_UNINIT(m_pState);

	// ��ԃC���X�^���X��ύX
	assert(m_pState == nullptr);
	m_pState = pState;

	// ��ԂɃR���e�L�X�g��ݒ�
	m_pState->SetContext(this);

	// ��ԃC���X�^���X��������
	if (FAILED(m_pState->Init()))
	{ // �������Ɏ��s�����ꍇ

		assert(false);
		return E_FAIL;
	}

	return S_OK;
}

//============================================================
//	�Q�[����ʑJ�ڏ���
//============================================================
void CResultManager::TransGame()
{
	// �t�F�[�h���̏ꍇ������
	if (GET_MANAGER->GetFade()->IsFade()) { return; }

	// �����L���O�X�V���̏�����
	CRetention* pRetention = GET_MANAGER->GetRetention();	// �f�[�^�ۑ��}�l�[�W���[
	pRetention->SetUpdateRankIdx(NONE_IDX);

	// �Q�[����ʂɑJ�ڂ���
	GET_MANAGER->SetLoadScene(CScene::MODE_GAME);
}

//============================================================
//	�����L���O��ʑJ�ڏ���
//============================================================
void CResultManager::TransRanking()
{
	// �t�F�[�h���̏ꍇ������
	if (GET_MANAGER->GetFade()->IsFade()) { return; }

	// �����L���O��ʂɑJ�ڂ���
	GET_MANAGER->SetLoadScene(CScene::MODE_RANKING);
}

//============================================================
//	��������
//============================================================
CResultManager* CResultManager::Create()
{
	// ���U���g�}�l�[�W���[�̐���
	CResultManager* pResultManager = new CResultManager;
	if (pResultManager == nullptr)
	{ // �����Ɏ��s�����ꍇ

		return nullptr;
	}
	else
	{ // �����ɐ��������ꍇ

		// ���U���g�}�l�[�W���[�̏�����
		if (FAILED(pResultManager->Init()))
		{ // �������Ɏ��s�����ꍇ

			// ���U���g�}�l�[�W���[�̔j��
			SAFE_DELETE(pResultManager);
			return nullptr;
		}

		// �m�ۂ����A�h���X��Ԃ�
		return pResultManager;
	}
}

//============================================================
//	�j������
//============================================================
void CResultManager::Release(CResultManager*& prResultManager)
{
	// ���U���g�}�l�[�W���[�̏I��
	assert(prResultManager != nullptr);
	prResultManager->Uninit();

	// �������J��
	SAFE_DELETE(prResultManager);
}

//============================================================
//	�I���̍X�V����
//============================================================
void CResultManager::UpdateSelect()
{
	// �O��̑I������ۑ�
	m_nOldSelect = m_nCurSelect;

	// �I��������
	CInputKeyboard* pKey = GET_INPUTKEY;	// �L�[�{�[�h���
	if (pKey->IsTrigger(DIK_LEFT))
	{
		// ���ɑI�������炷
		m_nCurSelect = (m_nCurSelect + 1) % SELECT_MAX;
	}
	if (pKey->IsTrigger(DIK_RIGHT))
	{
		// �E�ɑI�������炷
		m_nCurSelect = (m_nCurSelect + (SELECT_MAX - 1)) % SELECT_MAX;
	}

	// �O��̑I��v�f�̐F�𔒐F�ɐݒ�
	m_apSelect[m_nOldSelect]->SetColor(select::COL_DEFAULT);

	// ���݂̑I��v�f�̐F�����F�ɐݒ�
	m_apSelect[m_nCurSelect]->SetColor(select::COL_CHOICE);
}

//============================================================
//	����̍X�V����
//============================================================
void CResultManager::UpdateDecide()
{
	if (input::Decide())
	{
		// �I�����ɉ����đJ�ڐ��ύX
		switch (m_nCurSelect)
		{ // ���݂̑I�������Ƃ̏���
		case SELECT_CONTINUE:
		{
			// �Q�[����ʂɑJ�ڂ���
			TransGame();
			break;
		}
		case SELECT_RANKING:
		{
			// �����L���O��ʂɑJ�ڂ���
			TransRanking();
			break;
		}
		default:
			assert(false);
			break;
		}
	}
}

//============================================================
//	��������I���t���O�̎擾����
//============================================================
bool CResultManager::IsEndScroll() const
{
	// �������������̏ꍇ��������I���ς݂Ƃ���
	if (m_pUpdateRank == nullptr) { return true; }
	
	// ��������I���t���O��Ԃ�
	return m_pUpdateRank->IsEndScroll();
}
