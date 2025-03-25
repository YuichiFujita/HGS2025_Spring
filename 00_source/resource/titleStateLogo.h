//============================================================
//
//	���S�\����ԃw�b�_�[ [titleStateLogo.h]
//	Author�F���c�E��
//
//============================================================
//************************************************************
//	��d�C���N���[�h�h�~
//************************************************************
#ifndef _TITLE_STATE_LOGO_H_
#define _TITLE_STATE_LOGO_H_

//************************************************************
//	�C���N���[�h�t�@�C��
//************************************************************
#include "titleState.h"

//************************************************************
//	�O���錾
//************************************************************
class CObject2D;	// �I�u�W�F�N�g2D�N���X

//************************************************************
//	�N���X��`
//************************************************************
// ���S�\����ԃN���X
class CTitleStateLogo : public CTitleState
{
public:
	// �R���X�g���N�^
	CTitleStateLogo();

	// �f�X�g���N�^
	~CTitleStateLogo() override;

	// �I�[�o�[���C�h�֐�
	HRESULT Init() override;	// ������
	void Uninit() override;		// �I��
	void Update(const float fDeltaTime) override;	// �X�V

private:
	// �����o�֐�
	void UpdateDecide();	// ����X�V
	void UpdateTransSelect(const float fDeltaTime);	// �^�C�g���J�ڍX�V

	// �����o�ϐ�
	CObject2D* m_pLogo;	// ���S���
	float m_fCurTime;	// ���݂̌o�ߎ���
};

#endif	// _TITLE_STATE_LOGO_H_
