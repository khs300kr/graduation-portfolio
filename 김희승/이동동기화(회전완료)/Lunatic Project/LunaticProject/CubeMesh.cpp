#include "stdafx.h"
#include "CubeMesh.h"


//
// CCubeMeshDiffused Class
//////////////////////////////////////////////////////////////////////////////////////////
CCubeMeshDiffused::CCubeMeshDiffused(ID3D11Device *pd3dDevice, float fWidth, float fHeight, float fDepth, D3DXCOLOR d3dxColor) : CMeshDiffused(pd3dDevice)
{
	m_nVertices = 8;
	m_d3dPrimitiveTopology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP;

	float fx = fWidth*0.5f, fy = fHeight*0.5f, fz = fDepth*0.5f;

	// 직육면체 메쉬는 2개의 정점 버퍼(위치 벡터 버퍼와 색상 버퍼)로 구성된다.
	// 직육면체 메쉬의 정점 버퍼(위치 벡터 버퍼)를 생성한다.
	m_pd3dxvPositions = new D3DXVECTOR3[m_nVertices];
	m_pd3dxvPositions[0] = D3DXVECTOR3(-fx, +fy, -fz);
	m_pd3dxvPositions[1] = D3DXVECTOR3(+fx, +fy, -fz);
	m_pd3dxvPositions[2] = D3DXVECTOR3(+fx, +fy, +fz);
	m_pd3dxvPositions[3] = D3DXVECTOR3(-fx, +fy, +fz);
	m_pd3dxvPositions[4] = D3DXVECTOR3(-fx, -fy, -fz);
	m_pd3dxvPositions[5] = D3DXVECTOR3(+fx, -fy, -fz);
	m_pd3dxvPositions[6] = D3DXVECTOR3(+fx, -fy, +fz);
	m_pd3dxvPositions[7] = D3DXVECTOR3(-fx, -fy, +fz);

	D3D11_BUFFER_DESC d3dBufferDesc;
	ZeroMemory(&d3dBufferDesc, sizeof(D3D11_BUFFER_DESC));
	d3dBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	d3dBufferDesc.ByteWidth = sizeof(D3DXVECTOR3) * m_nVertices;
	d3dBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	d3dBufferDesc.CPUAccessFlags = 0;
	D3D11_SUBRESOURCE_DATA d3dBufferData;
	ZeroMemory(&d3dBufferData, sizeof(D3D11_SUBRESOURCE_DATA));
	d3dBufferData.pSysMem = m_pd3dxvPositions;
	pd3dDevice->CreateBuffer(&d3dBufferDesc, &d3dBufferData, &m_pd3dPositionBuffer);

	//직육면체 메쉬의 정점 버퍼(색상 버퍼)를 생성한다.
	D3DXCOLOR pd3dxColors[8];
	for (int i = 0; i < 8; i++) pd3dxColors[i] = d3dxColor + RANDOM_COLOR;

	d3dBufferDesc.ByteWidth = sizeof(D3DXCOLOR) * m_nVertices;
	d3dBufferData.pSysMem = pd3dxColors;
	pd3dDevice->CreateBuffer(&d3dBufferDesc, &d3dBufferData, &m_pd3dColorBuffer);

	ID3D11Buffer *pd3dBuffers[2] = { m_pd3dPositionBuffer, m_pd3dColorBuffer };
	UINT pnBufferStrides[2] = { sizeof(D3DXVECTOR3), sizeof(D3DXCOLOR) };
	UINT pnBufferOffsets[2] = { 0, 0 };
	AssembleToVertexBuffer(2, pd3dBuffers, pnBufferStrides, pnBufferOffsets);

	m_nIndices = 18;
	m_pnIndices = new UINT[m_nIndices];
	m_pnIndices[0] = 5; //5,6,4 - cw
	m_pnIndices[1] = 6; //6,4,7 - ccw
	m_pnIndices[2] = 4; //4,7,0 - cw
	m_pnIndices[3] = 7; //7,0,3 - ccw
	m_pnIndices[4] = 0; //0,3,1 - cw
	m_pnIndices[5] = 3; //3,1,2 - ccw
	m_pnIndices[6] = 1; //1,2,2 - cw 
	m_pnIndices[7] = 2; //2,2,3 - ccw
	m_pnIndices[8] = 2; //2,3,3 - cw  - Degenerated Index(2)
	m_pnIndices[9] = 3; //3,3,7 - ccw - Degenerated Index(3)
	m_pnIndices[10] = 3;//3,7,2 - cw  - Degenerated Index(3)
	m_pnIndices[11] = 7;//7,2,6 - ccw
	m_pnIndices[12] = 2;//2,6,1 - cw
	m_pnIndices[13] = 6;//6,1,5 - ccw
	m_pnIndices[14] = 1;//1,5,0 - cw
	m_pnIndices[15] = 5;//5,0,4 - ccw
	m_pnIndices[16] = 0;
	m_pnIndices[17] = 4;

	ZeroMemory(&d3dBufferDesc, sizeof(D3D11_BUFFER_DESC));
	d3dBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	d3dBufferDesc.ByteWidth = sizeof(UINT) * m_nIndices;
	d3dBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	d3dBufferDesc.CPUAccessFlags = 0;
	ZeroMemory(&d3dBufferData, sizeof(D3D11_SUBRESOURCE_DATA));
	d3dBufferData.pSysMem = m_pnIndices;
	pd3dDevice->CreateBuffer(&d3dBufferDesc, &d3dBufferData, &m_pd3dIndexBuffer);

	CreateRasterizerState(pd3dDevice);

	// 정점 버퍼 데이터를 생성한 다음 최소점과 최대점을 저장한다. 
	m_bcBoundingCube.m_d3dxvMinimum = D3DXVECTOR3(-fx, -fy, -fz);
	m_bcBoundingCube.m_d3dxvMaximum = D3DXVECTOR3(+fx, +fy, +fz);
}

CCubeMeshDiffused::~CCubeMeshDiffused()
{
}


void CCubeMeshDiffused::CreateRasterizerState(ID3D11Device *pd3dDevice)
{
	D3D11_RASTERIZER_DESC d3dRasterizerDesc;
	ZeroMemory(&d3dRasterizerDesc, sizeof(D3D11_RASTERIZER_DESC));
	d3dRasterizerDesc.CullMode = D3D11_CULL_BACK;
	d3dRasterizerDesc.FillMode = D3D11_FILL_SOLID;
	pd3dDevice->CreateRasterizerState(&d3dRasterizerDesc, &m_pd3dRasterizerState);
}


void CCubeMeshDiffused::Render(ID3D11DeviceContext *pd3dDeviceContext)
{
	CMesh::Render(pd3dDeviceContext);
}



//
// CCubeMeshIlluminated Class
//////////////////////////////////////////////////////////////////////////////////////////
CCubeMeshIlluminated::CCubeMeshIlluminated(ID3D11Device *pd3dDevice, float fWidth, float fHeight, float fDepth) : CMeshIlluminated(pd3dDevice)
{
	m_nVertices = 8;
	m_d3dPrimitiveTopology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;

	float fx = fWidth*0.5f, fy = fHeight*0.5f, fz = fDepth*0.5f;

	m_pd3dxvPositions = new D3DXVECTOR3[m_nVertices];

	m_pd3dxvPositions[0] = D3DXVECTOR3(-fx, +fy, -fz);
	m_pd3dxvPositions[1] = D3DXVECTOR3(+fx, +fy, -fz);
	m_pd3dxvPositions[2] = D3DXVECTOR3(+fx, +fy, +fz);
	m_pd3dxvPositions[3] = D3DXVECTOR3(-fx, +fy, +fz);
	m_pd3dxvPositions[4] = D3DXVECTOR3(-fx, -fy, -fz);
	m_pd3dxvPositions[5] = D3DXVECTOR3(+fx, -fy, -fz);
	m_pd3dxvPositions[6] = D3DXVECTOR3(+fx, -fy, +fz);
	m_pd3dxvPositions[7] = D3DXVECTOR3(-fx, -fy, +fz);

	D3D11_BUFFER_DESC d3dBufferDesc;
	ZeroMemory(&d3dBufferDesc, sizeof(D3D11_BUFFER_DESC));
	d3dBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	d3dBufferDesc.ByteWidth = sizeof(D3DXVECTOR3) * m_nVertices;
	d3dBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	d3dBufferDesc.CPUAccessFlags = 0;
	D3D11_SUBRESOURCE_DATA d3dBufferData;
	ZeroMemory(&d3dBufferData, sizeof(D3D11_SUBRESOURCE_DATA));
	d3dBufferData.pSysMem = m_pd3dxvPositions;
	pd3dDevice->CreateBuffer(&d3dBufferDesc, &d3dBufferData, &m_pd3dPositionBuffer);

	m_nIndices = 36;
	m_pnIndices = new UINT[m_nIndices];

	m_pnIndices[0] = 3; m_pnIndices[1] = 1; m_pnIndices[2] = 0;
	m_pnIndices[3] = 2; m_pnIndices[4] = 1; m_pnIndices[5] = 3;
	m_pnIndices[6] = 0; m_pnIndices[7] = 5; m_pnIndices[8] = 4;
	m_pnIndices[9] = 1; m_pnIndices[10] = 5; m_pnIndices[11] = 0;
	m_pnIndices[12] = 3; m_pnIndices[13] = 4; m_pnIndices[14] = 7;
	m_pnIndices[15] = 0; m_pnIndices[16] = 4; m_pnIndices[17] = 3;
	m_pnIndices[18] = 1; m_pnIndices[19] = 6; m_pnIndices[20] = 5;
	m_pnIndices[21] = 2; m_pnIndices[22] = 6; m_pnIndices[23] = 1;
	m_pnIndices[24] = 2; m_pnIndices[25] = 7; m_pnIndices[26] = 6;
	m_pnIndices[27] = 3; m_pnIndices[28] = 7; m_pnIndices[29] = 2;
	m_pnIndices[30] = 6; m_pnIndices[31] = 4; m_pnIndices[32] = 5;
	m_pnIndices[33] = 7; m_pnIndices[34] = 4; m_pnIndices[35] = 6;

	D3DXVECTOR3 pd3dxvNormals[8];
	CalculateVertexNormal(pd3dxvNormals);

	d3dBufferDesc.ByteWidth = sizeof(D3DXVECTOR3) * m_nVertices;
	d3dBufferData.pSysMem = pd3dxvNormals;
	pd3dDevice->CreateBuffer(&d3dBufferDesc, &d3dBufferData, &m_pd3dNormalBuffer);

	ID3D11Buffer *pd3dBuffers[2] = { m_pd3dPositionBuffer, m_pd3dNormalBuffer };
	UINT pnBufferStrides[2] = { sizeof(D3DXVECTOR3), sizeof(D3DXVECTOR3) };
	UINT pnBufferOffsets[2] = { 0, 0 };
	AssembleToVertexBuffer(2, pd3dBuffers, pnBufferStrides, pnBufferOffsets);

	ZeroMemory(&d3dBufferDesc, sizeof(D3D11_BUFFER_DESC));
	d3dBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	d3dBufferDesc.ByteWidth = sizeof(UINT) * m_nIndices;
	d3dBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	d3dBufferDesc.CPUAccessFlags = 0;
	ZeroMemory(&d3dBufferData, sizeof(D3D11_SUBRESOURCE_DATA));
	d3dBufferData.pSysMem = m_pnIndices;
	pd3dDevice->CreateBuffer(&d3dBufferDesc, &d3dBufferData, &m_pd3dIndexBuffer);

	m_bcBoundingCube.m_d3dxvMinimum = D3DXVECTOR3(-fx, -fy, -fz);
	m_bcBoundingCube.m_d3dxvMaximum = D3DXVECTOR3(+fx, +fy, +fz);
}

CCubeMeshIlluminated::~CCubeMeshIlluminated()
{
}

CCubeMeshTextured::CCubeMeshTextured(ID3D11Device *pd3dDevice, float fWidth, float fHeight, float fDepth) : CMeshTextured(pd3dDevice)
{
	m_nVertices = 36;
	m_d3dPrimitiveTopology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;

	float fx = fWidth*0.5f, fy = fHeight*0.5f, fz = fDepth*0.5f;

	m_pd3dxvPositions = new D3DXVECTOR3[m_nVertices];
	D3DXVECTOR2 pd3dxvTexCoords[36];
	int i = 0;

	//직육면체의 각 면(삼각형 2개)에 하나의 텍스쳐 이미지 전체가 맵핑되도록 텍스쳐 좌표를 설정한다.
	m_pd3dxvPositions[i] = D3DXVECTOR3(-fx, +fy, -fz);
	pd3dxvTexCoords[i++] = D3DXVECTOR2(0.0f, 0.0f);
	m_pd3dxvPositions[i] = D3DXVECTOR3(+fx, +fy, -fz);
	pd3dxvTexCoords[i++] = D3DXVECTOR2(1.0f, 0.0f);
	m_pd3dxvPositions[i] = D3DXVECTOR3(+fx, -fy, -fz);
	pd3dxvTexCoords[i++] = D3DXVECTOR2(1.0f, 1.0f);

	m_pd3dxvPositions[i] = D3DXVECTOR3(-fx, +fy, -fz);
	pd3dxvTexCoords[i++] = D3DXVECTOR2(0.0f, 0.0f);
	m_pd3dxvPositions[i] = D3DXVECTOR3(+fx, -fy, -fz);
	pd3dxvTexCoords[i++] = D3DXVECTOR2(1.0f, 1.0f);
	m_pd3dxvPositions[i] = D3DXVECTOR3(-fx, -fy, -fz);
	pd3dxvTexCoords[i++] = D3DXVECTOR2(0.0f, 1.0f);
	
	m_pd3dxvPositions[i] = D3DXVECTOR3(-fx, +fy, +fz);
	pd3dxvTexCoords[i++] = D3DXVECTOR2(0.0f, 0.0f);
	m_pd3dxvPositions[i] = D3DXVECTOR3(+fx, +fy, +fz);
	pd3dxvTexCoords[i++] = D3DXVECTOR2(1.0f, 0.0f);
	m_pd3dxvPositions[i] = D3DXVECTOR3(+fx, +fy, -fz);
	pd3dxvTexCoords[i++] = D3DXVECTOR2(1.0f, 1.0f);

	m_pd3dxvPositions[i] = D3DXVECTOR3(-fx, +fy, +fz);
	pd3dxvTexCoords[i++] = D3DXVECTOR2(0.0f, 0.0f);
	m_pd3dxvPositions[i] = D3DXVECTOR3(+fx, +fy, -fz);
	pd3dxvTexCoords[i++] = D3DXVECTOR2(1.0f, 1.0f);
	m_pd3dxvPositions[i] = D3DXVECTOR3(-fx, +fy, -fz);
	pd3dxvTexCoords[i++] = D3DXVECTOR2(0.0f, 1.0f);

	m_pd3dxvPositions[i] = D3DXVECTOR3(-fx, -fy, +fz);
	pd3dxvTexCoords[i++] = D3DXVECTOR2(0.0f, 0.0f);
	m_pd3dxvPositions[i] = D3DXVECTOR3(+fx, -fy, +fz);
	pd3dxvTexCoords[i++] = D3DXVECTOR2(1.0f, 0.0f);
	m_pd3dxvPositions[i] = D3DXVECTOR3(+fx, +fy, +fz);
	pd3dxvTexCoords[i++] = D3DXVECTOR2(1.0f, 1.0f);

	m_pd3dxvPositions[i] = D3DXVECTOR3(-fx, -fy, +fz);
	pd3dxvTexCoords[i++] = D3DXVECTOR2(0.0f, 0.0f);
	m_pd3dxvPositions[i] = D3DXVECTOR3(+fx, +fy, +fz);
	pd3dxvTexCoords[i++] = D3DXVECTOR2(1.0f, 1.0f);
	m_pd3dxvPositions[i] = D3DXVECTOR3(-fx, +fy, +fz);
	pd3dxvTexCoords[i++] = D3DXVECTOR2(0.0f, 1.0f);

	m_pd3dxvPositions[i] = D3DXVECTOR3(-fx, -fy, -fz);
	pd3dxvTexCoords[i++] = D3DXVECTOR2(0.0f, 0.0f);
	m_pd3dxvPositions[i] = D3DXVECTOR3(+fx, -fy, -fz);
	pd3dxvTexCoords[i++] = D3DXVECTOR2(1.0f, 0.0f);
	m_pd3dxvPositions[i] = D3DXVECTOR3(+fx, -fy, +fz);
	pd3dxvTexCoords[i++] = D3DXVECTOR2(1.0f, 1.0f);

	m_pd3dxvPositions[i] = D3DXVECTOR3(-fx, -fy, -fz);
	pd3dxvTexCoords[i++] = D3DXVECTOR2(0.0f, 0.0f);
	m_pd3dxvPositions[i] = D3DXVECTOR3(+fx, -fy, +fz);
	pd3dxvTexCoords[i++] = D3DXVECTOR2(1.0f, 1.0f);
	m_pd3dxvPositions[i] = D3DXVECTOR3(-fx, -fy, +fz);
	pd3dxvTexCoords[i++] = D3DXVECTOR2(0.0f, 1.0f);

	m_pd3dxvPositions[i] = D3DXVECTOR3(-fx, +fy, +fz);
	pd3dxvTexCoords[i++] = D3DXVECTOR2(0.0f, 0.0f);
	m_pd3dxvPositions[i] = D3DXVECTOR3(-fx, +fy, -fz);
	pd3dxvTexCoords[i++] = D3DXVECTOR2(1.0f, 0.0f);
	m_pd3dxvPositions[i] = D3DXVECTOR3(-fx, -fy, -fz);
	pd3dxvTexCoords[i++] = D3DXVECTOR2(1.0f, 1.0f);

	m_pd3dxvPositions[i] = D3DXVECTOR3(-fx, +fy, +fz);
	pd3dxvTexCoords[i++] = D3DXVECTOR2(0.0f, 0.0f);
	m_pd3dxvPositions[i] = D3DXVECTOR3(-fx, -fy, -fz);
	pd3dxvTexCoords[i++] = D3DXVECTOR2(1.0f, 1.0f);
	m_pd3dxvPositions[i] = D3DXVECTOR3(-fx, -fy, +fz);
	pd3dxvTexCoords[i++] = D3DXVECTOR2(0.0f, 1.0f);

	m_pd3dxvPositions[i] = D3DXVECTOR3(+fx, +fy, -fz);
	pd3dxvTexCoords[i++] = D3DXVECTOR2(0.0f, 0.0f);
	m_pd3dxvPositions[i] = D3DXVECTOR3(+fx, +fy, +fz);
	pd3dxvTexCoords[i++] = D3DXVECTOR2(1.0f, 0.0f);
	m_pd3dxvPositions[i] = D3DXVECTOR3(+fx, -fy, +fz);
	pd3dxvTexCoords[i++] = D3DXVECTOR2(1.0f, 1.0f);

	m_pd3dxvPositions[i] = D3DXVECTOR3(+fx, +fy, -fz);
	pd3dxvTexCoords[i++] = D3DXVECTOR2(0.0f, 0.0f);
	m_pd3dxvPositions[i] = D3DXVECTOR3(+fx, -fy, +fz);
	pd3dxvTexCoords[i++] = D3DXVECTOR2(1.0f, 1.0f);
	m_pd3dxvPositions[i] = D3DXVECTOR3(+fx, -fy, -fz);
	pd3dxvTexCoords[i++] = D3DXVECTOR2(0.0f, 1.0f);

	D3D11_BUFFER_DESC d3dBufferDesc;
	ZeroMemory(&d3dBufferDesc, sizeof(D3D11_BUFFER_DESC));
	d3dBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	d3dBufferDesc.ByteWidth = sizeof(D3DXVECTOR3) * m_nVertices;
	d3dBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	d3dBufferDesc.CPUAccessFlags = 0;
	D3D11_SUBRESOURCE_DATA d3dBufferData;
	ZeroMemory(&d3dBufferData, sizeof(D3D11_SUBRESOURCE_DATA));
	d3dBufferData.pSysMem = m_pd3dxvPositions;
	pd3dDevice->CreateBuffer(&d3dBufferDesc, &d3dBufferData, &m_pd3dPositionBuffer);

	d3dBufferData.pSysMem = pd3dxvTexCoords;
	pd3dDevice->CreateBuffer(&d3dBufferDesc, &d3dBufferData, &m_pd3dTexCoordBuffer);

	ID3D11Buffer *pd3dBuffers[2] = { m_pd3dPositionBuffer, m_pd3dTexCoordBuffer };
	UINT pnBufferStrides[2] = { sizeof(D3DXVECTOR3), sizeof(D3DXVECTOR2) };
	UINT pnBufferOffsets[2] = { 0, 0 };
	AssembleToVertexBuffer(2, pd3dBuffers, pnBufferStrides, pnBufferOffsets);

	/*직육면체의 각 면에 텍스쳐를 맵핑하려면 인덱스를 사용할 수 없으므로 인덱스 버퍼는 생성하지 않는다.*/

	m_bcBoundingCube.m_d3dxvMinimum = D3DXVECTOR3(-fx, -fy, -fz);
	m_bcBoundingCube.m_d3dxvMaximum = D3DXVECTOR3(+fx, +fy, +fz);
}

CCubeMeshTextured::~CCubeMeshTextured()
{
}



//
// CCubeMeshTexturedIlluminated Class
//////////////////////////////////////////////////////////////////////////////////////////
CCubeMeshTexturedIlluminated::CCubeMeshTexturedIlluminated(ID3D11Device *pd3dDevice, float fWidth, float fHeight, float fDepth) : CMeshTexturedIlluminated(pd3dDevice)
{
	m_nVertices = 36;
	m_d3dPrimitiveTopology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;

	float fx = fWidth*0.5f, fy = fHeight*0.5f, fz = fDepth*0.5f;

	m_pd3dxvPositions = new D3DXVECTOR3[m_nVertices];
	D3DXVECTOR2 pd3dxvTexCoords[36];
	int i = 0;

	m_pd3dxvPositions[i] = D3DXVECTOR3(-fx, +fy, -fz);	pd3dxvTexCoords[i++] = D3DXVECTOR2(0.0f, 0.0f);
	m_pd3dxvPositions[i] = D3DXVECTOR3(+fx, +fy, -fz);	pd3dxvTexCoords[i++] = D3DXVECTOR2(1.0f, 0.0f);
	m_pd3dxvPositions[i] = D3DXVECTOR3(+fx, -fy, -fz);	pd3dxvTexCoords[i++] = D3DXVECTOR2(1.0f, 1.0f);

	m_pd3dxvPositions[i] = D3DXVECTOR3(-fx, +fy, -fz);	pd3dxvTexCoords[i++] = D3DXVECTOR2(0.0f, 0.0f);
	m_pd3dxvPositions[i] = D3DXVECTOR3(+fx, -fy, -fz);	pd3dxvTexCoords[i++] = D3DXVECTOR2(1.0f, 1.0f);
	m_pd3dxvPositions[i] = D3DXVECTOR3(-fx, -fy, -fz);	pd3dxvTexCoords[i++] = D3DXVECTOR2(0.0f, 1.0f);

	m_pd3dxvPositions[i] = D3DXVECTOR3(-fx, +fy, +fz);	pd3dxvTexCoords[i++] = D3DXVECTOR2(0.0f, 0.0f);
	m_pd3dxvPositions[i] = D3DXVECTOR3(+fx, +fy, +fz);	pd3dxvTexCoords[i++] = D3DXVECTOR2(1.0f, 0.0f);
	m_pd3dxvPositions[i] = D3DXVECTOR3(+fx, +fy, -fz);	pd3dxvTexCoords[i++] = D3DXVECTOR2(1.0f, 1.0f);

	m_pd3dxvPositions[i] = D3DXVECTOR3(-fx, +fy, +fz);	pd3dxvTexCoords[i++] = D3DXVECTOR2(0.0f, 0.0f);
	m_pd3dxvPositions[i] = D3DXVECTOR3(+fx, +fy, -fz);	pd3dxvTexCoords[i++] = D3DXVECTOR2(1.0f, 1.0f);
	m_pd3dxvPositions[i] = D3DXVECTOR3(-fx, +fy, -fz);	pd3dxvTexCoords[i++] = D3DXVECTOR2(0.0f, 1.0f);

	m_pd3dxvPositions[i] = D3DXVECTOR3(-fx, -fy, +fz);	pd3dxvTexCoords[i++] = D3DXVECTOR2(0.0f, 0.0f);
	m_pd3dxvPositions[i] = D3DXVECTOR3(+fx, -fy, +fz);	pd3dxvTexCoords[i++] = D3DXVECTOR2(1.0f, 0.0f);
	m_pd3dxvPositions[i] = D3DXVECTOR3(+fx, +fy, +fz);	pd3dxvTexCoords[i++] = D3DXVECTOR2(1.0f, 1.0f);

	m_pd3dxvPositions[i] = D3DXVECTOR3(-fx, -fy, +fz);	pd3dxvTexCoords[i++] = D3DXVECTOR2(0.0f, 0.0f);
	m_pd3dxvPositions[i] = D3DXVECTOR3(+fx, +fy, +fz);	pd3dxvTexCoords[i++] = D3DXVECTOR2(1.0f, 1.0f);
	m_pd3dxvPositions[i] = D3DXVECTOR3(-fx, +fy, +fz);	pd3dxvTexCoords[i++] = D3DXVECTOR2(0.0f, 1.0f);

	m_pd3dxvPositions[i] = D3DXVECTOR3(-fx, -fy, -fz);	pd3dxvTexCoords[i++] = D3DXVECTOR2(0.0f, 0.0f);
	m_pd3dxvPositions[i] = D3DXVECTOR3(+fx, -fy, -fz);	pd3dxvTexCoords[i++] = D3DXVECTOR2(1.0f, 0.0f);
	m_pd3dxvPositions[i] = D3DXVECTOR3(+fx, -fy, +fz);	pd3dxvTexCoords[i++] = D3DXVECTOR2(1.0f, 1.0f);

	m_pd3dxvPositions[i] = D3DXVECTOR3(-fx, -fy, -fz);	pd3dxvTexCoords[i++] = D3DXVECTOR2(0.0f, 0.0f);
	m_pd3dxvPositions[i] = D3DXVECTOR3(+fx, -fy, +fz);	pd3dxvTexCoords[i++] = D3DXVECTOR2(1.0f, 1.0f);
	m_pd3dxvPositions[i] = D3DXVECTOR3(-fx, -fy, +fz);	pd3dxvTexCoords[i++] = D3DXVECTOR2(0.0f, 1.0f);

	m_pd3dxvPositions[i] = D3DXVECTOR3(-fx, +fy, +fz);	pd3dxvTexCoords[i++] = D3DXVECTOR2(0.0f, 0.0f);
	m_pd3dxvPositions[i] = D3DXVECTOR3(-fx, +fy, -fz);	pd3dxvTexCoords[i++] = D3DXVECTOR2(1.0f, 0.0f);
	m_pd3dxvPositions[i] = D3DXVECTOR3(-fx, -fy, -fz);	pd3dxvTexCoords[i++] = D3DXVECTOR2(1.0f, 1.0f);

	m_pd3dxvPositions[i] = D3DXVECTOR3(-fx, +fy, +fz);	pd3dxvTexCoords[i++] = D3DXVECTOR2(0.0f, 0.0f);
	m_pd3dxvPositions[i] = D3DXVECTOR3(-fx, -fy, -fz);	pd3dxvTexCoords[i++] = D3DXVECTOR2(1.0f, 1.0f);
	m_pd3dxvPositions[i] = D3DXVECTOR3(-fx, -fy, +fz);	pd3dxvTexCoords[i++] = D3DXVECTOR2(0.0f, 1.0f);

	m_pd3dxvPositions[i] = D3DXVECTOR3(+fx, +fy, -fz);	pd3dxvTexCoords[i++] = D3DXVECTOR2(0.0f, 0.0f);
	m_pd3dxvPositions[i] = D3DXVECTOR3(+fx, +fy, +fz);	pd3dxvTexCoords[i++] = D3DXVECTOR2(1.0f, 0.0f);
	m_pd3dxvPositions[i] = D3DXVECTOR3(+fx, -fy, +fz);	pd3dxvTexCoords[i++] = D3DXVECTOR2(1.0f, 1.0f);

	m_pd3dxvPositions[i] = D3DXVECTOR3(+fx, +fy, -fz);	pd3dxvTexCoords[i++] = D3DXVECTOR2(0.0f, 0.0f);
	m_pd3dxvPositions[i] = D3DXVECTOR3(+fx, -fy, +fz);	pd3dxvTexCoords[i++] = D3DXVECTOR2(1.0f, 1.0f);
	m_pd3dxvPositions[i] = D3DXVECTOR3(+fx, -fy, -fz);	pd3dxvTexCoords[i++] = D3DXVECTOR2(0.0f, 1.0f);
	
	D3D11_BUFFER_DESC d3dBufferDesc;
	ZeroMemory(&d3dBufferDesc, sizeof(D3D11_BUFFER_DESC));
	d3dBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	d3dBufferDesc.ByteWidth = sizeof(D3DXVECTOR3) * m_nVertices;
	d3dBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	d3dBufferDesc.CPUAccessFlags = 0;
	D3D11_SUBRESOURCE_DATA d3dBufferData;
	ZeroMemory(&d3dBufferData, sizeof(D3D11_SUBRESOURCE_DATA));
	d3dBufferData.pSysMem = m_pd3dxvPositions;
	pd3dDevice->CreateBuffer(&d3dBufferDesc, &d3dBufferData, &m_pd3dPositionBuffer);

	D3DXVECTOR3 pd3dxvNormals[36];
	CalculateVertexNormal(pd3dxvNormals);

	d3dBufferDesc.ByteWidth = sizeof(D3DXVECTOR3) * m_nVertices;
	d3dBufferData.pSysMem = pd3dxvNormals;
	pd3dDevice->CreateBuffer(&d3dBufferDesc, &d3dBufferData, &m_pd3dNormalBuffer);

	d3dBufferDesc.ByteWidth = sizeof(D3DXVECTOR2) * m_nVertices;
	d3dBufferData.pSysMem = pd3dxvTexCoords;
	pd3dDevice->CreateBuffer(&d3dBufferDesc, &d3dBufferData, &m_pd3dTexCoordBuffer);

	ID3D11Buffer *pd3dBuffers[3] = { m_pd3dPositionBuffer, m_pd3dNormalBuffer, m_pd3dTexCoordBuffer };
	UINT pnBufferStrides[3] = { sizeof(D3DXVECTOR3), sizeof(D3DXVECTOR3), sizeof(D3DXVECTOR2) };
	UINT pnBufferOffsets[3] = { 0, 0, 0 };
	AssembleToVertexBuffer(3, pd3dBuffers, pnBufferStrides, pnBufferOffsets);

	m_bcBoundingCube.m_d3dxvMinimum = D3DXVECTOR3(-fx, -fy, -fz);
	m_bcBoundingCube.m_d3dxvMaximum = D3DXVECTOR3(+fx, +fy, +fz);
}

CCubeMeshTexturedIlluminated::~CCubeMeshTexturedIlluminated()
{
}