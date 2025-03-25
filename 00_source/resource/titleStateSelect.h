//============================================================
//
//	�J�ڐ�I����ԃw�b�_�[ [titleStateSelect.h]
//	Author�F���c�E��
//
//============================================================
//************************************************************
//	��d�C���N���[�h�h�~
//************************************************************
#ifndef _TITLE_STATE_SELECT_H_
#define _TITLE_STATE_SELECT_H_

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
// �J�ڐ�I����ԃN���X
class CTitleStateSelect : public CTitleState
{
public:
	// �R���X�g���N�^
	CTitleStateSelect();

	// �f�X�g���N�^
	~CTitleStateSelect() override;

	// �I�[�o�[���C�h�֐�
	HRESULT Init() override;	// ������
	void Uninit() override;		// �I��
	void Update(const float fDeltaTime) override;	// �X�V

private:
	// �I���
	enum ESelect
	{
		SELECT_START = 0,	// �J�n
		SELECT_TUTORIAL,	// �`���[�g���A��
		SELECT_MAX			// ���̗񋓌^�̑���
	};

	// �����o�֐�
	void UpdateSelect();	// �I���X�V
	void UpdateDecide();	// ����X�V
	void UpdateTransLogo(const float fDeltaTime);	// ���S�J�ڍX�V

	// �����o�ϐ�
	CObject2D* m_apSelect[SELECT_MAX];	// �I�������
	CObject2D* m_pTitle;	// �^�C�g�����
	int m_nCurSelect;		// ���݂̑I����
	int m_nOldSelect;		// �O��̑I����
	float m_fCurTime;		// ���݂̌o�ߎ���
};

#endif	// _TITLE_STATE_SELECT_H_
