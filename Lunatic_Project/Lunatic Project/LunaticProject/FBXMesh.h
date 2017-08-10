#pragma once
#include "Mesh.h"


// 상수 버퍼를 위한 구조체
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
	
	// i번 뼈대의 부모 색인(parentIndex)를 담는다.
	// i번 뼈대는 애니메이션 클립의 i번째 BoneAnimation 인스턴스에 대응.
	UINT *m_pBoneHierarchy;
	D3DMATRIX *m_pd3dxmtxBoneOffsets;

	ID3D11Buffer *m_pd3dWeightBuffer;
	ID3D11Buffer *m_pd3dBoneIndiceBuffer;
	// 뼈대 상수버퍼
	ID3D11Buffer *m_pd3dcbBones;

	float m_fFBXModelSize;		// 모델의 사이즈 (보통 Animate에서 조절해주기 위함)
	float m_fFBXAnimationTime;	// 모델의 AnimationTime (공통적으로 0.0333333f 씩 증가)
	int m_nFBXAnimationNum;		// 모델이 실행할 애니메이션의 값을 관리한다.
	int m_nFBXMaxFrameNum;		// 모델이 실행할 애니메이션의 최대 프레임 수.
	int m_nFBXNowFrameNum;		// 모델이 진행중인 애니메이션의 현재 프레임 값.
	

public:
	CFBXMesh(ID3D11Device *pd3dDevice, char *pszFileName, float fSize);
	virtual ~CFBXMesh();	

	// 해당 프레임의 SR(Q)T 회전이 반영된 행렬을 반환
	void MakeBoneMatrix(int nNowframe, int nAnimationNum, int nBoneNum, D3DXMATRIX& BoneMatrix);
	// 상수 버퍼로 전달할 최종 본 행렬을 구한다.
	void UpdateBoneTransform(ID3D11DeviceContext *pd3dDeviceContext, int nAnimationNum, int nNowFrame);
	// 뼈대 상수 버퍼 설정
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

