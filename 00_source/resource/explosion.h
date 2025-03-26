//============================================================
//
//	�����w�b�_�[ [explosion.h]
//	Author�F���c�E��
//
//============================================================
//************************************************************
//	��d�C���N���[�h�h�~
//************************************************************
#ifndef _EXPLOSION_H_
#define _EXPLOSION_H_

//************************************************************
//	�C���N���[�h�t�@�C��
//************************************************************
#include "anim3D.h"

//************************************************************
//	�N���X��`
//************************************************************
// �����N���X
class CExplosion : public CAnim3D
{
public:
	// ��ޗ�
	enum EType
	{
		TYPE_FIRE = 0,	// �΃e�N�X�`��
		TYPE_SMOKE,		// ���e�N�X�`��
		TYPE_MAX		// ���̗񋓌^�̑���
	};

	// �R���X�g���N�^
	CExplosion();

	// �f�X�g���N�^
	~CExplosion() override;

	// �I�[�o�[���C�h�֐�
	HRESULT Init() override;	// ������
	void Uninit() override;		// �I��
	void Update(const float fDeltaTime) override;	// �X�V
	void Draw(CShader* pShader = nullptr) override;	// �`��

	// �ÓI�����o�֐�
	static CExplosion* Create	// ����
	( // ����
		const EType type,		// ���
		const VECTOR3& rPos,	// �ʒu
		const VECTOR3& rSize	// �傫��
	);

	// �����o�֐�
	void SetType(const EType type);	// ��ސݒ�
	inline EType GetType() const { return m_type; }	// ��ގ擾

private:
	// �����o�ϐ�
	EType m_type;	// ���
};

#endif	// _EXPLOSION_H_
