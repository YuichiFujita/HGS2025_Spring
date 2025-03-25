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
//	�萔�錾
//************************************************************
namespace
{
	const char*	SETUP_TXT	= "data\\CHARACTER\\player.txt";	// �Z�b�g�A�b�v�e�L�X�g���΃p�X
	const int	PRIORITY	= 3;		// �v���C���[�̗D�揇��
	const float	MOVE		= 1.5f;		// �ړ���
	const float	GRAVITY		= 0.6f;		// �d��
	const float	RADIUS		= 20.0f;	// ���a
	const float HEIGHT		= 80.0f;	// �g��
	const float	REV_ROTA	= 0.25f;	// �����ύX�̕␳�W��
	const float	JUMP_REV	= 0.18f;	// �ʏ��Ԏ��̋󒆂̈ړ��ʂ̌����W��
	const float	LAND_REV	= 0.18f;	// �ʏ��Ԏ��̒n��̈ړ��ʂ̌����W��

	namespace camera
	{
		const CCamera::SSwing HIT_SWING = CCamera::SSwing(8.0f, 1.8f, 0.14f);	// �q�b�g���̃J�����h��
	}

	namespace motion
	{
		const int BLEND_FRAME_OTHER = 5;	// ���[�V�����̊�{�I�ȃu�����h�t���[��
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
CPlayer::CPlayer() : CObjectChara(CObject::LABEL_PLAYER, CObject::DIM_3D, PRIORITY),
	m_oldPos	(VEC3_ZERO),	// �ߋ��ʒu
	m_move		(VEC3_ZERO),	// �ړ���
	m_destRot	(VEC3_ZERO),	// �ڕW����
	m_state		(STATE_NONE),	// ���
	m_bJump		(false)			// �W�����v��
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
	m_oldPos	= VEC3_ZERO;	// �ߋ��ʒu
	m_move		= VEC3_ZERO;	// �ړ���
	m_destRot	= VEC3_ZERO;	// �ڕW����
	m_state		= STATE_NORMAL;	// ���
	m_bJump		= true;			// �W�����v��

	// �I�u�W�F�N�g�L�����N�^�[�̏�����
	if (FAILED(CObjectChara::Init()))
	{ // �������Ɏ��s�����ꍇ

		// ���s��Ԃ�
		assert(false);
		return E_FAIL;
	}

	// �L�����N�^�[���̊���
	BindCharaData(SETUP_TXT);

	// �������[�V�����̊���
	SetMotion(MOTION_IDOL);

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
	CObjectChara::Uninit();
}

//============================================================
//	�X�V����
//============================================================
void CPlayer::Update(const float fDeltaTime)
{
	// �ߋ��ʒu�̍X�V
	UpdateOldPosition();

	// ��Ԃ̍X�V
	EMotion curMotion = (this->*(m_aFuncState[m_state]))(fDeltaTime);

	// ���[�V�����E�I�u�W�F�N�g�L�����N�^�[�̍X�V
	UpdateMotion(curMotion, fDeltaTime);
}

//============================================================
//	�`�揈��
//============================================================
void CPlayer::Draw(CShader* pShader)
{
	// �I�u�W�F�N�g�L�����N�^�[�̕`��
	CObjectChara::Draw(pShader);
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
		pPlayer->m_destRot = rRot;	// �ڕW����������̌����ɂ���

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
CPlayer::EMotion CPlayer::UpdateNone(const float fDeltaTime)
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
	UpdateRotation(&rotPlayer, fDeltaTime);

	// �ʒu�𔽉f
	SetVec3Position(posPlayer);

	// �����𔽉f
	SetVec3Rotation(rotPlayer);

	// �ҋ@���[�V������Ԃ�
	return MOTION_IDOL;
}

//============================================================
//	�ʏ��Ԏ��̍X�V����
//============================================================
CPlayer::EMotion CPlayer::UpdateNormal(const float fDeltaTime)
{
	// ����s�\�ȏꍇ������
	CGameManager* pGameManager = CSceneGame::GetGameManager();	// �Q�[���}�l�[�W���[
	if (pGameManager != nullptr)
	{ // �Q�[���}�l�[�W���[������ꍇ

		if (!pGameManager->IsControlOK()) { return MOTION_IDOL; }
	}

	EMotion curMotion = MOTION_IDOL;		// ���݂̃��[�V����
	VECTOR3 posPlayer = GetVec3Position();	// �v���C���[�ʒu
	VECTOR3 rotPlayer = GetVec3Rotation();	// �v���C���[����

	// �ړ�����
	curMotion = UpdateMove(fDeltaTime);

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
	UpdateRotation(&rotPlayer, fDeltaTime);

	// �ʒu�𔽉f
	SetVec3Position(posPlayer);

	// �����𔽉f
	SetVec3Rotation(rotPlayer);

	// ���݂̃��[�V������Ԃ�
	return curMotion;
}

//============================================================
//	���S��Ԏ��̍X�V����
//============================================================
CPlayer::EMotion CPlayer::UpdateDeath(const float fDeltaTime)
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
	UpdateRotation(&rotPlayer, fDeltaTime);

	// �ʒu�𔽉f
	SetVec3Position(posPlayer);

	// �����𔽉f
	SetVec3Rotation(rotPlayer);

	// ���S���[�V������Ԃ�
	//return MOTION_DEATH;	// TODO�F���S�ł�����ύX
	return MOTION_IDOL;
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
//	�ړ���/�ڕW�����̍X�V����
//============================================================
CPlayer::EMotion CPlayer::UpdateMove(const float fDeltaTime)
{
	float fDeltaRate = fDeltaTime / (1.0f / (float)main::FPS);	// �o�ߎ��Ԃ̊���
	CInputKeyboard* pKey = GET_INPUTKEY;			// �L�[�{�[�h���
	CInputPad* pPad = GET_INPUTPAD;					// �p�b�h���
	CCamera* pCamera = GET_MANAGER->GetCamera();	// �J�������
	float fCameraRotY = pCamera->GetRotation().y;	// �J��������
	EMotion curMotion = MOTION_IDOL;				// ���݂̃��[�V����
	if (pKey->IsPress(DIK_W) || pPad->IsPress(CInputPad::KEY_UP) || pPad->GetPressLStickY() < 0)
	{ // ���ړ��̑��삪�s��ꂽ�ꍇ

		// �ړ����[�V�����ɂ���
		curMotion = MOTION_WALK;

		if (pKey->IsPress(DIK_A) || pPad->IsPress(CInputPad::KEY_LEFT) || pPad->GetPressLStickX() < 0)
		{ // ���ړ��̑�����s��ꂽ�ꍇ (�����ړ�)

			// �ړ��ʂ��X�V
			m_move.x += sinf(fCameraRotY - (D3DX_PI * 0.25f)) * MOVE * fDeltaRate;
			m_move.z += cosf(fCameraRotY - (D3DX_PI * 0.25f)) * MOVE * fDeltaRate;

			// �ڕW�������X�V
			m_destRot.y = D3DXToRadian(135) + fCameraRotY;
		}
		else if (pKey->IsPress(DIK_D) || pPad->IsPress(CInputPad::KEY_RIGHT) || pPad->GetPressLStickX() > 0)
		{ // �E�ړ��̑�����s��ꂽ�ꍇ (�E���ړ�)

			// �ړ��ʂ��X�V
			m_move.x -= sinf(fCameraRotY - (D3DX_PI * 0.75f)) * MOVE * fDeltaRate;
			m_move.z -= cosf(fCameraRotY - (D3DX_PI * 0.75f)) * MOVE * fDeltaRate;

			// �ڕW�������X�V
			m_destRot.y = D3DXToRadian(225) + fCameraRotY;
		}
		else
		{ // ���ړ��̑��삾�����s��ꂽ�ꍇ (���ړ�)

			// �ړ��ʂ��X�V
			m_move.x += sinf(fCameraRotY) * MOVE * fDeltaRate;
			m_move.z += cosf(fCameraRotY) * MOVE * fDeltaRate;

			// �ڕW�������X�V
			m_destRot.y = D3DXToRadian(180) + fCameraRotY;
		}
	}
	else if (pKey->IsPress(DIK_S) || pPad->IsPress(CInputPad::KEY_DOWN) || pPad->GetPressLStickY() > 0)
	{ // ��O�ړ��̑��삪�s��ꂽ�ꍇ

		// �ړ����[�V�����ɂ���
		curMotion = MOTION_WALK;

		if (pKey->IsPress(DIK_A) || pPad->IsPress(CInputPad::KEY_LEFT) || pPad->GetPressLStickX() < 0)
		{ // ���ړ��̑�����s��ꂽ�ꍇ (����O�ړ�)

			// �ړ��ʂ��X�V
			m_move.x += sinf(fCameraRotY - (D3DX_PI * 0.75f)) * MOVE * fDeltaRate;
			m_move.z += cosf(fCameraRotY - (D3DX_PI * 0.75f)) * MOVE * fDeltaRate;

			// �ڕW�������X�V
			m_destRot.y = D3DXToRadian(45) + fCameraRotY;
		}
		else if (pKey->IsPress(DIK_D) || pPad->IsPress(CInputPad::KEY_RIGHT) || pPad->GetPressLStickX() > 0)
		{ // �E�ړ��̑�����s��ꂽ�ꍇ (�E��O�ړ�)

			// �ړ��ʂ��X�V
			m_move.x -= sinf(fCameraRotY - (D3DX_PI * 0.25f)) * MOVE * fDeltaRate;
			m_move.z -= cosf(fCameraRotY - (D3DX_PI * 0.25f)) * MOVE * fDeltaRate;

			// �ڕW�������X�V
			m_destRot.y = D3DXToRadian(315) + fCameraRotY;
		}
		else
		{ // ��O�ړ��̑��삾�����s��ꂽ�ꍇ (��O�ړ�)

			// �ړ��ʂ��X�V
			m_move.x -= sinf(fCameraRotY) * MOVE * fDeltaRate;
			m_move.z -= cosf(fCameraRotY) * MOVE * fDeltaRate;

			// �ڕW�������X�V
			m_destRot.y = D3DXToRadian(0) + fCameraRotY;
		}
	}
	else if (pKey->IsPress(DIK_A) || pPad->IsPress(CInputPad::KEY_LEFT) || pPad->GetPressLStickX() < 0)
	{ // ���ړ��̑��삪�s��ꂽ�ꍇ

		// �ړ����[�V�����ɂ���
		curMotion = MOTION_WALK;

		// �ړ��ʂ��X�V
		m_move.x += sinf(fCameraRotY - (D3DX_PI * 0.5f)) * MOVE * fDeltaRate;
		m_move.z += cosf(fCameraRotY - (D3DX_PI * 0.5f)) * MOVE * fDeltaRate;

		// �ڕW�������X�V
		m_destRot.y = D3DXToRadian(90) + fCameraRotY;
	}
	else if (pKey->IsPress(DIK_D) || pPad->IsPress(CInputPad::KEY_RIGHT) || pPad->GetPressLStickX() > 0)
	{ // �E�ړ��̑��삪�s��ꂽ�ꍇ

		// �ړ����[�V�����ɂ���
		curMotion = MOTION_WALK;

		// �ړ��ʂ��X�V
		m_move.x -= sinf(fCameraRotY - (D3DX_PI * 0.5f)) * MOVE * fDeltaRate;
		m_move.z -= cosf(fCameraRotY - (D3DX_PI * 0.5f)) * MOVE * fDeltaRate;

		// �ڕW�������X�V
		m_destRot.y = D3DXToRadian(270) + fCameraRotY;
	}

	// ���[�V������Ԃ�
	return curMotion;
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
		m_bJump = false;
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

		m_move.x += (0.0f - m_move.x) * JUMP_REV * fDeltaRate;
		m_move.z += (0.0f - m_move.z) * JUMP_REV * fDeltaRate;
	}
	else
	{ // �n��̏ꍇ

		m_move.x += (0.0f - m_move.x) * LAND_REV * fDeltaRate;
		m_move.z += (0.0f - m_move.z) * LAND_REV * fDeltaRate;
	}
}

//============================================================
//	�����̍X�V����
//============================================================
void CPlayer::UpdateRotation(VECTOR3* pRot, const float fDeltaTime)
{
	float fDeltaRate = fDeltaTime / (1.0f / (float)main::FPS);	// �o�ߎ��Ԃ̊���
	float fDiffRot = 0.0f;	// ��������

	// �ڕW�����̐��K��
	useful::NormalizeRot(m_destRot.y);

	// �ڕW�����܂ł̍������v�Z
	fDiffRot = m_destRot.y - pRot->y;

	// ���������̐��K��
	useful::NormalizeRot(fDiffRot);

	// �����̍X�V
	pRot->y += fDiffRot * REV_ROTA * fDeltaRate;

	// �����̐��K��
	useful::NormalizeRot(pRot->y);
}

//============================================================
//	���[�V����/�L�����N�^�[�̍X�V����
//============================================================
void CPlayer::UpdateMotion(int nMotion, const float fDeltaTime)
{
	// ����ł��甲����
	if (IsDeath()) { return; }

	int nAnimMotion = GetMotionType();	// ���ݍĐ����̃��[�V����
	if (nMotion != NONE_IDX)
	{ // ���[�V�������ݒ肳��Ă���ꍇ

		if (IsMotionLoop())
		{ // ���[�v���郂�[�V�������̏ꍇ

			if (nAnimMotion != nMotion)
			{ // ���݂̃��[�V�������Đ����̃��[�V�����ƈ�v���Ȃ��ꍇ

				// ���݂̃��[�V�����̐ݒ�
				SetMotion(nMotion, motion::BLEND_FRAME_OTHER);
			}
		}
		else
		{ // ���[�v���Ȃ����[�V�������̏ꍇ

#if 0
			switch (GetMotionType())
			{ // ���[�V�������Ƃ̏���
			case MOTION_:

				if (nMotion != MOTION_IDOL)
				{ // �ҋ@���[�V�����ȊO�̏ꍇ

					// ���݂̃��[�V�����̐ݒ�
					SetMotion(nMotion, motion::BLEND_FRAME_OTHER);
				}
				break;
			}
#endif
		}
	}

	// �I�u�W�F�N�g�L�����N�^�[�̍X�V
	CObjectChara::Update(fDeltaTime);

	switch (GetMotionType())
	{ // ���[�V�����̎�ނ��Ƃ̏���
	case MOTION_IDOL:
		break;

	case MOTION_WALK:
		break;
	}
}
