//============================================================
//
//	�Q�[���}�l�[�W���[�w�b�_�[ [gameManager.h]
//	Author�F���c�E��
//
//============================================================
//************************************************************
//	��d�C���N���[�h�h�~
//************************************************************
#ifndef _GAMEMANAGER_H_
#define _GAMEMANAGER_H_

//************************************************************
//	�O���錾
//************************************************************
class CGameState;	// �Q�[����ԃN���X

#ifdef SCORE
class CMultiValue;	// �}���`�����N���X
#else TIMER
class CTimerUI;		// �^�C�}�[UI�N���X
#endif

//************************************************************
//	�N���X��`
//************************************************************
// �Q�[���}�l�[�W���[�N���X
class CGameManager
{
public:
	// �R���X�g���N�^
	CGameManager();

	// �f�X�g���N�^
	~CGameManager();

	// �����o�֐�
	HRESULT Init();	// ������
	void Uninit();	// �I��
	void Update(const float fDeltaTime);		// �X�V
	HRESULT ChangeState(CGameState* pState);	// ��ԕύX
	void TransResult();		// ���U���g��ʑJ��
	bool IsNormal() const;	// �ʏ��Ԋm�F
	inline void SetEnableControlOK(const bool bOK)	{ m_bControlOK = bOK; }		// ����\�t���O�ݒ�
	inline bool IsControlOK() const					{ return m_bControlOK; }	// ����\�t���O�擾

	// �ÓI�����o�֐�
	static CGameManager* Create();	// ����
	static void Release(CGameManager*& prGameManager);	// �j��

private:
	// �����o�ϐ�
#ifdef SCORE
	CMultiValue* m_pScore;	// �X�R�A���
#else TIMER
	CTimerUI* m_pTimer;		// �^�C�}�[���
#endif
	CGameState* m_pState;	// ���
	bool m_bControlOK;		// ����\�t���O
};

#endif	// _GAMEMANAGER_H_
