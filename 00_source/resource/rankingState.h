//============================================================
//
//	�����L���O��ԃw�b�_�[ [rankingState.h]
//	Author�F���c�E��
//
//============================================================
//************************************************************
//	��d�C���N���[�h�h�~
//************************************************************
#ifndef _RANKING_STATE_H_
#define _RANKING_STATE_H_

//************************************************************
//	�O���錾
//************************************************************
class CRankingManager;	// �����L���O�}�l�[�W���[�N���X

//************************************************************
//	�N���X��`
//************************************************************
// �����L���O��ԃN���X
class CRankingState
{
public:
	// �R���X�g���N�^
	CRankingState();

	// �f�X�g���N�^
	virtual ~CRankingState();

	// �������z�֐�
	virtual HRESULT Init()	= 0;	// ������
	virtual void Uninit()	= 0;	// �I��
	virtual void Update(const float fDeltaTime) = 0;	// �X�V

	// �����o�֐�
	inline void SetContext(CRankingManager* pContext) { m_pContext = pContext; }	// �R���e�L�X�g�ݒ�

protected:
	// �����o�ϐ�
	CRankingManager* m_pContext;	// �R���e�L�X�g
};

//************************************************************
//	�C���N���[�h�t�@�C��
//************************************************************
#include "rankingStateNormal.h"

#endif	// _RANKING_STATE_H_
