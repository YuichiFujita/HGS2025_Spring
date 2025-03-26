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
class CGameState;		// �Q�[����ԃN���X
class CComboUI;			// �R���{UI
class CBlockFactory;	// �u���b�N�t�@�N�g���[

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
	void Update(const float fDeltaTime);			// �X�V
	HRESULT ChangeState(CGameState* pState);		// ��ԕύX
	void AddScore(const int nAdd);					// �X�R�A���Z
	void TransResult();								// ���U���g��ʑJ��
	bool IsNormal() const;							// �ʏ��Ԋm�F
	void AddBaseScore(const int nAdd);				// ��b�X�R�A���Z
	void InitBaseScore();							// ��b�X�R�A������
	void SetComboValue(const float fMulti);			// �R���{�{���̐ݒ菈��
	void AddComboScore(const int nScore);			// �R���{�X�R�A�̒ǉ�����
	inline int GetBaseScore()						{ return m_nBaseScore; }	// ��b�X�R�A�擾
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
	CGameState* m_pState;			// ���
	CComboUI* m_pComboUI;			// �R���{UI
	CBlockFactory* m_pBlockFactory;	// �u���b�N�t�@�N�g���[
	bool m_bControlOK;				// ����\�t���O
	int m_nBaseScore;				// ��b�X�R�A
};

#endif	// _GAMEMANAGER_H_
