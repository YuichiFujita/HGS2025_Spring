//============================================================
//
//	�ʏ��ԃw�b�_�[ [rankingStateNormal.h]
//	Author�F���c�E��
//
//============================================================
//************************************************************
//	��d�C���N���[�h�h�~
//************************************************************
#ifndef _RANKING_STATE_NORMAL_H_
#define _RANKING_STATE_NORMAL_H_

//************************************************************
//	�C���N���[�h�t�@�C��
//************************************************************
#include "rankingState.h"

//************************************************************
//	�N���X��`
//************************************************************
// �ʏ��ԃN���X
class CRankingStateNormal : public CRankingState
{
public:
	// �R���X�g���N�^
	CRankingStateNormal();

	// �f�X�g���N�^
	~CRankingStateNormal() override;

	// �I�[�o�[���C�h�֐�
	HRESULT Init() override;	// ������
	void Uninit() override;		// �I��
	void Update(const float fDeltaTime) override;	// �X�V
};

#endif	// _RANKING_STATE_NORMAL_H_
