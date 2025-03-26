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
#include "block.h"
#include "bullet.h"
#include "comboArea.h"
#include "collision.h"

//************************************************************
//	�}�N����`
//************************************************************
// �W�����v���ړ�ON/OFF
#if 1
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
	const float	JUMP		= 4.7f;		// �W�����v��
	const float	GRAVITY		= 0.39f;	// �d��
	const float	RADIUS		= 20.0f;	// ���a
	const float WIDTH		= 70.0f;	// ����
	const float HEIGHT		= 70.0f;	// �c��
	const float	REV_ROTA	= 0.25f;	// �����ύX�̕␳�W��
	const float	JUMP_REV	= 0.955f;	// �ʏ��Ԏ��̋󒆂̈ړ��ʂ̌����W��
	const float	LAND_REV	= 0.18f;	// �ʏ��Ԏ��̒n��̈ړ��ʂ̌����W��
	const float	HOVER_PRESS_TIME	= 0.65f;	// �z�o�[����
	const float HOVER_MIN_GRAVITY	= -2.45f;	// �z�o�[���̕␳�d��
	const float	JUMP_PRESS_TIME		= 0.46f;	// �W�����v���Z����
	const float	MAX_ADD_JUMP		= 1.38f;	// �W�����v���Z�ő�l
	const float	SHOT_INTERVAL		= 0.4f;		// �U���C���^�[�o��
	const float	BULLET_OFFSET_X		= 8.0f;		// �e�������W��X�I�t�Z�b�g

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
	m_oldPos		(VEC3_ZERO),	// �ߋ��ʒu
	m_move			(VEC3_ZERO),	// �ړ���
	m_state			(STATE_NONE),	// ���
	m_bRight		(false),		// ���E�t���O
	m_bJump			(false),		// ���݃W�����v��
	m_bOldJump		(false),		// �ߋ��W�����v��
	m_bReleaseJump	(false),		// �W�����v��������t���O
	m_bJumpPress	(false),		// �W�����v����t���O
	m_fJumpTimer	(0.0f),			// �W�����v���쎞��
	m_fShotTimer	(0.0f),			// �U���C���^�[�o��
	m_fHoverTimer	(0.0f),			// �z�o�[����
	m_fMaxMulti		(0.0f)			// �W�����v���̍ō��{��
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
	m_oldPos		= VEC3_ZERO;	// �ߋ��ʒu
	m_move			= VEC3_ZERO;	// �ړ���
	m_state			= STATE_NORMAL;	// ���
	m_bRight		= false;		// ���E�t���O
	m_bJump			= true;			// �W�����v��
	m_bOldJump		= true;			// �ߋ��W�����v��
	m_bReleaseJump	= false;		// �W�����v��������t���O
	m_bJumpPress	= false;		// �W�����v����t���O
	m_fJumpTimer	= 0.0f;			// �W�����v���쎞��
	m_fShotTimer	= 0.0f;			// �U���C���^�[�o��
	m_fHoverTimer	= 0.0f;			// �z�o�[����
	m_fMaxMulti		= 0.0f;			// �W�����v���̍ō��{��

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

	// �ߋ��W�����v�󋵂̍X�V
	m_bOldJump = m_bJump;

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
//	�u���b�N�Ƃ̓����蔻��
//============================================================
bool CPlayer::CollisionBlock(const VECTOR3& rPos)
{
	// �u���b�N���Ȃ��ꍇ������
	CListManager<CBlock>* pList = CBlock::GetList();
	if (pList == nullptr) { return false; }

	// �������X�g���擾
	std::list<CBlock*> listBlock = pList->GetList();

	for (const auto& rBlock : listBlock)
	{ // �v�f�����J��Ԃ�

		// XY���ʂ̓����蔻��
		bool bHit = collision::BoxXY
		( // ����
			rPos,
			rBlock->GetVec3Position() - VECTOR3(0.0f, HEIGHT * 0.5f, 0.0f),
			GetVec3Size() * 0.5f,
			GetVec3Size() * 0.5f,
			rBlock->GetVec3Size() * 0.5f,
			rBlock->GetVec3Size() * 0.5f
		);
		if (bHit)
		{ // ���������ꍇ

			// �q�b�g����
			Hit();
			return true;
		}
	}

	return false;
}

//============================================================
//	�{���G���A�Ƃ̓����蔻��
//============================================================
void CPlayer::CollisionMulti(const VECTOR3& rPos)
{
	// �W�����v���ł͂Ȃ��ꍇ������
	if (!m_bJump) { return; }

	// �{���G���A���Ȃ��ꍇ������
	CListManager<CComboArea>* pList = CComboArea::GetList();
	if (pList == nullptr) { return; }

	// �������X�g���擾
	std::list<CComboArea*> listArea = pList->GetList();

	float fMaxMulti = 0.0f;	// �ō��{��
	for (const auto& rArea : listArea)
	{ // �v�f�����J��Ԃ�

		// XY���ʂ̓����蔻��
		bool bHit = collision::BoxXY
		( // ����
			rPos,
			rArea->GetVec3Position() - VECTOR3(0.0f, HEIGHT * 0.5f, 0.0f),
			GetVec3Size() * 0.5f,
			GetVec3Size() * 0.5f,
			rArea->GetVec3Size() * 0.5f,
			rArea->GetVec3Size() * 0.5f
		);

		// �������Ă��Ȃ��ꍇ����
		if (!bHit) { continue; }

		const float fCurMulti = rArea->GetMulti();	// �G���A�{��
		if (fMaxMulti < fCurMulti)
		{ // ���傫���{�����������ꍇ

			// �{�����X�V
			fMaxMulti = fCurMulti;
		}
	}

	if (fMaxMulti > m_fMaxMulti)
	{ // �{�������݂̍ő�{�����������ꍇ

		// �{�����X�V
		m_fMaxMulti = fMaxMulti;
	}
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

	// �����𔽉f
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

	// �{���G���A�Ƃ̓����蔻��
	CollisionMulti(posPlayer);

	// �R���{�{���̐ݒ�
	CSceneGame::GetGameManager()->SetComboValue(m_fMaxMulti);

	// ���n����
	UpdateLanding(&posPlayer, fDeltaTime);

	// �ʒu�␳
	CStage* pStage = GET_MANAGER->GetStage();	// �X�e�[�W���
	pStage->LimitPosition(posPlayer, RADIUS);

	// �u���b�N�Ƃ̓����蔻��
	CollisionBlock(posPlayer);

	// �U���̍X�V
	UpdateShot(posPlayer, fDeltaTime);

	// �����𔽉f
	UpdateDirection();

	// �ʒu�𔽉f
	SetVec3Position(posPlayer);

	// �����𔽉f
	SetVec3Rotation(rotPlayer);

	GET_MANAGER->GetDebugProc()->Print(CDebugProc::POINT_RIGHT, "�X�R�A�{���F[%f]\n", m_fMaxMulti);
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

	// �����𔽉f
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

			// �W�����v��������t���O��������
			m_bReleaseJump = false;

			// �W�����v���쎞�Ԃ�������
			m_fJumpTimer = 0.0f;

#ifndef JUMP_MOVE
			// �ړ��ʂ�������
			m_move.x = 0.0f;
			m_move.z = 0.0f;
#endif
		}
	}

	if (pKey->IsRelease(DIK_SPACE) || pPad->IsRelease(CInputPad::KEY_A))
	{ // �܂��v���X���̏ꍇ

		// �W�����v��������ς݂ɂ���
		m_bReleaseJump = true;
	}

	if (m_bReleaseJump)
	{ // �W�����v����������ς݂̏ꍇ

		if (pKey->IsPress(DIK_SPACE) || pPad->IsPress(CInputPad::KEY_A))
		{ // �܂��v���X���̏ꍇ

			// �z�o�[���Ԃ��o�߂�����
			m_fHoverTimer += fDeltaTime;

			if (m_bJump								// �W�����v��
			&&  m_move.y <= HOVER_MIN_GRAVITY		// �d�͂ɕ����n�߂Ă���
			&&  m_fHoverTimer < HOVER_PRESS_TIME)	// �z�o�[���Ԃ��c���Ă���
			{ // ��L�̏ꍇ

				// �ړ��ʂ�␳
				m_move.y = -2.45f;
			}
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
//	�U���̍X�V����
//============================================================
void CPlayer::UpdateShot(const VECTOR3& rPos, const float fDeltaTime)
{
	// �W�����v���Ă��Ȃ��ꍇ������
	if (!m_bJump) { return; }

	// �C���^�[�o������o�ߎ��ԕ����Z
	m_fShotTimer -= fDeltaTime;
	useful::LimitMinNum(m_fShotTimer, 0.0f);

	CInputKeyboard* pKey = GET_INPUTKEY;	// �L�[�{�[�h���
	CInputPad* pPad = GET_INPUTPAD;			// �p�b�h���
	if (pKey->IsPress(DIK_RETURN) || pPad->IsPress(CInputPad::KEY_B))
	{
		if (m_fShotTimer <= 0.0f)
		{ // �U���\�ȏꍇ

			// �e�����ʒu���v�Z
			VECTOR3 posShot = rPos;		// �v���C���[�ʒu���_
			posShot.y += HEIGHT * 0.5f;	// �v���C���[���SY���W�ɂ���
			posShot.x += (m_bRight) ? BULLET_OFFSET_X : -BULLET_OFFSET_X;	// �������WX�ɃI�t�Z�b�g��^����

			// �e�̐���
			CBullet::Create(posShot, m_bRight);

			// �C���^�[�o����ݒ�
			m_fShotTimer = SHOT_INTERVAL;
		}
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

		CGameManager* pGameManager = CSceneGame::GetGameManager();	// �Q�[���}�l�[�W���[
		if (pGameManager != nullptr)
		{ // �Q�[���}�l�[�W���[������ꍇ

			// �X�R�A�����Z
			pGameManager->AddScore((int)((float)pGameManager->GetBaseScore() * m_fMaxMulti));
			if (m_bOldJump)
			{ // �O�t���[���Œ��n���Ă��Ȃ��ꍇ

				// ��b�X�R�A��������
				pGameManager->InitBaseScore();
			}
		}

		// ���n���Ă����Ԃɂ���
		bLand = true;

		// �W�����v���Ă��Ȃ���Ԃɂ���
		m_bJumpPress = m_bJump = false;

		// �W�����v���쎞�Ԃ�������
		m_fJumpTimer = 0.0f;

		// �U���C���^�[�o����������
		m_fShotTimer = 0.0f;

		// �z�o�[���Ԃ�������
		m_fHoverTimer = 0.0f;

		// �{����������
		m_fMaxMulti = 0.0f;
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
