//============================================================
//
//	����}�l�[�W���[���� [inputManager.cpp]
//	Author�F���c�E��
//
//============================================================
//************************************************************
//	�C���N���[�h�t�@�C��
//************************************************************
#include "inputManager.h"
#include "manager.h"

//************************************************************
//	������
//************************************************************
//============================================================
//	�m�葀��
//============================================================
bool input::Decide()
{
	CInputKeyboard* pKey = GET_INPUTKEY;	// �L�[�{�[�h���
	CInputPad* pPad = GET_INPUTPAD;			// �p�b�h���
	return (pKey->IsTrigger(DIK_Z) || pKey->IsTrigger(DIK_RETURN) || pPad->IsTrigger(CInputPad::KEY_A) || pPad->IsTrigger(CInputPad::KEY_START));
}

//============================================================
//	�L�����Z������
//============================================================
bool input::Cancel()
{
	CInputKeyboard* pKey = GET_INPUTKEY;	// �L�[�{�[�h���
	CInputPad* pPad = GET_INPUTPAD;			// �p�b�h���
	return (pKey->IsTrigger(DIK_X) || pKey->IsTrigger(DIK_LSHIFT) || pKey->IsTrigger(DIK_RSHIFT) || pPad->IsTrigger(CInputPad::KEY_B) || pPad->IsTrigger(CInputPad::KEY_BACK));
}

//============================================================
//	�t�B�[���h���j���[����
//============================================================
bool input::FieldMenu()
{
	CInputKeyboard* pKey = GET_INPUTKEY;	// �L�[�{�[�h���
	return (pKey->IsTrigger(DIK_C) || pKey->IsTrigger(DIK_LCONTROL) || pKey->IsTrigger(DIK_RCONTROL));
}
