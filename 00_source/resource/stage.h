//============================================================
//
//	�X�e�[�W�w�b�_�[ [stage.h]
//	Author�F���c�E��
//
//============================================================
//************************************************************
//	��d�C���N���[�h�h�~
//************************************************************
#ifndef _STAGE_H_
#define _STAGE_H_

//************************************************************
//	�C���N���[�h�t�@�C��
//************************************************************
#include "scene.h"

//************************************************************
//	�O���錾
//************************************************************
class CField;	// �n�ʃN���X

//************************************************************
//	�N���X��`
//************************************************************
// �X�e�[�W�N���X
class CStage
{
public:
	// ������
	enum ELimit
	{
		LIMIT_NONE = 0,	// �͈͐����Ȃ�
		LIMIT_BOX,		// ��`�͈�
		LIMIT_CIRCLE,	// �~�͈�
		LIMIT_MAX		// ���̗񋓌^�̑���
	};

	// �R���X�g���N�^
	CStage();

	// �f�X�g���N�^
	~CStage();

	// �͈͍\����
	struct SLimit
	{
		ELimit mode;	// �������[�h
		VECTOR3 center;	// ���S���W
		float fNear;	// �����ʒu (�O)
		float fFar;		// �����ʒu (��)
		float fRight;	// �����ʒu (�E)
		float fLeft;	// �����ʒu (��)
		float fRadius;	// �����ʒu (���a)
		float fField;	// �����ʒu (�n��)
		float fKillY;	// �����ʒu (�폜)
	};

	// �����o�֐�
	HRESULT Init();	// ������
	void Uninit();	// �I��
	HRESULT BindStage(const char* pPath);		// �X�e�[�W����
	void DeleteStage();							// �X�e�[�W�폜
	inline void SetLimit(const SLimit& rLimit)	{ m_limit = rLimit; }	// �͈͐ݒ�
	inline SLimit GetLimit() const				{ return m_limit; }		// �͈͎擾

	bool LandLimitPosition(VECTOR3& rPos, VECTOR3& rMove, const float fHeight);	// �͈͊O�̒��n����
	bool LandFieldPosition(VECTOR3& rPos, VECTOR3& rOldPos, VECTOR3& rMove, CField** ppCollField = nullptr);	// �n�ʒ��n
	bool LandFieldPositionTop(VECTOR3& rPos, VECTOR3& rMove, CField** ppCollField = nullptr);					// ��ԏ�̒n�ʒ��n
	void LimitPosition(VECTOR3& rPos, const float fRadius);	// �ʒu�␳
	bool CollisionKillY(const VECTOR3& rPos);				// �L��Y���W�Ƃ̓����蔻��
	bool IsFieldPositionRange(const VECTOR3& rPos);			// ���b�V���͈͓̔��擾
	float GetFieldPositionHeight(const VECTOR3& rPos);		// ���b�V���̒��n�ʒu�擾
	float GetFieldDownPositionHeight(const VECTOR3& rPos);	// ���b�V���̒��n�ʒu�擾 (���݈ʒu���͉�)

	// �ÓI�����o�֐�
	static CStage* Create();				// ����
	static void Release(CStage*& prStage);	// �j��

private:
	// �����o�֐�
	HRESULT LoadLimit(const char* pString, FILE* pFile);	// �͈͏��̓Ǎ�
	HRESULT LoadField(const char* pString, FILE* pFile);	// �n�ʏ��̓Ǎ�
	HRESULT LoadWall(const char* pString, FILE* pFile);		// �Ǐ��̓Ǎ�
	HRESULT LoadScenery(const char* pString, FILE* pFile);	// �i�F���̓Ǎ�
	HRESULT LoadSky(const char* pString, FILE* pFile);		// ����̓Ǎ�
	HRESULT LoadLiquid(const char* pString, FILE* pFile);	// �t�̏��̓Ǎ�

	// �����o�ϐ�
	std::string m_sCurPath;	// ���݂̃X�e�[�W�p�X
	SLimit m_limit;			// �͈͏��
};

#endif	// _STAGE_H_
