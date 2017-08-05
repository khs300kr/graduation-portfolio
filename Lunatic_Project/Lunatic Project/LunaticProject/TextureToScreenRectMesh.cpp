#include "stdafx.h"
#include "TextureToScreenRectMesh.h"


CTextureToScreenRectMesh::CTextureToScreenRectMesh(ID3D11Device *pd3dDevice, float fWidth, float fHeight) : CMeshTextured(pd3dDevice)
{
	m_nVertices = 4;
	m_d3dPrimitiveTopology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP;

	float fx = fWidth*0.5f, fy = fHeight*0.5f;

	m_pPositions = new D3DXVECTOR3[m_nVertices];
	D3DXVECTOR2 *pd3dxvTexCoords = new D3DXVECTOR2[m_nVertices];

	m_pPositions[0] = D3DXVECTOR3(-fx, -fy, 0.9f);
	m_pPositions[1] = D3DXVECTOR3(-fx, +fy, 0.9f);
	m_pPositions[2] = D3DXVECTOR3(+fx, -fy, 0.9f);
	m_pPositions[3] = D3DXVECTOR3(+fx, +fy, 0.9f);
	
	pd3dxvTexCoords[0] = D3DXVECTOR2(0.0f, 1.0f);
	pd3dxvTexCoords[1] = D3DXVECTOR2(0.0f, 0.0f);
	pd3dxvTexCoords[2] = D3DXVECTOR2(1.0f, 1.0f);
	pd3dxvTexCoords[3] = D3DXVECTOR2(1.0f, 0.0f);

	m_pd3dPositionBuffer = CreateBuffer(pd3dDevice, sizeof(D3DXVECTOR3), m_nVertices, m_pPositions, D3D11_BIND_VERTEX_BUFFER, D3D11_USAGE_DEFAULT, 0);
	m_pd3dTexCoordBuffer = CreateBuffer(pd3dDevice, sizeof(D3DXVECTOR2), m_nVertices, pd3dxvTexCoords, D3D11_BIND_VERTEX_BUFFER, D3D11_USAGE_DEFAULT, 0);

	delete[] pd3dxvTexCoords;

	ID3D11Buffer *pd3dBuffers[2] = { m_pd3dPositionBuffer, m_pd3dTexCoordBuffer };
	UINT pnBufferStrides[2] = { sizeof(XMFLOAT3), sizeof(XMFLOAT2) };
	UINT pnBufferOffsets[2] = { 0, 0 };
	AssembleToVertexBuffer(2, pd3dBuffers, pnBufferStrides, pnBufferOffsets);

	//m_bcBoundingBox.Center = { 0.f, 0.f, 0.f };
	//m_bcBoundingBox.Extents = { fx, fy, 0.0f };

	//DXUT_SetDebugName(m_pd3dPositionBuffer, "Position");
	//(m_pd3dTexCoordBuffer, "TexCoord");
//	DXUT_SetDebugName(m_pd3dIndexBuffer, "Index");
}

CTextureToScreenRectMesh::~CTextureToScreenRectMesh()
{
}
