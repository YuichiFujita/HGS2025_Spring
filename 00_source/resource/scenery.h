//============================================================
//
//	�i�F�w�b�_�[ [scenery.h]
//	Author�F���c�E��
//
//============================================================
//************************************************************
//	��d�C���N���[�h�h�~
//************************************************************
#ifndef _SCENERY_H_
#define _SCENERY_H_

//************************************************************
//	�C���N���[�h�t�@�C��
//************************************************************
#include "objectMeshCylinder.h"

//************************************************************
//	�N���X��`
//************************************************************
// �i�F�N���X
class CScenery : public CObjectMeshCylinder
{
public:
	// ��ޗ�
	enum EType
	{
		TYPE_MOUNTAIN_SMALL_00 = 0,	// �R�e�N�X�`��
		TYPE_MOUNTAIN_SMALL_01,		// �R�e�N�X�`��
		TYPE_MOUNTAIN_BIG,			// �R�e�N�X�`��
		TYPE_MAX					// ���̗񋓌^�̑���
	};

	// �R���X�g���N�^
	CScenery();

	// �f�X�g���N�^
	~CScenery() override;

	// �I�[�o�[���C�h�֐�
	HRESULT Init() override;	// ������
	void Uninit() override;		// �I��
	void Update(const float fDeltaTime) override;	// �X�V
	void Draw(CShader* pShader = nullptr) override;	// �`��

	// �ÓI�����o�֐�
	static CScenery *Create	// ����
	( // ����
		const EType type,			// ���
		const VECTOR3& rPos,	// �ʒu
		const VECTOR3& rRot,	// ����
		const COLOR& rCol,		// �F
		const POSGRID2& rPart,		// ������
		const POSGRID2& rTexPart,	// �e�N�X�`��������
		const float fRadius,		// ���a
		const float fHeight			// �c��
	);
	static CListManager<CScenery>* GetList();	// ���X�g�擾

	// �����o�֐�
	void SetType(const EType type);	// ��ސݒ�
	inline EType GetType() const { return m_type; }	// ��ގ擾

private:
	// �ÓI�����o�ϐ�
	static CListManager<CScenery>* m_pList;	// �I�u�W�F�N�g���X�g

	// �����o�ϐ�
	CListManager<CScenery>::AIterator m_iterator;	// �C�e���[�^�[
	EType m_type;	// ���
};

#endif	// _SCENERY_H_
