//============================================================
//
//	�֗��֐����� [useful.cpp]
//	Author�F���c�E��
//
//============================================================
//************************************************************
//	�C���N���[�h�t�@�C��
//************************************************************
#include "useful.h"
#include "manager.h"
#include "texture.h"
#include <regex>

//************************************************************
//	�ʏ�֐�
//************************************************************
//	�֗��֐����
//************************************************************
//============================================================
//	���l�̌������Ƃ̕���
//============================================================
//	�X�R�A�\���̍ۂȂǂɎg�p
//============================================================
void useful::DivideDigitNum
(
	int* pNumDivide,	// �������ʂ̊i�[�z��
	const int nNum,		// �������鐔�l
	const int nMaxDigit	// �������鐔���̌���
)
{
	int nDigit = 1;	// �e���̐��l�̌v�Z�p
	for (int nCntDigit = 0; nCntDigit < nMaxDigit; nCntDigit++)
	{ // ���̍ő吔���J��Ԃ�

		// ��������Z����
		nDigit *= 10;
	}

	for (int nCntTex = nMaxDigit - 1; nCntTex >= 0; nCntTex--, pNumDivide++)
	{ // �����̍ő傩�猅���̍ŏ��܂ŌJ��Ԃ�

		// �������Ƃ̐��l�����߂�
		*pNumDivide = (nNum % nDigit) / (nDigit / 10);

		// ���Z���Ď��̌����ɍ��킹��
		nDigit /= 10;
	}
}

//============================================================
//	�@���̐��K��
//============================================================
void useful::NormalizeNormal
(
	const VECTOR3& rPosLeft,	// ���ʒu
	const VECTOR3& rPosCenter,	// ���S�ʒu
	const VECTOR3& rPosRight,	// �E�ʒu
	VECTOR3& rNor				// �@��
)
{
	VECTOR3 vecLine[2];	// �Ӄx�N�g��

	// �Ӄx�N�g�������߂�
	vecLine[0] = rPosLeft - rPosCenter;
	vecLine[1] = rPosRight - rPosCenter;

	// �@�����v�Z
	rNor.x = (vecLine[0].y * vecLine[1].z) - (vecLine[0].z * vecLine[1].y);
	rNor.y = (vecLine[0].z * vecLine[1].x) - (vecLine[0].x * vecLine[1].z);
	rNor.z = (vecLine[0].x * vecLine[1].y) - (vecLine[0].y * vecLine[1].x);

	// �@���𐳋K��
	D3DXVec3Normalize(&rNor, &rNor);
}

//============================================================
//	��������̘A�������̒u��
//============================================================
void useful::ReplaceConsecChar
(
	std::string* pDestStr,		// �u���������s��������
	const char cRepChar,		// ���o���镶��
	const std::string& rRepStr,	// �u�������镶����
	const int nRepStrLength		// �u�������镶����̒��� (�ʏ��string��size)

)
{
	int nStartIdx = 0;	// �������o�̊J�n�C���f�b�N�X
	while (1)
	{ // ��؂蕶�������������ꍇ

		// ���������������̕��������o
		int nFindIdx = pDestStr->find(cRepChar, nStartIdx);	// ���o�C���f�b�N�X

		// �����̕��������o����Ȃ������ꍇ���[�v�𔲂���
		if ((size_t)nFindIdx == std::string::npos) { break; }

		// �u�������̐擪�܂ŕ�������폜����
		std::string sFindStr = *pDestStr;	// �u���������������
		sFindStr.erase(0, nFindIdx);		// ���o�C���f�b�N�X�܂ŕ�����폜

		int nCntConsec = 0;	// �����A���J�E���^�[
		for (char cChar : sFindStr)
		{ // ������̒������J��Ԃ�

			if (cChar != cRepChar)
			{ // ���o�����ƕʂ̕����ɂȂ����ꍇ

				// �A�����������ׂč폜���A�u���������������
				pDestStr->replace(nFindIdx, nCntConsec, rRepStr);

				// �������o�̊J�n�C���f�b�N�X��ݒ�
				nStartIdx = nFindIdx + nRepStrLength;

				break;
			}

			// �����A���������Z
			nCntConsec++;
		}
	}
}

//============================================================
//	�������̌v�Z����
//============================================================
VECTOR3 useful::CalcParabola3D
(
	const VECTOR3& rPosStart,	// �ˏo�ʒu
	const VECTOR3& rPosEnd,		// ���e�ʒu
	const float fMaxHeight,		// �ō����B����
	const float fEndTime,		// �I������
	const float fCurTime		// ���ݎ���
)
{
	// �o�ߎ��Ԃ̊������v�Z
	float fTimeRate = fCurTime / fEndTime;
	fTimeRate = useful::LimitNum(fTimeRate, 0.0f, 1.0f);	// ������␳

	// XZ���ʂ��v�Z
	VECTOR3 pos;
	pos.x = rPosStart.x + (rPosEnd.x - rPosStart.x) * fTimeRate;
	pos.z = rPosStart.z + (rPosEnd.z - rPosStart.z) * fTimeRate;

	// �������v�Z
	pos.y = rPosStart.y + (rPosEnd.y - rPosStart.y) * fTimeRate + sinf(D3DX_PI * fTimeRate) * fMaxHeight;

	// �ʒu��Ԃ�
	return pos;
}

//============================================================
//	�����̐��K��
//============================================================
void useful::NormalizeRot(float& rRot)
{
	if (rRot > D3DX_PI)
	{ // ������ 3.14 �𒴂����ꍇ

		// �����̐��K��
		rRot -= D3DX_PI * 2.0f;
	}
	else if (rRot < -D3DX_PI)
	{ // ������ -3.14 �𒴂����ꍇ

		// �����̐��K��
		rRot += D3DX_PI * 2.0f;
	}
}

//============================================================
//	�O�������̐��K��
//============================================================
void useful::NormalizeRot(VECTOR3& rRot)
{
	// �S�Ă̌����𐳋K��
	NormalizeRot(rRot.x);
	NormalizeRot(rRot.y);
	NormalizeRot(rRot.z);
}

//============================================================
//	�p�X�̃x�[�X�l�[���ϊ�
//============================================================
void useful::PathToBaseName(std::string* pPath)
{
	std::filesystem::path fsPath(*pPath);	// �p�X

	// �p�X����x�[�X�l�[�����擾
	*pPath = fsPath.stem().string();
}

//============================================================
//	�p�X��؂�̕W����
//============================================================
void useful::StandardizePathPart(std::string* pPath)
{
	// �X���b�V����u��
	ReplaceConsecChar
	( // ����
		pPath,	// �u���������s��������
		'/',	// ���o���镶��
		"\\",	// �u�������镶����
		1		// �u�������镶����̒��� (�ʏ핶���ƈႢ\�͓�ňꕶ���ƂȂ�)
	);

	// �o�b�N�X���b�V����u��
	ReplaceConsecChar
	( // ����
		pPath,	// �u���������s��������
		'\\',	// ���o���镶��
		"\\",	// �u�������镶����
		1		// �u�������镶����̒��� (�ʏ핶���ƈႢ\�͓�ňꕶ���ƂȂ�)
	);
}

//============================================================
//	������̍��E�ǉ����� (�}���`�o�C�g������)
//============================================================
std::string useful::SandString(const std::string& rTop, const std::string& rMain, const std::string& rCur)
{
	// ���E�ɒǉ�������������쐬���Ԃ�
	std::string sTemp;
	sTemp.append(rTop);		// �擪�ɒǉ�
	sTemp.append(rMain);	// ���S�ɒǉ�
	sTemp.append(rCur);		// �Ō���ɒǉ�
	return sTemp;
}

//============================================================
//	������̍��E�ǉ����� (���C�h������)
//============================================================
std::wstring useful::SandString(const std::wstring& rTop, const std::wstring& rMain, const std::wstring& rCur)
{
	// ���E�ɒǉ�������������쐬���Ԃ�
	std::wstring wsTemp;
	wsTemp.append(rTop);	// �擪�ɒǉ�
	wsTemp.append(rMain);	// ���S�ɒǉ�
	wsTemp.append(rCur);	// �Ō���ɒǉ�
	return wsTemp;
}

//============================================================
//	�}���`�o�C�g������̃��C�h������ϊ�
//============================================================
std::wstring useful::MultiByteToWide(const std::string& rSrcStr)
{
	int nSrcSize = (int)rSrcStr.size();	// �ϊ��O�̕�����̃T�C�Y
	if (nSrcSize <= 0) { return L""; }	// �����񂪂Ȃ��ꍇ������

	// �������ϊ�
	std::wstring wsDest(nSrcSize, L'\0');	// �ϊ���̕�����
	int nDestSize = MultiByteToWideChar
	( // ����
		CP_ACP,				// �ϊ��R�[�h�y�[�W
		0,					// �ϊ��t���O
		&rSrcStr.front(),	// �ϊ��O������̐擪�A�h���X
		nSrcSize,			// �ϊ��O������̃T�C�Y
		&wsDest.front(),	// �ϊ��㕶����̐擪�A�h���X
		(int)wsDest.size()	// �ϊ��㕶����̃T�C�Y
	);

	// ������T�C�Y���C��
	wsDest.resize(nDestSize);

	// �ϊ���̕������Ԃ�
	return wsDest;
}

//============================================================
//	���C�h������̃}���`�o�C�g������ϊ�
//============================================================
std::string useful::WideToMultiByte(const std::wstring& rSrcStr)
{
	int nSrcSize = (int)rSrcStr.size();	// �ϊ��O�̕�����̃T�C�Y
	if (nSrcSize <= 0) { return ""; }	// �����񂪂Ȃ��ꍇ������

	// �ϊ���̕�����T�C�Y���擾
	int nDestSize = WideCharToMultiByte
	( // ����
		CP_ACP,				// �ϊ��R�[�h�y�[�W
		0,					// �ϊ��t���O
		&rSrcStr.front(),	// �ϊ��O������̐擪�A�h���X
		nSrcSize,			// �ϊ��O������̃T�C�Y
		nullptr,			// �ϊ��㕶����̐擪�A�h���X
		0,					// �ϊ��㕶����̃T�C�Y
		nullptr,			// �ϊ��s���̒u������
		nullptr				// �ϊ��s�ȕ��������݂�����
	);

	// �������ϊ�
	std::string sDest(nDestSize, '\0');	// �ϊ���̕�����
	WideCharToMultiByte
	( // ����
		CP_ACP,				// �ϊ��R�[�h�y�[�W
		0,					// �ϊ��t���O
		&rSrcStr.front(),	// �ϊ��O������̐擪�A�h���X
		nSrcSize,			// �ϊ��O������̃T�C�Y
		&sDest.front(),		// �ϊ��㕶����̐擪�A�h���X
		(int)sDest.size(),	// �ϊ��㕶����̃T�C�Y
		nullptr,			// �ϊ��s���̒u������
		nullptr				// �ϊ��s�ȕ��������݂�����
	);

	// �ϊ���̕������Ԃ�
	return sDest;
}

//============================================================
//	�x�N�g���̌����ϊ�
//============================================================
void useful::VecToRot(const VECTOR3& rVec, float* pPhi, float* pTheta)
{
	// ���ʊp�̌v�Z
	*pPhi = atan2f(rVec.y, rVec.x);

	// �p�̌v�Z
	*pTheta = atan2f(sqrtf((rVec.x * rVec.x) + (rVec.y * rVec.y)), rVec.z);
}

//============================================================
//	�����̃x�N�g���ϊ�
//============================================================
void useful::RotToVec(const float fPhi, const float fTheta, VECTOR3* pVec)
{
	// �x�N�g���̌v�Z
	pVec->x = sinf(fTheta) * cosf(fPhi);
	pVec->y = sinf(fTheta) * sinf(fPhi);
	pVec->z = cosf(fTheta);
}

//============================================================
//	�c������e�N�X�`���̃A�X�y�N�g����l�����������̎擾����
//============================================================
float useful::GetTexWidthFromAspect(const float fHeight, const char* pTexturePath)
{
	// �A�X�y�N�g�䂩��v�Z����������Ԃ�
	return GetTexWidthFromAspect(fHeight, GET_MANAGER->GetTexture()->Regist(pTexturePath));
}

//============================================================
//	�c������e�N�X�`���̃A�X�y�N�g����l�����������̎擾����
//============================================================
float useful::GetTexWidthFromAspect(const float fHeight, const int nTexIdx)
{
	// �e�N�X�`���A�X�y�N�g����擾
	VECTOR2 aspect = GET_MANAGER->GetTexture()->GetInfo(nTexIdx).aspect;

	// �A�X�y�N�g�䂩��v�Z����������Ԃ�
	return fHeight * aspect.x;
}

//============================================================
//	��������e�N�X�`���̃A�X�y�N�g����l�������c���̎擾����
//============================================================
float useful::GetTexHeightFromAspect(const float fWidth, const char* pTexturePath)
{
	// �A�X�y�N�g�䂩��v�Z�����c����Ԃ�
	return GetTexHeightFromAspect(fWidth, GET_MANAGER->GetTexture()->Regist(pTexturePath));
}

//============================================================
//	��������e�N�X�`���̃A�X�y�N�g����l�������c���̎擾����
//============================================================
float useful::GetTexHeightFromAspect(const float fWidth, const int nTexIdx)
{
	// �e�N�X�`���A�X�y�N�g����擾
	VECTOR2 aspect = GET_MANAGER->GetTexture()->GetInfo(nTexIdx).aspect;

	// �A�X�y�N�g�䂩��v�Z�����c����Ԃ�
	return fWidth * aspect.y;
}

//============================================================
//	�����_�������擾
//============================================================
float useful::RandomRot()
{
	// �����_��������Ԃ�
	return (float)(rand() % 629 - 314) * 0.01f;
}
