//============================================================
//
//	�O�Ճw�b�_�[ [orbit.h]
//	Author�F���c�E��
//
//============================================================
//************************************************************
//	��d�C���N���[�h�h�~
//************************************************************
#ifndef _ORBIT_H_
#define _ORBIT_H_

//************************************************************
//	�C���N���[�h�t�@�C��
//************************************************************
#include "object.h"
#include "renderState.h"

//************************************************************
//	�萔�錾
//************************************************************
namespace orbit
{
	const int MAX_OFFSET		= 2;	// �I�t�Z�b�g�̐�
	const int DEFAULT_PART		= 10;	// �f�t�H���g�̕�����
	const int DEFAULT_TEXPART	= 1;	// �f�t�H���g�̃e�N�X�`��������
}

//************************************************************
//	�N���X��`
//************************************************************
// �O�ՃN���X
class COrbit : public CObject
{
public:
	// ��ԗ�
	enum EState
	{
		STATE_NONE = 0,	// �������Ȃ����
		STATE_NORMAL,	// �ʏ���
		STATE_VANISH,	// �������
		STATE_MAX		// ���̗񋓌^�̑���
	};

	// �R���X�g���N�^
	COrbit();

	// �f�X�g���N�^
	~COrbit() override;

	// �I�t�Z�b�g�\����
	struct SOffset
	{
	public:
		// �R���X�g���N�^
		SOffset() {}

		SOffset(const VECTOR3& rOffset0, const VECTOR3& rOffset1, const COLOR& rCol)
		{ aOffset[0] = rOffset0; aOffset[1] = rOffset1; aCol[0] = aCol[1] = rCol; }

		SOffset(const VECTOR3& rOffset0, const VECTOR3& rOffset1, const COLOR& rCol0, const COLOR& rCol1)
		{ aOffset[0] = rOffset0; aOffset[1] = rOffset1; aCol[0] = rCol0; aCol[1] = rCol1; }

		// �f�X�g���N�^
		~SOffset() {}

		// �����o�ϐ�
		VECTOR3	aOffset[orbit::MAX_OFFSET];	// ���[�̃I�t�Z�b�g
		COLOR	aCol[orbit::MAX_OFFSET];	// ���[�̊�F
	};

	// �O�Ս\����
	struct SOrbit
	{
		MATRIX		aMtxWorldPoint[orbit::MAX_OFFSET];	// ���[�̃��[���h�}�g���b�N�X
		SOffset			offset;		// �I�t�Z�b�g���
		VECTOR3*	pPosPoint;	// �e���_���W
		COLOR*		pColPoint;	// �e���_�J���[
		MATRIX*		pMtxParent;	// �e�̃}�g���b�N�X
		MATRIX		mtxVanish;	// �����J�n���̐e�̃}�g���b�N�X
		int nPart;		// ������
		int nTexPart;	// �e�N�X�`��������
		bool bAlpha;	// ��������
		bool bInit;		// ��������
	};

	// �I�[�o�[���C�h�֐�
	HRESULT Init() override;	// ������
	void Uninit() override;		// �I��
	void Update(const float fDeltaTime) override;	// �X�V
	void Draw(CShader* pShader = nullptr) override;	// �`��

	// �ÓI�����o�֐�
	static COrbit *Create	// ����
	( // ����
		MATRIX *pMtxParent,	// �e�}�g���b�N�X
		const SOffset offset,	// �I�t�Z�b�g���
		const int nPart		= orbit::DEFAULT_PART,		// ������
		const int nTexPart	= orbit::DEFAULT_TEXPART,	// �e�N�X�`��������
		const bool bAlpha	= true,						// ��������
		const CRenderState::EBlend blend = CRenderState::BLEND_ADD	// ���u�����h��
	);

	// �����o�֐�
	CRenderState* GetRenderState();				// �����_�[�X�e�[�g���擾
	void BindTexture(const int nTextureID);			// �e�N�X�`������ (�C���f�b�N�X)
	void BindTexture(const char* pTexturePath);		// �e�N�X�`������ (�p�X)
	void DeleteMatrixParent();					// �e�}�g���b�N�X�폜
	void SetState(const EState state);				// ��Ԑݒ�
	void SetMatrixParent(MATRIX* pMtxParent);	// �e�̃}�g���b�N�X�ݒ�
	void SetOffset(const SOffset offset);			// �I�t�Z�b�g���ݒ�
	void SetTexPart(const int nTexPart);			// �e�N�X�`���������ݒ�
	void SetEnableAlpha(const bool bAlpha);			// �������󋵐ݒ�
	void SetEnableInit(const bool bInit);			// �������󋵐ݒ�
	HRESULT SetLength(const int nPart);				// �����ݒ�
	inline int GetTextureIndex() const	{ return m_nTextureID; }	// �e�N�X�`���C���f�b�N�X�擾
	inline EState GetState() const		{ return m_state; }			// ��Ԏ擾

protected:
	// �����o�֐�
	void SetVtx();	// ���_���̐ݒ�

private:
	// �I�[�o�[���C�h�֐�
	void Release() override { CObject::Release(); }	// �j��

	// �����o�֐�
	void DrawNormal();	// �ʏ�`��
	void DrawShader(CShader* pShader);	// �V�F�[�_�[�`��

	// �����o�ϐ�
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;	// ���_�o�b�t�@�ւ̃|�C���^
	CRenderState* m_pRenderState;		// �����_�[�X�e�[�g�̏��
	SOrbit m_orbit;			// �O�Ղ̏��
	EState m_state;			// ���
	int m_nCounterState;	// ��ԊǗ��J�E���^�[
	int m_nNumVtx;			// �K�v���_��
	int m_nTextureID;		// �e�N�X�`���C���f�b�N�X
};

#endif	// _ORBIT_H_
