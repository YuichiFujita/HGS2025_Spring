//============================================================
//
//	�I����ԃw�b�_�[ [gameStateEnd.h]
//	Author�F���c�E��
//
//============================================================
//************************************************************
//	��d�C���N���[�h�h�~
//************************************************************
#ifndef _GAME_STATE_END_H_
#define _GAME_STATE_END_H_

//************************************************************
//	�C���N���[�h�t�@�C��
//************************************************************
#include "gameState.h"

//************************************************************
//	�N���X��`
//************************************************************
// �I����ԃN���X
class CGameStateEnd : public CGameState
{
public:
	// �R���X�g���N�^
	CGameStateEnd();

	// �f�X�g���N�^
	~CGameStateEnd() override;

	// �I�[�o�[���C�h�֐�
	HRESULT Init() override;	// ������
	void Uninit() override;		// �I��
	void Update(const float fDeltaTime) override;	// �X�V

private:
	// �����o�ϐ�
	float m_fCurTime;	// �o�ߎ���
};

#endif	// _GAME_STATE_END_H_
