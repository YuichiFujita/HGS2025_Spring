//============================================================
//
//	�ʏ��ԃw�b�_�[ [gameStateNormal.h]
//	Author�F���c�E��
//
//============================================================
//************************************************************
//	��d�C���N���[�h�h�~
//************************************************************
#ifndef _GAME_STATE_NORMAL_H_
#define _GAME_STATE_NORMAL_H_

//************************************************************
//	�C���N���[�h�t�@�C��
//************************************************************
#include "gameState.h"

//************************************************************
//	�N���X��`
//************************************************************
// �ʏ��ԃN���X
class CGameStateNormal : public CGameState
{
public:
	// �R���X�g���N�^
	CGameStateNormal();

	// �f�X�g���N�^
	~CGameStateNormal() override;

	// �I�[�o�[���C�h�֐�
	HRESULT Init() override;	// ������
	void Uninit() override;		// �I��
	void Update(const float fDeltaTime) override;	// �X�V
};

#endif	// _GAME_STATE_NORMAL_H_
