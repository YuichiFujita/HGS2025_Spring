//============================================================
//
//	�u���b�N�w�b�_�[ [block.h]
//	Author�F��������
// 
//============================================================
//************************************************************
//	��d�C���N���[�h�h�~
//************************************************************
#ifndef _BLOCK_H_
#define _BLOCK_H_

//************************************************************
//	�C���N���[�h�t�@�C��
//************************************************************
#include "object3D.h"

//************************************************************
//	�N���X��`
//************************************************************
// �u���b�N�N���X
class CBlock : public CObject3D
{
public:

	// ��ނ̗񋓌^
	enum EType
	{
		TYPE_BREAK = 0,		// �j��u���b�N
		TYPE_MAX			// ���̗񋓌^�̑���
	};

	// �R���X�g���N�^
	CBlock();

	// �f�X�g���N�^
	~CBlock() override;

	// �I�[�o�[���C�h�֐�
	HRESULT Init() override;	// ������
	void Uninit() override;		// �I��
	void Update(const float fDeltaTime) override;		// �X�V
	void Draw(CShader* pShader = nullptr) override;		// �`��

	// �ÓI�����o�֐�
	static CBlock* Create	// ����
	( // ����
		const VECTOR3& rPos,	// �ʒu
		const EType type,		// ���
		const float fSpeed,		// ���x
		const bool bRight		// �E��
	);
	static CListManager<CBlock>* GetList();	// ���X�g�擾

	// �����o�֐�
	virtual bool Hit() = 0;		// �q�b�g����

private:

	// �����o�֐�
	void Move(void);			// �ړ�����

	// �ÓI�����o�ϐ�
	static CListManager<CBlock>* m_pList;	// �I�u�W�F�N�g���X�g

	// �����o�ϐ�
	CListManager<CBlock>::AIterator m_iterator;	// �C�e���[�^�[
	EType m_type;		// ���
	float m_fSpeed;		// ���x
	bool m_bRight;		// �E����
};

#endif	// _BLOCK_H_
