#include "stdafx.h"
#include "Mesh.h"
#include "GameObject.h"

//
// AABB Collision Check
//////////////////////////////////////////////////////////////////////////////////////////
void AABB::Union(D3DXVECTOR3& d3dxvMinimum, D3DXVECTOR3& d3dxvMaximum)
{
	if (d3dxvMinimum.x < m_d3dxvMinimum.x) m_d3dxvMinimum.x = d3dxvMinimum.x;
	if (d3dxvMinimum.y < m_d3dxvMinimum.y) m_d3dxvMinimum.y = d3dxvMinimum.y;
	if (d3dxvMinimum.z < m_d3dxvMinimum.z) m_d3dxvMinimum.z = d3dxvMinimum.z;
	if (d3dxvMaximum.x > m_d3dxvMaximum.x) m_d3dxvMaximum.x = d3dxvMaximum.x;
	if (d3dxvMaximum.y > m_d3dxvMaximum.y) m_d3dxvMaximum.y = d3dxvMaximum.y;
	if (d3dxvMaximum.z > m_d3dxvMaximum.z) m_d3dxvMaximum.z = d3dxvMaximum.z;
}

void AABB::Union(AABB *pAABB)
{
	Union(pAABB->m_d3dxvMinimum, pAABB->m_d3dxvMaximum);
}

void AABB::Update(D3DXMATRIX *pmtxTransform)
{
	// �ٿ�� �ڽ��� �ּ����� �ִ����� ȸ���� �ϸ� �� �̻� �ּ����� �ִ����� ���� �ʴ´�. 
	// �׷��Ƿ� �ٿ�� �ڽ��� �ּ����� �ִ������� 8���� ������ ���ϰ� ��ȯ(ȸ��)�� �� ����
	// �ּ����� �ִ����� �ٽ� ����Ѵ�.	
	D3DXVECTOR3 vVertices[8];
	vVertices[0] = D3DXVECTOR3(m_d3dxvMinimum.x, m_d3dxvMinimum.y, m_d3dxvMinimum.z);
	vVertices[1] = D3DXVECTOR3(m_d3dxvMinimum.x, m_d3dxvMinimum.y, m_d3dxvMaximum.z);
	vVertices[2] = D3DXVECTOR3(m_d3dxvMaximum.x, m_d3dxvMinimum.y, m_d3dxvMaximum.z);
	vVertices[3] = D3DXVECTOR3(m_d3dxvMaximum.x, m_d3dxvMinimum.y, m_d3dxvMinimum.z);
	vVertices[4] = D3DXVECTOR3(m_d3dxvMinimum.x, m_d3dxvMaximum.y, m_d3dxvMinimum.z);
	vVertices[5] = D3DXVECTOR3(m_d3dxvMinimum.x, m_d3dxvMaximum.y, m_d3dxvMaximum.z);
	vVertices[6] = D3DXVECTOR3(m_d3dxvMaximum.x, m_d3dxvMaximum.y, m_d3dxvMaximum.z);
	vVertices[7] = D3DXVECTOR3(m_d3dxvMaximum.x, m_d3dxvMaximum.y, m_d3dxvMinimum.z);
	m_d3dxvMinimum = D3DXVECTOR3(+FLT_MAX, +FLT_MAX, +FLT_MAX);
	m_d3dxvMaximum = D3DXVECTOR3(-FLT_MAX, -FLT_MAX, -FLT_MAX);

	// 8���� �������� x, y, z ��ǥ�� �ּҰ��� �ִ밪�� ���Ѵ�.
	for (int i = 0; i < 8; i++)
	{
		// ������ ��ȯ�Ѵ�.
		D3DXVec3TransformCoord(&vVertices[i], &vVertices[i], pmtxTransform);
		if (vVertices[i].x < m_d3dxvMinimum.x) m_d3dxvMinimum.x = vVertices[i].x;
		if (vVertices[i].y < m_d3dxvMinimum.y) m_d3dxvMinimum.y = vVertices[i].y;
		if (vVertices[i].z < m_d3dxvMinimum.z) m_d3dxvMinimum.z = vVertices[i].z;
		if (vVertices[i].x > m_d3dxvMaximum.x) m_d3dxvMaximum.x = vVertices[i].x;
		if (vVertices[i].y > m_d3dxvMaximum.y) m_d3dxvMaximum.y = vVertices[i].y;
		if (vVertices[i].z > m_d3dxvMaximum.z) m_d3dxvMaximum.z = vVertices[i].z;
	}
}


//
// CMesh Class
//////////////////////////////////////////////////////////////////////////////////////////
CMesh::CMesh(ID3D11Device *pd3dDevice)
{
	m_nBuffers = 0;
	m_pd3dPositionBuffer = NULL;
	m_ppd3dVertexBuffers = NULL;

	m_d3dPrimitiveTopology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
	m_nSlot = 0;
	m_nStartVertex = 0;

	m_pd3dIndexBuffer = NULL;
	m_nIndices = 0;
	m_nStartIndex = 0;
	m_nBaseVertex = 0;
	m_nIndexOffset = 0;
	m_dxgiIndexFormat = DXGI_FORMAT_R32_UINT;

	m_pd3dRasterizerState = NULL;

	m_bcBoundingCube.m_d3dxvMinimum = D3DXVECTOR3(+FLT_MAX, +FLT_MAX, +FLT_MAX);
	m_bcBoundingCube.m_d3dxvMaximum = D3DXVECTOR3(-FLT_MAX, -FLT_MAX, -FLT_MAX);

	m_nReferences = 0;

	m_pd3dxvPositions = NULL;
	m_pnIndices = NULL;
}

CMesh::~CMesh()
{
	if (m_pd3dRasterizerState) m_pd3dRasterizerState->Release();
	if (m_pd3dPositionBuffer) m_pd3dPositionBuffer->Release();
	if (m_pd3dIndexBuffer) m_pd3dIndexBuffer->Release();

	if (m_ppd3dVertexBuffers) delete[] m_ppd3dVertexBuffers;
	if (m_pnVertexStrides) delete[] m_pnVertexStrides;
	if (m_pnVertexOffsets) delete[] m_pnVertexOffsets;

	if (m_pd3dxvPositions) delete[] m_pd3dxvPositions;
	if (m_pnIndices) delete[] m_pnIndices;
}

void CMesh::Render(ID3D11DeviceContext *pd3dDeviceContext)
{
	pd3dDeviceContext->IASetVertexBuffers(m_nSlot, m_nBuffers, m_ppd3dVertexBuffers, m_pnVertexStrides, m_pnVertexOffsets);
	pd3dDeviceContext->IASetIndexBuffer(m_pd3dIndexBuffer, m_dxgiIndexFormat, m_nIndexOffset);
	pd3dDeviceContext->IASetPrimitiveTopology(m_d3dPrimitiveTopology);
	pd3dDeviceContext->RSSetState(m_pd3dRasterizerState);

	if (m_pd3dIndexBuffer)
		pd3dDeviceContext->DrawIndexed(m_nIndices, m_nStartIndex, m_nBaseVertex);
	else
		pd3dDeviceContext->Draw(m_nVertices, m_nStartVertex);
}

void CMesh::RenderInstanced(ID3D11DeviceContext *pd3dDeviceContext, int nInstances, int nStartInstance)
{
	//�ν��Ͻ��� ��� �Է� �����⿡ �޽��� ���� ���ۿ� �ν��Ͻ� ���� ���۰� ����ȴ�.
	pd3dDeviceContext->IASetVertexBuffers(m_nSlot, m_nBuffers, m_ppd3dVertexBuffers, m_pnVertexStrides, m_pnVertexOffsets);
	pd3dDeviceContext->IASetIndexBuffer(m_pd3dIndexBuffer, m_dxgiIndexFormat, m_nIndexOffset);
	pd3dDeviceContext->IASetPrimitiveTopology(m_d3dPrimitiveTopology);
	pd3dDeviceContext->RSSetState(m_pd3dRasterizerState);

	if (m_pd3dIndexBuffer)
		pd3dDeviceContext->DrawIndexedInstanced(m_nIndices, nInstances, m_nStartIndex, m_nBaseVertex, nStartInstance);
	else
		pd3dDeviceContext->DrawInstanced(m_nVertices, nInstances, m_nStartVertex, nStartInstance);
}

void CMesh::CreateRasterizerState(ID3D11Device *pd3dDevice)
{
	// Default
	D3D11_RASTERIZER_DESC d3dRasterizerDesc;
	ZeroMemory(&d3dRasterizerDesc, sizeof(D3D11_RASTERIZER_DESC));
	d3dRasterizerDesc.CullMode = D3D11_CULL_BACK;
	d3dRasterizerDesc.FillMode = D3D11_FILL_SOLID;
	pd3dDevice->CreateRasterizerState(&d3dRasterizerDesc, &m_pd3dRasterizerState);
}

void CMesh::AssembleToVertexBuffer(int nBuffers, ID3D11Buffer **ppd3dBuffers, UINT *pnBufferStrides, UINT *pnBufferOffsets)
{
	ID3D11Buffer **ppd3dNewVertexBuffers = new ID3D11Buffer*[m_nBuffers + nBuffers];
	UINT *pnNewVertexStrides = new UINT[m_nBuffers + nBuffers];
	UINT *pnNewVertexOffsets = new UINT[m_nBuffers + nBuffers];

	if (m_nBuffers > 0)
	{
		for (int i = 0; i < m_nBuffers; i++)
		{
			ppd3dNewVertexBuffers[i] = m_ppd3dVertexBuffers[i];
			pnNewVertexStrides[i] = m_pnVertexStrides[i];
			pnNewVertexOffsets[i] = m_pnVertexOffsets[i];
		}
		if (m_ppd3dVertexBuffers) delete[] m_ppd3dVertexBuffers;
		if (m_pnVertexStrides) delete[] m_pnVertexStrides;
		if (m_pnVertexOffsets) delete[] m_pnVertexOffsets;
	}

	for (int i = 0; i < nBuffers; i++)
	{
		ppd3dNewVertexBuffers[m_nBuffers + i] = ppd3dBuffers[i];
		pnNewVertexStrides[m_nBuffers + i] = pnBufferStrides[i];
		pnNewVertexOffsets[m_nBuffers + i] = pnBufferOffsets[i];
	}

	m_nBuffers += nBuffers;
	m_ppd3dVertexBuffers = ppd3dNewVertexBuffers;
	m_pnVertexStrides = pnNewVertexStrides;
	m_pnVertexOffsets = pnNewVertexOffsets;
}


//
// CMeshDiffused Class
//////////////////////////////////////////////////////////////////////////////////////////
CMeshDiffused::CMeshDiffused(ID3D11Device *pd3dDevice) : CMesh(pd3dDevice)
{
	m_pd3dColorBuffer = NULL;
}

CMeshDiffused::~CMeshDiffused()
{
	if (m_pd3dColorBuffer) m_pd3dColorBuffer->Release();
}


//
// CMeshIlluminated Class
//////////////////////////////////////////////////////////////////////////////////////////
CMeshIlluminated::CMeshIlluminated(ID3D11Device *pd3dDevice) : CMesh(pd3dDevice)
{
	m_pd3dNormalBuffer = NULL;
}

CMeshIlluminated::~CMeshIlluminated()
{
	if (m_pd3dNormalBuffer) m_pd3dNormalBuffer->Release();
}

void CMeshIlluminated::CalculateVertexNormal(D3DXVECTOR3 *pd3dxvNormals)
{
	switch (m_d3dPrimitiveTopology)
	{
		// ������Ƽ�갡 �ﰢ�� ����Ʈ�� �� �ε��� ���۰� �ִ� ���� ���� ��츦 �����Ͽ� ������ ���� ���͸� ���
		// �ε��� ���۸� ������� �ʴ� ��� �� ������ ���� ���ʹ� �� ������ ���Ե� �ﰢ���� ���� ���ͷ� ���
		// �ε��� ���۸� ����ϴ� ��� �� ������ ���� ���ʹ� �� ������ ���Ե� �ﰢ������ ���� ������ �������(���Ͽ�) ���
	case D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST:
		if (m_pnIndices)
			SetAverageVertexNormal(pd3dxvNormals, (m_nIndices / 3), 3, false);
		else
			SetTriAngleListVertexNormal(pd3dxvNormals);
		break;
		// ������Ƽ�갡 �ﰢ�� ��Ʈ���� �� �� ������ ���� ���ʹ� �� ������ ���Ե� �ﰢ������ ���� ������ �������(���Ͽ�) ���
	case D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP:
		SetAverageVertexNormal(pd3dxvNormals, (m_nIndices) ? (m_nIndices - 2) : (m_nVertices - 2), 1, true);
		break;
	default:
		break;
	}
}

// �ε��� ���۸� ������� �ʴ� �ﰢ�� ����Ʈ�� ���� ������ ���� ���� ���
void CMeshIlluminated::SetTriAngleListVertexNormal(D3DXVECTOR3 *pd3dxvNormals)
{
	// �ﰢ��(������Ƽ��)�� ������ ���ϰ� �� �ﰢ���� ���� ���͸� ����ϰ� �ﰢ���� �����ϴ� �� ������ ���� ���ͷ� ����
	int nPrimitives = m_nVertices / 3;
	for (int i = 0; i < nPrimitives; i++)
	{
		D3DXVECTOR3 d3dxvNormal = CalculateTriAngleNormal((i * 3 + 0), (i * 3 + 1), (i * 3 + 2));
		pd3dxvNormals[i * 3 + 0] = pd3dxvNormals[i * 3 + 1] = pd3dxvNormals[i * 3 + 2] = d3dxvNormal;
	}
}

// �ﰢ���� �� ������ ����Ͽ� �ﰢ���� ���� ���͸� ����Ѵ�.
D3DXVECTOR3 CMeshIlluminated::CalculateTriAngleNormal(UINT nIndex0, UINT nIndex1, UINT nIndex2)
{
	D3DXVECTOR3 d3dxvNormal;
	D3DXVECTOR3 d3dxvP0 = m_pd3dxvPositions[nIndex0];
	D3DXVECTOR3 d3dxvP1 = m_pd3dxvPositions[nIndex1];
	D3DXVECTOR3 d3dxvP2 = m_pd3dxvPositions[nIndex2];
	D3DXVECTOR3 d3dxvEdge1 = d3dxvP1 - d3dxvP0;
	D3DXVECTOR3 d3dxvEdge2 = d3dxvP2 - d3dxvP0;
	D3DXVec3Cross(&d3dxvNormal, &d3dxvEdge1, &d3dxvEdge2);
	D3DXVec3Normalize(&d3dxvNormal, &d3dxvNormal);
	return(d3dxvNormal);
}

// ������Ƽ�갡 �ε��� ���۸� ����ϴ� �ﰢ�� ����Ʈ �Ǵ� �ﰢ�� ��Ʈ���� ��� ������ 
// ���� ���ʹ� �� ������ �����ϴ� �ﰢ���� ���� ���͵��� ������� ����Ѵ�.
void CMeshIlluminated::SetAverageVertexNormal(D3DXVECTOR3 *pd3dxvNormals, int nPrimitives, int nOffset, bool bStrip)
{
	for (int j = 0; j < m_nVertices; j++)
	{
		D3DXVECTOR3 d3dxvSumOfNormal = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		for (int i = 0; i < nPrimitives; i++)
		{
			UINT nIndex0 = (bStrip) ? (((i % 2) == 0) ? (i*nOffset + 0) : (i*nOffset + 1)) : (i*nOffset + 0);
			if (m_pnIndices) nIndex0 = m_pnIndices[nIndex0];
			UINT nIndex1 = (bStrip) ? (((i % 2) == 0) ? (i*nOffset + 1) : (i*nOffset + 0)) : (i*nOffset + 1);
			if (m_pnIndices) nIndex1 = m_pnIndices[nIndex1];
			UINT nIndex2 = (m_pnIndices) ? m_pnIndices[i*nOffset + 2] : (i*nOffset + 2);
			if ((nIndex0 == j) || (nIndex1 == j) || (nIndex2 == j)) d3dxvSumOfNormal += CalculateTriAngleNormal(nIndex0, nIndex1, nIndex2);
		}
		D3DXVec3Normalize(&d3dxvSumOfNormal, &d3dxvSumOfNormal);
		pd3dxvNormals[j] = d3dxvSumOfNormal;
	}
}



//
// CMeshTextured
//////////////////////////////////////////////////////////////////////////////////////////
CMeshTextured::CMeshTextured(ID3D11Device *pd3dDevice) : CMesh(pd3dDevice)
{
	m_pd3dTexCoordBuffer = NULL;
}

CMeshTextured::~CMeshTextured()
{
	if (m_pd3dTexCoordBuffer) m_pd3dTexCoordBuffer->Release();
}



//
// CMeshTexturedIlluminated Class
//////////////////////////////////////////////////////////////////////////////////////////
CMeshTexturedIlluminated::CMeshTexturedIlluminated(ID3D11Device *pd3dDevice) : CMeshIlluminated(pd3dDevice)
{
	m_pd3dTexCoordBuffer = NULL;
}

CMeshTexturedIlluminated::~CMeshTexturedIlluminated()
{
	if (m_pd3dTexCoordBuffer) m_pd3dTexCoordBuffer->Release();
}

ID3D11Buffer *CMesh::CreateBuffer(ID3D11Device *pd3dDevice, UINT nStride, int nElements, void *pBufferData, UINT nBindFlags, D3D11_USAGE d3dUsage, UINT nCPUAccessFlags)
{
	D3D11_BUFFER_DESC d3dBufferDesc;
	ZeroMemory(&d3dBufferDesc, sizeof(D3D11_BUFFER_DESC));
	d3dBufferDesc.Usage = d3dUsage;
	d3dBufferDesc.ByteWidth = nStride * nElements;
	d3dBufferDesc.BindFlags = nBindFlags;
	d3dBufferDesc.CPUAccessFlags = nCPUAccessFlags;
	D3D11_SUBRESOURCE_DATA d3dBufferData;
	ZeroMemory(&d3dBufferData, sizeof(D3D11_SUBRESOURCE_DATA));
	d3dBufferData.pSysMem = pBufferData;

	ID3D11Buffer *pd3dBuffer = NULL;
	pd3dDevice->CreateBuffer(&d3dBufferDesc, &d3dBufferData, &pd3dBuffer);
	return pd3dBuffer;
}

// 160424 FBXMesh���� ����� �Լ�
void CMesh::UpdateBoneTransform(ID3D11DeviceContext *pd3dDeviceContext, int nAnimationNum, int nNowFrame)
{
}




// Terrain
//
//CHeightMapGridMesh::CHeightMapGridMesh(ID3D11Device *pd3dDevice, int xStart, int zStart, int nWidth, int nLength, D3DXVECTOR3 d3dxvScale, D3DXCOLOR d3dxColor, void *pContext) : CMeshIlluminated(pd3dDevice)
//{
//	//������ ����(����)�� ������ (nWidth * nLength)�̴�.
//	m_nVertices = nWidth * nLength;
//	m_nStride = sizeof(m_pd3dNormalBuffer);
//	m_nOffset = 0;
//	//���ڴ� �ﰢ�� ��Ʈ������ �����Ѵ�.
//	m_d3dPrimitiveTopology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP;
//
//	//CNormalVertex *pVertices = new CNormalVertex[m_nVertices];
//
//	m_nWidth = nWidth;
//	m_nLength = nLength;
//	m_d3dxvScale = d3dxvScale;
//
//	float fHeight = 0.0f;
//	//d3dxColor�� ������ �⺻ �����̴�.
//	D3DXCOLOR d3dxVertexColor = d3dxColor;
//
//
//	for (int i = 0, z = zStart; z < (zStart + nLength); z++)
//	{
//		for (int x = xStart; x < (xStart + nWidth); x++)
//		{
//			fHeight = OnSetHeight(x, z, pContext);
//			d3dxVertexColor = OnSetColor(x, z, pContext) + d3dxColor;
//			//pVertices[i++] = CNormalVertex(D3DXVECTOR3((x*m_d3dxvScale.x), fHeight, (z*m_d3dxvScale.z)), D3DXVECTOR3(0.0f, 0.0f, 0.0f));
//			//CDiffusedVertex(, d3dxVertexColor);
//		}
//	}
//
//	D3D11_BUFFER_DESC d3dBufferDesc;
//	ZeroMemory(&d3dBufferDesc, sizeof(D3D11_BUFFER_DESC));
//	d3dBufferDesc.Usage = D3D11_USAGE_DEFAULT;
//	d3dBufferDesc.ByteWidth = sizeof(CDiffusedVertex)* m_nVertices;
//	d3dBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
//	d3dBufferDesc.CPUAccessFlags = 0;
//	D3D11_SUBRESOURCE_DATA d3dBufferData;
//	ZeroMemory(&d3dBufferData, sizeof(D3D11_SUBRESOURCE_DATA));
//	//d3dBufferData.pSysMem = pVertices;
//	pd3dDevice->CreateBuffer(&d3dBufferDesc, &d3dBufferData, m_ppd3dVertexBuffers);
//
//
//
//	m_nIndices = ((nWidth * 2)*(nLength - 1)) + ((nLength - 1) - 1);
//	WORD *pIndices = new WORD[m_nIndices];
//	for (int j = 0, z = 0; z < nLength - 1; z++)
//	{
//		if ((z % 2) == 0)
//		{
//			//Ȧ�� ��° ���̹Ƿ�(z = 0, 2, 4, ...) �ε����� ���� ������ ���ʿ��� ������ �����̴�.
//			for (int x = 0; x < nWidth; x++)
//			{
//				//ù ��° ���� �����ϰ� ���� �ٲ� ������(x == 0) ù ��° �ε����� �߰��Ѵ�.
//				if ((x == 0) && (z > 0)) pIndices[j++] = (USHORT)(x + (z * nWidth));
//				//�Ʒ�, ���� ������ �ε����� �߰��Ѵ�.
//				pIndices[j++] = (USHORT)(x + (z * nWidth));
//				pIndices[j++] = (USHORT)((x + (z * nWidth)) + nWidth);
//			}
//		}
//		else
//		{
//			//¦�� ��° ���̹Ƿ�(z = 1, 3, 5, ...) �ε����� ���� ������ �����ʿ��� ���� �����̴�.
//			for (int x = nWidth - 1; x >= 0; x--)
//			{
//				//���� �ٲ� ������(x == (nWidth-1)) ù ��° �ε����� �߰��Ѵ�.
//				if (x == (nWidth - 1)) pIndices[j++] = (USHORT)(x + (z * nWidth));
//				//�Ʒ�, ���� ������ �ε����� �߰��Ѵ�.
//				pIndices[j++] = (USHORT)(x + (z * nWidth));
//				pIndices[j++] = (USHORT)((x + (z * nWidth)) + nWidth);
//			}
//		}
//	}
//
//	//CalculateVertexNormal((D3DXVECTOR3 *)pVertices);
//
//	ZeroMemory(&d3dBufferDesc, sizeof(D3D11_BUFFER_DESC));
//	d3dBufferDesc.Usage = D3D11_USAGE_DEFAULT;
//	d3dBufferDesc.ByteWidth = sizeof(WORD)* m_nIndices;
//	d3dBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
//	d3dBufferDesc.CPUAccessFlags = 0;
//	ZeroMemory(&d3dBufferData, sizeof(D3D11_SUBRESOURCE_DATA));
//	d3dBufferData.pSysMem = pIndices;
//	pd3dDevice->CreateBuffer(&d3dBufferDesc, &d3dBufferData, &m_pd3dIndexBuffer);
//
//	//delete[] pVertices;
//	delete[] pIndices;
//
//	SetRasterizerState(pd3dDevice);
//}
//
//CHeightMapGridMesh::~CHeightMapGridMesh()
//{
//}
//
//void CHeightMapGridMesh::SetRasterizerState(ID3D11Device *pd3dDevice)
//{
//	D3D11_RASTERIZER_DESC d3dRasterizerDesc;
//	ZeroMemory(&d3dRasterizerDesc, sizeof(D3D11_RASTERIZER_DESC));
//	d3dRasterizerDesc.CullMode = D3D11_CULL_BACK;
//	d3dRasterizerDesc.FillMode = D3D11_FILL_SOLID;
//	d3dRasterizerDesc.DepthClipEnable = true;
//	pd3dDevice->CreateRasterizerState(&d3dRasterizerDesc, &m_pd3dRasterizerState);
//}
//
//void CHeightMapGridMesh::Render(ID3D11DeviceContext *pd3dImmediateDeviceContext)
//{
//	CMesh::Render(pd3dImmediateDeviceContext);
//}
//
//float CHeightMapGridMesh::OnSetHeight(int x, int z, void *pContext)
//{
//	//���� �� ��ü�� ���� �� �̹����� �ȼ� ���� ������ ���̷� ��ȯ�Ѵ�. 
//	CHeightMap *pHeightMap = (CHeightMap *)pContext;
//	BYTE *pHeightMapImage = pHeightMap->GetHeightMapImage();
//	D3DXVECTOR3 d3dxvScale = pHeightMap->GetScale();
//	int cxTerrain = pHeightMap->GetHeightMapWidth();
//	float fHeight = pHeightMapImage[x + (z*cxTerrain)] * d3dxvScale.y;
//	return(fHeight);
//}
//
//D3DXCOLOR CHeightMapGridMesh::OnSetColor(int x, int z, void *pContext)
//{
//	//������ ���� �����̴�. 
//	D3DXVECTOR3 d3dxvLightDirection = -D3DXVECTOR3(0.650945f, -0.390567f, 0.650945f);
//	CHeightMap *pHeightMap = (CHeightMap *)pContext;
//	BYTE *pHeightMapImage = pHeightMap->GetHeightMapImage();
//	D3DXVECTOR3 d3dxvScale = pHeightMap->GetScale();
//	float fRed = 1.0f, fGreen = 0.8f, fBlue = 0.6f, fScale = 0.25f;
//
//	fScale = D3DXVec3Dot(&pHeightMap->GetHeightMapNormal(x, z), &d3dxvLightDirection);
//	fScale += D3DXVec3Dot(&pHeightMap->GetHeightMapNormal(x + 1, z), &d3dxvLightDirection);
//	fScale += D3DXVec3Dot(&pHeightMap->GetHeightMapNormal(x + 1, z + 1), &d3dxvLightDirection);
//	fScale += D3DXVec3Dot(&pHeightMap->GetHeightMapNormal(x, z + 1), &d3dxvLightDirection);
//	fScale = (fScale / 4.0f) + 0.05f;
//	if (fScale > 1.0f) fScale = 1.0f;
//	if (fScale < 0.25f) fScale = 0.25f;
//	D3DXCOLOR d3dxColor = D3DXCOLOR(fRed * fScale, fGreen * fScale, fBlue * fScale, 1.0f);
//	return(d3dxColor);
//}