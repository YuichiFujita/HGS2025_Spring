//============================================================
//
//	�j��\�u���b�N�w�b�_�[ [blockBreak.h]
//	Author�F��������
// 
//============================================================
//************************************************************
//	��d�C���N���[�h�h�~
//************************************************************
#ifndef _BREAK_BLOCK_BREAK_H_
#define _BREAK_BLOCK_BREAK_H_

//************************************************************
//	�C���N���[�h�t�@�C��
//************************************************************
#include "block.h"

//************************************************************
//	�N���X��`
//************************************************************
// �j��\�u���b�N�N���X
class CBlockBreak : public CBlock
{
public:

	// �R���X�g���N�^
	CBlockBreak();

	// �f�X�g���N�^
	~CBlockBreak() override;

	// �I�[�o�[���C�h�֐�
	HRESULT Init() override;	// ������
	void Uninit() override;		// �I��
	void Update(const float fDeltaTime) override;		// �X�V
	void Draw(CShader* pShader = nullptr) override;		// �`��

	// �����o�֐�
	void Hit() override;		// �q�b�g����

private:

};

#endif	// _BREAK_BLOCK_H_
