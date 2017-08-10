#pragma once
#include "Mesh.h"


// ��� ���۸� ���� ����ü
struct VS_CB_SKINNED
{
	D3DXMATRIX	m_d3dxmtxBone[96];
};


struct BoneAnimationData
{
	int m_nFrameCount;
	float	*m_pfAniTime;
	D3DXVECTOR3 *m_pd3dxvScale;
	D3DXVECTOR3 *m_pd3dxvTranslate;
	D3DXVECTOR4 *m_pd3dxvQuaternion;
};

class CFBXMesh : public CMeshTexturedIlluminated
{
private:
	D3DXVECTOR3 *m_pd3dxvPositions;
	D3DXVECTOR3	*m_pd3dxvNormals;
	D3DXVECTOR2 *m_pd3dxvTexCoords;
	D3DXVECTOR4 *m_pd3dxvBoneWeights;
	D3DXVECTOR4 *m_pd3dxvBoneIndices;

	BoneAnimationData **m_ppBoneAnimationData;
	D3DXMATRIX *m_pd3dxmtxSQTTransform;
	D3DXMATRIX *m_pd3dxmtxFinalBone;

	int m_nBoneCount;
	int m_nAnimationClip;
	
	// i�� ������ �θ� ����(parentIndex)�� ��´�.
	// i�� ����� �ִϸ��̼� Ŭ���� i��° BoneAnimation �ν��Ͻ��� ����.
	UINT *m_pBoneHierarchy;
	D3DMATRIX *m_pd3dxmtxBoneOffsets;

	ID3D11Buffer *m_pd3dWeightBuffer;
	ID3D11Buffer *m_pd3dBoneIndiceBuffer;
	// ���� �������
	ID3D11Buffer *m_pd3dcbBones;

	float m_fFBXModelSize;		// ���� ������ (���� Animate���� �������ֱ� ����)
	float m_fFBXAnimationTime;	// ���� AnimationTime (���������� 0.0333333f �� ����)
	int m_nFBXAnimationNum;		// ���� ������ �ִϸ��̼��� ���� �����Ѵ�.
	int m_nFBXMaxFrameNum;		// ���� ������ �ִϸ��̼��� �ִ� ������ ��.
	int m_nFBXNowFrameNum;		// ���� �������� �ִϸ��̼��� ���� ������ ��.
	

public:
	CFBXMesh(ID3D11Device *pd3dDevice, char *pszFileName, float fSize);
	virtual ~CFBXMesh();	

	// �ش� �������� SR(Q)T ȸ���� �ݿ��� ����� ��ȯ
	void MakeBoneMatrix(int nNowframe, int nAnimationNum, int nBoneNum, D3DXMATRIX& BoneMatrix);
	// ��� ���۷� ������ ���� �� ����� ���Ѵ�.
	void UpdateBoneTransform(ID3D11DeviceContext *pd3dDeviceContext, int nAnimationNum, int nNowFrame);
	// ���� ��� ���� ����
	void CreateConstantBuffer(ID3D11Device *pd3dDevice);

	float GetFBXModelSize() { return m_fFBXModelSize; }
	void SetFBXModelSize(float fsize) { m_fFBXModelSize = fsize; }
	float GetFBXAnimationTime() { return m_fFBXAnimationTime; }
	int GetFBXAnimationNum() { return m_nFBXAnimationNum; }
	int GetFBXNowFrameNum() { return m_nFBXNowFrameNum; }
	int GetFBXMaxFrameNum() { return m_nFBXMaxFrameNum; }
	//void SetFBXAnimationTime(float fFBXAnimationTime) { m_fFBXAnimationTime = fFBXAnimationTime; }
	bool FBXFrameAdvance(float fTimeElapsed);
	void SetAnimation(int nFBXAnimationNum);
};

