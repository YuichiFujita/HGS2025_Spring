//============================================================
//
//	�ʏ��ԃw�b�_�[ [resultStateNormal.h]
//	Author�F���c�E��
//
//============================================================
//************************************************************
//	��d�C���N���[�h�h�~
//************************************************************
#ifndef _RESULT_STATE_NORMAL_H_
#define _RESULT_STATE_NORMAL_H_

//************************************************************
//	�C���N���[�h�t�@�C��
//************************************************************
#include "resultState.h"

//************************************************************
//	�N���X��`
//************************************************************
// �ʏ��ԃN���X
class CResultStateNormal : public CResultState
{
public:
	// �R���X�g���N�^
	CResultStateNormal();

	// �f�X�g���N�^
	~CResultStateNormal() override;

	// �I�[�o�[���C�h�֐�
	HRESULT Init() override;	// ������
	void Uninit() override;		// �I��
	void Update(const float fDeltaTime) override;	// �X�V
};

#endif	// _RESULT_STATE_NORMAL_H_
