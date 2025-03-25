//============================================================
//
//	�V�[���w�b�_�[ [scene.h]
//	Author�F���c�E��
//
//============================================================
//************************************************************
//	��d�C���N���[�h�h�~
//************************************************************
#ifndef _SCENE_H_
#define _SCENE_H_

//************************************************************
//	�O���錾
//************************************************************
class CPlayer;	// �v���C���[�N���X

//************************************************************
//	�N���X��`
//************************************************************
// �V�[���N���X
class CScene
{
public:
	// ���[�h��
	enum EMode
	{
		MODE_TITLE = 0,	// �^�C�g�����
		MODE_GAME,		// �Q�[�����
		MODE_RESULT,	// ���U���g���
		MODE_RANKING,	// �����L���O���
		MODE_MAX		// ���̗񋓌^�̑���
	};

	// �R���X�g���N�^
	explicit CScene(const EMode mode);

	// �f�X�g���N�^
	virtual ~CScene();

	// ���z�֐�
	virtual HRESULT Init();	// ������
	virtual void Uninit();	// �I��
	virtual void Update(const float fDeltaTime);	// �X�V

	// �ÓI�����o�֐�
	static CScene* Create(const EMode mode);	// ����
	static void Release(CScene*& prScene);		// �j��
	static CPlayer* GetPlayer();				// �v���C���[�擾

	// �����o�֐�
	inline void SetMode(const EMode mode)	{ m_mode = mode; }	// ���[�h�ݒ�
	inline EMode GetMode() const			{ return m_mode; }	// ���[�h�擾

private:
	// �����o�ϐ�
	EMode m_mode;	// ���[�h
};

#endif	// _SCENE_H_
