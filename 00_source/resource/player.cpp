//============================================================
//
//	�v���C���[���� [player.cpp]
//	Author�F���c�E��
//
//============================================================
//************************************************************
//	�C���N���[�h�t�@�C��
//************************************************************
#include "player.h"
#include "main.h"
#include "manager.h"
#include "renderer.h"
#include "sound.h"
#include "camera.h"
#include "texture.h"
#include "fade.h"
#include "stage.h"
#include "sceneGame.h"
#include "gameManager.h"

//************************************************************
//	�}�N����`
//************************************************************
// �W�����v���ړ�ON/OFF
#if 0
#define JUMP_MOVE
#endif

//************************************************************
//	�萔�錾
//************************************************************
namespace
{
	const char*	TEXTURE_RIGHT	= "data\\TEXTURE\\player_right000.png";	// �v���C���[�E�����̃e�N�X�`���p�X
	const char*	TEXTURE_LEFT	= "data\\TEXTURE\\player_left000.png";	// �v���C���[�������̃e�N�X�`���p�X
	const int	PRIORITY	= 3;		// �v���C���[�̗D�揇��
	const float	MOVE		= 0.9f;		// �ړ���
	const float	JUMP		= 6.0f;		// �W�����v��
	const float	GRAVITY		= 0.4f;		// �d��
	const float	RADIUS		= 20.0f;	// ���a
	const float WIDTH		= 50.0f;	// ����
	const float HEIGHT		= 50.0f;	// �c��
	const float	REV_ROTA	= 0.25f;	// �����ύX�̕␳�W��
	const float	JUMP_REV	= 0.18f;	// �ʏ��Ԏ��̋󒆂̈ړ��ʂ̌����W��
	const float	LAND_REV	= 0.18f;	// �ʏ��Ԏ��̒n��̈ړ��ʂ̌����W��
	const float	JUMP_PRESS_TIME	= 0.3f;	// �W�����v���Z����
	const float	MAX_ADD_JUMP	= 0.9f;	// �W�����v���Z�ő�l

	namespace camera
	{
		const CCamera::SSwing HIT_SWING = CCamera::SSwing(8.0f, 1.8f, 0.14f);	// �q�b�g���̃J�����h��
	}
}

//************************************************************
//	�ÓI�����o�ϐ��錾
//************************************************************
CListManager<CPlayer>* CPlayer::m_pList = nullptr;	// �I�u�W�F�N�g���X�g
CPlayer::AFuncState CPlayer::m_aFuncState[] =		// ��ԍX�V�֐����X�g
{
	&CPlayer::UpdateNone,	// �������Ȃ���Ԃ̍X�V
	&CPlayer::UpdateNormal,	// �ʏ��Ԃ̍X�V
	&CPlayer::UpdateDeath,	// ���S��Ԃ̍X�V
};

//************************************************************
//	�q�N���X [CPlayer] �̃����o�֐�
//************************************************************
//============================================================
//	�R���X�g���N�^
//============================================================
CPlayer::CPlayer() : CObject3D(CObject::LABEL_PLAYER, CObject::DIM_3D, PRIORITY),
	m_oldPos	 (VEC3_ZERO),	// �ߋ��ʒu
	m_move		 (VEC3_ZERO),	// �ړ���
	m_state		 (STATE_NONE),	// ���
	m_bRight	 (false),		// ���E�t���O
	m_bJump		 (false),		// �W�����v��
	m_bJumpPress (false),		// �W�����v����t���O
	m_fJumpTimer (0.0f)			// �W�����v���쎞��
{
	// �X�^�e�B�b�N�A�T�[�g
	static_assert(NUM_ARRAY(m_aFuncState) == CPlayer::STATE_MAX, "ERROR : State Count Mismatch");
}

//============================================================
//	�f�X�g���N�^
//============================================================
CPlayer::~CPlayer()
{

}

//============================================================
//	����������
//============================================================
HRESULT CPlayer::Init()
{
	// �����o�ϐ���������
	m_oldPos	 = VEC3_ZERO;		// �ߋ��ʒu
	m_move		 = VEC3_ZERO;		// �ړ���
	m_state		 = STATE_NORMAL;	// ���
	m_bRight	 = false;			// ���E�t���O
	m_bJump		 = true;			// �W�����v��
	m_bJumpPress = false;			// �W�����v����t���O
	m_fJumpTimer = 0.0f;			// �W�����v���쎞��

	// �I�u�W�F�N�g�L�����N�^�[�̏�����
	if (FAILED(CObject3D::Init()))
	{ // �������Ɏ��s�����ꍇ

		// ���s��Ԃ�
		assert(false);
		return E_FAIL;
	}

	// �����̍X�V
	UpdateDirection();

	// �傫���̐ݒ�
	SetVec3Size(VECTOR3(WIDTH, HEIGHT, 0.0f));

	// ���_�̐ݒ�
	SetOrigin(CObject3D::ORIGIN_DOWN);

	if (m_pList == nullptr)
	{ // ���X�g�}�l�[�W���[�����݂��Ȃ��ꍇ

		// ���X�g�}�l�[�W���[�̐���
		m_pList = CListManager<CPlayer>::Create();
		if (m_pList == nullptr)
		{ // �����Ɏ��s�����ꍇ

			// ���s��Ԃ�
			assert(false);
			return E_FAIL;
		}
	}

	// ���X�g�Ɏ��g�̃I�u�W�F�N�g��ǉ��E�C�e���[�^�[���擾
	m_iterator = m_pList->AddList(this);

	// ������Ԃ�
	return S_OK;
}

//============================================================
//	�I������
//============================================================
void CPlayer::Uninit()
{
	// ���X�g���玩�g�̃I�u�W�F�N�g���폜
	m_pList->DelList(m_iterator);

	if (m_pList->GetNumAll() == 0)
	{ // �I�u�W�F�N�g������Ȃ��ꍇ

		// ���X�g�}�l�[�W���[�̔j��
		m_pList->Release(m_pList);
	}

	// �I�u�W�F�N�g�L�����N�^�[�̏I��
	CObject3D::Uninit();
}

//============================================================
//	�X�V����
//============================================================
void CPlayer::Update(const float fDeltaTime)
{
	// �ߋ��ʒu�̍X�V
	UpdateOldPosition();

	// ��Ԃ̍X�V
	(this->*(m_aFuncState[m_state]))(fDeltaTime);

	// �I�u�W�F�N�g�L�����N�^�[�̍X�V
	CObject3D::Update(fDeltaTime);
}

//============================================================
//	�`�揈��
//============================================================
void CPlayer::Draw(CShader* pShader)
{
	// �I�u�W�F�N�g�L�����N�^�[�̕`��
	CObject3D::Draw(pShader);
}

//============================================================
//	�X�V�󋵂̐ݒ菈��
//============================================================
void CPlayer::SetEnableUpdate(const bool bUpdate)
{
	// �����̍X�V�󋵂�ݒ�
	CObject::SetEnableUpdate(bUpdate);	// ���g
}

//============================================================
//	�`��󋵂̐ݒ菈��
//============================================================
void CPlayer::SetEnableDraw(const bool bDraw)
{
	// �����̕`��󋵂�ݒ�
	CObject::SetEnableDraw(bDraw);	// ���g
}

//============================================================
//	��������
//============================================================
CPlayer* CPlayer::Create
(
	const VECTOR3& rPos,	// �ʒu
	const VECTOR3& rRot		// ����
)
{
	// �v���C���[�̐���
	CPlayer* pPlayer = new CPlayer;
	if (pPlayer == nullptr)
	{ // �����Ɏ��s�����ꍇ

		return nullptr;
	}
	else
	{ // �����ɐ��������ꍇ

		// �v���C���[�̏�����
		if (FAILED(pPlayer->Init()))
		{ // �������Ɏ��s�����ꍇ

			// �v���C���[�̔j��
			SAFE_DELETE(pPlayer);
			return nullptr;
		}

		// �ʒu��ݒ�
		pPlayer->SetVec3Position(rPos);
		pPlayer->m_oldPos = rPos;	// �ߋ��ʒu������̈ʒu�ɂ���

		// ������ݒ�
		pPlayer->SetVec3Rotation(rRot);

		// �m�ۂ����A�h���X��Ԃ�
		return pPlayer;
	}
}

//============================================================
//	���X�g�擾����
//============================================================
CListManager<CPlayer>* CPlayer::GetList()
{
	// �I�u�W�F�N�g���X�g��Ԃ�
	return m_pList;
}

//============================================================
//	�q�b�g����
//============================================================
bool CPlayer::Hit()
{
	// ����ł�ꍇ������
	if (IsDeath()) { return false; }

	// �J�����h���ݒ�
	GET_MANAGER->GetCamera()->SetSwing(camera::HIT_SWING);

	// ���S��Ԃɂ���
	SetState(STATE_DEATH);

	CGameManager* pGameManager = CSceneGame::GetGameManager();	// �Q�[���}�l�[�W���[
	if (pGameManager != nullptr)
	{ // �Q�[���}�l�[�W���[������ꍇ

		// ���U���g��ʂɑJ�ڂ���
		pGameManager->TransResult();
	}

	return true;
}

//============================================================
//	��Ԃ̐ݒ菈��
//============================================================
void CPlayer::SetState(const EState state)
{
	if (state > NONE_IDX && state < STATE_MAX)
	{ // �͈͓��̏ꍇ

		// �����̏�Ԃ�ݒ�
		m_state = state;
	}
	else { assert(false); }	// �͈͊O
}

//============================================================
//	��Ԏ擾����
//============================================================
CPlayer::EState CPlayer::GetState() const
{
	// ��Ԃ�Ԃ�
	return m_state;
}

//============================================================
//	���a�擾����
//============================================================
float CPlayer::GetRadius() const
{
	// ���a��Ԃ�
	return RADIUS;
}

//============================================================
//	�c���擾����
//============================================================
float CPlayer::GetHeight() const
{
	// �c����Ԃ�
	return HEIGHT;
}

//============================================================
//	�������Ȃ���Ԏ��̍X�V����
//============================================================
void CPlayer::UpdateNone(const float fDeltaTime)
{
	VECTOR3 posPlayer = GetVec3Position();	// �v���C���[�ʒu
	VECTOR3 rotPlayer = GetVec3Rotation();	// �v���C���[����

	// �d�͂̍X�V
	UpdateGravity(fDeltaTime);

	// �ʒu�X�V
	UpdatePosition(&posPlayer, fDeltaTime);

	// ���n����
	UpdateLanding(&posPlayer, fDeltaTime);

	// �ʒu�␳
	CStage* pStage = GET_MANAGER->GetStage();	// �X�e�[�W���
	pStage->LimitPosition(posPlayer, RADIUS);

	// �����X�V
	UpdateDirection();

	// �ʒu�𔽉f
	SetVec3Position(posPlayer);

	// �����𔽉f
	SetVec3Rotation(rotPlayer);
}

//============================================================
//	�ʏ��Ԏ��̍X�V����
//============================================================
void CPlayer::UpdateNormal(const float fDeltaTime)
{
	// ����s�\�ȏꍇ������
	CGameManager* pGameManager = CSceneGame::GetGameManager();	// �Q�[���}�l�[�W���[
	if (pGameManager != nullptr)
	{ // �Q�[���}�l�[�W���[������ꍇ

		// ����s�\�ȏꍇ������
		if (!pGameManager->IsControlOK()) { return; }
	}

	VECTOR3 posPlayer = GetVec3Position();	// �v���C���[�ʒu
	VECTOR3 rotPlayer = GetVec3Rotation();	// �v���C���[����

	// �ړ�����
	UpdateMove(fDeltaTime);

	// �d�͂̍X�V
	UpdateGravity(fDeltaTime);

	// �W�����v�̍X�V
	UpdateJump(fDeltaTime);

	// �ʒu�X�V
	UpdatePosition(&posPlayer, fDeltaTime);

	// ���n����
	UpdateLanding(&posPlayer, fDeltaTime);

	// �ʒu�␳
	CStage* pStage = GET_MANAGER->GetStage();	// �X�e�[�W���
	pStage->LimitPosition(posPlayer, RADIUS);

	// �����X�V
	UpdateDirection();

	// �ʒu�𔽉f
	SetVec3Position(posPlayer);

	// �����𔽉f
	SetVec3Rotation(rotPlayer);
}

//============================================================
//	���S��Ԏ��̍X�V����
//============================================================
void CPlayer::UpdateDeath(const float fDeltaTime)
{
	VECTOR3 posPlayer = GetVec3Position();	// �v���C���[�ʒu
	VECTOR3 rotPlayer = GetVec3Rotation();	// �v���C���[����

	// �d�͂̍X�V
	UpdateGravity(fDeltaTime);

	// �ʒu�X�V
	UpdatePosition(&posPlayer, fDeltaTime);

	// ���n����
	UpdateLanding(&posPlayer, fDeltaTime);

	// �ʒu�␳
	CStage* pStage = GET_MANAGER->GetStage();	// �X�e�[�W���
	pStage->LimitPosition(posPlayer, RADIUS);

	// �����X�V
	UpdateDirection();

	// �ʒu�𔽉f
	SetVec3Position(posPlayer);

	// �����𔽉f
	SetVec3Rotation(rotPlayer);
}

//============================================================
//	�ߋ��ʒu�̍X�V����
//============================================================
void CPlayer::UpdateOldPosition()
{
	// �ߋ��ʒu���X�V
	m_oldPos = GetVec3Position();
}

//============================================================
//	�����̍X�V����
//============================================================
void CPlayer::UpdateDirection()
{
	if (m_bRight)
	{ // �E�����̏ꍇ

		BindTexture(TEXTURE_RIGHT);
	}
	else
	{ // �������̏ꍇ

		BindTexture(TEXTURE_LEFT);
	}
}

//============================================================
//	�ړ���/�ڕW�����̍X�V����
//============================================================
void CPlayer::UpdateMove(const float fDeltaTime)
{
	float fDeltaRate = fDeltaTime / (1.0f / (float)main::FPS);	// �o�ߎ��Ԃ̊���
	CInputKeyboard* pKey = GET_INPUTKEY;	// �L�[�{�[�h���
	CInputPad* pPad = GET_INPUTPAD;			// �p�b�h���
	if (pKey->IsPress(DIK_A) || pPad->IsPress(CInputPad::KEY_LEFT) || pPad->GetPressLStickX() < 0)
	{ // ���ړ��̑��삪�s��ꂽ�ꍇ

#ifdef JUMP_MOVE
		// �ړ��ʂ��X�V
		m_move.x -= MOVE * fDeltaRate;
#else
		// �ړ��ʂ��X�V
		if (!m_bJump) { m_move.x -= MOVE * fDeltaRate; }
#endif

		// �������ɂ���
		m_bRight = false;
	}
	else if (pKey->IsPress(DIK_D) || pPad->IsPress(CInputPad::KEY_RIGHT) || pPad->GetPressLStickX() > 0)
	{ // �E�ړ��̑��삪�s��ꂽ�ꍇ

#ifdef JUMP_MOVE
		// �ړ��ʂ��X�V
		m_move.x += MOVE * fDeltaRate;
#else
		// �ړ��ʂ��X�V
		if (!m_bJump) { m_move.x += MOVE * fDeltaRate; }
#endif

		// �E�����ɂ���
		m_bRight = true;
	}
}

//============================================================
//	�d�͂̍X�V����
//============================================================
void CPlayer::UpdateGravity(const float fDeltaTime)
{
	// �d�͂����Z
	float fDeltaRate = fDeltaTime / (1.0f / (float)main::FPS);	// �o�ߎ��Ԃ̊���
	m_move.y -= GRAVITY * fDeltaRate;
}

//============================================================
//	�W�����v�̍X�V����
//============================================================
void CPlayer::UpdateJump(const float fDeltaTime)
{
	CInputKeyboard* pKey = GET_INPUTKEY;	// �L�[�{�[�h���
	CInputPad* pPad = GET_INPUTPAD;			// �p�b�h���
	if (pKey->IsTrigger(DIK_SPACE) || pPad->IsTrigger(CInputPad::KEY_A))
	{
		if (!m_bJump)
		{ // �W�����v���Ă��Ȃ��ꍇ

			// �W�����v�ʂ�^����
			m_move.y = JUMP;

			// �W�����v�J�n����
			m_bJumpPress = m_bJump = true;

			// �W�����v���쎞�Ԃ�������
			m_fJumpTimer = 0.0f;

#ifndef JUMP_MOVE
			// �ړ��ʂ�������
			m_move.x = 0.0f;
			m_move.z = 0.0f;
#endif
		}
	}

	// �W�����v���Ԃ��o�߂�����
	m_fJumpTimer += fDeltaTime;

	if (m_bJumpPress && m_fJumpTimer < JUMP_PRESS_TIME)
	{ // �W�����v���삵�����Ă��銎�A�W�����v����̎�t���ԓ��̏ꍇ

		if (pKey->IsPress(DIK_SPACE) || pPad->IsPress(CInputPad::KEY_A))
		{ // �܂��v���X���̏ꍇ

			// �W�����v���Z�ʂ̊����v�Z
			float fJumpRate = useful::ValueToRate(m_fJumpTimer, JUMP_PRESS_TIME, 0.0f);

			// ��ړ��ʂ����Z
			float fDeltaRate = fDeltaTime / (1.0f / (float)main::FPS);
			m_move.y += (MAX_ADD_JUMP * fJumpRate) * fDeltaRate;
		}
		else
		{ // �v���X���ł͂Ȃ��ꍇ

			// �W�����v����OFF�ɂ���
			m_bJumpPress = false;
		}
	}
	else
	{ // �W�����v����ł��Ȃ��ꍇ

		// �W�����v����OFF�ɂ���
		m_bJumpPress = false;
	}
}

//============================================================
//	���n�󋵂̍X�V����
//============================================================
bool CPlayer::UpdateLanding(VECTOR3* pPos, const float fDeltaTime)
{
	CStage* pStage = GET_MANAGER->GetStage();	// �X�e�[�W���
	bool bLand = false;	// ���n�t���O

	// �n�ʁE�����ʒu���n����
	if (pStage->LandFieldPosition(*pPos, m_oldPos, m_move)
	||  pStage->LandLimitPosition(*pPos, m_move, 0.0f))
	{ // �v���C���[�����n���Ă����ꍇ

		// ���n���Ă����Ԃɂ���
		bLand = true;

		// �W�����v���Ă��Ȃ���Ԃɂ���
		m_bJumpPress = m_bJump = false;

		// �W�����v���쎞�Ԃ�������
		m_fJumpTimer = 0.0f;
	}

	// ���n�t���O��Ԃ�
	return bLand;
}

//============================================================
//	�ʒu�̍X�V����
//============================================================
void CPlayer::UpdatePosition(VECTOR3* pPos, const float fDeltaTime)
{
	float fDeltaRate = fDeltaTime / (1.0f / (float)main::FPS);	// �o�ߎ��Ԃ̊���

	// �ړ��ʂ����Z
	*pPos += m_move * fDeltaRate;

	// �ړ��ʂ�����
	if (m_bJump)
	{ // �󒆂̏ꍇ

#ifdef JUMP_MOVE
		m_move.x += (0.0f - m_move.x) * JUMP_REV * fDeltaRate;
		m_move.z += (0.0f - m_move.z) * JUMP_REV * fDeltaRate;
#else
		m_move.x = 0.0f;
		m_move.z = 0.0f;
#endif
	}
	else
	{ // �n��̏ꍇ

		m_move.x += (0.0f - m_move.x) * LAND_REV * fDeltaRate;
		m_move.z += (0.0f - m_move.z) * LAND_REV * fDeltaRate;
	}
}
