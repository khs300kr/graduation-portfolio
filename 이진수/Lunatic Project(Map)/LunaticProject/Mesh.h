#pragma once

// 정점의 색상을 무작위로(Random) 설정하기 위해 사용한다.
// 각 정점의 색상은 난수(Random Number)를 생성하여 지정한다
//#define RANDOM_COLOR D3DXCOLOR((rand() * 0xFFFFFF) / RAND_MAX)
//
//class CDiffusedVertex
//{
//	D3DXVECTOR3 m_d3dxvPosition;
//	//정점의 색상을 나타내는 멤버 변수(D3DXCOLOR 구조체)를 선언한다. 
//	D3DXCOLOR m_d3dxcDiffuse;
//public:
//	//생성자와 소멸자를 선언한다.
//	CDiffusedVertex(float x, float y, float z, D3DXCOLOR d3dxcDiffuse) { m_d3dxvPosition = D3DXVECTOR3(x, y, z); m_d3dxcDiffuse = d3dxcDiffuse; }
//	CDiffusedVertex(D3DXVECTOR3 d3dxvPosition, D3DXCOLOR d3dxcDiffuse) { m_d3dxvPosition = d3dxvPosition; m_d3dxcDiffuse = d3dxcDiffuse; }
//	CDiffusedVertex() { m_d3dxvPosition = D3DXVECTOR3(0.0f, 0.0f, 0.0f); m_d3dxcDiffuse = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f); }
//	~CDiffusedVertex() { }
//};

//class CNormalVertex
//{
//	D3DXVECTOR3 m_d3dxvPosition;
//public:
//	//조명의 영향을 계산하기 위하여 법선벡터가 필요하다.
//	D3DXVECTOR3 m_d3dxvNormal;
//
//	CNormalVertex(float x, float y, float z, float nx, float ny, float nz) { m_d3dxvPosition = D3DXVECTOR3(x, y, z); m_d3dxvNormal = D3DXVECTOR3(nx, ny, nz); }
//	CNormalVertex(D3DXVECTOR3 d3dxvPosition, D3DXVECTOR3 d3dxvNormal) { m_d3dxvPosition = d3dxvPosition; m_d3dxvNormal = d3dxvNormal; }
//	CNormalVertex() { m_d3dxvPosition = D3DXVECTOR3(0.0f, 0.0f, 0.0f); m_d3dxvNormal = D3DXVECTOR3(0.0f, 0.0f, 0.0f); }
//	~CNormalVertex() { }
//};

class AABB
{
public:
	D3DXVECTOR3 m_d3dxvMinimum;
	D3DXVECTOR3 m_d3dxvMaximum;

public:
	AABB() { m_d3dxvMinimum = D3DXVECTOR3(+FLT_MAX, +FLT_MAX, +FLT_MAX); m_d3dxvMaximum = D3DXVECTOR3(-FLT_MAX, -FLT_MAX, -FLT_MAX); }
	AABB(D3DXVECTOR3 d3dxvMinimum, D3DXVECTOR3 d3dxvMaximum) { m_d3dxvMinimum = d3dxvMinimum; m_d3dxvMaximum = d3dxvMaximum; }

	// 두 개의 바운딩 박스를 합한다.
	void Union(D3DXVECTOR3& d3dxvMinimum, D3DXVECTOR3& d3dxvMaximum);
	void Union(AABB *pAABB);
	// 바운딩 박스의 8개의 꼭지점을 행렬로 변환하고 최소점과 최대점을 다시 계산한다.
	void Update(D3DXMATRIX *pd3dxmtxTransform);
};


class CMesh
{
private:
	int m_nReferences;

protected:
	AABB m_bcBoundingCube;

	D3D11_PRIMITIVE_TOPOLOGY m_d3dPrimitiveTopology;

	// 정점의 위치, 색상점의 위치 벡터와 색상을 저장하기 위한 버퍼에 대한 인터페이스 포인터
	ID3D11Buffer *m_pd3dPositionBuffer;
		
	UINT m_nSlot;		// 버퍼들을 입력조립기에 연결하기 위한 시작 슬롯 번호이다. 
	// 인스턴싱을 위한 정점 버퍼 = 메쉬의 정점 데이터 + 인스턴싱 데이터(위치, 방향)
	// 따라서, 인스턴싱을 위한 정점 버퍼는 하나가 아니라 버퍼들의 배열이 된다.
	ID3D11Buffer **m_ppd3dVertexBuffers;
	int m_nBuffers;		// 정점을 조립하기 위해 필요한 버퍼의 개수

	int m_nVertices;
	UINT m_nStartVertex;
	UINT *m_pnVertexStrides; // 버퍼의 원소 바이트 수를 나타내는 배열
	UINT *m_pnVertexOffsets; // 버퍼들의 시작 위치를 나타내는 배열

	ID3D11Buffer *m_pd3dIndexBuffer; // 인덱스 버퍼에 대한 인터페이스 포인터
	UINT m_nIndices;
	UINT m_nStartIndex;		// 인덱스 버퍼에서 메쉬 표현을 위한 시작 인덱스
	int m_nBaseVertex;		// 같 인덱스에 더해질 인덱스
	UINT m_nIndexOffset;	
	DXGI_FORMAT	m_dxgiIndexFormat; // 각 인덱스의 형식(DXGI_FORMAT_R32_UINT 또는 DXGI_FORMAT_R16_UINT) 

	ID3D11RasterizerState *m_pd3dRasterizerState;


	/*정점 버퍼의 정점 개수, 정점의 바이트 수, 정점 데이터가 정점 버퍼의 어디에서부터 시작하는 가를 나타내는 변수를 선언한다.*/
	//UINT m_nVertices;
	//UINT m_nStride;
	//UINT m_nOffset;


public:
	CMesh(ID3D11Device *pd3dDevice);
	virtual ~CMesh();

	void AddRef() { m_nReferences++; }
	void Release() { if (--m_nReferences <= 0) delete this; }
	AABB GetBoundingCube() { return(m_bcBoundingCube); }

	// 메쉬의 정점 버퍼들을 배열로 조립 
	void AssembleToVertexBuffer(int nBuffers = 0, ID3D11Buffer **m_pd3dBuffers = NULL, UINT *pnBufferStrides = NULL, UINT *pnBufferOffsets = NULL);

	virtual void CreateRasterizerState(ID3D11Device *pd3dDevice);
	virtual void Render(ID3D11DeviceContext *pd3dDeviceContext);
	virtual void RenderInstanced(ID3D11DeviceContext *pd3dDeviceContext, int nInstances = 0, int nStartInstance = 0);
	ID3D11Buffer* CreateBuffer(ID3D11Device *pd3dDevice, UINT nStride, int nElements, void *pBufferData, UINT nBindFlags, D3D11_USAGE d3dUsage, UINT nCPUAccessFlags);
	
	// 각 정점의 위치 벡터를 픽킹을 위하여 저장한다(정점 버퍼를 DYNAMIC으로 생성하고 Map()을 하지 않아도 되도록).
	D3DXVECTOR3 *m_pd3dxvPositions;
	UINT *m_pnIndices;

	// 160424 FBXMesh에서 사용할 함수
	virtual void UpdateBoneTransform(ID3D11DeviceContext *pd3dDeviceContext, int nAnimationNum, int nNowFrame);
	virtual float GetFBXModelSize() { return 0; }
	virtual float GetFBXAnimationTime() { return 0; }
	virtual int GetFBXAnimationNum() { return 0; }
	virtual int GetFBXNowFrameNum() { return 0; }
	virtual int GetFBXMaxFrameNum() { return 0; }
	//virtual void SetFBXAnimationTime(float fFBXAnimationTime) { }
	virtual bool FBXFrameAdvance(float fTimeElapsed) { return 0; }
	virtual void SetAnimation(int nFBXAnimationNum) {}

};

class CMeshDiffused : public CMesh
{
public:
	CMeshDiffused(ID3D11Device *pd3dDevice);
	virtual ~CMeshDiffused();

protected:
	ID3D11Buffer *m_pd3dColorBuffer;
};

class CMeshIlluminated : public CMesh
{
public:
	CMeshIlluminated(ID3D11Device *pd3dDevice);
	virtual ~CMeshIlluminated();

protected:
	// 조명의 영향을 계산하기 위한 법선벡터
	ID3D11Buffer *m_pd3dNormalBuffer;

public:
	// 정점이 포함된 삼각형의 법선벡터를 계산
	D3DXVECTOR3 CalculateTriAngleNormal(UINT nIndex0, UINT nIndex1, UINT nIndex2);
	void SetTriAngleListVertexNormal(D3DXVECTOR3 *pd3dxvNormals);
	// 정점의 법선벡터의 평균을 계산
	void SetAverageVertexNormal(D3DXVECTOR3 *pd3dxvNormals, int nPrimitives, int nOffset, bool bStrip);
	void CalculateVertexNormal(D3DXVECTOR3 *pd3dxvNormals);
};

class CMeshTextured : public CMesh
{
public:
	CMeshTextured(ID3D11Device *pd3dDevice);
	virtual ~CMeshTextured();

protected:
	ID3D11Buffer *m_pd3dTexCoordBuffer;
};


class CMeshTexturedIlluminated : public CMeshIlluminated
{
public:
	CMeshTexturedIlluminated(ID3D11Device *pd3dDevice);
	virtual ~CMeshTexturedIlluminated();

protected:
	ID3D11Buffer *m_pd3dTexCoordBuffer;
};


//////////// Terrain
//
//class CHeightMapGridMesh : public CMeshIlluminated
//{
//protected:
//	//격자의 크기(가로: x-방향, 세로: z-방향)이다.
//	int m_nWidth;
//	int m_nLength;
//	/*격자의 스케일(가로: x-방향, 세로: z-방향, 높이: y-방향) 벡터이다. 실제 격자 메쉬의 각 정점의 x-좌표, y-좌표, z-좌표는 스케일 벡터의 x-좌표, y-좌표, z-좌표로 곱한 값을 갖는다. 즉, 실제 격자의 x-축 방향의 간격은 1이 아니라 스케일 벡터의 x-좌표가 된다. 이렇게 하면 작은 격자를 사용하더라도 큰 격자를 생성할 수 있다.*/
//	D3DXVECTOR3 m_d3dxvScale;
//
//public:
//	CHeightMapGridMesh(ID3D11Device *pd3dDevice, int xStart, int zStart, int nWidth, int nLength, D3DXVECTOR3 d3dxvScale = D3DXVECTOR3(1.0f, 1.0f, 1.0f), D3DXCOLOR d3dxColor = D3DXCOLOR(1.0f, 1.0f, 0.0f, 0.0f), void *pContext = NULL);
//	~CHeightMapGridMesh();
//
//	D3DXVECTOR3 GetScale() { return(m_d3dxvScale); }
//	int GetWidth() { return(m_nWidth); }
//	int GetLength() { return(m_nLength); }
//
//	//격자의 교점(정점)의 높이를 설정한다.
//	virtual float OnSetHeight(int x, int z, void *pContext);
//	//격자의 교점(정점)의 색상을 설정한다.
//	virtual D3DXCOLOR OnSetColor(int x, int z, void *pContext);
//
//	virtual void SetRasterizerState(ID3D11Device *pd3dDevice);
//	virtual void Render(ID3D11DeviceContext *pd3dImmediateDeviceContext);
//};
