#include "stdafx.h"
#include "FBXMesh.h"

CFBXMesh::CFBXMesh(ID3D11Device *pd3dDevice, char *pszFileName, float fSize) : CMeshTexturedIlluminated(pd3dDevice)
{
	m_fFBXModelSize = fSize;
	m_fFBXAnimationTime = 0.0f;
	m_nFBXAnimationNum = 0;

	ifstream fin;

	int filename_length = strlen(pszFileName);
	char* ch = new char[200];
	char* sToken = new char[50];
	char* temp;
	fin.open(pszFileName);
	if (!fin.fail())
	{
		// 데이터를 읽어와 필요한 정점, 인덱스, 본, 애니메이션 수 파악
		fin.getline(ch, 200);
		fin.getline(ch, 200);
		m_nVertices = stoi(ch);
		fin.getline(ch, 200);
		m_nIndices = stoi(ch);
		fin.getline(ch, 200);
		m_nBoneCount = stoi(ch);
		fin.getline(ch, 200);
		m_nAnimationClip = stoi(ch);
		
		// 정점 데이터를 저장
		fin.getline(ch, 200);
		m_pd3dxvPositions = new D3DXVECTOR3[m_nVertices];
		m_pd3dxvNormals = new D3DXVECTOR3[m_nVertices];
		m_pd3dxvTexCoords = new D3DXVECTOR2[m_nVertices];
		if (m_nBoneCount)
		{
			m_pd3dxvBoneIndices = new D3DXVECTOR4[m_nVertices];
			m_pd3dxvBoneWeights = new D3DXVECTOR4[m_nVertices];
		}

		for (int i = 0; i < m_nVertices; i++)
		{
			// Vertice
			fin.getline(ch, 200);
			sToken = strtok_s(ch, " ", &temp);	 m_pd3dxvPositions[i].x = stof(sToken);
			sToken = strtok_s(NULL, " ", &temp); m_pd3dxvPositions[i].y = stof(sToken);
			sToken = strtok_s(NULL, " ", &temp); m_pd3dxvPositions[i].z = stof(sToken);
			// Normal
			fin.getline(ch, 200);
			sToken = strtok_s(ch, " ", &temp);   m_pd3dxvNormals[i].x = stof(sToken);
			sToken = strtok_s(NULL, " ", &temp); m_pd3dxvNormals[i].y = stof(sToken);
			sToken = strtok_s(NULL, " ", &temp); m_pd3dxvNormals[i].z = stof(sToken);
			// UV
			fin.getline(ch, 200);
			sToken = strtok_s(ch, " ", &temp);   m_pd3dxvTexCoords[i].x = stof(sToken);
			sToken = strtok_s(NULL, " ", &temp); m_pd3dxvTexCoords[i].y = stof(sToken);
			if (m_nBoneCount)
			{			// BoneIndice
				fin.getline(ch, 200);
				sToken = strtok_s(ch, " ", &temp); 	 m_pd3dxvBoneIndices[i].x = stof(sToken);
				sToken = strtok_s(NULL, " ", &temp); m_pd3dxvBoneIndices[i].y = stof(sToken);
				sToken = strtok_s(NULL, " ", &temp); m_pd3dxvBoneIndices[i].z = stof(sToken);
				sToken = strtok_s(NULL, " ", &temp); m_pd3dxvBoneIndices[i].w = stof(sToken);
				// BoneWeight
				fin.getline(ch, 200);
				sToken = strtok_s(ch, " ", &temp);   m_pd3dxvBoneWeights[i].x = stof(sToken);
				sToken = strtok_s(NULL, " ", &temp); m_pd3dxvBoneWeights[i].y = stof(sToken);
				sToken = strtok_s(NULL, " ", &temp); m_pd3dxvBoneWeights[i].z = stof(sToken);
				sToken = strtok_s(NULL, " ", &temp); m_pd3dxvBoneWeights[i].w = stof(sToken);
			}
		}

		// 인덱스 데이터 저장
		fin.getline(ch, 200);
		m_pnIndices = new UINT[m_nIndices];
		
		for (int i = 0; i < m_nIndices; i+=3)
		{
			fin.getline(ch, 200);
			sToken = strtok_s(ch, " ", &temp);   m_pnIndices[i] = stoi(sToken);
			sToken = strtok_s(NULL, " ", &temp); m_pnIndices[i+1] = stoi(sToken);
			sToken = strtok_s(NULL, " ", &temp); m_pnIndices[i+2] = stoi(sToken);
		}

		// (애니메이션을 포함한 메쉬일 경우) 본 정보와 애니메이션 정보 저장
		if (m_nBoneCount)
		{
			fin.getline(ch, 200);
			m_pBoneHierarchy = new UINT[m_nBoneCount];
			m_pd3dxmtxBoneOffsets = new D3DXMATRIX[m_nBoneCount];
			m_pd3dxmtxSQTTransform = new D3DXMATRIX[m_nBoneCount];
			m_pd3dxmtxFinalBone = new D3DXMATRIX[m_nBoneCount];

			// 부모 뼈대를 가리키는 BoneHierarchy를 저장
			for (int i = 0; i < m_nBoneCount; i++)
			{
				fin.getline(ch, 200);
				m_pBoneHierarchy[i] = stoi(ch);
			}
			// 뼈대 자체의 오프셋 행렬을 저장
			fin.getline(ch, 200);
			for (int i = 0; i < m_nBoneCount; i++)
			{
				fin.getline(ch, 200);
				sToken = strtok_s(ch, " ", &temp);   m_pd3dxmtxBoneOffsets[i]._11 = stof(sToken);
				sToken = strtok_s(NULL, " ", &temp); m_pd3dxmtxBoneOffsets[i]._12 = stof(sToken);
				sToken = strtok_s(NULL, " ", &temp); m_pd3dxmtxBoneOffsets[i]._13 = stof(sToken);
				sToken = strtok_s(NULL, " ", &temp); m_pd3dxmtxBoneOffsets[i]._14 = stof(sToken);
				sToken = strtok_s(NULL, " ", &temp); m_pd3dxmtxBoneOffsets[i]._21 = stof(sToken);
				sToken = strtok_s(NULL, " ", &temp); m_pd3dxmtxBoneOffsets[i]._22 = stof(sToken);
				sToken = strtok_s(NULL, " ", &temp); m_pd3dxmtxBoneOffsets[i]._23 = stof(sToken);
				sToken = strtok_s(NULL, " ", &temp); m_pd3dxmtxBoneOffsets[i]._24 = stof(sToken);
				sToken = strtok_s(NULL, " ", &temp); m_pd3dxmtxBoneOffsets[i]._31 = stof(sToken);
				sToken = strtok_s(NULL, " ", &temp); m_pd3dxmtxBoneOffsets[i]._32 = stof(sToken);
				sToken = strtok_s(NULL, " ", &temp); m_pd3dxmtxBoneOffsets[i]._33 = stof(sToken);
				sToken = strtok_s(NULL, " ", &temp); m_pd3dxmtxBoneOffsets[i]._34 = stof(sToken);
				sToken = strtok_s(NULL, " ", &temp); m_pd3dxmtxBoneOffsets[i]._41 = stof(sToken);
				sToken = strtok_s(NULL, " ", &temp); m_pd3dxmtxBoneOffsets[i]._42 = stof(sToken);
				sToken = strtok_s(NULL, " ", &temp); m_pd3dxmtxBoneOffsets[i]._43 = stof(sToken);
				sToken = strtok_s(NULL, " ", &temp); m_pd3dxmtxBoneOffsets[i]._44 = stof(sToken);
			}

			// 여기에서부터 애니메이션을 담는다.
			m_ppBoneAnimationData = new BoneAnimationData*[m_nAnimationClip];

			BoneAnimationData *pBoneAnimationData;
			for (int k = 0; k < m_nAnimationClip; k++)
			{
				pBoneAnimationData = new BoneAnimationData[m_nBoneCount];

				fin.getline(ch, 200);
				for (int i = 0; i < m_nBoneCount; i++)
				{
					fin.getline(ch, 200);
					sToken = strtok_s(ch, " ", &temp);
					sToken = strtok_s(NULL, " ", &temp);
					pBoneAnimationData[i].m_nFrameCount = stoi(sToken);
					pBoneAnimationData[i].m_pd3dxvTranslate = new D3DXVECTOR3[pBoneAnimationData[i].m_nFrameCount];
					pBoneAnimationData[i].m_pd3dxvScale = new D3DXVECTOR3[pBoneAnimationData[i].m_nFrameCount];
					pBoneAnimationData[i].m_pd3dxvQuaternion = new D3DXVECTOR4[pBoneAnimationData[i].m_nFrameCount];
					pBoneAnimationData[i].m_pfAniTime = new float[pBoneAnimationData[i].m_nFrameCount];

					for (int j = 0; j < pBoneAnimationData[i].m_nFrameCount; j++)
					{
						fin.getline(ch, 200);
						sToken = strtok_s(ch, " ", &temp);
						pBoneAnimationData[i].m_pfAniTime[j] = stof(sToken);
						sToken = strtok_s(NULL, " ", &temp);
						pBoneAnimationData[i].m_pd3dxvTranslate[j].x = stof(sToken);
						sToken = strtok_s(NULL, " ", &temp);
						pBoneAnimationData[i].m_pd3dxvTranslate[j].y = stof(sToken);
						sToken = strtok_s(NULL, " ", &temp);
						pBoneAnimationData[i].m_pd3dxvTranslate[j].z = stof(sToken);
						sToken = strtok_s(NULL, " ", &temp);
						pBoneAnimationData[i].m_pd3dxvScale[j].x = stof(sToken);
						sToken = strtok_s(NULL, " ", &temp);
						pBoneAnimationData[i].m_pd3dxvScale[j].y = stof(sToken);
						sToken = strtok_s(NULL, " ", &temp);
						pBoneAnimationData[i].m_pd3dxvScale[j].z = stof(sToken);
						sToken = strtok_s(NULL, " ", &temp);
						pBoneAnimationData[i].m_pd3dxvQuaternion[j].x = stof(sToken);
						sToken = strtok_s(NULL, " ", &temp);
						pBoneAnimationData[i].m_pd3dxvQuaternion[j].y = stof(sToken);
						sToken = strtok_s(NULL, " ", &temp);
						pBoneAnimationData[i].m_pd3dxvQuaternion[j].z = stof(sToken);
						sToken = strtok_s(NULL, " ", &temp);
						pBoneAnimationData[i].m_pd3dxvQuaternion[j].w = stof(sToken);
					}
				}
				m_ppBoneAnimationData[k] = pBoneAnimationData;
			}
		}
	}
	fin.close();

	m_d3dPrimitiveTopology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;

	m_pd3dPositionBuffer = CreateBuffer(pd3dDevice, sizeof(D3DXVECTOR3), m_nVertices, m_pd3dxvPositions, D3D11_BIND_VERTEX_BUFFER, D3D11_USAGE_DEFAULT, 0);
	m_pd3dNormalBuffer = CreateBuffer(pd3dDevice, sizeof(D3DXVECTOR3), m_nVertices, m_pd3dxvNormals, D3D11_BIND_VERTEX_BUFFER, D3D11_USAGE_DEFAULT, 0);
	m_pd3dTexCoordBuffer = CreateBuffer(pd3dDevice, sizeof(D3DXVECTOR2), m_nVertices, m_pd3dxvTexCoords, D3D11_BIND_VERTEX_BUFFER, D3D11_USAGE_DEFAULT, 0);
	m_pd3dWeightBuffer = CreateBuffer(pd3dDevice, sizeof(D3DXVECTOR4), m_nVertices, m_pd3dxvBoneWeights, D3D11_BIND_VERTEX_BUFFER, D3D11_USAGE_DEFAULT, 0);
	m_pd3dBoneIndiceBuffer = CreateBuffer(pd3dDevice, sizeof(D3DXVECTOR4), m_nVertices, m_pd3dxvBoneIndices, D3D11_BIND_VERTEX_BUFFER, D3D11_USAGE_DEFAULT, 0);
	ID3D11Buffer *pd3dBuffers[5] = { m_pd3dPositionBuffer, m_pd3dNormalBuffer, m_pd3dTexCoordBuffer, m_pd3dWeightBuffer, m_pd3dBoneIndiceBuffer };
	UINT pnBufferStrides[5] = { sizeof(D3DXVECTOR3), sizeof(D3DXVECTOR3), sizeof(D3DXVECTOR2), sizeof(D3DXVECTOR4), sizeof(D3DXVECTOR4) };
	UINT pnBufferOffsets[5] = { 0, 0, 0, 0, 0 };
	AssembleToVertexBuffer(5, pd3dBuffers, pnBufferStrides, pnBufferOffsets);


	m_pd3dIndexBuffer = CreateBuffer(pd3dDevice, sizeof(UINT), m_nIndices, m_pnIndices, D3D11_BIND_INDEX_BUFFER, D3D11_USAGE_DEFAULT, 0);
	
	CreateRasterizerState(pd3dDevice);
	CreateConstantBuffer(pd3dDevice);

	// idle 상태의 프레임 값을 체크.
	if (m_nBoneCount)
		m_nFBXMaxFrameNum = m_ppBoneAnimationData[0][0].m_nFrameCount - 1;
}


CFBXMesh::~CFBXMesh()
{
	if (m_pd3dWeightBuffer) m_pd3dWeightBuffer->Release();
	if (m_pd3dBoneIndiceBuffer) m_pd3dBoneIndiceBuffer->Release();
	if (m_pd3dcbBones) m_pd3dcbBones->Release();

	if (m_pd3dxvPositions) delete[] m_pd3dxvPositions;
	if (m_pd3dxvNormals) delete[] m_pd3dxvNormals;
	if (m_pd3dxvTexCoords) delete[] m_pd3dxvTexCoords;
	if (m_pd3dxvBoneWeights) delete[] m_pd3dxvBoneWeights;
	if (m_pd3dxvBoneIndices) delete[] m_pd3dxvBoneIndices;
	if (m_ppBoneAnimationData) delete[] m_ppBoneAnimationData;
	if (m_pd3dxmtxFinalBone) delete[] m_pd3dxmtxFinalBone;
	if (m_pd3dxmtxSQTTransform) delete[] m_pd3dxmtxSQTTransform;
	if (m_pBoneHierarchy) delete[] m_pBoneHierarchy;
	if (m_pd3dxmtxBoneOffsets) delete[] m_pd3dxmtxBoneOffsets;

}

void CFBXMesh::UpdateBoneTransform(ID3D11DeviceContext *pd3dDeviceContext, int nAnimationNum, int nNowFrame)
{
	for (int i = 0; i < m_nBoneCount; i++)
	{
		MakeBoneMatrix(nNowFrame, nAnimationNum, i, *(m_pd3dxmtxSQTTransform + i));
	}


	// 마지막으로 본의 기본 오프셋행렬을 곱해주어 최종 행렬을 만들어준다.
	for (int i = 0; i < m_nBoneCount; i++)
	{
		D3DXMATRIX offset = m_pd3dxmtxBoneOffsets[i];
		D3DXMATRIX toRoot = m_pd3dxmtxSQTTransform[i];
		D3DXMatrixMultiply(&m_pd3dxmtxFinalBone[i], &offset, &toRoot);
	}

	// 상수버퍼로 최종 행렬값을 넘겨주자.
	D3D11_MAPPED_SUBRESOURCE d3dMappedResource;
	pd3dDeviceContext->Map(m_pd3dcbBones, 0, D3D11_MAP_WRITE_DISCARD, 0, &d3dMappedResource);
	VS_CB_SKINNED *pcbBones = (VS_CB_SKINNED*)d3dMappedResource.pData;
	for (int i = 0; i < m_nBoneCount; i++)
	{
		D3DXMatrixTranspose(&pcbBones->m_d3dxmtxBone[i], &m_pd3dxmtxFinalBone[i]);
	}
	pd3dDeviceContext->Unmap(m_pd3dcbBones, 0);

	//상수 버퍼를 슬롯(VS_SLOT_SKINNEDBONE)에 설정한다.
	pd3dDeviceContext->VSSetConstantBuffers(VS_SLOT_SKINNEDBONE, 1, &m_pd3dcbBones);
}

void CFBXMesh::MakeBoneMatrix(int nNowframe, int nAnimationNum, int nBoneNum, D3DXMATRIX& BoneMatrix)
{
	// XMAffine 함수에서는 scale의 VECTOR3을 쓰지만
	// D3DXAffine 함수에서는 scale의 계수를 사용한다.
	if (m_ppBoneAnimationData[nAnimationNum][nBoneNum].m_nFrameCount != 0)
	{
		float fScale = m_ppBoneAnimationData[nAnimationNum][nBoneNum].m_pd3dxvScale[nNowframe].z;
		D3DXVECTOR3 d3dxvTranslate = m_ppBoneAnimationData[nAnimationNum][nBoneNum].m_pd3dxvTranslate[nNowframe];
		D3DXQUATERNION d3dxvQuaternion = m_ppBoneAnimationData[nAnimationNum][nBoneNum].m_pd3dxvQuaternion[nNowframe];
		D3DXVECTOR3 d3dxvZero = { 0.0f, 0.0f, 0.0f };

		D3DXMatrixAffineTransformation(&BoneMatrix, fScale, &d3dxvZero, &d3dxvQuaternion, &d3dxvTranslate);
	}
	else // 해당 본에 애니메이션 프레임이 아예 없을 경우 단위행렬을 리턴하자.
	{
		D3DXMatrixIdentity(&BoneMatrix);
	}
}

void CFBXMesh::CreateConstantBuffer(ID3D11Device *pd3dDevice)
{
	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DYNAMIC;
	bd.ByteWidth = sizeof(VS_CB_SKINNED);
	bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	pd3dDevice->CreateBuffer(&bd, NULL, &m_pd3dcbBones);
}

// 다음 애니메이션을 위한 프레임으로 넘긴다.
// 추가적인 애니메이션 관리를 위해 마지막 프레임일 경우 true를 리턴한다.
bool CFBXMesh::FBXFrameAdvance(float fTimeElapsed)
{
	m_fFBXAnimationTime += fTimeElapsed;

	if (m_fFBXAnimationTime > ANIFRAMETIME)	// 0.0333333f초가 지나면 1프레임 올리자.
	{
		if (m_nFBXNowFrameNum < m_nFBXMaxFrameNum - 1)
		{
			m_nFBXNowFrameNum++;
			m_fFBXAnimationTime = 0.0f;
			return false;
		}
		else
		{
			m_nFBXNowFrameNum = 0;
			m_fFBXAnimationTime = 0.0f;
			return true;
		}
	}
	else
		return false;
}

void CFBXMesh::SetAnimation(int nFBXAnimation)
{
	m_nFBXAnimationNum = nFBXAnimation;
	m_nFBXNowFrameNum = 0;
	m_nFBXMaxFrameNum = m_ppBoneAnimationData[nFBXAnimation][0].m_nFrameCount;
	m_fFBXAnimationTime = 0.0f;
}