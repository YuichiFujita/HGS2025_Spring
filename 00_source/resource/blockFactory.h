//============================================================
//
//	�u���b�N�t�@�N�g���[�w�b�_�[ [blockFactory.h]
//	Author�F��������
//
//============================================================
//************************************************************
//	��d�C���N���[�h�h�~
//************************************************************
#ifndef _BLOCK_FACTORY_H_
#define _BLOCK_FACTORY_H_

//************************************************************
//	�C���N���[�h�t�@�C��
//************************************************************
#include "object.h"

//************************************************************
//	�N���X��`
//************************************************************
// �u���b�N�t�@�N�g���[�N���X
class CBlockFactory : public CObject
{
public:

	// �R���X�g���N�^
	CBlockFactory();

	// �f�X�g���N�^
	~CBlockFactory() override;

	// �I�[�o�[���C�h�֐�
	HRESULT Init() override;	// ������
	void Uninit() override;		// �I��
	void Update(const float fDeltaTime) override;		// �X�V
	void Draw(CShader* pShader = nullptr) override;		// �`��

	// �ÓI�����o�֐�
	static CBlockFactory* Create();	// ����

private:

	// �I�[�o�[���C�h�֐�
	inline void Release() override { CObject::Release(); }	// �j��

	// �����o�ϐ�
	float m_fRate;			// ����
	float m_fCreateRate;	// ��������
};

#endif	// _TIME_UI_H_
