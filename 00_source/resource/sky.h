//============================================================
//
//	��w�b�_�[ [sky.h]
//	Author�F���c�E��
//
//============================================================
//************************************************************
//	��d�C���N���[�h�h�~
//************************************************************
#ifndef _SKY_H_
#define _SKY_H_

//************************************************************
//	�C���N���[�h�t�@�C��
//************************************************************
#include "objectMeshDome.h"

//************************************************************
//	�N���X��`
//************************************************************
// ��N���X
class CSky : public CObjectMeshDome
{
public:
	// ��ޗ�
	enum EType
	{
		TYPE_NORMAL = 0,	// �ʏ�e�N�X�`��
		TYPE_MAX			// ���̗񋓌^�̑���
	};

	// �R���X�g���N�^
	CSky();

	// �f�X�g���N�^
	~CSky() override;

	// �I�[�o�[���C�h�֐�
	HRESULT Init() override;	// ������
	void Uninit() override;		// �I��
	void Update(const float fDeltaTime) override;	// �X�V
	void Draw(CShader* pShader = nullptr) override;	// �`��

	// �ÓI�����o�֐�
	static CSky* Create	// ����
	( // ����
		const EType type,			// ���
		const VECTOR3& rPos,	// �ʒu
		const VECTOR3& rRot,	// ����
		const COLOR& rCol,		// �F
		const POSGRID2& rPart,		// ������
		const POSGRID2& rTexPart,	// �e�N�X�`��������
		const float fRadius			// ���a
	);
	static CListManager<CSky>* GetList();	// ���X�g�擾

	// �����o�֐�
	void SetType(const EType type);	// ��ސݒ�
	inline EType GetType() const { return m_type; }	// ��ގ擾

private:
	// �ÓI�����o�ϐ�
	static CListManager<CSky>* m_pList;	// �I�u�W�F�N�g���X�g

	// �����o�ϐ�
	CListManager<CSky>::AIterator m_iterator;	// �C�e���[�^�[
	EType m_type;	// ���
};

#endif	// _SKY_H_
