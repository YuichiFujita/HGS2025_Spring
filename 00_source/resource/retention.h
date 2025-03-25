//============================================================
//
//	�f�[�^�ۑ��w�b�_�[ [retention.h]
//	Author�F���c�E��
//
//============================================================
//************************************************************
//	��d�C���N���[�h�h�~
//************************************************************
#ifndef _RETENTION_H_
#define _RETENTION_H_

//************************************************************
//	�N���X��`
//************************************************************
// �f�[�^�ۑ��N���X
class CRetention
{
public:
	// �R���X�g���N�^
	CRetention();

	// �f�X�g���N�^
	~CRetention();

	// �����o�֐�
	HRESULT Init();	// ������
	void Uninit();	// �I��

#ifdef SCORE
	inline void SetScore(const int nScore)	{ m_nScore = nScore; }	// �N���A�X�R�A�ݒ�
	inline int GetScore() const				{ return m_nScore; }	// �N���A�X�R�A�擾
#else TIMER
	inline void SetTime(const float fTime)	{ m_fTime = fTime; }	// �N���A�^�C���ݒ�
	inline float GetTime() const			{ return m_fTime; }		// �N���A�^�C���擾
#endif
	inline void SetUpdateRankIdx(const int nUpdateIdx)	{ m_nUpdateRankIdx = nUpdateIdx; }	// �����L���O�X�V�C���f�b�N�X�ݒ�
	inline int GetUpdateRankIdx() const					{ return m_nUpdateRankIdx; }		// �����L���O�X�V�C���f�b�N�X�擾

	// �ÓI�����o�֐�
	static CRetention* Create();	// ����
	static void Release(CRetention*& prRetention);	// �j��

private:
	// �����o�ϐ�
#ifdef SCORE
	int m_nScore;			// �N���A�X�R�A
#else TIMER
	float m_fTime;			// �N���A�^�C��
#endif
	int m_nUpdateRankIdx;	// �����L���O�X�V�C���f�b�N�X
};

#endif	// _RETENTION_H_
