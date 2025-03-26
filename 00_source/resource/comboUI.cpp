//============================================================
//
//	�R���{UI���� [comboUI.cpp]
//	Author�F��������
//
//============================================================
//************************************************************
//	�C���N���[�h�t�@�C��
//************************************************************
#include "comboUI.h"

#include "multiValue.h"

//************************************************************
//	�萔�錾
//************************************************************
namespace
{
	const int PRIORITY = 6;	// �^�C��UI�̗D�揇��

	// �R���{�֌W
	namespace combo
	{
		const int DIGIT = 2;	// ����
		const VECTOR3 POS = VECTOR3(1000.0f, 50.0f, 0.0f);			// �ʒu
		const VECTOR3 SIZE = VECTOR3(30.0f, 45.0f, 0.0f);			// �T�C�Y
		const VECTOR3 SPACE = VECTOR3(SIZE.x * 0.85f, 0.0f, 0.0f);	// �X�R�A������
	}

	// �X�R�A�֌W
	namespace score
	{
		const int DIGIT = 3;	// ����
		const VECTOR3 POS = VECTOR3(1100.0f, 80.0f, 0.0f);			// �ʒu
		const VECTOR3 SIZE = VECTOR3(40.0f, 60.0f, 0.0f);			// �T�C�Y
		const VECTOR3 SPACE = VECTOR3(SIZE.x * 0.85f, 0.0f, 0.0f);	// �X�R�A������
	}
}

//************************************************************
//	�q�N���X [CComboUI] �̃����o�֐�
//************************************************************
//============================================================
//	�R���X�g���N�^
//============================================================
CComboUI::CComboUI() : CObject(CObject::LABEL_UI, CObject::DIM_2D, PRIORITY),
m_pMulti(nullptr),		// �{���̏��
m_pScore(nullptr)		// �X�R�A�̏��
{

}

//============================================================
//	�f�X�g���N�^
//============================================================
CComboUI::~CComboUI()
{

}

//============================================================
//	����������
//============================================================
HRESULT CComboUI::Init()
{
	return S_OK;
}

//============================================================
//	�I������
//============================================================
void CComboUI::Uninit()
{
	if (m_pMulti != nullptr)
	{ // �{���� NULL ����Ȃ��ꍇ

		// �I������
		m_pMulti->Uninit();
		m_pMulti = nullptr;
	}

	if (m_pScore != nullptr)
	{ // �X�R�A�� NULL ����Ȃ��ꍇ

		// �I������
		m_pScore->Uninit();
		m_pScore = nullptr;
	}

	// �I�u�W�F�N�g��j��
	CObject::Release();
}

//============================================================
//	�X�V����
//============================================================
void CComboUI::Update(const float fDeltaTime)
{

}

//============================================================
//	�`�揈��
//============================================================
void CComboUI::Draw(CShader* pShader)
{

}

//============================================================
// ���̐ݒ菈��
//============================================================
void CComboUI::SetData()
{
	// �R���{�{���̏��𐶐�
	m_pMulti = CMultiValue::Create
	(
		CValue::TYPE_NORMAL,		// ���
		0,							// �l
		combo::DIGIT,				// ����
		combo::POS,					// �ʒu
		combo::SIZE,				// �T�C�Y
		combo::SPACE				// �]��
	);
	m_pMulti->SetLabel(CObject::LABEL_NONE);

	// �X�R�A�̏��𐶐�
	m_pScore = CMultiValue::Create
	(
		CValue::TYPE_NORMAL,		// ���
		0,							// �l
		score::DIGIT,				// ����
		score::POS,					// �ʒu
		score::SIZE,				// �T�C�Y
		score::SPACE				// �]��
	);
	m_pScore->SetLabel(CObject::LABEL_NONE);
}

//============================================================
//	��������
//============================================================
CComboUI* CComboUI::Create()
{
	// �^�C��UI�̐���
	CComboUI* pTimeUI = new CComboUI;
	if (pTimeUI == nullptr)
	{ // �����Ɏ��s�����ꍇ

		return nullptr;
	}
	else
	{ // �����ɐ��������ꍇ

		// �^�C��UI�̏�����
		if (FAILED(pTimeUI->Init()))
		{ // �������Ɏ��s�����ꍇ

			// �^�C��UI�̔j��
			SAFE_DELETE(pTimeUI);
			return nullptr;
		}

		// �m�ۂ����A�h���X��Ԃ�
		return pTimeUI;
	}
}
