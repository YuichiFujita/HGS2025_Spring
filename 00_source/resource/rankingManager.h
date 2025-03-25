//============================================================
//
//	�����L���O�}�l�[�W���[�w�b�_�[ [rankingManager.h]
//	Author�F���c�E��
//
//============================================================
//************************************************************
//	��d�C���N���[�h�h�~
//************************************************************
#ifndef _RANKINGMANAGER_H_
#define _RANKINGMANAGER_H_

//************************************************************
//	�O���錾
//************************************************************
class CRankingState;	// �����L���O��ԃN���X
class CAnim2D;			// �A�j���[�V����2D�N���X
class CObject2D;		// �I�u�W�F�N�g2D�N���X

#ifdef SCORE
class CMultiValue;	// �}���`�����N���X
#else TIMER
class CTimeUI;		// �^�C��UI�N���X
#endif

//************************************************************
//	�N���X��`
//************************************************************
// �����L���O�}�l�[�W���[�N���X
class CRankingManager
{
public:
	// �萔
	static constexpr int MAX_RANK = 5;	// �����L���O��ʕ\����

	// �R���X�g���N�^
	CRankingManager();

	// �f�X�g���N�^
	~CRankingManager();

	// �����o�֐�
	HRESULT Init();	// ������
	void Uninit();	// �I��
	void Update(const float fDeltaTime);		// �X�V
	HRESULT ChangeState(CRankingState* pState);	// ��ԕύX
	void TransTitle();	// �^�C�g����ʑJ��

	// �ÓI�����o�֐�
	static CRankingManager* Create();	// ����
	static void Release(CRankingManager*& prRankingManager);	// �j��

#ifdef SCORE
	static int SetRank(const int nNewScore);	// �����L���O�ݒ�
#else TIMER
	static int SetRank(const float fNewTime);	// �����L���O�ݒ�
#endif

private:
	// �ÓI�����o�֐�
#ifdef SCORE
	static int SortRank(const int nNewScore, int* pRankArray);		// �����L���O�\�[�g
	static HRESULT LoadRank(const char* pPath, int* pRankArray);	// �����L���O�Ǎ�
	static HRESULT SaveRank(const char* pPath, int* pRankArray);	// �����L���O�ۑ�
#else TIMER
	static int SortRank(const float fNewTime, float* pRankArray);	// �����L���O�\�[�g
	static HRESULT LoadRank(const char* pPath, float* pRankArray);	// �����L���O�Ǎ�
	static HRESULT SaveRank(const char* pPath, float* pRankArray);	// �����L���O�ۑ�
#endif

	// �����o�ϐ�
	CAnim2D* m_apRank[MAX_RANK];		// ���ʏ��
#ifdef SCORE
	CMultiValue* m_apScore[MAX_RANK];	// �X�R�A���
#else TIMER
	CTimeUI* m_apTime[MAX_RANK];		// �^�C�����
#endif
	CObject2D* m_pTitle;				// �^�C�g�����
	CRankingState* m_pState;			// ���
};

#endif	// _RANKINGMANAGER_H_
