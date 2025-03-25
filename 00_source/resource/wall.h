//============================================================
//
//	�ǃw�b�_�[ [wall.h]
//	Author�F���c�E��
//
//============================================================
//************************************************************
//	��d�C���N���[�h�h�~
//************************************************************
#ifndef _WALL_H_
#define _WALL_H_

//************************************************************
//	�C���N���[�h�t�@�C��
//************************************************************
#include "objectMeshWall.h"

//************************************************************
//	�N���X��`
//************************************************************
// �ǃN���X
class CWall : public CObjectMeshWall
{
public:
	// ��ޗ�
	enum EType
	{
		TYPE_SOIL = 0,	// �y�e�N�X�`��
		TYPE_MAX		// ���̗񋓌^�̑���
	};

	// �R���X�g���N�^
	CWall();

	// �f�X�g���N�^
	~CWall() override;

	// �I�[�o�[���C�h�֐�
	HRESULT Init() override;	// ������
	void Uninit() override;		// �I��
	void Update(const float fDeltaTime) override;	// �X�V
	void Draw(CShader* pShader = nullptr) override;	// �`��

	// �ÓI�����o�֐�
	static CWall* Create	// ����
	( // ����
		const EType type,			// ���
		const VECTOR3& rPos,	// �ʒu
		const VECTOR3& rRot,	// ����
		const VECTOR2& rSize,	// �傫��
		const COLOR& rCol,		// �F
		const POSGRID2& rPart,		// ������
		const POSGRID2& rTexPart	// �e�N�X�`��������
	);
	static CListManager<CWall>* GetList();	// ���X�g�擾

	// �����o�֐�
	void SetType(const EType type);	// ��ސݒ�
	inline EType GetType() const { return m_type; }	// ��ގ擾

private:
	// �ÓI�����o�ϐ�
	static CListManager<CWall>* m_pList;	// �I�u�W�F�N�g���X�g

	// �����o�ϐ�
	CListManager<CWall>::AIterator m_iterator;	// �C�e���[�^�[
	EType m_type;	// ���
};

#endif	// _WALL_H_
