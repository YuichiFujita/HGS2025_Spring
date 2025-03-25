//============================================================
//
//	�Q�[����ʃw�b�_�[ [sceneGame.h]
//	Author�F���c�E��
//
//============================================================
//************************************************************
//	��d�C���N���[�h�h�~
//************************************************************
#ifndef _SCENE_GAME_H_
#define _SCENE_GAME_H_

//************************************************************
//	�C���N���[�h�t�@�C��
//************************************************************
#include "scene.h"

//************************************************************
//	�O���錾
//************************************************************
class CGameManager;	// �Q�[���}�l�[�W���[�N���X
class CPause;		// �|�[�Y�N���X
class CHitStop;		// �q�b�g�X�g�b�v�N���X

//************************************************************
//	�N���X��`
//************************************************************
// �Q�[����ʃN���X
class CSceneGame : public CScene
{
public:
	// �R���X�g���N�^
	explicit CSceneGame(const EMode mode);

	// �f�X�g���N�^
	~CSceneGame() override;

	// �I�[�o�[���C�h�֐�
	HRESULT Init() override;	// ������
	void Uninit() override;		// �I��
	void Update(const float fDeltaTime) override;	// �X�V

	// �ÓI�����o�֐�
	static CGameManager* GetGameManager();	// �Q�[���}�l�[�W���[�擾
	static CPause* GetPause();		// �|�[�Y�擾
	static CHitStop* GetHitStop();	// �q�b�g�X�g�b�v�擾

private:
	// �ÓI�����o�ϐ�
	static CGameManager* m_pGameManager;	// �Q�[���}�l�[�W���[
	static CPause* m_pPause;		// �|�[�Y���
	static CHitStop* m_pHitStop;	// �q�b�g�X�g�b�v���
};

#endif	// _SCENE_GAME_H_
