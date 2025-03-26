//============================================================
//
//	�R���{�G���A�}�l�[�W���[�w�b�_�[ [comboAreaManager.h]
//	Author�F��������
// 
//============================================================
//************************************************************
//	��d�C���N���[�h�h�~
//************************************************************
#ifndef _COMBO_AREA_MANAGER_H_
#define _COMBO_AREA_MANAGER_H_

//************************************************************
//	�C���N���[�h�t�@�C��
//************************************************************
#include "object.h"

//************************************************************
//	�N���X��`
//************************************************************
// �R���{�G���A�}�l�[�W���[�N���X
class CComboAreaManager : public CObject
{
public:

	// �R���X�g���N�^
	CComboAreaManager();

	// �f�X�g���N�^
	~CComboAreaManager() override;

	// �I�[�o�[���C�h�֐�
	HRESULT Init() override;	// ������
	void Uninit() override;		// �I��
	void Update(const float fDeltaTime) override;		// �X�V
	void Draw(CShader* pShader = nullptr) override;		// �`��

	// �ÓI�����o�֐�
	static CComboAreaManager* Create();		// ����

private:

	// �I�[�o�[���C�h�֐�
	inline void Release() override { CObject::Release(); }	// �j��
};

#endif	// _COMBO_AREA_MANAGER_H_
