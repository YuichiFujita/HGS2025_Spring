//============================================================
//
//	�n�ʃw�b�_�[ [field.h]
//	Author�F���c�E��
//
//============================================================
//************************************************************
//	��d�C���N���[�h�h�~
//************************************************************
#ifndef _FIELD_H_
#define _FIELD_H_

//************************************************************
//	�C���N���[�h�t�@�C��
//************************************************************
#include "objectMeshField.h"

//************************************************************
//	�N���X��`
//************************************************************
// �n�ʃN���X
class CField : public CObjectMeshField
{
public:
	// ��ޗ�
	enum EType
	{
		TYPE_SOIL = 0,	// �y�e�N�X�`��
		TYPE_MAX,		// ���̗񋓌^�̑���
	};

	// �R���X�g���N�^
	CField();

	// �f�X�g���N�^
	~CField() override;

	// �I�[�o�[���C�h�֐�
	HRESULT Init() override;	// ������
	void Uninit() override;		// �I��
	void Update(const float fDeltaTime) override;	// �X�V
	void Draw(CShader* pShader = nullptr) override;	// �`��

	// �ÓI�����o�֐�
	static CField* Create	// ����
	( // ����
		const EType type,			// ���
		const VECTOR3& rPos,	// �ʒu
		const VECTOR3& rRot,	// ����
		const VECTOR2& rSize,	// �傫��
		const COLOR& rCol,		// �F
		const POSGRID2& rPart,		// ������
		const POSGRID2& rTexPart	// �e�N�X�`��������
	);
	static CListManager<CField>* GetList();	// ���X�g�擾

	// �����o�֐�
	void SetType(const EType type);	// ��ސݒ�
	inline EType GetType() const { return m_type; }	// ��ގ擾

private:

	// �ÓI�����o�ϐ�
	static CListManager<CField>* m_pList;	// �I�u�W�F�N�g���X�g

	// �����o�ϐ�
	CListManager<CField>::AIterator m_iterator;	// �C�e���[�^�[
	EType m_type;	// ���
};

#endif	// _FIELD_H_
