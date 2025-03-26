//============================================================
//
//	�R���{�G���A���� [comboArea.cpp]
//	Author�F��������
//
//============================================================
//************************************************************
//	�C���N���[�h�t�@�C��
//************************************************************
#include "comboArea.h"

//************************************************************
//	�萔�錾
//************************************************************
namespace
{
	const std::string TEXTURE[] =	// �e�N�X�`��
	{
		"data\\TEXTURE\\ComboArea000.png",
		"data\\TEXTURE\\ComboArea001.png",
		"data\\TEXTURE\\ComboArea002.png",
	};
	const D3DXCOLOR COL[] =	// �e�N�X�`��
	{
		D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f),
		D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f),
		D3DXCOLOR(0.0f, 0.0f, 1.0f, 1.0f),
	};
	const D3DXVECTOR3 RADIUS = D3DXVECTOR3(1440.0f, 240.0f, 0.0f);		// ���a
	const float MULTI_RATE = 0.5f;		// �R���{�{��
}

//************************************************************
//	�ÓI�����o�ϐ��錾
//************************************************************
CListManager<CComboArea>* CComboArea::m_pList = nullptr;	// �I�u�W�F�N�g���X�g

//************************************************************
//	�q�N���X [CComboArea] �̃����o�֐�
//************************************************************
//============================================================
//	�R���X�g���N�^
//============================================================
CComboArea::CComboArea() : CObject3D(CObject::LABEL_BLOCK, CObject::DIM_3D, 0),
m_nIdx(0)		// �ԍ�
{

}

//============================================================
//	�f�X�g���N�^
//============================================================
CComboArea::~CComboArea()
{

}

//============================================================
//	����������
//============================================================
HRESULT CComboArea::Init()
{
	// �I�u�W�F�N�g3D�̏�����
	if (FAILED(CObject3D::Init()))
	{ // �������Ɏ��s�����ꍇ

		// ���s��Ԃ�
		assert(false);
		return E_FAIL;
	}

	if (m_pList == nullptr)
	{ // ���X�g�}�l�[�W���[�����݂��Ȃ��ꍇ

		// ���X�g�}�l�[�W���[�̐���
		m_pList = CListManager<CComboArea>::Create();
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
void CComboArea::Uninit()
{
	// ���X�g���玩�g�̃I�u�W�F�N�g���폜
	m_pList->DelList(m_iterator);

	if (m_pList->GetNumAll() == 0)
	{ // �I�u�W�F�N�g������Ȃ��ꍇ

		// ���X�g�}�l�[�W���[�̔j��
		m_pList->Release(m_pList);
	}

	// �I�u�W�F�N�g3D�̏I��
	CObject3D::Uninit();
}

//============================================================
//	�X�V����
//============================================================
void CComboArea::Update(const float fDeltaTime)
{
	// ���_���W�̐ݒ菈��
	SetVtx();
}

//============================================================
//	�`�揈��
//============================================================
void CComboArea::Draw(CShader* pShader)
{
	// �I�u�W�F�N�g3D�̕`��
	CObject3D::Draw(pShader);
}

//============================================================
// �{���̎擾����
//============================================================
float CComboArea::GetMulti(void) const
{
	// �R���{�{����Ԃ�
	return (float)m_nIdx * MULTI_RATE;
}

//============================================================
//	��������
//============================================================
CComboArea* CComboArea::Create
(
	const VECTOR3& rPos,	// �ʒu
	const int nMul			// �R���{�{��
)
{
	// �R���{�̍ő�l�𒴂����ꍇ�A��~
	assert(nMul - 1 < NUM_ARRAY(TEXTURE));

	// �R���{�G���A�̐���
	CComboArea* pBlock = new CComboArea;

	if (pBlock == nullptr)
	{ // �����Ɏ��s�����ꍇ

		return nullptr;
	}
	else
	{ // �����ɐ��������ꍇ

		// �v���C���[�̏�����
		if (FAILED(pBlock->Init()))
		{ // �������Ɏ��s�����ꍇ

			// �v���C���[�̔j��
			SAFE_DELETE(pBlock);
			return nullptr;
		}

		// �ʒu��ݒ�
		pBlock->SetVec3Position(rPos);

		// ������ݒ�
		pBlock->SetVec3Rotation(VEC3_ZERO);

		// �T�C�Y��ݒ�
		pBlock->SetVec3Size(RADIUS);

		// �{����ݒ�
		pBlock->m_nIdx = nMul;

		// �e�N�X�`���̊��蓖�ď���
		pBlock->BindTexture(TEXTURE[nMul - 1].c_str());

		pBlock->SetColor(COL[nMul - 1]);

		// �m�ۂ����A�h���X��Ԃ�
		return pBlock;
	}
}

//============================================================
//	���X�g�擾����
//============================================================
CListManager<CComboArea>* CComboArea::GetList()
{
	// �I�u�W�F�N�g���X�g��Ԃ�
	return m_pList;
}
