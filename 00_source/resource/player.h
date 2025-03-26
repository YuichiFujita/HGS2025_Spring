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
#include "object3D.h"

//************************************************************
//	�N���X��`
//************************************************************
// �v���C���[�N���X
class CPlayer : public CObject3D
{
public:
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
	inline void SetMove(const VECTOR3& rMove)	{ m_move = rMove; }		// �ړ��ʐݒ�
	inline VECTOR3 GetMove() const				{ return m_move; }		// �ړ��ʎ擾
	inline void SetEnableJump(const bool bJump)	{ m_bJump = bJump; }	// �W�����v�󋵐ݒ�
	inline bool IsJump() const					{ return m_bJump; }		// �W�����v�󋵐ݒ�

private:
	// �G�C���A�X��`
	typedef void(CPlayer::*AFuncState)(const float);	// ��ԍX�V�֐��|�C���^

	// �֐��z��
	static AFuncState m_aFuncState[];	// ��ԍX�V�֐����X�g

	// �����o�֐�
	void UpdateNone(const float fDeltaTime);		// �������Ȃ���Ԏ��̍X�V
	void UpdateNormal(const float fDeltaTime);		// �ʏ��Ԏ��̍X�V
	void UpdateDeath(const float fDeltaTime);		// ���S��Ԏ��̍X�V
	void UpdateMove(const float fDeltaTime);		// �ړ���/�ڕW�����̍X�V
	void UpdateOldPosition();						// �ߋ��ʒu�̍X�V
	void UpdateDirection();							// �����̍X�V
	void UpdateGravity(const float fDeltaTime);		// �d�͂̍X�V
	void UpdateJump(const float fDeltaTime);		// �W�����v�̍X�V
	void UpdateShot(const VECTOR3& rPos, const float fDeltaTime);	// �U���̍X�V
	bool UpdateLanding(VECTOR3* pPos, const float fDeltaTime);		// ���n�󋵂̍X�V
	void UpdatePosition(VECTOR3* pPos, const float fDeltaTime);		// �ʒu�̍X�V

	// �ÓI�����o�ϐ�
	static CListManager<CPlayer>* m_pList;	// �I�u�W�F�N�g���X�g

	// �����o�ϐ�
	CListManager<CPlayer>::AIterator m_iterator;	// �C�e���[�^�[
	VECTOR3	m_oldPos;		// �ߋ��ʒu
	VECTOR3	m_move;			// �ړ���
	EState	m_state;		// ���
	bool	m_bRight;		// ���E�t���O
	bool	m_bJump;		// �W�����v��
	bool	m_bJumpPress;	// �W�����v����t���O
	float	m_fJumpTimer;	// �W�����v���쎞��
	float	m_fShotTimer;	// �U���C���^�[�o��
};

#endif	// _PLAYER_H_
