//============================================================
//
//	�Q�[���}�l�[�W���[���� [gameManager.cpp]
//	Author�F���c�E��
//
//============================================================
//************************************************************
//	�C���N���[�h�t�@�C��
//************************************************************
#include "gameManager.h"
#include "gameState.h"
#include "manager.h"
#include "retention.h"
#include "rankingManager.h"

#ifdef SCORE
#include "multiValue.h"
#else TIMER
#include "timerUI.h"
#endif

#include "player.h"

//************************************************************
//	�萔�錾
//************************************************************
namespace
{
#ifdef SCORE
	namespace score
	{
		const CValue::EType TYPE = CValue::TYPE_NORMAL;	// �������
		const float DIGIT	= 8;			// �X�R�A����
		const VECTOR3 POS	= SCREEN_CENT;	// �X�R�A�ʒu
		const VECTOR3 SIZE	= VECTOR3(52.8f, 62.4f, 0.0f) * 1.4f;	// �X�R�A�����傫��
		const VECTOR3 SPACE	= VECTOR3(SIZE.x * 0.85f, 0.0f, 0.0f);	// �X�R�A������
	}
#else TIMER
	namespace timer
	{
		const float TIME_START	 = 0.0f;				// �J�n����
		const CValue::EType TYPE = CValue::TYPE_NORMAL;	// �������
		const VECTOR3 POS		 = SCREEN_CENT;			// �^�C�}�[�ʒu
		const VECTOR3 VAL_SIZE	 = VECTOR3(52.8f, 62.4f, 0.0f) * 1.4f;			// �^�C�}�[�����傫��
		const VECTOR3 PART_SIZE	 = VECTOR3(27.3f, 62.4f, 0.0f) * 1.2f;			// �^�C�}�[��؂�傫��
		const VECTOR3 VAL_SPACE	 = VECTOR3(VAL_SIZE.x * 0.85f, 0.0f, 0.0f);		// �^�C�}�[������
		const VECTOR3 PART_SPACE = VECTOR3(PART_SIZE.x * 0.85f, 0.0f, 0.0f);	// �^�C�}�[��؂��
	}
#endif
}

//************************************************************
//	�e�N���X [CGameManager] �̃����o�֐�
//************************************************************
//============================================================
//	�R���X�g���N�^
//============================================================
CGameManager::CGameManager() :
#ifdef SCORE
	m_pScore	 (nullptr),	// �X�R�A���
#else TIMER
	m_pTimer	 (nullptr),	// �^�C�}�[���
#endif
	m_pState	 (nullptr),	// ���
	m_bControlOK (false)	// ����\�t���O
{

}

//============================================================
//	�f�X�g���N�^
//============================================================
CGameManager::~CGameManager()
{

}

//============================================================
//	����������
//============================================================
HRESULT CGameManager::Init()
{
	// �����o�ϐ���������
#ifdef SCORE
	m_pScore	 = nullptr;	// �X�R�A���
#else TIMER
	m_pTimer	 = nullptr;	// �^�C�}�[���
#endif
	m_pState	 = nullptr;	// ���
	m_bControlOK = true;	// ����\�t���O

	// �ʏ��Ԃɂ���
	ChangeState(new CGameStateNormal);

#ifdef SCORE
	// �X�R�A�̐���
	m_pScore = CMultiValue::Create
	( // ����
		score::TYPE,	// �������
		0,				// ����
		score::DIGIT,	// ����
		score::POS,		// �ʒu
		score::SIZE,	// �傫��
		score::SPACE	// ��
	);
	if (m_pScore == nullptr)
	{ // �����Ɏ��s�����ꍇ

		assert(false);
		return E_FAIL;
	}
#else TIMER
	// �^�C�}�[�̐���
	m_pTimer = CTimerUI::Create
	( // ����
		timer::TIME_START,	// �J�n����
		0.0f,				// ��������
		timer::POS,			// �ʒu
		timer::VAL_SIZE,	// �����̑傫��
		timer::PART_SIZE,	// ��؂�̑傫��
		timer::VAL_SPACE,	// �����̋�
		timer::PART_SPACE,	// ��؂�̋�
		timer::TYPE			// �������
	);
	if (m_pTimer == nullptr)
	{ // �����Ɏ��s�����ꍇ

		assert(false);
		return E_FAIL;
	}

	// �^�C�}�[�̌v���J�n
	m_pTimer->Start();
#endif

	// �v���C���[�̐���
	if (CPlayer::Create(VEC3_ZERO, VEC3_ZERO) == nullptr)
	{ // �����Ɏ��s�����ꍇ

		assert(false);
		return E_FAIL;
	}

	return S_OK;
}

//============================================================
//	�I������
//============================================================
void CGameManager::Uninit()
{
	// ��Ԃ̏I��
	SAFE_UNINIT(m_pState);

#ifdef SCORE
	// �X�R�A�̏I��
	//SAFE_UNINIT(m_pScore);
#else TIMER
	// �^�C�}�[�̏I��
	//SAFE_UNINIT(m_pTimer);
#endif
}

//============================================================
//	�X�V����
//============================================================
void CGameManager::Update(const float fDeltaTime)
{
	// ��Ԃ��Ƃ̍X�V
	assert(m_pState != nullptr);
	m_pState->Update(fDeltaTime);

#ifdef SCORE
#ifdef _DEBUG
	// TODO�F�X�R�A���Z
	m_pScore->AddNum((int)(100.0f * fDeltaTime));
#endif
	// �X�R�A�̍X�V
	//m_pScore->Update(fDeltaTime);
#else TIMER
	// �^�C�}�[�̍X�V
	//m_pTimer->Update(fDeltaTime);
#endif
}

//============================================================
//	��Ԃ̕ύX����
//============================================================
HRESULT CGameManager::ChangeState(CGameState* pState)
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
//	���U���g��ʑJ�ڏ���
//============================================================
void CGameManager::TransResult()
{
	CRetention* pRetention = GET_MANAGER->GetRetention();	// �f�[�^�ۑ��}�l�[�W���[

	// �t�F�[�h���̏ꍇ������
	if (GET_MANAGER->GetFade()->IsFade()) { return; }

#ifdef SCORE
	// �^�C���̎擾
	int nScore = m_pScore->GetNum();

	// �X�R�A�̕ۑ�
	pRetention->SetScore(nScore);

	// �����L���O�̍X�V
	pRetention->SetUpdateRankIdx(CRankingManager::SetRank(nScore));	// �����L���O�X�V�C���f�b�N�X�̕ۑ�
#else TIMER
	// �^�C�}�[�̌v���I��
	m_pTimer->End();

	// �^�C���̎擾
	float fTime = m_pTimer->GetTime();

	// �^�C���̕ۑ�
	pRetention->SetTime(fTime);

	// �����L���O�̍X�V
	pRetention->SetUpdateRankIdx(CRankingManager::SetRank(fTime));	// �����L���O�X�V�C���f�b�N�X�̕ۑ�
#endif

	// ���U���g��ʂɑJ�ڂ���
	GET_MANAGER->SetLoadScene(CScene::MODE_RESULT);
}

//============================================================
//	�ʏ��Ԃ��̊m�F����
//============================================================
bool CGameManager::IsNormal() const
{
	// �ʏ��Ԃ��𔻒肵�Ԃ�
	return (typeid(*m_pState) == typeid(CGameStateNormal));
}

//============================================================
//	��������
//============================================================
CGameManager* CGameManager::Create()
{
	// �Q�[���}�l�[�W���[�̐���
	CGameManager* pGameManager = new CGameManager;
	if (pGameManager == nullptr)
	{ // �����Ɏ��s�����ꍇ

		return nullptr;
	}
	else
	{ // �����ɐ��������ꍇ

		// �Q�[���}�l�[�W���[�̏�����
		if (FAILED(pGameManager->Init()))
		{ // �������Ɏ��s�����ꍇ

			// �Q�[���}�l�[�W���[�̔j��
			SAFE_DELETE(pGameManager);
			return nullptr;
		}

		// �m�ۂ����A�h���X��Ԃ�
		return pGameManager;
	}
}

//============================================================
//	�j������
//============================================================
void CGameManager::Release(CGameManager*& prGameManager)
{
	// �Q�[���}�l�[�W���[�̏I��
	assert(prGameManager != nullptr);
	prGameManager->Uninit();

	// �������J��
	SAFE_DELETE(prGameManager);
}
