//============================================================
//
//	�t�̃w�b�_�[ [liquid.h]
//	Author�F���c�E��
//
//============================================================
//************************************************************
//	��d�C���N���[�h�h�~
//************************************************************
#ifndef _LIQUID_H_
#define _LIQUID_H_

//************************************************************
//	�C���N���[�h�t�@�C��
//************************************************************
#include "object.h"
#include "scrollMeshField.h"

//************************************************************
//	�N���X��`
//************************************************************
// �t�̃N���X
class CLiquid : public CObject
{
public:
	// ��ޗ�
	enum EType
	{
		TYPE_WATER = 0,	// ��
		TYPE_MAX		// ���̗񋓌^�̑���
	};

	// �㉺��
	enum ELiquid
	{
		LIQUID_LOW = 0,	// ��
		LIQUID_HIGH,	// ��
		LIQUID_MAX		// ���̗񋓌^�̑���
	};

	// �R���X�g���N�^
	CLiquid();

	// �f�X�g���N�^
	~CLiquid() override;

	// �e�N�X�`���ړ��ʍ\����
	struct STexMove
	{
	public:
		// �R���X�g���N�^
		STexMove() {}
		STexMove(const VECTOR2 TexMoveLow, const VECTOR2 TexMoveHigh)
		{ texMoveLow = TexMoveLow; texMoveHigh = TexMoveHigh; }

		// �f�X�g���N�^
		~STexMove() {}

		// �����o�ϐ�
		VECTOR2 texMoveLow;		// ���t�̂̃e�N�X�`���ړ���
		VECTOR2 texMoveHigh;	// ��t�̂̃e�N�X�`���ړ���
	};

	HRESULT Init() override;	// ������
	void Uninit() override;		// �I��
	void Update(const float fDeltaTime) override;		// �X�V
	void Draw(CShader* pShader = nullptr) override;		// �`��
	void SetVec3Position(const VECTOR3& rPos) override;	// �ʒu�ݒ�
	void SetVec3Rotation(const VECTOR3& rRot) override;	// �����ݒ�
	void SetVec2Size(const VECTOR2& rSize) override;	// �傫���ݒ�
	VECTOR3 GetVec3Position() const override	{ return m_apLiquid[LIQUID_LOW]->GetVec3Position(); }	// �ʒu�擾
	VECTOR3 GetVec3Rotation() const override	{ return m_apLiquid[LIQUID_LOW]->GetVec3Rotation(); }	// �����擾
	VECTOR2 GetVec2Size() const override		{ return m_apLiquid[LIQUID_LOW]->GetVec2Size(); }		// �傫���擾
	MATRIX* GetPtrMtxWorld() override			{ return m_apLiquid[LIQUID_LOW]->GetPtrMtxWorld(); }	// �}�g���b�N�X�|�C���^�擾
	MATRIX GetMtxWorld() const override			{ return m_apLiquid[LIQUID_LOW]->GetMtxWorld(); }		// �}�g���b�N�X�擾

	// �ÓI�����o�֐�
	static CLiquid* Create	// ����
	( // ����
		const EType type,				// ���
		const VECTOR3& rPos,			// �ʒu
		const VECTOR3& rRot,			// ����
		const VECTOR2& rSize,			// �傫��
		const COLOR& rCol,				// �F
		const POSGRID2& rPart,			// ������
		const STexMove& rTexMove,		// �e�N�X�`���ړ���
		const float fMaxUp = 0.0f,		// �g�̍ō��㏸��
		const float fAddSinRot = 0.0f,	// �g�ł��������Z��
		const float fAddVtxRot = 0.0f	// �הg�̌������Z��
	);
	static CListManager<CLiquid>* GetList();	// ���X�g�擾

	// �����o�֐�
	void SetColor(const COLOR& rCol);			// �F�ݒ�
	HRESULT SetPattern(const POSGRID2& rPart);	// �������ݒ�
	void SetType(const EType type);				// ��ސݒ�
	void SetTexMove(const STexMove texMove);	// �e�N�X�`���ړ��ʐݒ�
	void SetMaxUp(const float fMaxUp);			// �g�̍ō��㏸�ʐݒ�
	void SetAddSinRot(const float fAddSinRot);	// �g�ł��������Z�ʐݒ�
	void SetAddVtxRot(const float fAddVtxRot);	// �הg�̌������Z�ʐݒ�
	inline COLOR GetColor() const		{ return m_apLiquid[LIQUID_LOW]->GetColor(); }		// �F�擾
	inline POSGRID2 GetPattern() const	{ return m_apLiquid[LIQUID_LOW]->GetPattern(); }	// �������擾
	inline EType GetType() const		{ return m_type; }			// ��ގ擾
	inline float GetMaxUp() const		{ return m_fMaxUp; }		// �g�̍ō��㏸�ʎ擾
	inline float GetAddSinRot() const	{ return m_fAddSinRot; }	// �g�ł��������Z�ʎ擾
	inline float GetAddVtxRot() const	{ return m_fAddVtxRot; }	// �הg�̌������Z�ʎ擾

	STexMove GetTexMove() const;	// �e�N�X�`���ړ��ʎ擾
	CScrollMeshField* GetMeshField(const int nID) const;	// ���b�V���t�B�[���h�擾

private:
	// �I�[�o�[���C�h�֐�
	void Release() override { CObject::Release(); }	// �j��

	// �ÓI�����o�ϐ�
	static CListManager<CLiquid>* m_pList;	// �I�u�W�F�N�g���X�g

	// �����o�ϐ�
	CListManager<CLiquid>::AIterator m_iterator;	// �C�e���[�^�[
	CScrollMeshField* m_apLiquid[LIQUID_MAX];		// �t�̂̏��
	EType m_type;		// ���
	float m_fMaxUp;		// �g�̍ō��㏸��
	float m_fSinRot;	// �g�ł�����
	float m_fAddSinRot;	// �g�ł��������Z��
	float m_fAddVtxRot;	// �הg�̌������Z��
};

#endif	// _LIQUID_H_
