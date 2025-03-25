//============================================================
//
//	�Q�[����ʏ��� [sceneGame.cpp]
//	Author�F���c�E��
//
//============================================================
//************************************************************
//	�C���N���[�h�t�@�C��
//************************************************************
#include "sceneGame.h"
#include "manager.h"
#include "sound.h"
#include "camera.h"
#include "gameManager.h"
#include "pause.h"
#include "hitStop.h"

//************************************************************
//	�ÓI�����o�ϐ��錾
//************************************************************
CGameManager* CSceneGame::m_pGameManager = nullptr;	// �Q�[���}�l�[�W���[
CPause* CSceneGame::m_pPause = nullptr;		// �|�[�Y���
CHitStop* CSceneGame::m_pHitStop = nullptr;	// �q�b�g�X�g�b�v���

//************************************************************
//	�q�N���X [CSceneGame] �̃����o�֐�
//************************************************************
//============================================================
//	�R���X�g���N�^
//============================================================
CSceneGame::CSceneGame(const EMode mode) : CScene(mode)
{

}

//============================================================
//	�f�X�g���N�^
//============================================================
CSceneGame::~CSceneGame()
{

}

//============================================================
//	����������
//============================================================
HRESULT CSceneGame::Init()
{
	//--------------------------------------------------------
	//	��������
	//--------------------------------------------------------
	// �V�[���̏�����
	if (FAILED(CScene::Init()))
	{ // �������Ɏ��s�����ꍇ

		assert(false);
		return E_FAIL;
	}

	// �Q�[���}�l�[�W���[�̐���
	m_pGameManager = CGameManager::Create();
	if (m_pGameManager == nullptr)
	{ // �����Ɏ��s�����ꍇ

		assert(false);
		return E_FAIL;
	}

	// �|�[�Y�̐���
	m_pPause = CPause::Create();
	if (m_pPause == nullptr)
	{ // �����Ɏ��s�����ꍇ

		assert(false);
		return E_FAIL;
	}

	// �q�b�g�X�g�b�v�̐���
	m_pHitStop = CHitStop::Create();
	if (m_pHitStop == nullptr)
	{ // �����Ɏ��s�����ꍇ

		assert(false);
		return E_FAIL;
	}

	// TODO�F�J�����ݒ�
#if 0
	// �Ǐ]�J�����ɂ���
	CCamera* pCamera = GET_MANAGER->GetCamera();	// �J�������
	pCamera->SetState(CCamera::STATE_FOLLOW);		// �Ǐ]��Ԃ�ݒ�
#endif

	// BGM�̍Đ�
	PLAY_SOUND(CSound::LABEL_BGM_GENERAL);

	return S_OK;
}

//============================================================
//	�I������
//============================================================
void CSceneGame::Uninit()
{
	// �Q�[���}�l�[�W���[�̔j��
	SAFE_REF_RELEASE(m_pGameManager);

	// �|�[�Y�̔j��
	SAFE_REF_RELEASE(m_pPause);

	// �q�b�g�X�g�b�v�̔j��
	SAFE_REF_RELEASE(m_pHitStop);

	// �V�[���̏I��
	CScene::Uninit();
}

//============================================================
//	�X�V����
//============================================================
void CSceneGame::Update(const float fDeltaTime)
{
	// �t�F�[�h���̏ꍇ������
	if (GET_MANAGER->GetFade()->IsFade()) { return; }

	// �Q�[���}�l�[�W���[�̍X�V
	assert(m_pGameManager != nullptr);
	m_pGameManager->Update(fDeltaTime);

	// �q�b�g�X�g�b�v�̍X�V
	assert(m_pHitStop != nullptr);
	m_pHitStop->Update(fDeltaTime);

	if (m_pGameManager->IsNormal())
	{ // �Q�[�����ʏ��Ԃ̏ꍇ

		// �|�[�Y�̍X�V
		assert(m_pPause != nullptr);
		m_pPause->Update(fDeltaTime);
	}

	if (!m_pPause->IsPause()
	&&  !m_pHitStop->IsStop())
	{ // �|�[�Y���E�q�b�g�X�g�b�v���ł͂Ȃ��ꍇ

		// �V�[���̍X�V
		CScene::Update(fDeltaTime);
	}
	else if (m_pHitStop->IsStop())
	{ // �q�b�g�X�g�b�v���̏ꍇ

		// �J�����̍X�V
		GET_MANAGER->GetCamera()->Update(fDeltaTime);
	}

#ifdef _DEBUG
	else
	{ // �|�[�Y���̏ꍇ

		if (GET_MANAGER->GetCamera()->GetState() == CCamera::STATE_CONTROL)
		{ // �J�����������Ԃ̏ꍇ

			// �J�����̍X�V
			GET_MANAGER->GetCamera()->Update(fDeltaTime);
		}
	}
#endif	// _DEBUG
}

//============================================================
//	�Q�[���}�l�[�W���[�擾����
//============================================================
CGameManager* CSceneGame::GetGameManager()
{
	// �C���X�^���X���g�p
	assert(m_pGameManager != nullptr);

	// �Q�[���}�l�[�W���[�̃|�C���^��Ԃ�
	return m_pGameManager;
}

//============================================================
//	�|�[�Y�擾����
//============================================================
CPause* CSceneGame::GetPause()
{
	// �C���X�^���X���g�p
	assert(m_pPause != nullptr);

	// �|�[�Y�̃|�C���^��Ԃ�
	return m_pPause;
}

//============================================================
//	�q�b�g�X�g�b�v�擾����
//============================================================
CHitStop* CSceneGame::GetHitStop()
{
	// �C���X�^���X���g�p
	assert(m_pHitStop != nullptr);

	// �q�b�g�X�g�b�v�̃|�C���^��Ԃ�
	return m_pHitStop;
}
