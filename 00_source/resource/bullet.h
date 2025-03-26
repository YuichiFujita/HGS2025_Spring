//============================================================
//
//	�e�e�w�b�_�[ [bullet.h]
//	Author�F��������
// 
//============================================================
//************************************************************
//	��d�C���N���[�h�h�~
//************************************************************
#ifndef _BULLET_H_
#define _BULLET_H_

//************************************************************
//	�C���N���[�h�t�@�C��
//************************************************************
#include "object3D.h"

//************************************************************
//	�N���X��`
//************************************************************
// �e�e�N���X
class CBullet : public CObject3D
{
public:

	// �R���X�g���N�^
	CBullet();

	// �f�X�g���N�^
	~CBullet() override;

	// �I�[�o�[���C�h�֐�
	HRESULT Init() override;	// ������
	void Uninit() override;		// �I��
	void Update(const float fDeltaTime) override;		// �X�V
	void Draw(CShader* pShader = nullptr) override;		// �`��

	// �ÓI�����o�֐�
	static CBullet* Create	// ����
	( // ����
		const VECTOR3& rPos,	// �ʒu
		const bool bRight		// �E��
	);
	static CListManager<CBullet>* GetList();	// ���X�g�擾

private:

	// �����o�֐�
	void SetMove(const bool bRight);	// �ړ��ʂ̐ݒ菈��
	void BlockCollision(void);			// �u���b�N�Ƃ̓����蔻��

	// �����o�֐�
	void Move(void);			// �ړ�����

	// �ÓI�����o�ϐ�
	static CListManager<CBullet>* m_pList;	// �I�u�W�F�N�g���X�g

	// �����o�ϐ�
	CListManager<CBullet>::AIterator m_iterator;	// �C�e���[�^�[
	VECTOR3 m_move;		// �ړ���
};

#endif	// _BLOCK_H_
