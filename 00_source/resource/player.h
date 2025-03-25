//============================================================
//
//	�v���C���[�w�b�_�[ [player.h]
//	Author�F���c�E��
// 
//============================================================
//************************************************************
//	��d�C���N���[�h�h�~
//************************************************************
#ifndef _PLAYER_H_
#define _PLAYER_H_

//************************************************************
//	�C���N���[�h�t�@�C��
//************************************************************
#include "objectChara.h"

//************************************************************
//	�N���X��`
//************************************************************
// �v���C���[�N���X
class CPlayer : public CObjectChara
{
public:
	// ���f����
	enum EModel
	{
		MODEL_WAIST = 0,	// ��
		MODEL_BODY,			// ��
		MODEL_HEAD,			// ��
		MODEL_ARMUL,		// ����r
		MODEL_ARMUR,		// �E��r
		MODEL_ARMDL,		// �����r
		MODEL_ARMDR,		// �E���r
		MODEL_HANDL,		// ����
		MODEL_HANDR,		// �E��
		MODEL_LEGUL,		// ��������
		MODEL_LEGUR,		// �E������
		MODEL_LEGDL,		// ����
		MODEL_LEGDR,		// �E��
		MODEL_FOOTL,		// ����
		MODEL_FOOTR,		// �E��
		MODEL_MAX			// ���̗񋓌^�̑���
	};

	// ���[�V������
	enum EMotion
	{
		MOTION_IDOL = 0,	// �ҋ@���[�V����
		MOTION_WALK,		// ���s���[�V����
		MOTION_MAX			// ���̗񋓌^�̑���
	};

	// ��ԗ�
	enum EState
	{
		STATE_NONE = 0,	// �������Ȃ����
		STATE_NORMAL,	// �ʏ���
		STATE_DEATH,	// ���S���
		STATE_MAX		// ���̗񋓌^�̑���
	};

	// �R���X�g���N�^
	CPlayer();

	// �f�X�g���N�^
	~CPlayer() override;

	// �I�[�o�[���C�h�֐�
	HRESULT Init() override;	// ������
	void Uninit() override;		// �I��
	void Update(const float fDeltaTime) override;		// �X�V
	void Draw(CShader* pShader = nullptr) override;		// �`��
	void SetEnableUpdate(const bool bUpdate) override;	// �X�V�󋵐ݒ�
	void SetEnableDraw(const bool bDraw) override;		// �`��󋵐ݒ�

	// �ÓI�����o�֐�
	static CPlayer* Create	// ����
	( // ����
		const VECTOR3& rPos,	// �ʒu
		const VECTOR3& rRot		// ����
	);
	static CListManager<CPlayer>* GetList();	// ���X�g�擾

	// �����o�֐�
	bool Hit();	// �q�b�g
	void SetState(const EState state);	// ��Ԑݒ�
	EState GetState() const;			// ��Ԏ擾
	float GetRadius() const;			// ���a�擾
	float GetHeight() const;			// �c���擾
	inline void SetDestRotation(const VECTOR3& rRot)	{ m_destRot = rRot; }	// �ڕW�����ݒ�
	inline VECTOR3 GetDestRotation() const				{ return m_destRot; }	// �ڕW�����擾
	inline void SetMove(const VECTOR3& rMove)			{ m_move = rMove; }		// �ړ��ʐݒ�
	inline VECTOR3 GetMove() const						{ return m_move; }		// �ړ��ʎ擾
	inline void SetEnableJump(const bool bJump)			{ m_bJump = bJump; }	// �W�����v�󋵐ݒ�
	inline bool IsJump() const							{ return m_bJump; }		// �W�����v�󋵐ݒ�

private:
	// �G�C���A�X��`
	typedef EMotion(CPlayer::*AFuncState)(const float);	// ��ԍX�V�֐��|�C���^

	// �֐��z��
	static AFuncState m_aFuncState[];	// ��ԍX�V�֐����X�g

	// �����o�֐�
	EMotion UpdateNone(const float fDeltaTime);		// �������Ȃ���Ԏ��̍X�V
	EMotion UpdateNormal(const float fDeltaTime);	// �ʏ��Ԏ��̍X�V
	EMotion UpdateDeath(const float fDeltaTime);	// ���S��Ԏ��̍X�V
	EMotion UpdateMove(const float fDeltaTime);		// �ړ���/�ڕW�����̍X�V
	void UpdateOldPosition();						// �ߋ��ʒu�̍X�V
	void UpdateGravity(const float fDeltaTime);		// �d�͂̍X�V
	bool UpdateLanding(VECTOR3* pPos, const float fDeltaTime);	// ���n�󋵂̍X�V
	void UpdatePosition(VECTOR3* pPos, const float fDeltaTime);	// �ʒu�̍X�V
	void UpdateRotation(VECTOR3* pRot, const float fDeltaTime);	// �����̍X�V
	void UpdateMotion(int nMotion, const float fDeltaTime);		// ���[�V����/�L�����N�^�[�̍X�V

	// �ÓI�����o�ϐ�
	static CListManager<CPlayer>* m_pList;	// �I�u�W�F�N�g���X�g

	// �����o�ϐ�
	CListManager<CPlayer>::AIterator m_iterator;	// �C�e���[�^�[
	VECTOR3	m_oldPos;	// �ߋ��ʒu
	VECTOR3	m_move;		// �ړ���
	VECTOR3	m_destRot;	// �ڕW����
	EState	m_state;	// ���
	bool	m_bJump;	// �W�����v��
};

#endif	// _PLAYER_H_
