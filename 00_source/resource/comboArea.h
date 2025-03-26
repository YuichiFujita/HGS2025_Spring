//============================================================
//
//	�R���{�G���A�w�b�_�[ [comboArea.h]
//	Author�F��������
// 
//============================================================
//************************************************************
//	��d�C���N���[�h�h�~
//************************************************************
#ifndef _COMBO_AREA_H_
#define _COMBO_AREA_H_

//************************************************************
//	�C���N���[�h�t�@�C��
//************************************************************
#include "object3D.h"

//************************************************************
//	�N���X��`
//************************************************************
// �R���{�G���A�N���X
class CComboArea : public CObject3D
{
public:

	// �R���X�g���N�^
	CComboArea();

	// �f�X�g���N�^
	~CComboArea() override;

	// �I�[�o�[���C�h�֐�
	HRESULT Init() override;	// ������
	void Uninit() override;		// �I��
	void Update(const float fDeltaTime) override;		// �X�V
	void Draw(CShader* pShader = nullptr) override;		// �`��

	// �Z�b�g�E�Q�b�g�֌W
	float GetMulti(void) const;	// �{���̎擾����

	// �ÓI�����o�֐�
	static CComboArea* Create	// ����
	( // ����
		const VECTOR3& rPos,	// �ʒu
		const int nMul			// �R���{�{��
	);
	static CListManager<CComboArea>* GetList();	// ���X�g�擾

private:

	// �ÓI�����o�ϐ�
	static CListManager<CComboArea>* m_pList;	// �I�u�W�F�N�g���X�g

	// �����o�ϐ�
	CListManager<CComboArea>::AIterator m_iterator;	// �C�e���[�^�[
	int m_nIdx;		// �ԍ�
};

#endif	// _COMBO_AREA_H_
