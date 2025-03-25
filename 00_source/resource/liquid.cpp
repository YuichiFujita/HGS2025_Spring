//============================================================
//
//	�t�̏��� [liquid.cpp]
//	Author�F���c�E��
//
//============================================================
//************************************************************
//	�C���N���[�h�t�@�C��
//************************************************************
#include "liquid.h"
#include "manager.h"
#include "renderer.h"
#include "scrollMeshField.h"

//************************************************************
//	�萔�錾
//************************************************************
namespace
{
	const char *TEXTURE_FILE[][CLiquid::LIQUID_MAX] =	// �e�N�X�`���t�@�C��
	{
		{ // ���e�N�X�`��
			"data\\TEXTURE\\sea000.jpg",
			"data\\TEXTURE\\sea000.png",
		},
	};

	const int PRIORITY = 0;	// �t�̗̂D�揇��
}

//************************************************************
//	�X�^�e�B�b�N�A�T�[�g
//************************************************************
static_assert(NUM_ARRAY(TEXTURE_FILE) == CLiquid::TYPE_MAX, "ERROR : Type Count Mismatch");

//************************************************************
//	�ÓI�����o�ϐ��錾
//************************************************************
CListManager<CLiquid>* CLiquid::m_pList = nullptr;	// �I�u�W�F�N�g���X�g

//************************************************************
//	�q�N���X [CLiquid] �̃����o�֐�
//************************************************************
//============================================================
//	�R���X�g���N�^
//============================================================
CLiquid::CLiquid() : CObject(CObject::LABEL_LIQUID, CObject::DIM_3D, PRIORITY),
	m_type			(TYPE_WATER),	// ���
	m_fMaxUp		(0.0f),			// �g�̍ō��㏸��
	m_fSinRot		(0.0f),			// �g�ł�����
	m_fAddSinRot	(0.0f),			// �g�ł��������Z��
	m_fAddVtxRot	(0.0f)			// �הg�̌������Z��
{
	// �����o�ϐ����N���A
	memset(&m_apLiquid[0], 0, sizeof(m_apLiquid));	// �t�̂̏��
}

//============================================================
//	�f�X�g���N�^
//============================================================
CLiquid::~CLiquid()
{

}

//============================================================
//	����������
//============================================================
HRESULT CLiquid::Init()
{
	// �����o�ϐ���������
	memset(&m_apLiquid[0], 0, sizeof(m_apLiquid));	// �t�̂̏��
	m_type			= TYPE_WATER;	// ���
	m_fMaxUp		= 0.0f;			// �g�̍ō��㏸��
	m_fSinRot		= 0.0f;			// �g�ł�����
	m_fAddSinRot	= 0.0f;			// �g�ł��������Z��
	m_fAddVtxRot	= 0.0f;			// �הg�̌������Z��

	for (int nCntLiquid = 0; nCntLiquid < LIQUID_MAX; nCntLiquid++)
	{ // �t�̂̍ő吔���J��Ԃ�

		// �t�̂̐���
		m_apLiquid[nCntLiquid] = CScrollMeshField::Create
		( // ����
			0.0f,			// �����W�̈ړ���
			0.0f,			// �c���W�̈ړ���
			VEC3_ZERO,		// �ʒu
			VEC3_ZERO,		// ����
			VEC2_ZERO,		// �傫��
			color::White(),	// �F
			GRID2_ONE		// ������
		);
		if (m_apLiquid[nCntLiquid] == nullptr)
		{ // �����Ɏ��s�����ꍇ

			// ���s��Ԃ�
			assert(false);
			return E_FAIL;
		}

		// �e�N�X�`����o�^�E����
		m_apLiquid[nCntLiquid]->BindTexture(TEXTURE_FILE[m_type][nCntLiquid]);

		// �D�揇�ʂ�e�N���X�ƈ�v������
		m_apLiquid[nCntLiquid]->SetPriority(PRIORITY);

		// �����X�V�E�����`���OFF�ɂ���
		m_apLiquid[nCntLiquid]->SetEnableUpdate(false);
		m_apLiquid[nCntLiquid]->SetEnableDraw(false);
	}

	if (m_pList == nullptr)
	{ // ���X�g�}�l�[�W���[�����݂��Ȃ��ꍇ

		// ���X�g�}�l�[�W���[�̐���
		m_pList = CListManager<CLiquid>::Create();
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
void CLiquid::Uninit()
{
	for (int nCntLiquid = 0; nCntLiquid < LIQUID_MAX; nCntLiquid++)
	{ // �t�̂̍ő吔���J��Ԃ�

		// �t�̂̏I��
		SAFE_UNINIT(m_apLiquid[nCntLiquid]);
	}

	// ���X�g���玩�g�̃I�u�W�F�N�g���폜
	m_pList->DelList(m_iterator);

	if (m_pList->GetNumAll() == 0)
	{ // �I�u�W�F�N�g������Ȃ��ꍇ

		// ���X�g�}�l�[�W���[�̔j��
		m_pList->Release(m_pList);
	}

	// ���g�̃I�u�W�F�N�g��j��
	Release();
}

//============================================================
//	�X�V����
//============================================================
void CLiquid::Update(const float fDeltaTime)
{
	POSGRID2 part = GetPattern();	// ������
	VECTOR3 pos;	// ���_���W�擾�p
	int nLine;		// ���_�̍s
	float fSinRot;	// �T�C���J�[�u�Ɏg�p����p�x

	// ���b�V���̒��_���W�̍X�V
	for (int nCntVtx = 0; nCntVtx < (part.x + 1) * (part.y + 1); nCntVtx++)
	{ // �g�p���_�����J��Ԃ�

		for (int nCntLiquid = 0; nCntLiquid < LIQUID_MAX; nCntLiquid++)
		{ // �t�̂̍ő吔���J��Ԃ�

			// ���_���W���擾
			pos = m_apLiquid[nCntLiquid]->GetMeshVertexPosition(nCntVtx);

			// �g�ł��p�̊p�x�����߂�
			nLine = nCntVtx / (part.x + 1);				// ���_�̍s
			fSinRot = m_fSinRot + nLine * m_fAddVtxRot;	// �T�C���J�[�u�Ɏg�p����p�x

			// �T�C���J�[�u�p�̌�����␳
			useful::NormalizeRot(fSinRot);

			// ���_���W��ݒ�
			m_apLiquid[nCntLiquid]->SetMeshVertexPosition(nCntVtx, VECTOR3(pos.x, (sinf(fSinRot) * m_fMaxUp) + m_fMaxUp, pos.z));
		}
	}

	// �g�ł����������Z
	m_fSinRot += m_fAddSinRot;

	// ������␳
	useful::NormalizeRot(m_fSinRot);

	for (int nCntLiquid = 0; nCntLiquid < LIQUID_MAX; nCntLiquid++)
	{ // �t�̂̍ő吔���J��Ԃ�

		// �t�̂̍X�V
		m_apLiquid[nCntLiquid]->Update(fDeltaTime);
	}
}

//============================================================
//	�`�揈��
//============================================================
void CLiquid::Draw(CShader* pShader)
{
	for (int nCntLiquid = 0; nCntLiquid < LIQUID_MAX; nCntLiquid++)
	{ // �t�̂̍ő吔���J��Ԃ�

		// �t�̂̕`��
		m_apLiquid[nCntLiquid]->Draw(pShader);
	}
}

//============================================================
//	�ʒu�̐ݒ菈��
//============================================================
void CLiquid::SetVec3Position(const VECTOR3& rPos)
{
	// �t�̂̈ʒu��ݒ�
	for (int nCntLiquid = 0; nCntLiquid < LIQUID_MAX; nCntLiquid++)
	{ // �t�̂̍ő吔���J��Ԃ�

		// �����̈ʒu��ݒ�
		m_apLiquid[nCntLiquid]->SetVec3Position(rPos);
	}
}

//============================================================
//	�����̐ݒ菈��
//============================================================
void CLiquid::SetVec3Rotation(const VECTOR3& rRot)
{
	// �t�̂̌�����ݒ�
	for (int nCntLiquid = 0; nCntLiquid < LIQUID_MAX; nCntLiquid++)
	{ // �t�̂̍ő吔���J��Ԃ�

		// �����̌�����ݒ�
		m_apLiquid[nCntLiquid]->SetVec3Rotation(rRot);
	}
}

//============================================================
//	�傫���̐ݒ菈��
//============================================================
void CLiquid::SetVec2Size(const VECTOR2& rSize)
{
	// �t�̂̑傫����ݒ�
	for (int nCntLiquid = 0; nCntLiquid < LIQUID_MAX; nCntLiquid++)
	{ // �t�̂̍ő吔���J��Ԃ�

		// �����̑傫����ݒ�
		m_apLiquid[nCntLiquid]->SetVec2Size(rSize);
	}
}

//============================================================
//	��������
//============================================================
CLiquid* CLiquid::Create
(
	const EType type,			// ���
	const VECTOR3& rPos,		// �ʒu
	const VECTOR3& rRot,		// ����
	const VECTOR2& rSize,		// �傫��
	const COLOR& rCol,			// �F
	const POSGRID2& rPart,		// ������
	const STexMove& rTexMove,	// �e�N�X�`���ړ���
	const float fMaxUp,			// �g�̍ō��㏸��
	const float fAddSinRot,		// �g�ł��������Z��
	const float fAddVtxRot		// �הg�̌������Z��
)
{
	// �t�̂̐���
	CLiquid* pLiquid = new CLiquid;
	if (pLiquid == nullptr)
	{ // �����Ɏ��s�����ꍇ

		return nullptr;
	}
	else
	{ // �����ɐ��������ꍇ

		// �t�̂̏�����
		if (FAILED(pLiquid->Init()))
		{ // �������Ɏ��s�����ꍇ

			// �t�̂̔j��
			SAFE_DELETE(pLiquid);
			return nullptr;
		}

		// ��ނ�ݒ�
		pLiquid->SetType(type);

		// �ʒu��ݒ�
		pLiquid->SetVec3Position(rPos);

		// ������ݒ�
		pLiquid->SetVec3Rotation(rRot);

		// �傫����ݒ�
		pLiquid->SetVec2Size(rSize);

		// �F��ݒ�
		pLiquid->SetColor(rCol);

		// �e�N�X�`���ړ��ʂ�ݒ�
		pLiquid->SetTexMove(rTexMove);

		// �g�̍ō��㏸�ʂ�ݒ�
		pLiquid->SetMaxUp(fMaxUp);

		// �g�ł��������Z�ʂ�ݒ�
		pLiquid->SetAddSinRot(fAddSinRot);

		// �הg�̌������Z�ʂ�ݒ�
		pLiquid->SetAddVtxRot(fAddVtxRot);

		// ��������ݒ�
		if (FAILED(pLiquid->SetPattern(rPart)))
		{ // �������̐ݒ�Ɏ��s�����ꍇ

			// �t�̂̔j��
			SAFE_DELETE(pLiquid);
			return nullptr;
		}

		// �m�ۂ����A�h���X��Ԃ�
		return pLiquid;
	}
}

//============================================================
//	���X�g�擾����
//============================================================
CListManager<CLiquid>* CLiquid::GetList()
{
	// �I�u�W�F�N�g���X�g��Ԃ�
	return m_pList;
}

//============================================================
//	�F�̐ݒ菈��
//============================================================
void CLiquid::SetColor(const COLOR& rCol)
{
	// �t�̂̐F��ݒ�
	for (int nCntLiquid = 0; nCntLiquid < LIQUID_MAX; nCntLiquid++)
	{ // �t�̂̍ő吔���J��Ԃ�

		// �����̐F��ݒ�
		m_apLiquid[nCntLiquid]->SetColor(rCol);
	}
}

//============================================================
//	�������̐ݒ菈��
//============================================================
HRESULT CLiquid::SetPattern(const POSGRID2& rPart)
{
	// �t�̂̕�������ݒ�
	for (int nCntLiquid = 0; nCntLiquid < LIQUID_MAX; nCntLiquid++)
	{ // �t�̂̍ő吔���J��Ԃ�

		// �����̕�������ݒ�
		if (FAILED(m_apLiquid[nCntLiquid]->SetPattern(rPart)))
		{ // �ݒ�Ɏ��s�����ꍇ

			// ���s��Ԃ�
			assert(false);
			return E_FAIL;
		}
	}

	// ������Ԃ�
	return S_OK;
}

//============================================================
//	��ނ̐ݒ菈��
//============================================================
void CLiquid::SetType(const EType type)
{
	if (type > NONE_IDX && type < TYPE_MAX)
	{ // ��ނ��͈͓��̏ꍇ

		// �����̎�ނ�ݒ�
		m_type = type;

		for (int nCntLiquid = 0; nCntLiquid < LIQUID_MAX; nCntLiquid++)
		{ // �t�̂̍ő吔���J��Ԃ�

			// �e�N�X�`����o�^�E����
			m_apLiquid[nCntLiquid]->BindTexture(TEXTURE_FILE[(int)m_type][nCntLiquid]);
		}
	}
	else { assert(false); }	// �͈͊O
}

//============================================================
//	�e�N�X�`���ړ��ʂ̐ݒ菈��
//============================================================
void CLiquid::SetTexMove(const STexMove texMove)
{
	// ���t�̂̃e�N�X�`���ړ��ʂ�ݒ�
	m_apLiquid[LIQUID_LOW]->SetMoveU(texMove.texMoveLow.x);
	m_apLiquid[LIQUID_LOW]->SetMoveV(texMove.texMoveLow.y);

	// ��t�̂̃e�N�X�`���ړ��ʂ�ݒ�
	m_apLiquid[LIQUID_HIGH]->SetMoveU(texMove.texMoveHigh.x);
	m_apLiquid[LIQUID_HIGH]->SetMoveV(texMove.texMoveHigh.y);
}

//============================================================
//	�g�̍ō��㏸�ʂ̐ݒ菈��
//============================================================
void CLiquid::SetMaxUp(const float fMaxUp)
{
	// �����̔g�̍ō��㏸�ʂ�ݒ�
	m_fMaxUp = fMaxUp;
}

//============================================================
//	�g�ł��������Z�ʂ̐ݒ菈��
//============================================================
void CLiquid::SetAddSinRot(const float fAddSinRot)
{
	// �����̔g�ł��������Z�ʂ�ݒ�
	m_fAddSinRot = fAddSinRot;
}

//============================================================
//	�הg�̌������Z�ʂ̐ݒ菈��
//============================================================
void CLiquid::SetAddVtxRot(const float fAddVtxRot)
{
	// �����̗הg�̌������Z�ʂ�ݒ�
	m_fAddVtxRot = fAddVtxRot;
}

//============================================================
//	�e�N�X�`���ړ��ʎ擾����
//============================================================
CLiquid::STexMove CLiquid::GetTexMove() const
{
	STexMove texMove;	// �e�N�X�`���ړ���

	// ���t�̂̃e�N�X�`���ړ��ʂ�ݒ�
	texMove.texMoveLow.x = m_apLiquid[LIQUID_LOW]->GetMoveU();
	texMove.texMoveLow.y = m_apLiquid[LIQUID_LOW]->GetMoveV();

	// ��t�̂̃e�N�X�`���ړ��ʂ�ݒ�
	texMove.texMoveHigh.x = m_apLiquid[LIQUID_HIGH]->GetMoveU();
	texMove.texMoveHigh.y = m_apLiquid[LIQUID_HIGH]->GetMoveV();

	// �e�N�X�`���ړ��ʂ�Ԃ�
	return texMove;
}

//============================================================
//	���b�V���t�B�[���h�擾����
//============================================================
CScrollMeshField* CLiquid::GetMeshField(const int nID) const
{
	if (nID > NONE_IDX && nID < LIQUID_MAX)
	{ // �C���f�b�N�X���͈͓��̏ꍇ

		// ���b�V���t�B�[���h��Ԃ�
		return m_apLiquid[nID];
	}
	else { assert(false); return nullptr; }	// �͈͊O
}
