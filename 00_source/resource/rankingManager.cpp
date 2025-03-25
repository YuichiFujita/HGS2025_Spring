//============================================================
//
//	�����L���O�}�l�[�W���[���� [rankingManager.cpp]
//	Author�F���c�E��
//
//============================================================
//************************************************************
//	�C���N���[�h�t�@�C��
//************************************************************
#include "rankingManager.h"
#include "rankingState.h"
#include "manager.h"
#include "anim2D.h"
#include "retention.h"

#ifdef SCORE
#include "multiValue.h"
#else TIMER
#include "timeUI.h"
#endif

//************************************************************
//	�萔�錾
//************************************************************
namespace
{
	namespace title
	{
		const char*		TEXTURE	= "data\\TEXTURE\\ranking_title000.png";	// �e�N�X�`���p�X
		const VECTOR3	POS		= VECTOR3(SCREEN_CENT.x, 80.0f, 0.0f);		// �ʒu
		const float		HEIGHT	= 140.0f;	// �c��
	}

	namespace rank
	{
		const char*		TEXTURE = "data\\TEXTURE\\ranking000.png";			// �e�N�X�`���p�X
		const POSGRID2	PTRN	= POSGRID2(CRankingManager::MAX_RANK, 1);	// �e�N�X�`��������
		const VECTOR3	POS		= VECTOR3(290.0f, 190.0f, 0.0f);			// �ʒu
		const VECTOR3	OFFSET	= VECTOR3(0.0f, 112.0f, 0.0f);				// �I�t�Z�b�g
		const float		WIDTH	= 160.0f * 1.3f;	// ����
	}

#ifdef SCORE
	const int	RANK_INIT[CRankingManager::MAX_RANK] = { 0, 0, 0, 0, 0 };	// ���������L���O
	const char*	RANK_PATH	= "data\\BIN\\rank_score.bin";	// �����L���O�p�X
	const COLOR	COL_UPDATE	= color::Yellow();	// �����L���O�X�V�F
	namespace score
	{
		const CValue::EType TYPE = CValue::TYPE_NORMAL;	// �������
		const int DIGIT		 = 8;						// �X�R�A����
		const VECTOR3 POS	 = VECTOR3(740.0f, 160.0f, 0.0f);		// �X�R�A�ʒu
		const VECTOR3 OFFSET = VECTOR3(0.0f, 112.0f, 0.0f);			// �X�R�A�I�t�Z�b�g
		const VECTOR3 SIZE	 = VECTOR3(52.8f, 62.4f, 0.0f) * 1.4f;	// �X�R�A�����傫��
		const VECTOR3 SPACE	 = VECTOR3(SIZE.x * 0.85f, 0.0f, 0.0f);	// �X�R�A������
	}
#else TIMER
	const float	RANK_INIT[CRankingManager::MAX_RANK] = { 0.0f, 0.0f, 0.0f, 0.0f, 0.0f };	// ���������L���O
	const char*	RANK_PATH	= "data\\BIN\\rank_time.bin";	// �����L���O�p�X
	const COLOR	COL_UPDATE	= color::Yellow();	// �����L���O�X�V�F
	namespace time
	{
		const CValue::EType TYPE = CValue::TYPE_NORMAL;	// �������
		const VECTOR3 POS		 = VECTOR3(720.0f, 190.0f, 0.0f);				// �^�C�}�[�ʒu
		const VECTOR3 OFFSET	 = VECTOR3(0.0f, 112.0f, 0.0f);					// �^�C�}�[�I�t�Z�b�g
		const VECTOR3 VAL_SIZE	 = VECTOR3(52.8f, 62.4f, 0.0f) * 1.4f * 1.3f;			// �^�C�}�[�����傫��
		const VECTOR3 PART_SIZE	 = VECTOR3(27.3f, 62.4f, 0.0f) * 1.2f * 1.3f;			// �^�C�}�[��؂�傫��
		const VECTOR3 VAL_SPACE	 = VECTOR3(VAL_SIZE.x * 0.85f, 0.0f, 0.0f);		// �^�C�}�[������
		const VECTOR3 PART_SPACE = VECTOR3(PART_SIZE.x * 0.85f, 0.0f, 0.0f);	// �^�C�}�[��؂��
	}
#endif
}

//************************************************************
//	�e�N���X [CRankingManager] �̃����o�֐�
//************************************************************
//============================================================
//	�R���X�g���N�^
//============================================================
CRankingManager::CRankingManager() :
	m_pState	(nullptr)	// ���
{
	memset(&m_apRank[0], 0, sizeof(m_apRank));		// ���ʏ��
#ifdef SCORE
	memset(&m_apScore[0], 0, sizeof(m_apScore));	// �X�R�A���
#else TIMER
	memset(&m_apTime[0], 0, sizeof(m_apTime));		// �^�C�����
#endif
}

//============================================================
//	�f�X�g���N�^
//============================================================
CRankingManager::~CRankingManager()
{

}

//============================================================
//	����������
//============================================================
HRESULT CRankingManager::Init()
{
	CRetention* pRetention = GET_MANAGER->GetRetention();	// �f�[�^�ۑ��}�l�[�W���[

	// �����o�ϐ���������
	memset(&m_apRank[0], 0, sizeof(m_apRank));		// ���ʏ��
#ifdef SCORE
	memset(&m_apScore[0], 0, sizeof(m_apScore));	// �X�R�A���
#else TIMER
	memset(&m_apTime[0], 0, sizeof(m_apTime));		// �^�C�����
#endif
	m_pState = nullptr;	// ���

#ifdef SCORE
	// �����L���O�̓Ǎ�
	int aRankArray[MAX_RANK];	// �����L���O���l�z��
	LoadRank(RANK_PATH, &aRankArray[0]);
#else TIMER
	// �����L���O�̓Ǎ�
	float aRankArray[MAX_RANK];	// �����L���O���l�z��
	LoadRank(RANK_PATH, &aRankArray[0]);
#endif

	// �^�C�g���̐���
	m_pTitle = CObject2D::Create(title::POS);
	if (m_pTitle == nullptr)
	{ // �����Ɏ��s�����ꍇ

		assert(false);
		return E_FAIL;
	}

	// �������x����ݒ�
	m_pTitle->SetLabel(CObject::LABEL_UI);

	// �^�C�g���e�N�X�`���̊���
	m_pTitle->BindTexture(title::TEXTURE);

	// �^�C�g���傫���̐ݒ�
	float fTitleWidth = useful::GetTexWidthFromAspect(title::HEIGHT, title::TEXTURE);	// �e�N�X�`����̉���
	m_pTitle->SetVec3Size(VECTOR3(fTitleWidth, title::HEIGHT, 0.0f));

	for (int i = 0; i < MAX_RANK; i++)
	{ // �����L���O��ʕ\�������J��Ԃ�

		// ���ʂ̐���
		m_apRank[i] = CAnim2D::Create
		( // ����
			rank::PTRN.x,	// �e�N�X�`���̉��̕�����
			rank::PTRN.y,	// �e�N�X�`���̏c�̕�����
			rank::POS + (rank::OFFSET * (float)i)	// �ʒu
		);
		if (m_apRank[i] == nullptr)
		{ // �����Ɏ��s�����ꍇ

			assert(false);
			return E_FAIL;
		}

		// ���ʃe�N�X�`���̊���
		m_apRank[i]->BindTexture(rank::TEXTURE);

		// ���ʑ傫���̐ݒ�
		float fRankHeight = useful::GetTexHeightFromAspect(rank::WIDTH, rank::TEXTURE);	// �e�N�X�`����̏c��
		m_apRank[i]->SetVec3Size(VECTOR3(rank::WIDTH, fRankHeight * (float)rank::PTRN.x, 0.0f));

		// ���ʃe�N�X�`���p�^�[���̐ݒ�
		m_apRank[i]->SetPattern(i);

#ifdef SCORE
		// �X�R�A�̐���
		const VECTOR3 posScore = score::POS + (score::OFFSET * (float)i);	// �X�R�A�ʒu
		m_apScore[i] = CMultiValue::Create
		( // ����
			score::TYPE,	// �������
			aRankArray[i],	// ����
			score::DIGIT,	// ����
			posScore,		// �ʒu
			score::SIZE,	// �傫��
			score::SPACE	// ��
		);
		if (m_apScore[i] == nullptr)
		{ // �����Ɏ��s�����ꍇ

			assert(false);
			return E_FAIL;
		}
#else TIMER
		// �^�C���̐���
		const VECTOR3 posTime = time::POS + (time::OFFSET * (float)i);	// �^�C���ʒu
		m_apTime[i] = CTimeUI::Create
		( // ����
			aRankArray[i],		// ����
			posTime,			// �ʒu
			time::VAL_SIZE,		// �����̑傫��
			time::PART_SIZE,	// ��؂�̑傫��
			time::VAL_SPACE,	// �����̋�
			time::PART_SPACE,	// ��؂�̋�
			time::TYPE			// �������
		);
		if (m_apTime[i] == nullptr)
		{ // �����Ɏ��s�����ꍇ

			assert(false);
			return E_FAIL;
		}
#endif
	}

	const int nUpdateIdx = pRetention->GetUpdateRankIdx();	// �����L���O�X�V�C���f�b�N�X
	if (nUpdateIdx > NONE_IDX)
	{ // �����L���O���X�V����Ă����ꍇ

#ifdef SCORE
		// �F��ύX����
		m_apScore[nUpdateIdx]->SetColor(COL_UPDATE);
#else TIMER
		// �F��ύX����
		m_apTime[nUpdateIdx]->SetColor(COL_UPDATE);
#endif
	}

	// �ʏ��Ԃɂ���
	ChangeState(new CRankingStateNormal);

	return S_OK;
}

//============================================================
//	�I������
//============================================================
void CRankingManager::Uninit()
{
	// ��Ԃ̏I��
	SAFE_UNINIT(m_pState);

	for (int i = 0; i < MAX_RANK; i++)
	{ // �����L���O��ʕ\�������J��Ԃ�

		// ���ʂ̏I��
		SAFE_UNINIT(m_apRank[i]);

#ifdef SCORE
		// �X�R�A�̏I��
		SAFE_UNINIT(m_apScore[i]);
#else TIMER
		// �^�C���̏I��
		SAFE_UNINIT(m_apTime[i]);
#endif
	}
}

//============================================================
//	�X�V����
//============================================================
void CRankingManager::Update(const float fDeltaTime)
{
	// ��Ԃ��Ƃ̍X�V
	assert(m_pState != nullptr);
	m_pState->Update(fDeltaTime);

	if (input::Decide())
	{
		// �^�C�g����ʂɑJ�ڂ���
		TransTitle();
	}

	for (int i = 0; i < MAX_RANK; i++)
	{ // �����L���O��ʕ\�������J��Ԃ�

		// ���ʂ̍X�V
		m_apRank[i]->Update(fDeltaTime);

#ifdef SCORE
		// �X�R�A�̍X�V
		m_apScore[i]->Update(fDeltaTime);
#else TIMER
		// �^�C���̍X�V
		m_apTime[i]->Update(fDeltaTime);
#endif
	}
}

//============================================================
//	��Ԃ̕ύX����
//============================================================
HRESULT CRankingManager::ChangeState(CRankingState* pState)
{
	// ��Ԃ̐����Ɏ��s���Ă���ꍇ������
	if (pState == nullptr) { assert(false); return E_FAIL; }

	// ��ԃC���X�^���X���I��
	SAFE_UNINIT(m_pState);

	// ��ԃC���X�^���X��ύX
	assert(m_pState == nullptr);
	m_pState = pState;

	// ��ԂɃR���e�L�X�g��ݒ�
	m_pState->SetContext(this);

	// ��ԃC���X�^���X��������
	if (FAILED(m_pState->Init()))
	{ // �������Ɏ��s�����ꍇ

		assert(false);
		return E_FAIL;
	}

	return S_OK;
}

//============================================================
//	�^�C�g����ʑJ�ڏ���
//============================================================
void CRankingManager::TransTitle()
{
	// �t�F�[�h���̏ꍇ������
	if (GET_MANAGER->GetFade()->IsFade()) { return; }

	// �����L���O�X�V���̏�����
	CRetention* pRetention = GET_MANAGER->GetRetention();	// �f�[�^�ۑ��}�l�[�W���[
	pRetention->SetUpdateRankIdx(NONE_IDX);

	// �Q�[����ʂɑJ�ڂ���
	GET_MANAGER->SetLoadScene(CScene::MODE_TITLE);
}

//============================================================
//	��������
//============================================================
CRankingManager* CRankingManager::Create()
{
	// �����L���O�}�l�[�W���[�̐���
	CRankingManager* pRankingManager = new CRankingManager;
	if (pRankingManager == nullptr)
	{ // �����Ɏ��s�����ꍇ

		return nullptr;
	}
	else
	{ // �����ɐ��������ꍇ

		// �����L���O�}�l�[�W���[�̏�����
		if (FAILED(pRankingManager->Init()))
		{ // �������Ɏ��s�����ꍇ

			// �����L���O�}�l�[�W���[�̔j��
			SAFE_DELETE(pRankingManager);
			return nullptr;
		}

		// �m�ۂ����A�h���X��Ԃ�
		return pRankingManager;
	}
}

//============================================================
//	�j������
//============================================================
void CRankingManager::Release(CRankingManager*& prRankingManager)
{
	// �����L���O�}�l�[�W���[�̏I��
	assert(prRankingManager != nullptr);
	prRankingManager->Uninit();

	// �������J��
	SAFE_DELETE(prRankingManager);
}

#ifdef SCORE
//============================================================
//	�����L���O�̐ݒ菈��
//============================================================
int CRankingManager::SetRank(const int nNewScore)
{
	int nLowIdx = MAX_RANK - 1;	// �ŉ��ʃC���f�b�N�X
	int nUpdateIdx = NONE_IDX;	// �X�V���ʃC���f�b�N�X
	int aRank[MAX_RANK];		// �����L���O����

	// �����L���O�̓Ǎ�
	if (FAILED(LoadRank(RANK_PATH, &aRank[0])))
	{ // �Ǎ��Ɏ��s�����ꍇ

		// �����L���O�̍ēǍ�
		if (FAILED(LoadRank(RANK_PATH, &aRank[0])))
		{ // �ēǍ��Ɏ��s�����ꍇ

			// ���s��Ԃ�
			assert(false);
			return NONE_IDX;
		}
	}

	if (nNewScore > aRank[nLowIdx])
	{ // �ŉ��ʂ̃X�R�A��荂���ꍇ

		// �����L���O�̃\�[�g
		nUpdateIdx = SortRank(nNewScore, &aRank[0]);

		// �����L���O�̕ۑ�
		SaveRank(RANK_PATH, &aRank[0]);
	}

	// ���ʂ��X�V���ꂽ�C���f�b�N�X��Ԃ�
	return nUpdateIdx;
}

//============================================================
//	�����L���O�̃\�[�g����
//============================================================
int CRankingManager::SortRank(const int nNewScore, int* pRankArray)
{
	int nLowIdx = MAX_RANK - 1;	// �ŉ��ʃC���f�b�N�X
	int	nCurMaxIdx;	// �ő�l�̃C���f�b�N�X

	std::vector<int> idxRank;	// �����L���O�C���f�b�N�X
	for (int i = 0; i < MAX_RANK; i++)
	{ idxRank.push_back(i); }	// ���g�̗v�f�ԍ���ۑ�

	// ���݂̍ŉ��ʂ̏��Ə�������
	pRankArray[nLowIdx] = nNewScore;

	for (int nCntKeep = 0; nCntKeep < nLowIdx; nCntKeep++)
	{ // ����ւ���l�̑��� -1�񕪌J��Ԃ�

		// ���݂̌J��Ԃ������� (�v�f1�Ƃ���)
		nCurMaxIdx = nCntKeep;

		for (int nCntSort = nCntKeep + 1; nCntSort < MAX_RANK; nCntSort++)
		{ // ����ւ���l�̑��� -nCntKeep���J��Ԃ�

			if (pRankArray[nCurMaxIdx] < pRankArray[nCntSort])
			{ // �ő�l�ɐݒ肳��Ă���l���A���݂̒l�̂ق����傫���ꍇ

				// ���݂̗v�f�ԍ����ő�l�ɐݒ�
				nCurMaxIdx = nCntSort;
			}
		}

		if (nCntKeep != nCurMaxIdx)
		{ // �ő�l�̗v�f�ԍ��ɕϓ����������ꍇ

			// �v�f�̓���ւ�
			int nKeepNum			= pRankArray[nCntKeep];
			pRankArray[nCntKeep]	= pRankArray[nCurMaxIdx];
			pRankArray[nCurMaxIdx]	= nKeepNum;

			// �v�f�̓���ւ�
			int nKeepIdx		= idxRank[nCntKeep];
			idxRank[nCntKeep]	= idxRank[nCurMaxIdx];
			idxRank[nCurMaxIdx]	= nKeepIdx;
		}
	}

	// �ŉ��ʃC���f�b�N�X������
	auto itr = std::find(idxRank.begin(), idxRank.end(), nLowIdx);

	// ���ʂ��X�V���ꂽ�C���f�b�N�X��Ԃ�
	return std::distance(idxRank.begin(), itr);
}

//============================================================
//	�����L���O�̓Ǎ�����
//============================================================
HRESULT CRankingManager::LoadRank(const char* pPath, int* pRankArray)
{
	// �t�@�C�����J��
	std::ifstream file(pPath, std::ios_base::binary);	// �t�@�C���X�g���[��
	if (file.fail())
	{ // �t�@�C�����J���Ȃ������ꍇ

#ifdef _DEBUG
		// �G���[���b�Z�[�W�{�b�N�X
		MessageBox(nullptr, "�����L���O�̓ǂݍ��݂Ɏ��s�I", "�x���I", MB_ICONWARNING);
#endif

		// �l�����ׂčő�^�C���ɂ���
		int aRank[MAX_RANK];	// �����L���O�����p
		for (int i = 0; i < MAX_RANK; i++)
		{ aRank[i] = RANK_INIT[i]; }

		// �N���A�^�C���̕ۑ�
		SaveRank(pPath, &aRank[0]);

		// ���s��Ԃ�
		return E_FAIL;
	}

	// �����̊l���󋵂�ǂݍ���
	file.read((char*)pRankArray, sizeof(int) * MAX_RANK);

	// �t�@�C�������
	file.close();

	// ������Ԃ�
	return S_OK;
}

//============================================================
//	�����L���O�̕ۑ�����
//============================================================
HRESULT CRankingManager::SaveRank(const char* pPath, int* pRankArray)
{
	// �t�@�C�����J��
	std::ofstream file(pPath, std::ios_base::binary);	// �t�@�C���X�g���[��
	if (file.fail())
	{ // �t�@�C�����J���Ȃ������ꍇ

		// �G���[���b�Z�[�W�{�b�N�X
		MessageBox(nullptr, "�����L���O�̏����o���Ɏ��s�I", "�x���I", MB_ICONWARNING);

		// ���s��Ԃ�
		return E_FAIL;
	}

	// �����̊l���󋵂������o��
	file.write((char*)pRankArray, sizeof(int) * MAX_RANK);

	// �t�@�C�������
	file.close();

	// ������Ԃ�
	return S_OK;
}
#else TIMER
//============================================================
//	�����L���O�̐ݒ菈��
//============================================================
int CRankingManager::SetRank(const float fNewTime)
{
	int nLowIdx = MAX_RANK - 1;	// �ŉ��ʃC���f�b�N�X
	int nUpdateIdx = NONE_IDX;	// �X�V���ʃC���f�b�N�X
	float aRank[MAX_RANK];		// �����L���O����

	// �����L���O�̓Ǎ�
	if (FAILED(LoadRank(RANK_PATH, &aRank[0])))
	{ // �Ǎ��Ɏ��s�����ꍇ

		// �����L���O�̍ēǍ�
		if (FAILED(LoadRank(RANK_PATH, &aRank[0])))
		{ // �ēǍ��Ɏ��s�����ꍇ

			// ���s��Ԃ�
			assert(false);
			return NONE_IDX;
		}
	}

	if (fNewTime > aRank[nLowIdx])
	{ // �ŉ��ʂ̎��Ԃ�葬���ꍇ

		// �����L���O�̃\�[�g
		nUpdateIdx = SortRank(fNewTime, &aRank[0]);

		// �����L���O�̕ۑ�
		SaveRank(RANK_PATH, &aRank[0]);
	}

	// ���ʂ��X�V���ꂽ�C���f�b�N�X��Ԃ�
	return nUpdateIdx;
}

//============================================================
//	�����L���O�̃\�[�g����
//============================================================
int CRankingManager::SortRank(const float fNewTime, float* pRankArray)
{
	int nLowIdx = MAX_RANK - 1;	// �ŉ��ʃC���f�b�N�X
	int	nCurMinIdx;	// �ŏ��l�̃C���f�b�N�X

	std::vector<int> idxRank;	// �����L���O�C���f�b�N�X
	for (int i = 0; i < MAX_RANK; i++)
	{ idxRank.push_back(i); }	// ���g�̗v�f�ԍ���ۑ�

	// ���݂̍ŉ��ʂ̏��Ə�������
	pRankArray[nLowIdx] = fNewTime;

	for (int nCntKeep = 0; nCntKeep < nLowIdx; nCntKeep++)
	{ // ����ւ���l�̑��� -1�񕪌J��Ԃ�

		// ���݂̌J��Ԃ������� (�v�f1�Ƃ���)
		nCurMinIdx = nCntKeep;

		for (int nCntSort = nCntKeep + 1; nCntSort < MAX_RANK; nCntSort++)
		{ // ����ւ���l�̑��� -nCntKeep���J��Ԃ�

			if (pRankArray[nCurMinIdx] < pRankArray[nCntSort])
			{ // �ŏ��l�ɐݒ肳��Ă���l���A���݂̒l�̂ق����������ꍇ

				// ���݂̗v�f�ԍ����ŏ��l�ɐݒ�
				nCurMinIdx = nCntSort;
			}
		}

		if (nCntKeep != nCurMinIdx)
		{ // �ŏ��l�̗v�f�ԍ��ɕϓ����������ꍇ

			// �v�f�̓���ւ�
			float fKeepNum			= pRankArray[nCntKeep];
			pRankArray[nCntKeep]	= pRankArray[nCurMinIdx];
			pRankArray[nCurMinIdx]	= fKeepNum;

			// �v�f�̓���ւ�
			int nKeepIdx		= idxRank[nCntKeep];
			idxRank[nCntKeep]	= idxRank[nCurMinIdx];
			idxRank[nCurMinIdx]	= nKeepIdx;
		}
	}

	// �ŉ��ʃC���f�b�N�X������
	auto itr = std::find(idxRank.begin(), idxRank.end(), nLowIdx);

	// ���ʂ��X�V���ꂽ�C���f�b�N�X��Ԃ�
	return std::distance(idxRank.begin(), itr);
}

//============================================================
//	�����L���O�̓Ǎ�����
//============================================================
HRESULT CRankingManager::LoadRank(const char* pPath, float* pRankArray)
{
	// �t�@�C�����J��
	std::ifstream file(pPath, std::ios_base::binary);	// �t�@�C���X�g���[��
	if (file.fail())
	{ // �t�@�C�����J���Ȃ������ꍇ

#ifdef _DEBUG
		// �G���[���b�Z�[�W�{�b�N�X
		MessageBox(nullptr, "�����L���O�̓ǂݍ��݂Ɏ��s�I", "�x���I", MB_ICONWARNING);
#endif

		// �l�����ׂčő�^�C���ɂ���
		float aRank[MAX_RANK];	// �����L���O�����p
		for (int i = 0; i < MAX_RANK; i++)
		{ aRank[i] = RANK_INIT[i]; }

		// �N���A�^�C���̕ۑ�
		SaveRank(pPath, &aRank[0]);

		// ���s��Ԃ�
		return E_FAIL;
	}

	// �����̊l���󋵂�ǂݍ���
	file.read((char*)pRankArray, sizeof(float) * MAX_RANK);

	// �t�@�C�������
	file.close();

	// ������Ԃ�
	return S_OK;
}

//============================================================
//	�����L���O�̕ۑ�����
//============================================================
HRESULT CRankingManager::SaveRank(const char* pPath, float* pRankArray)
{
	// �t�@�C�����J��
	std::ofstream file(pPath, std::ios_base::binary);	// �t�@�C���X�g���[��
	if (file.fail())
	{ // �t�@�C�����J���Ȃ������ꍇ

		// �G���[���b�Z�[�W�{�b�N�X
		MessageBox(nullptr, "�����L���O�̏����o���Ɏ��s�I", "�x���I", MB_ICONWARNING);

		// ���s��Ԃ�
		return E_FAIL;
	}

	// �����̊l���󋵂������o��
	file.write((char*)pRankArray, sizeof(float) * MAX_RANK);

	// �t�@�C�������
	file.close();

	// ������Ԃ�
	return S_OK;
}
#endif
