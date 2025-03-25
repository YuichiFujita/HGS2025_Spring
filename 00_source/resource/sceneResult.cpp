//============================================================
//
//	���U���g��ʏ��� [sceneResult.cpp]
//	Author�F���c�E��
//
//============================================================
//************************************************************
//	�C���N���[�h�t�@�C��
//************************************************************
#include "sceneResult.h"
#include "manager.h"
#include "sound.h"
#include "camera.h"
#include "resultManager.h"

//************************************************************
//	�ÓI�����o�ϐ��錾
//************************************************************
CResultManager* CSceneResult::m_pResultManager = nullptr;	// ���U���g�}�l�[�W���[

//************************************************************
//	�q�N���X [CSceneResult] �̃����o�֐�
//************************************************************
//============================================================
//	�R���X�g���N�^
//============================================================
CSceneResult::CSceneResult(const EMode mode) : CScene(mode)
{

}

//============================================================
//	�f�X�g���N�^
//============================================================
CSceneResult::~CSceneResult()
{

}

//============================================================
//	����������
//============================================================
HRESULT CSceneResult::Init()
{
	// �V�[���̏�����
	if (FAILED(CScene::Init()))
	{ // �������Ɏ��s�����ꍇ

		assert(false);
		return E_FAIL;
	}

	// ���U���g�}�l�[�W���[�̐���
	m_pResultManager = CResultManager::Create();
	if (m_pResultManager == nullptr)
	{ // �����Ɏ��s�����ꍇ

		assert(false);
		return E_FAIL;
	}

	// TODO�F�J�����ݒ�
#if 0
	// �Œ�J�����ɂ���
	CCamera* pCamera = GET_MANAGER->GetCamera();	// �J�������
	pCamera->SetState(CCamera::STATE_NONE);			// �Œ��Ԃ�ݒ�
#endif

	// BGM�̍Đ�
	PLAY_SOUND(CSound::LABEL_BGM_GENERAL);

	return S_OK;
}

//============================================================
//	�I������
//============================================================
void CSceneResult::Uninit()
{
	// ���U���g�}�l�[�W���[�̔j��
	SAFE_REF_RELEASE(m_pResultManager);

	// �V�[���̏I��
	CScene::Uninit();
}

//============================================================
//	�X�V����
//============================================================
void CSceneResult::Update(const float fDeltaTime)
{
	// ���U���g�}�l�[�W���[�̍X�V
	assert(m_pResultManager != nullptr);
	m_pResultManager->Update(fDeltaTime);
}

//============================================================
//	���U���g�}�l�[�W���[�擾����
//============================================================
CResultManager* CSceneResult::GetResultManager()
{
	// �C���X�^���X���g�p
	assert(m_pResultManager != nullptr);

	// ���U���g�}�l�[�W���[�̃|�C���^��Ԃ�
	return m_pResultManager;
}
