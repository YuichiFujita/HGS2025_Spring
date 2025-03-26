//============================================================
//
//	�R���{UI�w�b�_�[ [comboUI.h]
//	Author�F��������
//
//============================================================
//************************************************************
//	��d�C���N���[�h�h�~
//************************************************************
#ifndef _COMBO_UI_H_
#define _COMBO_UI_H_

//************************************************************
//	�C���N���[�h�t�@�C��
//************************************************************
#include "object.h"

//************************************************************
//	�O���錾
//************************************************************
class CMultiValue;	// �}���`�����N���X

//************************************************************
//	�N���X��`
//************************************************************
// �R���{UI�N���X
class CComboUI : public CObject
{
public:

	// �R���X�g���N�^
	CComboUI();

	// �f�X�g���N�^
	~CComboUI() override;

	// �I�[�o�[���C�h�֐�
	HRESULT Init() override;	// ������
	void Uninit() override;		// �I��
	void Update(const float fDeltaTime) override;		// �X�V
	void Draw(CShader* pShader = nullptr) override;		// �`��
	
	void SetData();				// ���̐ݒ菈��
	void ResetCombo();			// �R���{�̃��Z�b�g����
	void SetMultiValue(const float fMulti);		// �{�����l�̐ݒ菈��
	void AddScoreValue(const int nScore);		// �X�R�A���l�̒ǉ�����

	// �ÓI�����o�֐�
	static CComboUI* Create();	// ����

private:

	// �I�[�o�[���C�h�֐�
	inline void Release() override { CObject::Release(); }	// �j��

	// �����o�ϐ�
	CMultiValue* m_pMulti;		// �{���̏��
	CMultiValue* m_pScore;		// �X�R�A�̏��
};

#endif	// _TIME_UI_H_
