//============================================================
//
//	�X�e�[�W���� [stage.cpp]
//	Author�F���c�E��
//
//============================================================
//************************************************************
//	�C���N���[�h�t�@�C��
//************************************************************
#include "stage.h"
#include "manager.h"
#include "texture.h"
#include "collision.h"
#include "retention.h"
#include "field.h"
#include "wall.h"
#include "scenery.h"
#include "sky.h"
#include "liquid.h"

//************************************************************
//	�萔�錾
//************************************************************
//	�e�N���X [CStage] �̃����o�֐�
//************************************************************
//============================================================
//	�R���X�g���N�^
//============================================================
CStage::CStage() :
	m_sCurPath	({})	// ���݂̃X�e�[�W�p�X
{
	// �����o�ϐ����N���A
	memset(&m_limit, 0, sizeof(m_limit));	// �͈͏��
}

//============================================================
//	�f�X�g���N�^
//============================================================
CStage::~CStage()
{

}

//============================================================
//	����������
//============================================================
HRESULT CStage::Init()
{
	// �����o�ϐ���������
	memset(&m_limit, 0, sizeof(m_limit));	// �͈͏��
	m_sCurPath = {};	// ���݂̃X�e�[�W�p�X

	// ������Ԃ�
	return S_OK;
}

//============================================================
//	�I������
//============================================================
void CStage::Uninit()
{

}

//============================================================
//	�X�e�[�W��������
//============================================================
HRESULT CStage::BindStage(const char* pPath)
{
	char aString[128];	// �e�L�X�g�̕�����̑���p
	int nEnd = 0;		// �e�L�X�g�ǂݍ��ݏI���̊m�F�p

	// �X�e�[�W�̍폜
	DeleteStage();

	// �ǂݍ��ރX�e�[�W�p�X��ۑ�
	m_sCurPath = pPath;

	// �t�@�C����ǂݍ��݌`���ŊJ��
	FILE* pFile = fopen(pPath, "r");

	if (pFile != nullptr)
	{ // �t�@�C�����J�����ꍇ

		while (1)
		{ // EOF�܂Ŗ������[�v

			// �t�@�C�����當�����ǂݍ���
			nEnd = fscanf(pFile, "%s", &aString[0]);	// �I�[�̏ꍇ EOF��Ԃ�
			if (nEnd == EOF)
			{ // �t�@�C�����I�[�̏ꍇ

				// �����𔲂���
				break;
			}

			// �͈͏��̓Ǎ�
			if (FAILED(LoadLimit(&aString[0], pFile)))
			{ // �ǂݍ��݂Ɏ��s�����ꍇ

				// ���s��Ԃ�
				assert(false);
				return E_FAIL;
			}

			// �n�ʂ̓Ǎ�
			else if (FAILED(LoadField(&aString[0], pFile)))
			{ // �ǂݍ��݂Ɏ��s�����ꍇ

				// ���s��Ԃ�
				assert(false);
				return E_FAIL;
			}

			// �ǂ̓Ǎ�
			else if (FAILED(LoadWall(&aString[0], pFile)))
			{ // �ǂݍ��݂Ɏ��s�����ꍇ

				// ���s��Ԃ�
				assert(false);
				return E_FAIL;
			}

			// �i�F�̓Ǎ�
			else if (FAILED(LoadScenery(&aString[0], pFile)))
			{ // �ǂݍ��݂Ɏ��s�����ꍇ

				// ���s��Ԃ�
				assert(false);
				return E_FAIL;
			}

			// ��̓Ǎ�
			else if (FAILED(LoadSky(&aString[0], pFile)))
			{ // �ǂݍ��݂Ɏ��s�����ꍇ

				// ���s��Ԃ�
				assert(false);
				return E_FAIL;
			}

			// �t�̂̓Ǎ�
			else if (FAILED(LoadLiquid(&aString[0], pFile)))
			{ // �ǂݍ��݂Ɏ��s�����ꍇ

				// ���s��Ԃ�
				assert(false);
				return E_FAIL;
			}
		}
		
		// �t�@�C�������
		fclose(pFile);

		// ������Ԃ�
		return S_OK;
	}
	else
	{ // �t�@�C�����J���Ȃ������ꍇ

		// �G���[���b�Z�[�W�{�b�N�X
		MessageBox(nullptr, "�X�e�[�W�Z�b�g�A�b�v�̓ǂݍ��݂Ɏ��s�I", "�x���I", MB_ICONWARNING);

		// ���s��Ԃ�
		return E_FAIL;
	}
}

//============================================================
//	�X�e�[�W�폜����
//============================================================
void CStage::DeleteStage()
{
	// ���݂̃X�e�[�W�p�X���N���A
	m_sCurPath.clear();

	// �͈͏��̃N���A
	memset(&m_limit, 0, sizeof(m_limit));

	// �n�ʂ̑S�j��
	CListManager<CField>* pListField = CField::GetList();
	if (pListField != nullptr)
	{ // ��ł��n�ʂ�����ꍇ

		std::list<CField*> list = pListField->GetList();
		for (auto& rField : list)
		{ // �S�v�f�����J��Ԃ�

			// �n�ʂ̏I��
			SAFE_UNINIT(rField);
		}
	}

	// �ǂ̑S�j��
	CListManager<CWall>* pListWall = CWall::GetList();
	if (pListWall != nullptr)
	{ // ��ł��ǂ�����ꍇ

		std::list<CWall*> list = pListWall->GetList();
		for (auto& rWall : list)
		{ // �S�v�f�����J��Ԃ�

			// �ǂ̏I��
			SAFE_UNINIT(rWall);
		}
	}

	// �i�F�̑S�j��
	CListManager<CScenery>* pListScenery = CScenery::GetList();
	if (pListScenery != nullptr)
	{ // ��ł��i�F������ꍇ

		std::list<CScenery*> list = pListScenery->GetList();
		for (auto& rScenery : list)
		{ // �S�v�f�����J��Ԃ�

			// �i�F�̏I��
			SAFE_UNINIT(rScenery);
		}
	}

	// ��̑S�j��
	CListManager<CSky>* pListSky = CSky::GetList();
	if (pListSky != nullptr)
	{ // ��ł��󂪂���ꍇ

		std::list<CSky*> list = pListSky->GetList();
		for (auto& rSky : list)
		{ // �S�v�f�����J��Ԃ�

			// ��̏I��
			SAFE_UNINIT(rSky);
		}
	}

	// �t�̂̑S�j��
	CListManager<CLiquid>* pListLiquid = CLiquid::GetList();
	if (pListLiquid != nullptr)
	{ // ��ł��t�̂�����ꍇ

		std::list<CLiquid*> list = pListLiquid->GetList();
		for (auto& rLiquid : list)
		{ // �S�v�f�����J��Ԃ�

			// �t�̂̏I��
			SAFE_UNINIT(rLiquid);
		}
	}
}

//============================================================
//	�͈͊O�̒��n����
//============================================================
bool CStage::LandLimitPosition(VECTOR3& rPos, VECTOR3& rMove, const float fHeight)
{
	if (rPos.y - fHeight < m_limit.fField)
	{ // �ʒu���n�ʂ�艺�̏ꍇ

		// �ʒu��␳
		rPos.y = m_limit.fField + fHeight;

		// �ړ��ʂ�������
		rMove.y = 0.0f;

		// ���n���Ă���󋵂�Ԃ�
		return true;
	}

	// ���n���Ă��Ȃ��󋵂�Ԃ�
	return false;
}

//============================================================
//	�n�ʂ̒��n����
//============================================================
bool CStage::LandFieldPosition(VECTOR3& rPos, VECTOR3& rOldPos, VECTOR3& rMove, CField** ppCollField)
{
	CListManager<CField> *pListManager = CField::GetList();	// �t�B�[���h���X�g�}�l�[�W���[
	if (pListManager == nullptr) { return false; }			// ���X�g���g�p�̏ꍇ������
	std::list<CField*> listField = pListManager->GetList();	// �t�B�[���h���X�g���
	CField *pCurField = nullptr;	// ���n�\��̒n��
	float fCurPos = m_limit.fField;	// ���n�\���Y���W
	bool bLand = false;				// �n�ʂ̒��n����

	for (auto& rList : listField)
	{ // �n�ʂ̑������J��Ԃ�

		assert(rList != nullptr);
		if (rList->IsPositionRange(rPos))
		{ // �n�ʂ͈͓̔��̏ꍇ

			float fPosHeight = rList->GetPositionHeight(rPos);	// ���nY���W
			if (fPosHeight <= rOldPos.y && fPosHeight > rPos.y)
			{
				if (fCurPos <= fPosHeight)
				{ // ���݂̒��n�\��Y���W��荂���ʒu�ɂ���ꍇ

					// ���n�\��̒n�ʂ��X�V
					pCurField = rList;

					// ���n�\���Y���W���X�V
					fCurPos = fPosHeight;
				}
			}
		}
	}

	if (pCurField == nullptr)
	{ // ���n�\��̒n�ʂ����݂��Ȃ��ꍇ

		if (ppCollField != nullptr)
		{ // �n�ʂ̕ۑ��A�h���X������ꍇ

			// �n�ʖ�����ۑ�
			*ppCollField = nullptr;
		}
	}
	else
	{ // ���n�\��̒n�ʂ����݂���ꍇ

		// ���b�V���̒��n�󋵂�ۑ�
		bLand = pCurField->LandPosition(rPos, rMove);
		if (ppCollField != nullptr)
		{ // �n�ʂ̕ۑ��A�h���X������ꍇ

			if (bLand)
			{ // ���n���Ă���ꍇ

				// ���n�����n�ʂ�ۑ�
				*ppCollField = pCurField;
			}
		}
	}

	// ���n�����Ԃ�
	return bLand;
}

//============================================================
//	��ԏ�̒n�ʒ��n����
//============================================================
bool CStage::LandFieldPositionTop(VECTOR3& rPos, VECTOR3& rMove, CField** ppCollField)
{
	CListManager<CField> *pListManager = CField::GetList();	// �t�B�[���h���X�g�}�l�[�W���[
	if (pListManager == nullptr) { return false; }			// ���X�g���g�p�̏ꍇ������
	std::list<CField*> listField = pListManager->GetList();	// �t�B�[���h���X�g���
	CField *pCurField = nullptr;	// ���n�\��̒n��
	float fCurPos = m_limit.fField;	// ���n�\���Y���W
	bool bLand = false;				// �n�ʂ̒��n����

	for (auto& rList : listField)
	{ // �n�ʂ̑������J��Ԃ�

		assert(rList != nullptr);
		if (rList->IsPositionRange(rPos))
		{ // �n�ʂ͈͓̔��̏ꍇ

			float fPosHeight = rList->GetPositionHeight(rPos);	// ���nY���W
			if (fCurPos <= fPosHeight)
			{ // ���݂̒��n�\��Y���W��荂���ʒu�ɂ���ꍇ

				// ���n�\��̒n�ʂ��X�V
				pCurField = rList;

				// ���n�\���Y���W���X�V
				fCurPos = fPosHeight;
			}
		}
	}

	if (pCurField == nullptr)
	{ // ���n�\��̒n�ʂ����݂��Ȃ��ꍇ������

		if (ppCollField != nullptr)
		{ // �n�ʂ̕ۑ��A�h���X������ꍇ

			// �n�ʖ�����ۑ�
			*ppCollField = pCurField;
		}
	}
	else
	{ // ���n�\��̒n�ʂ����݂��Ȃ��ꍇ������

		// ���b�V���̒��n�󋵂�ۑ�
		bLand = pCurField->LandPosition(rPos, rMove);
		if (ppCollField != nullptr)
		{ // �n�ʂ̕ۑ��A�h���X������ꍇ

			if (bLand)
			{ // ���n���Ă���ꍇ

				// ���n�����n�ʂ�ۑ�
				*ppCollField = pCurField;
			}
		}
	}

	// ���n�����Ԃ�
	return bLand;
}

//============================================================
//	�ʒu�̕␳����
//============================================================
void CStage::LimitPosition(VECTOR3& rPos, const float fRadius)
{
	switch (m_limit.mode)
	{ // �������[�h���Ƃ̏���
	case LIMIT_NONE:	// �����Ȃ�

		// �������Ȃ��ꍇ�֐����Ăяo���K�v���Ȃ�
		assert(false);

		break;

	case LIMIT_BOX:		// ��`�͈�

		// �p���̓�������
		collision::InBoxPillarXZ
		( // ����
			rPos,				// ����ʒu
			VEC3_ZERO,			// ���茴�_�ʒu
			VECTOR3(fRadius),	// ����T�C�Y(�E�E��E��)
			VECTOR3(fRadius),	// ����T�C�Y(���E���E�O)
			VECTOR3(fabsf(m_limit.fRight), 0.0f, fabsf(m_limit.fFar)),	// ���茴�_�T�C�Y(�E�E��E��)
			VECTOR3(fabsf(m_limit.fLeft), 0.0f, fabsf(m_limit.fNear))	// ���茴�_�T�C�Y(���E���E�O)
		);

		break;

	case LIMIT_CIRCLE:	// �~�͈�

		// �~���̓�������
		collision::InCirclePillar
		( // ����
			rPos,			// ����ʒu
			VEC3_ZERO,		// ���茴�_�ʒu
			fRadius,		// ���蔼�a
			m_limit.fRadius	// ���茴�_���a
		);

		break;

	default:	// ��O����
		assert(false);
		break;
	}
}

//============================================================
//	�L��Y���W�Ƃ̓����蔻��
//============================================================
bool CStage::CollisionKillY(const VECTOR3& rPos)
{
	if (rPos.y < m_limit.fKillY)
	{ // �ʒu���폜�̐����ʒu��艺�̏ꍇ

		// ���S���Ă���󋵂�Ԃ�
		return true;
	}

	// ���S���Ă��Ȃ��󋵂�Ԃ�
	return false;
}

//============================================================
//	�n�ʂ͈͓̔��̎擾����
//============================================================
bool CStage::IsFieldPositionRange(const VECTOR3& rPos)
{
	CListManager<CField> *pListManager = CField::GetList();	// �t�B�[���h���X�g�}�l�[�W���[
	if (pListManager == nullptr) { return false; }			// ���X�g���g�p�̏ꍇ������
	std::list<CField*> listField = pListManager->GetList();	// �t�B�[���h���X�g���

	for (auto& rList : listField)
	{ // �n�ʂ̑������J��Ԃ�

		assert(rList != nullptr);
		if (rList->IsPositionRange(rPos))
		{ // �n�ʂ͈͓̔��̏ꍇ

			// �͈͓��ɂ����Ԃ�Ԃ�
			return true;
		}
	}

	// �͈͓��ɂ��Ȃ���Ԃ�Ԃ�
	return false;
}

//============================================================
//	�n�ʂ̒��n�ʒu�̎擾����
//============================================================
float CStage::GetFieldPositionHeight(const VECTOR3& rPos)
{
	CListManager<CField> *pListManager = CField::GetList();	// �t�B�[���h���X�g�}�l�[�W���[
	if (pListManager == nullptr) { return false; }			// ���X�g���g�p�̏ꍇ������
	std::list<CField*> listField = pListManager->GetList();	// �t�B�[���h���X�g���
	CField *pCurField = nullptr;	// ���n�\��̒n��
	float fCurPos = m_limit.fField;	// ���n�\���Y���W

	for (auto& rList : listField)
	{ // �n�ʂ̑������J��Ԃ�

		assert(rList != nullptr);
		if (rList->IsPositionRange(rPos))
		{ // �n�ʂ͈͓̔��̏ꍇ

			float fPosHeight = rList->GetPositionHeight(rPos);	// ���nY���W
			if (fCurPos <= fPosHeight)
			{ // ���݂̒��n�\��Y���W��荂���ʒu�ɂ���ꍇ

				// ���n�\��̒n�ʂ��X�V
				pCurField = rList;

				// ���n�\���Y���W���X�V
				fCurPos = fPosHeight;
			}
		}
	}

	if (pCurField != nullptr)
	{ // ���n�\��̒n�ʂ����݂���ꍇ

		// ���n�ʒu��Ԃ�
		return fCurPos;
	}
	else
	{ // ���n�\��̒n�ʂ����݂��Ȃ��ꍇ

		// �����ʒu��Ԃ�
		return rPos.y;
	}
}

//============================================================
//	���b�V���̒��n�ʒu�̎擾���� (���݈ʒu���͉�)
//============================================================
float CStage::GetFieldDownPositionHeight(const VECTOR3& rPos)
{
	CListManager<CField> *pListManager = CField::GetList();	// �t�B�[���h���X�g�}�l�[�W���[
	if (pListManager == nullptr) { return false; }			// ���X�g���g�p�̏ꍇ������
	std::list<CField*> listField = pListManager->GetList();	// �t�B�[���h���X�g���
	CField *pCurField = nullptr;	// ���n�\��̒n��
	float fCurPos = m_limit.fField;	// ���n�\���Y���W

	for (auto& rList : listField)
	{ // �n�ʂ̑������J��Ԃ�

		assert(rList != nullptr);
		if (rList->IsPositionRange(rPos))
		{ // �n�ʂ͈͓̔��̏ꍇ

			float fPosHeight = rList->GetPositionHeight(rPos);	// ���nY���W
			if (fCurPos <= fPosHeight && rPos.y >= fPosHeight)
			{ // ���݂̒��n�\��Y���W��荂���ʒu�ɂ���ꍇ

				// ���n�\��̒n�ʂ��X�V
				pCurField = rList;

				// ���n�\���Y���W���X�V
				fCurPos = fPosHeight;
			}
		}
	}

	if (pCurField != nullptr)
	{ // ���n�\��̒n�ʂ����݂���ꍇ

		// ���n�ʒu��Ԃ�
		return fCurPos;
	}
	else
	{ // ���n�\��̒n�ʂ����݂��Ȃ��ꍇ

		// �����ʒu��Ԃ�
		return rPos.y;
	}
}

//============================================================
//	��������
//============================================================
CStage* CStage::Create()
{
	// �X�e�[�W�̐���
	CStage* pStage = new CStage;
	if (pStage == nullptr)
	{ // �����Ɏ��s�����ꍇ

		return nullptr;
	}
	else
	{ // �����ɐ��������ꍇ

		// �X�e�[�W�̏�����
		if (FAILED(pStage->Init()))
		{ // �������Ɏ��s�����ꍇ

			// �X�e�[�W�̔j��
			SAFE_DELETE(pStage);
			return nullptr;
		}

		// �m�ۂ����A�h���X��Ԃ�
		return pStage;
	}
}

//============================================================
//	�j������
//============================================================
void CStage::Release(CStage*& prStage)
{
	// �X�e�[�W�̏I��
	assert(prStage != nullptr);
	prStage->Uninit();

	// �������J��
	SAFE_DELETE(prStage);
}

//============================================================
//	�͈͏��̓Ǎ�����
//============================================================
HRESULT CStage::LoadLimit(const char* pString, FILE *pFile)
{
	char aString[128];	// �e�L�X�g�̕�����̑���p
	SLimit limit;		// �X�e�[�W�͈͂̑���p
	if (pString == nullptr || pFile == nullptr)
	{ // ������E�t�@�C�������݂��Ȃ��ꍇ

		// ���s��Ԃ�
		assert(false);
		return E_FAIL;
	}

	// �X�e�[�W�͈͂̐ݒ�
	if (strcmp(pString, "LIMITSET") == 0)
	{ // �ǂݍ��񂾕����� LIMITSET �̏ꍇ

		// �������[�h�𖳂��ɐݒ�
		limit.mode = LIMIT_NONE;

		do
		{ // �ǂݍ��񂾕����� END_LIMITSET �ł͂Ȃ��ꍇ���[�v

			// �t�@�C�����當�����ǂݍ���
			fscanf(pFile, "%s", &aString[0]);

			if (strcmp(&aString[0], "CENTER") == 0)
			{ // �ǂݍ��񂾕����� CENTER �̏ꍇ

				fscanf(pFile, "%s", &aString[0]);		// = ��ǂݍ��� (�s�v)
				fscanf(pFile, "%f", &limit.center.x);	// ���S���WX��ǂݍ���
				fscanf(pFile, "%f", &limit.center.y);	// ���S���WY��ǂݍ���
				fscanf(pFile, "%f", &limit.center.z);	// ���S���WZ��ǂݍ���
			}
			else if (strcmp(&aString[0], "NEAR") == 0)
			{ // �ǂݍ��񂾕����� NEAR �̏ꍇ

				fscanf(pFile, "%s", &aString[0]);		// = ��ǂݍ��� (�s�v)
				fscanf(pFile, "%f", &limit.fNear);		// �O�ʒu��ǂݍ���

				// �������[�h����`�͈͂ɐݒ�
				limit.mode = LIMIT_BOX;
			}
			else if (strcmp(&aString[0], "FAR") == 0)
			{ // �ǂݍ��񂾕����� FAR �̏ꍇ

				fscanf(pFile, "%s", &aString[0]);		// = ��ǂݍ��� (�s�v)
				fscanf(pFile, "%f", &limit.fFar);		// ��ʒu��ǂݍ���

				// �������[�h����`�͈͂ɐݒ�
				limit.mode = LIMIT_BOX;
			}
			else if (strcmp(&aString[0], "RIGHT") == 0)
			{ // �ǂݍ��񂾕����� RIGHT �̏ꍇ

				fscanf(pFile, "%s", &aString[0]);		// = ��ǂݍ��� (�s�v)
				fscanf(pFile, "%f", &limit.fRight);		// �E�ʒu��ǂݍ���

				// �������[�h����`�͈͂ɐݒ�
				limit.mode = LIMIT_BOX;
			}
			else if (strcmp(&aString[0], "LEFT") == 0)
			{ // �ǂݍ��񂾕����� LEFT �̏ꍇ

				fscanf(pFile, "%s", &aString[0]);		// = ��ǂݍ��� (�s�v)
				fscanf(pFile, "%f", &limit.fLeft);		// ���ʒu��ǂݍ���

				// �������[�h����`�͈͂ɐݒ�
				limit.mode = LIMIT_BOX;
			}
			else if (strcmp(&aString[0], "RADIUS") == 0)
			{ // �ǂݍ��񂾕����� RADIUS �̏ꍇ

				fscanf(pFile, "%s", &aString[0]);		// = ��ǂݍ��� (�s�v)
				fscanf(pFile, "%f", &limit.fRadius);	// ���a��ǂݍ���

				// �������[�h���~�͈͂ɐݒ�
				limit.mode = LIMIT_CIRCLE;
			}
			else if (strcmp(&aString[0], "FIELD") == 0)
			{ // �ǂݍ��񂾕����� FIELD �̏ꍇ

				fscanf(pFile, "%s", &aString[0]);		// = ��ǂݍ��� (�s�v)
				fscanf(pFile, "%f", &limit.fField);		// �n�ʈʒu��ǂݍ���
			}
			else if (strcmp(&aString[0], "KILLY") == 0)
			{ // �ǂݍ��񂾕����� KILLY �̏ꍇ

				fscanf(pFile, "%s", &aString[0]);		// = ��ǂݍ��� (�s�v)
				fscanf(pFile, "%f", &limit.fKillY);		// �폜�ʒu��ǂݍ���
			}
		} while (strcmp(&aString[0], "END_LIMITSET") != 0);	// �ǂݍ��񂾕����� END_LIMITSET �ł͂Ȃ��ꍇ���[�v

		// �X�e�[�W�͈͂̐ݒ�
		SetLimit(limit);
	}

	// ������Ԃ�
	return S_OK;
}

//============================================================
//	�n�ʏ��̓Ǎ�����
//============================================================
HRESULT CStage::LoadField(const char* pString, FILE *pFile)
{
	char aString[128];	// �e�L�X�g�̕�����̑���p
	int nType = 0;		// ��ނ̑���p
	VECTOR3 pos = VEC3_ZERO;	// �ʒu�̑���p
	VECTOR3 rot = VEC3_ZERO;	// �����̑���p
	VECTOR2 size = VEC2_ZERO;	// �傫���̑���p
	COLOR col = color::White();	// �F�̑���p
	POSGRID2 part = GRID2_ZERO;		// �������̑���p
	POSGRID2 texPart = GRID2_ZERO;	// �e�N�X�`���������̑���p

	if (pString == nullptr || pFile == nullptr)
	{ // ������E�t�@�C�������݂��Ȃ��ꍇ

		// ���s��Ԃ�
		assert(false);
		return E_FAIL;
	}

	// �n�ʂ̐ݒ�
	if (strcmp(pString, "STAGE_FIELDSET") == 0)
	{ // �ǂݍ��񂾕����� STAGE_FIELDSET �̏ꍇ

		do
		{ // �ǂݍ��񂾕����� END_STAGE_FIELDSET �ł͂Ȃ��ꍇ���[�v

			// �t�@�C�����當�����ǂݍ���
			fscanf(pFile, "%s", &aString[0]);

			if (strcmp(&aString[0], "FIELDSET") == 0)
			{ // �ǂݍ��񂾕����� FIELDSET �̏ꍇ
	
				do
				{ // �ǂݍ��񂾕����� END_FIELDSET �ł͂Ȃ��ꍇ���[�v
	
					// �t�@�C�����當�����ǂݍ���
					fscanf(pFile, "%s", &aString[0]);
	
					if (strcmp(&aString[0], "TYPE") == 0)
					{ // �ǂݍ��񂾕����� TYPE �̏ꍇ
	
						fscanf(pFile, "%s", &aString[0]);	// = ��ǂݍ��� (�s�v)
						fscanf(pFile, "%d", &nType);		// ��ނ�ǂݍ���
					}
					else if (strcmp(&aString[0], "POS") == 0)
					{ // �ǂݍ��񂾕����� POS �̏ꍇ

						fscanf(pFile, "%s", &aString[0]);	// = ��ǂݍ��� (�s�v)
						fscanf(pFile, "%f", &pos.x);		// �ʒuX��ǂݍ���
						fscanf(pFile, "%f", &pos.y);		// �ʒuY��ǂݍ���
						fscanf(pFile, "%f", &pos.z);		// �ʒuZ��ǂݍ���
					}
					else if (strcmp(&aString[0], "ROT") == 0)
					{ // �ǂݍ��񂾕����� ROT �̏ꍇ

						fscanf(pFile, "%s", &aString[0]);	// = ��ǂݍ��� (�s�v)
						fscanf(pFile, "%f", &rot.x);		// ����X��ǂݍ���
						fscanf(pFile, "%f", &rot.y);		// ����Y��ǂݍ���
						fscanf(pFile, "%f", &rot.z);		// ����Z��ǂݍ���
					}
					else if (strcmp(&aString[0], "SIZE") == 0)
					{ // �ǂݍ��񂾕����� SIZE �̏ꍇ

						fscanf(pFile, "%s", &aString[0]);	// = ��ǂݍ��� (�s�v)
						fscanf(pFile, "%f", &size.x);		// �傫��X��ǂݍ���
						fscanf(pFile, "%f", &size.y);		// �傫��Y��ǂݍ���
					}
					else if (strcmp(&aString[0], "COL") == 0)
					{ // �ǂݍ��񂾕����� COL �̏ꍇ

						fscanf(pFile, "%s", &aString[0]);	// = ��ǂݍ��� (�s�v)
						fscanf(pFile, "%f", &col.r);		// �FR��ǂݍ���
						fscanf(pFile, "%f", &col.g);		// �FG��ǂݍ���
						fscanf(pFile, "%f", &col.b);		// �FB��ǂݍ���
						fscanf(pFile, "%f", &col.a);		// �FA��ǂݍ���
					}
					else if (strcmp(&aString[0], "PART") == 0)
					{ // �ǂݍ��񂾕����� PART �̏ꍇ

						fscanf(pFile, "%s", &aString[0]);	// = ��ǂݍ��� (�s�v)
						fscanf(pFile, "%d", &part.x);		// ����������ǂݍ���
						fscanf(pFile, "%d", &part.y);		// �c��������ǂݍ���
					}
					else if (strcmp(&aString[0], "TEXPART") == 0)
					{ // �ǂݍ��񂾕����� TEXPART �̏ꍇ

						fscanf(pFile, "%s", &aString[0]);	// = ��ǂݍ��� (�s�v)
						fscanf(pFile, "%d", &texPart.x);	// �e�N�X�`������������ǂݍ���
						fscanf(pFile, "%d", &texPart.y);	// �e�N�X�`���c��������ǂݍ���
					}
				} while (strcmp(&aString[0], "END_FIELDSET") != 0);	// �ǂݍ��񂾕����� END_FIELDSET �ł͂Ȃ��ꍇ���[�v

				// �n�ʃI�u�W�F�N�g�̐���
				if (CField::Create((CField::EType)nType, pos, D3DXToRadian(rot), size, col, part, texPart) == nullptr)
				{ // �m�ۂɎ��s�����ꍇ
	
					// ���s��Ԃ�
					assert(false);
					return E_FAIL;
				}
			}
		} while (strcmp(&aString[0], "END_STAGE_FIELDSET") != 0);	// �ǂݍ��񂾕����� END_STAGE_FIELDSET �ł͂Ȃ��ꍇ���[�v
	}

	// ������Ԃ�
	return S_OK;
}

//============================================================
//	�Ǐ��̓Ǎ�����
//============================================================
HRESULT CStage::LoadWall(const char* pString, FILE *pFile)
{
	char aString[128];	// �e�L�X�g�̕�����̑���p
	int nType = 0;		// ��ނ̑���p
	VECTOR3 pos = VEC3_ZERO;	// �ʒu�̑���p
	VECTOR3 rot = VEC3_ZERO;	// �����̑���p
	VECTOR2 size = VEC2_ZERO;	// �傫���̑���p
	COLOR col = color::White();	// �F�̑���p
	POSGRID2 part = GRID2_ZERO;		// �������̑���p
	POSGRID2 texPart = GRID2_ZERO;	// �e�N�X�`���������̑���p

	if (pString == nullptr || pFile == nullptr)
	{ // ������E�t�@�C�������݂��Ȃ��ꍇ

		// ���s��Ԃ�
		assert(false);
		return E_FAIL;
	}

	// �ǂ̐ݒ�
	if (strcmp(pString, "STAGE_WALLSET") == 0)
	{ // �ǂݍ��񂾕����� STAGE_WALLSET �̏ꍇ

		do
		{ // �ǂݍ��񂾕����� END_STAGE_SCENERYSET �ł͂Ȃ��ꍇ���[�v

			// �t�@�C�����當�����ǂݍ���
			fscanf(pFile, "%s", &aString[0]);

			if (strcmp(&aString[0], "WALLSET") == 0)
			{ // �ǂݍ��񂾕����� WALLSET �̏ꍇ
	
				do
				{ // �ǂݍ��񂾕����� END_WALLSET �ł͂Ȃ��ꍇ���[�v
	
					// �t�@�C�����當�����ǂݍ���
					fscanf(pFile, "%s", &aString[0]);
	
					if (strcmp(&aString[0], "TYPE") == 0)
					{ // �ǂݍ��񂾕����� TYPE �̏ꍇ
	
						fscanf(pFile, "%s", &aString[0]);	// = ��ǂݍ��� (�s�v)
						fscanf(pFile, "%d", &nType);		// ��ނ�ǂݍ���
					}
					else if (strcmp(&aString[0], "POS") == 0)
					{ // �ǂݍ��񂾕����� POS �̏ꍇ

						fscanf(pFile, "%s", &aString[0]);	// = ��ǂݍ��� (�s�v)
						fscanf(pFile, "%f", &pos.x);		// �ʒuX��ǂݍ���
						fscanf(pFile, "%f", &pos.y);		// �ʒuY��ǂݍ���
						fscanf(pFile, "%f", &pos.z);		// �ʒuZ��ǂݍ���
					}
					else if (strcmp(&aString[0], "ROT") == 0)
					{ // �ǂݍ��񂾕����� ROT �̏ꍇ

						fscanf(pFile, "%s", &aString[0]);	// = ��ǂݍ��� (�s�v)
						fscanf(pFile, "%f", &rot.x);		// ����X��ǂݍ���
						fscanf(pFile, "%f", &rot.y);		// ����Y��ǂݍ���
						fscanf(pFile, "%f", &rot.z);		// ����Z��ǂݍ���
					}
					else if (strcmp(&aString[0], "SIZE") == 0)
					{ // �ǂݍ��񂾕����� SIZE �̏ꍇ

						fscanf(pFile, "%s", &aString[0]);	// = ��ǂݍ��� (�s�v)
						fscanf(pFile, "%f", &size.x);		// �傫��X��ǂݍ���
						fscanf(pFile, "%f", &size.y);		// �傫��Y��ǂݍ���
					}
					else if (strcmp(&aString[0], "COL") == 0)
					{ // �ǂݍ��񂾕����� COL �̏ꍇ

						fscanf(pFile, "%s", &aString[0]);	// = ��ǂݍ��� (�s�v)
						fscanf(pFile, "%f", &col.r);		// �FR��ǂݍ���
						fscanf(pFile, "%f", &col.g);		// �FG��ǂݍ���
						fscanf(pFile, "%f", &col.b);		// �FB��ǂݍ���
						fscanf(pFile, "%f", &col.a);		// �FA��ǂݍ���
					}
					else if (strcmp(&aString[0], "PART") == 0)
					{ // �ǂݍ��񂾕����� PART �̏ꍇ

						fscanf(pFile, "%s", &aString[0]);	// = ��ǂݍ��� (�s�v)
						fscanf(pFile, "%d", &part.x);		// ����������ǂݍ���
						fscanf(pFile, "%d", &part.y);		// �c��������ǂݍ���
					}
					else if (strcmp(&aString[0], "TEXPART") == 0)
					{ // �ǂݍ��񂾕����� TEXPART �̏ꍇ

						fscanf(pFile, "%s", &aString[0]);	// = ��ǂݍ��� (�s�v)
						fscanf(pFile, "%d", &texPart.x);	// �e�N�X�`������������ǂݍ���
						fscanf(pFile, "%d", &texPart.y);	// �e�N�X�`���c��������ǂݍ���
					}
				} while (strcmp(&aString[0], "END_WALLSET") != 0);	// �ǂݍ��񂾕����� END_WALLSET �ł͂Ȃ��ꍇ���[�v

				// �ǃI�u�W�F�N�g�̐���
				if (CWall::Create((CWall::EType)nType, pos, D3DXToRadian(rot), size, col, part, texPart) == nullptr)
				{ // �m�ۂɎ��s�����ꍇ

					// ���s��Ԃ�
					assert(false);
					return E_FAIL;
				}
			}
		} while (strcmp(&aString[0], "END_STAGE_WALLSET") != 0);	// �ǂݍ��񂾕����� END_STAGE_WALLSET �ł͂Ȃ��ꍇ���[�v
	}

	// ������Ԃ�
	return S_OK;
}

//============================================================
//	�i�F���̓Ǎ�����
//============================================================
HRESULT CStage::LoadScenery(const char* pString, FILE *pFile)
{
	VECTOR3 pos = VEC3_ZERO;	// �ʒu�̑���p
	VECTOR3 rot = VEC3_ZERO;	// �����̑���p
	COLOR col = color::White();	// �F�̑���p
	POSGRID2 part = GRID2_ZERO;		// �������̑���p
	POSGRID2 texPart = GRID2_ZERO;	// �e�N�X�`���������̑���p
	char aString[128];		// �e�L�X�g�̕�����̑���p
	int nType = 0;			// ��ނ̑���p
	float fRadius = 0.0f;	// ���a�̑���p
	float fHeight = 0.0f;	// �c���̑���p

	if (pString == nullptr || pFile == nullptr)
	{ // ������E�t�@�C�������݂��Ȃ��ꍇ

		// ���s��Ԃ�
		assert(false);
		return E_FAIL;
	}

	// �i�F�̐ݒ�
	if (strcmp(pString, "STAGE_SCENERYSET") == 0)
	{ // �ǂݍ��񂾕����� STAGE_SCENERYSET �̏ꍇ

		do
		{ // �ǂݍ��񂾕����� END_STAGE_SCENERYSET �ł͂Ȃ��ꍇ���[�v

			// �t�@�C�����當�����ǂݍ���
			fscanf(pFile, "%s", &aString[0]);

			if (strcmp(&aString[0], "SCENERYSET") == 0)
			{ // �ǂݍ��񂾕����� SCENERYSET �̏ꍇ
	
				do
				{ // �ǂݍ��񂾕����� END_SCENERYSET �ł͂Ȃ��ꍇ���[�v
	
					// �t�@�C�����當�����ǂݍ���
					fscanf(pFile, "%s", &aString[0]);
	
					if (strcmp(&aString[0], "TYPE") == 0)
					{ // �ǂݍ��񂾕����� TYPE �̏ꍇ
	
						fscanf(pFile, "%s", &aString[0]);	// = ��ǂݍ��� (�s�v)
						fscanf(pFile, "%d", &nType);		// ��ނ�ǂݍ���
					}
					else if (strcmp(&aString[0], "POS") == 0)
					{ // �ǂݍ��񂾕����� POS �̏ꍇ

						fscanf(pFile, "%s", &aString[0]);	// = ��ǂݍ��� (�s�v)
						fscanf(pFile, "%f", &pos.x);		// �ʒuX��ǂݍ���
						fscanf(pFile, "%f", &pos.y);		// �ʒuY��ǂݍ���
						fscanf(pFile, "%f", &pos.z);		// �ʒuZ��ǂݍ���
					}
					else if (strcmp(&aString[0], "ROT") == 0)
					{ // �ǂݍ��񂾕����� ROT �̏ꍇ

						fscanf(pFile, "%s", &aString[0]);	// = ��ǂݍ��� (�s�v)
						fscanf(pFile, "%f", &rot.x);		// ����X��ǂݍ���
						fscanf(pFile, "%f", &rot.y);		// ����Y��ǂݍ���
						fscanf(pFile, "%f", &rot.z);		// ����Z��ǂݍ���
					}
					else if (strcmp(&aString[0], "COL") == 0)
					{ // �ǂݍ��񂾕����� COL �̏ꍇ

						fscanf(pFile, "%s", &aString[0]);	// = ��ǂݍ��� (�s�v)
						fscanf(pFile, "%f", &col.r);		// �FR��ǂݍ���
						fscanf(pFile, "%f", &col.g);		// �FG��ǂݍ���
						fscanf(pFile, "%f", &col.b);		// �FB��ǂݍ���
						fscanf(pFile, "%f", &col.a);		// �FA��ǂݍ���
					}
					else if (strcmp(&aString[0], "PART") == 0)
					{ // �ǂݍ��񂾕����� PART �̏ꍇ

						fscanf(pFile, "%s", &aString[0]);	// = ��ǂݍ��� (�s�v)
						fscanf(pFile, "%d", &part.x);		// ����������ǂݍ���
						fscanf(pFile, "%d", &part.y);		// �c��������ǂݍ���
					}
					else if (strcmp(&aString[0], "TEXPART") == 0)
					{ // �ǂݍ��񂾕����� TEXPART �̏ꍇ

						fscanf(pFile, "%s", &aString[0]);	// = ��ǂݍ��� (�s�v)
						fscanf(pFile, "%d", &texPart.x);	// �e�N�X�`������������ǂݍ���
						fscanf(pFile, "%d", &texPart.y);	// �e�N�X�`���c��������ǂݍ���
					}
					else if (strcmp(&aString[0], "RADIUS") == 0)
					{ // �ǂݍ��񂾕����� RADIUS �̏ꍇ

						fscanf(pFile, "%s", &aString[0]);	// = ��ǂݍ��� (�s�v)
						fscanf(pFile, "%f", &fRadius);		// ���a��ǂݍ���
					}
					else if (strcmp(&aString[0], "HEIGHT") == 0)
					{ // �ǂݍ��񂾕����� HEIGHT �̏ꍇ

						fscanf(pFile, "%s", &aString[0]);	// = ��ǂݍ��� (�s�v)
						fscanf(pFile, "%f", &fHeight);		// �c����ǂݍ���
					}
				} while (strcmp(&aString[0], "END_SCENERYSET") != 0);	// �ǂݍ��񂾕����� END_SCENERYSET �ł͂Ȃ��ꍇ���[�v

				// �i�F�I�u�W�F�N�g�̐���
				if (CScenery::Create((CScenery::EType)nType, pos, D3DXToRadian(rot), col, part, texPart, fRadius, fHeight) == nullptr)
				{ // �m�ۂɎ��s�����ꍇ

					// ���s��Ԃ�
					assert(false);
					return E_FAIL;
				}
			}
		} while (strcmp(&aString[0], "END_STAGE_SCENERYSET") != 0);	// �ǂݍ��񂾕����� END_STAGE_SCENERYSET �ł͂Ȃ��ꍇ���[�v
	}

	// ������Ԃ�
	return S_OK;
}

//============================================================
//	����̓Ǎ�����
//============================================================
HRESULT CStage::LoadSky(const char* pString, FILE *pFile)
{
	VECTOR3 pos = VEC3_ZERO;	// �ʒu�̑���p
	VECTOR3 rot = VEC3_ZERO;	// �����̑���p
	COLOR col = color::White();	// �F�̑���p
	POSGRID2 part = GRID2_ZERO;		// �������̑���p
	POSGRID2 texPart = GRID2_ZERO;	// �e�N�X�`���������̑���p
	char aString[128];		// �e�L�X�g�̕�����̑���p
	int nType = 0;			// ��ނ̑���p
	float fRadius = 0.0f;	// ���a�̑���p

	if (pString == nullptr || pFile == nullptr)
	{ // ������E�t�@�C�������݂��Ȃ��ꍇ

		// ���s��Ԃ�
		assert(false);
		return E_FAIL;
	}

	// ��̐ݒ�
	if (strcmp(pString, "STAGE_SKYSET") == 0)
	{ // �ǂݍ��񂾕����� STAGE_SKYSET �̏ꍇ

		do
		{ // �ǂݍ��񂾕����� END_STAGE_SKYSET �ł͂Ȃ��ꍇ���[�v

			// �t�@�C�����當�����ǂݍ���
			fscanf(pFile, "%s", &aString[0]);

			if (strcmp(&aString[0], "SKYSET") == 0)
			{ // �ǂݍ��񂾕����� SKYSET �̏ꍇ

				do
				{ // �ǂݍ��񂾕����� END_SKYSET �ł͂Ȃ��ꍇ���[�v

					// �t�@�C�����當�����ǂݍ���
					fscanf(pFile, "%s", &aString[0]);

					if (strcmp(&aString[0], "TYPE") == 0)
					{ // �ǂݍ��񂾕����� TYPE �̏ꍇ

						fscanf(pFile, "%s", &aString[0]);	// = ��ǂݍ��� (�s�v)
						fscanf(pFile, "%d", &nType);		// ��ނ�ǂݍ���
					}
					else if (strcmp(&aString[0], "POS") == 0)
					{ // �ǂݍ��񂾕����� POS �̏ꍇ

						fscanf(pFile, "%s", &aString[0]);	// = ��ǂݍ��� (�s�v)
						fscanf(pFile, "%f", &pos.x);		// �ʒuX��ǂݍ���
						fscanf(pFile, "%f", &pos.y);		// �ʒuY��ǂݍ���
						fscanf(pFile, "%f", &pos.z);		// �ʒuZ��ǂݍ���
					}
					else if (strcmp(&aString[0], "ROT") == 0)
					{ // �ǂݍ��񂾕����� ROT �̏ꍇ

						fscanf(pFile, "%s", &aString[0]);	// = ��ǂݍ��� (�s�v)
						fscanf(pFile, "%f", &rot.x);		// ����X��ǂݍ���
						fscanf(pFile, "%f", &rot.y);		// ����Y��ǂݍ���
						fscanf(pFile, "%f", &rot.z);		// ����Z��ǂݍ���
					}
					else if (strcmp(&aString[0], "COL") == 0)
					{ // �ǂݍ��񂾕����� COL �̏ꍇ

						fscanf(pFile, "%s", &aString[0]);	// = ��ǂݍ��� (�s�v)
						fscanf(pFile, "%f", &col.r);		// �FR��ǂݍ���
						fscanf(pFile, "%f", &col.g);		// �FG��ǂݍ���
						fscanf(pFile, "%f", &col.b);		// �FB��ǂݍ���
						fscanf(pFile, "%f", &col.a);		// �FA��ǂݍ���
					}
					else if (strcmp(&aString[0], "PART") == 0)
					{ // �ǂݍ��񂾕����� PART �̏ꍇ

						fscanf(pFile, "%s", &aString[0]);	// = ��ǂݍ��� (�s�v)
						fscanf(pFile, "%d", &part.x);		// ����������ǂݍ���
						fscanf(pFile, "%d", &part.y);		// �c��������ǂݍ���
					}
					else if (strcmp(&aString[0], "TEXPART") == 0)
					{ // �ǂݍ��񂾕����� TEXPART �̏ꍇ

						fscanf(pFile, "%s", &aString[0]);	// = ��ǂݍ��� (�s�v)
						fscanf(pFile, "%d", &texPart.x);	// �e�N�X�`������������ǂݍ���
						fscanf(pFile, "%d", &texPart.y);	// �e�N�X�`���c��������ǂݍ���
					}
					else if (strcmp(&aString[0], "RADIUS") == 0)
					{ // �ǂݍ��񂾕����� RADIUS �̏ꍇ

						fscanf(pFile, "%s", &aString[0]);	// = ��ǂݍ��� (�s�v)
						fscanf(pFile, "%f", &fRadius);		// ���a��ǂݍ���
					}
				} while (strcmp(&aString[0], "END_SKYSET") != 0);	// �ǂݍ��񂾕����� END_SKYSET �ł͂Ȃ��ꍇ���[�v

				// ��I�u�W�F�N�g�̐���
				if (CSky::Create((CSky::EType)nType, pos, D3DXToRadian(rot), col, part, texPart, fRadius) == nullptr)
				{ // �m�ۂɎ��s�����ꍇ

					// ���s��Ԃ�
					assert(false);
					return E_FAIL;
				}
			}
		} while (strcmp(&aString[0], "END_STAGE_SKYSET") != 0);	// �ǂݍ��񂾕����� END_STAGE_SKYSET �ł͂Ȃ��ꍇ���[�v
	}

	// ������Ԃ�
	return S_OK;
}

//============================================================
//	�t�̏��̓Ǎ�����
//============================================================
HRESULT CStage::LoadLiquid(const char* pString, FILE *pFile)
{
	VECTOR3 pos = VEC3_ZERO;	// �ʒu�̑���p
	VECTOR3 rot = VEC3_ZERO;	// �����̑���p
	VECTOR2 size = VEC2_ZERO;	// �傫���̑���p
	COLOR col = color::White();	// �F�̑���p
	POSGRID2 part = GRID2_ZERO;		// �������̑���p
	CLiquid::STexMove texMove;		// �e�N�X�`���ړ��ʂ̑���p
	char aString[128];			// �e�L�X�g�̕�����̑���p
	int nType = 0;				// ��ނ̑���p
	float fMaxUp = 0.0f;		// �g�̍ō��㏸��
	float fAddSinRot = 0.0f;	// �g�ł��������Z��
	float fAddVtxRot = 0.0f;	// �הg�̌������Z��

	if (pString == nullptr || pFile == nullptr)
	{ // ������E�t�@�C�������݂��Ȃ��ꍇ

		// ���s��Ԃ�
		assert(false);
		return E_FAIL;
	}

	// �t�̂̐ݒ�
	if (strcmp(pString, "STAGE_LIQUIDSET") == 0)
	{ // �ǂݍ��񂾕����� STAGE_LIQUIDSET �̏ꍇ

		do
		{ // �ǂݍ��񂾕����� END_STAGE_LIQUIDSET �ł͂Ȃ��ꍇ���[�v

			// �t�@�C�����當�����ǂݍ���
			fscanf(pFile, "%s", &aString[0]);

			if (strcmp(&aString[0], "LIQUIDSET") == 0)
			{ // �ǂݍ��񂾕����� LIQUIDSET �̏ꍇ

				do
				{ // �ǂݍ��񂾕����� END_LIQUIDSET �ł͂Ȃ��ꍇ���[�v

					// �t�@�C�����當�����ǂݍ���
					fscanf(pFile, "%s", &aString[0]);

					if (strcmp(&aString[0], "TYPE") == 0)
					{ // �ǂݍ��񂾕����� TYPE �̏ꍇ

						fscanf(pFile, "%s", &aString[0]);	// = ��ǂݍ��� (�s�v)
						fscanf(pFile, "%d", &nType);		// ��ނ�ǂݍ���
					}
					else if (strcmp(&aString[0], "POS") == 0)
					{ // �ǂݍ��񂾕����� POS �̏ꍇ

						fscanf(pFile, "%s", &aString[0]);	// = ��ǂݍ��� (�s�v)
						fscanf(pFile, "%f", &pos.x);		// �ʒuX��ǂݍ���
						fscanf(pFile, "%f", &pos.y);		// �ʒuY��ǂݍ���
						fscanf(pFile, "%f", &pos.z);		// �ʒuZ��ǂݍ���
					}
					else if (strcmp(&aString[0], "ROT") == 0)
					{ // �ǂݍ��񂾕����� ROT �̏ꍇ

						fscanf(pFile, "%s", &aString[0]);	// = ��ǂݍ��� (�s�v)
						fscanf(pFile, "%f", &rot.x);		// ����X��ǂݍ���
						fscanf(pFile, "%f", &rot.y);		// ����Y��ǂݍ���
						fscanf(pFile, "%f", &rot.z);		// ����Z��ǂݍ���
					}
					else if (strcmp(&aString[0], "SIZE") == 0)
					{ // �ǂݍ��񂾕����� SIZE �̏ꍇ

						fscanf(pFile, "%s", &aString[0]);	// = ��ǂݍ��� (�s�v)
						fscanf(pFile, "%f", &size.x);		// �傫��X��ǂݍ���
						fscanf(pFile, "%f", &size.y);		// �傫��Y��ǂݍ���
					}
					else if (strcmp(&aString[0], "COL") == 0)
					{ // �ǂݍ��񂾕����� COL �̏ꍇ

						fscanf(pFile, "%s", &aString[0]);	// = ��ǂݍ��� (�s�v)
						fscanf(pFile, "%f", &col.r);		// �FR��ǂݍ���
						fscanf(pFile, "%f", &col.g);		// �FG��ǂݍ���
						fscanf(pFile, "%f", &col.b);		// �FB��ǂݍ���
						fscanf(pFile, "%f", &col.a);		// �FA��ǂݍ���
					}
					else if (strcmp(&aString[0], "PART") == 0)
					{ // �ǂݍ��񂾕����� PART �̏ꍇ

						fscanf(pFile, "%s", &aString[0]);	// = ��ǂݍ��� (�s�v)
						fscanf(pFile, "%d", &part.x);		// ����������ǂݍ���
						fscanf(pFile, "%d", &part.y);		// �c��������ǂݍ���
					}
					else if (strcmp(&aString[0], "TEXMOVE_LOW") == 0)
					{ // �ǂݍ��񂾕����� TEXMOVE_LOW �̏ꍇ

						fscanf(pFile, "%s", &aString[0]);			// = ��ǂݍ��� (�s�v)
						fscanf(pFile, "%f", &texMove.texMoveLow.x);	// ���t�̂̃e�N�X�`���ړ��ʂ�ǂݍ���
						fscanf(pFile, "%f", &texMove.texMoveLow.y);	// ���t�̂̃e�N�X�`���ړ��ʂ�ǂݍ���
					}
					else if (strcmp(&aString[0], "TEXMOVE_HIGH") == 0)
					{ // �ǂݍ��񂾕����� TEXMOVE_HIGH �̏ꍇ

						fscanf(pFile, "%s", &aString[0]);				// = ��ǂݍ��� (�s�v)
						fscanf(pFile, "%f", &texMove.texMoveHigh.x);	// ��t�̂̃e�N�X�`���ړ��ʂ�ǂݍ���
						fscanf(pFile, "%f", &texMove.texMoveHigh.y);	// ��t�̂̃e�N�X�`���ړ��ʂ�ǂݍ���
					}
					else if (strcmp(&aString[0], "MAX_UP") == 0)
					{ // �ǂݍ��񂾕����� MAX_UP �̏ꍇ

						fscanf(pFile, "%s", &aString[0]);	// = ��ǂݍ��� (�s�v)
						fscanf(pFile, "%f", &fMaxUp);		// �g�̍ō��㏸�ʂ�ǂݍ���
					}
					else if (strcmp(&aString[0], "ADD_SINROT") == 0)
					{ // �ǂݍ��񂾕����� ADD_SINROT �̏ꍇ

						fscanf(pFile, "%s", &aString[0]);	// = ��ǂݍ��� (�s�v)
						fscanf(pFile, "%f", &fAddSinRot);	// �g�ł��������Z�ʂ�ǂݍ���
					}
					else if (strcmp(&aString[0], "ADD_VTXROT") == 0)
					{ // �ǂݍ��񂾕����� ADD_VTXROT �̏ꍇ

						fscanf(pFile, "%s", &aString[0]);	// = ��ǂݍ��� (�s�v)
						fscanf(pFile, "%f", &fAddVtxRot);	// �הg�̌������Z�ʂ�ǂݍ���
					}
				} while (strcmp(&aString[0], "END_LIQUIDSET") != 0);	// �ǂݍ��񂾕����� END_LIQUIDSET �ł͂Ȃ��ꍇ���[�v

				// �t�̃I�u�W�F�N�g�̐���
				CLiquid *pLiquid = CLiquid::Create
				( // ����
					(CLiquid::EType)nType,		// ���
					pos,						// �ʒu
					D3DXToRadian(rot),			// ����
					size,						// �傫��
					col,						// �F
					part,						// ������
					texMove,					// �e�N�X�`���ړ���
					fMaxUp,						// �g�̍ō��㏸��
					D3DXToRadian(fAddSinRot),	// �g�ł��������Z��
					D3DXToRadian(fAddVtxRot)	// �הg�̌������Z��
				);
				if (pLiquid == nullptr)
				{ // �m�ۂɎ��s�����ꍇ

					// ���s��Ԃ�
					assert(false);
					return E_FAIL;
				}
			}
		} while (strcmp(&aString[0], "END_STAGE_LIQUIDSET") != 0);	// �ǂݍ��񂾕����� END_STAGE_LIQUIDSET �ł͂Ȃ��ꍇ���[�v
	}

	// ������Ԃ�
	return S_OK;
}
