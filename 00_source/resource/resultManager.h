//============================================================
//
//	���U���g�}�l�[�W���[�w�b�_�[ [resultManager.h]
//	Author�F���c�E��
//
//============================================================
//************************************************************
//	��d�C���N���[�h�h�~
//************************************************************
#ifndef _RESULTMANAGER_H_
#define _RESULTMANAGER_H_

//************************************************************
//	�O���錾
//************************************************************
class CResultState;		// ���U���g��ԃN���X
class CObject2D;		// �I�u�W�F�N�g2D�N���X
class CScrollString2D;	// �������蕶����2D�N���X

#ifdef SCORE
class CMultiValue;		// �}���`�����N���X
#else TIMER
class CTimeUI;			// �^�C��UI�N���X
#endif

//************************************************************
//	�N���X��`
//************************************************************
// ���U���g�}�l�[�W���[�N���X
class CResultManager
{
public:
	// �R���X�g���N�^
	CResultManager();

	// �f�X�g���N�^
	~CResultManager();

	// �����o�֐�
	HRESULT Init();	// ������
	void Uninit();	// �I��
	void Update(const float fDeltaTime);		// �X�V
	HRESULT ChangeState(CResultState* pState);	// ��ԕύX
	void TransGame();		// �Q�[����ʑJ��
	void TransRanking();	// �����L���O��ʑJ��

	// �ÓI�����o�֐�
	static CResultManager* Create();	// ����
	static void Release(CResultManager*& prResultManager);	// �j��

private:
	// �I���
	enum ESelect
	{
		SELECT_CONTINUE = 0,	// �R���e�j���[
		SELECT_RANKING,			// �����L���O
		SELECT_MAX				// ���̗񋓌^�̑���
	};

	// �����o�֐�
	void UpdateSelect();		// �I���X�V
	void UpdateDecide();		// ����X�V
	bool IsEndScroll() const;	// ��������I���t���O�擾

	// �����o�ϐ�
	CObject2D* m_apSelect[SELECT_MAX];	// �I�������
	CScrollString2D* m_pUpdateRank;		// �����L���O�X�V���
	CObject2D* m_pContinue;	// �R���e�j���[���
#ifdef SCORE
	CMultiValue* m_pScore;	// �X�R�A���
#else TIMER
	CTimeUI* m_pTime;		// �^�C�����
#endif
	CObject2D* m_pTitle;	// �^�C�g�����
	CResultState* m_pState;	// ���
	int m_nCurSelect;		// ���݂̑I����
	int m_nOldSelect;		// �O��̑I����
};

#endif	// _RESULTMANAGER_H_
