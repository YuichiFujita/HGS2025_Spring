//============================================================
//
//	���U���g��ԃw�b�_�[ [resultState.h]
//	Author�F���c�E��
//
//============================================================
//************************************************************
//	��d�C���N���[�h�h�~
//************************************************************
#ifndef _RESULT_STATE_H_
#define _RESULT_STATE_H_

//************************************************************
//	�O���錾
//************************************************************
class CResultManager;	// ���U���g�}�l�[�W���[�N���X

//************************************************************
//	�N���X��`
//************************************************************
// ���U���g��ԃN���X
class CResultState
{
public:
	// �R���X�g���N�^
	CResultState();

	// �f�X�g���N�^
	virtual ~CResultState();

	// �������z�֐�
	virtual HRESULT Init()	= 0;	// ������
	virtual void Uninit()	= 0;	// �I��
	virtual void Update(const float fDeltaTime) = 0;	// �X�V

	// �����o�֐�
	inline void SetContext(CResultManager* pContext) { m_pContext = pContext; }	// �R���e�L�X�g�ݒ�

protected:
	// �����o�ϐ�
	CResultManager* m_pContext;	// �R���e�L�X�g
};

//************************************************************
//	�C���N���[�h�t�@�C��
//************************************************************
#include "resultStateNormal.h"

#endif	// _RESULT_STATE_H_
