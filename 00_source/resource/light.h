//============================================================
//
//	���C�g�w�b�_�[ [light.h]
//	Author�F���c�E��
//
//============================================================
//************************************************************
//	��d�C���N���[�h�h�~
//************************************************************
#ifndef _LIGHT_H_
#define _LIGHT_H_

//************************************************************
//	�C���N���[�h�t�@�C��
//************************************************************
#include "object.h"

//************************************************************
//	�N���X��`
//************************************************************
// ���C�g�N���X
class CLight : public CObject
{
public:
	// �R���X�g���N�^
	CLight();

	// �f�X�g���N�^
	~CLight() override;

	// �I�[�o�[���C�h�֐�
	HRESULT Init() override;	// ������
	void Uninit() override;		// �I��
	void Update(const float fDeltaTime) override;	// �X�V
	void Draw(CShader* pShader = nullptr) override;	// �`��

	// �����o�֐�
	void SetType(const D3DLIGHTTYPE type);	// ��ސݒ�
	void SetDiffuse(const COLOR& rCol);		// �g�U���ݒ�
	void SetDirection(const VECTOR3& rDir);	// �����ݒ�
	void SetEnableLight(const bool bLight);	// �L���ݒ�
	inline D3DLIGHT9 GetLight() { return m_light; }	// ���C�g�擾

private:
	// �I�[�o�[���C�h�֐�
	inline void Release() override { CObject::Release(); }	// �j��

	// �ÓI�����o�ϐ�
	static int m_nNumLight;	// ���C�g�̑���

	// �����o�ϐ�
	D3DLIGHT9 m_light;		// ���C�g���
	const int m_nLightIdx;	// ���g�̃��C�g�C���f�b�N�X
};

#endif	// _LIGHT_H_
