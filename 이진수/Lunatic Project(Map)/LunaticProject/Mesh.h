#pragma once

// ������ ������ ��������(Random) �����ϱ� ���� ����Ѵ�.
// �� ������ ������ ����(Random Number)�� �����Ͽ� �����Ѵ�
//#define RANDOM_COLOR D3DXCOLOR((rand() * 0xFFFFFF) / RAND_MAX)
//
//class CDiffusedVertex
//{
//	D3DXVECTOR3 m_d3dxvPosition;
//	//������ ������ ��Ÿ���� ��� ����(D3DXCOLOR ����ü)�� �����Ѵ�. 
//	D3DXCOLOR m_d3dxcDiffuse;
//public:
//	//�����ڿ� �Ҹ��ڸ� �����Ѵ�.
//	CDiffusedVertex(float x, float y, float z, D3DXCOLOR d3dxcDiffuse) { m_d3dxvPosition = D3DXVECTOR3(x, y, z); m_d3dxcDiffuse = d3dxcDiffuse; }
//	CDiffusedVertex(D3DXVECTOR3 d3dxvPosition, D3DXCOLOR d3dxcDiffuse) { m_d3dxvPosition = d3dxvPosition; m_d3dxcDiffuse = d3dxcDiffuse; }
//	CDiffusedVertex() { m_d3dxvPosition = D3DXVECTOR3(0.0f, 0.0f, 0.0f); m_d3dxcDiffuse = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f); }
//	~CDiffusedVertex() { }
//};

//class CNormalVertex
//{
//	D3DXVECTOR3 m_d3dxvPosition;
//public:
//	//������ ������ ����ϱ� ���Ͽ� �������Ͱ� �ʿ��ϴ�.
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

	// �� ���� �ٿ�� �ڽ��� ���Ѵ�.
	void Union(D3DXVECTOR3& d3dxvMinimum, D3DXVECTOR3& d3dxvMaximum);
	void Union(AABB *pAABB);
	// �ٿ�� �ڽ��� 8���� �������� ��ķ� ��ȯ�ϰ� �ּ����� �ִ����� �ٽ� ����Ѵ�.
	void Update(D3DXMATRIX *pd3dxmtxTransform);
};


class CMesh
{
private:
	int m_nReferences;

protected:
	AABB m_bcBoundingCube;

	D3D11_PRIMITIVE_TOPOLOGY m_d3dPrimitiveTopology;

	// ������ ��ġ, �������� ��ġ ���Ϳ� ������ �����ϱ� ���� ���ۿ� ���� �������̽� ������
	ID3D11Buffer *m_pd3dPositionBuffer;
		
	UINT m_nSlot;		// ���۵��� �Է������⿡ �����ϱ� ���� ���� ���� ��ȣ�̴�. 
	// �ν��Ͻ��� ���� ���� ���� = �޽��� ���� ������ + �ν��Ͻ� ������(��ġ, ����)
	// ����, �ν��Ͻ��� ���� ���� ���۴� �ϳ��� �ƴ϶� ���۵��� �迭�� �ȴ�.
	ID3D11Buffer **m_ppd3dVertexBuffers;
	int m_nBuffers;		// ������ �����ϱ� ���� �ʿ��� ������ ����

	int m_nVertices;
	UINT m_nStartVertex;
	UINT *m_pnVertexStrides; // ������ ���� ����Ʈ ���� ��Ÿ���� �迭
	UINT *m_pnVertexOffsets; // ���۵��� ���� ��ġ�� ��Ÿ���� �迭

	ID3D11Buffer *m_pd3dIndexBuffer; // �ε��� ���ۿ� ���� �������̽� ������
	UINT m_nIndices;
	UINT m_nStartIndex;		// �ε��� ���ۿ��� �޽� ǥ���� ���� ���� �ε���
	int m_nBaseVertex;		// �� �ε����� ������ �ε���
	UINT m_nIndexOffset;	
	DXGI_FORMAT	m_dxgiIndexFormat; // �� �ε����� ����(DXGI_FORMAT_R32_UINT �Ǵ� DXGI_FORMAT_R16_UINT) 

	ID3D11RasterizerState *m_pd3dRasterizerState;


	/*���� ������ ���� ����, ������ ����Ʈ ��, ���� �����Ͱ� ���� ������ ��𿡼����� �����ϴ� ���� ��Ÿ���� ������ �����Ѵ�.*/
	//UINT m_nVertices;
	//UINT m_nStride;
	//UINT m_nOffset;


public:
	CMesh(ID3D11Device *pd3dDevice);
	virtual ~CMesh();

	void AddRef() { m_nReferences++; }
	void Release() { if (--m_nReferences <= 0) delete this; }
	AABB GetBoundingCube() { return(m_bcBoundingCube); }

	// �޽��� ���� ���۵��� �迭�� ���� 
	void AssembleToVertexBuffer(int nBuffers = 0, ID3D11Buffer **m_pd3dBuffers = NULL, UINT *pnBufferStrides = NULL, UINT *pnBufferOffsets = NULL);

	virtual void CreateRasterizerState(ID3D11Device *pd3dDevice);
	virtual void Render(ID3D11DeviceContext *pd3dDeviceContext);
	virtual void RenderInstanced(ID3D11DeviceContext *pd3dDeviceContext, int nInstances = 0, int nStartInstance = 0);
	ID3D11Buffer* CreateBuffer(ID3D11Device *pd3dDevice, UINT nStride, int nElements, void *pBufferData, UINT nBindFlags, D3D11_USAGE d3dUsage, UINT nCPUAccessFlags);
	
	// �� ������ ��ġ ���͸� ��ŷ�� ���Ͽ� �����Ѵ�(���� ���۸� DYNAMIC���� �����ϰ� Map()�� ���� �ʾƵ� �ǵ���).
	D3DXVECTOR3 *m_pd3dxvPositions;
	UINT *m_pnIndices;

	// 160424 FBXMesh���� ����� �Լ�
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
	// ������ ������ ����ϱ� ���� ��������
	ID3D11Buffer *m_pd3dNormalBuffer;

public:
	// ������ ���Ե� �ﰢ���� �������͸� ���
	D3DXVECTOR3 CalculateTriAngleNormal(UINT nIndex0, UINT nIndex1, UINT nIndex2);
	void SetTriAngleListVertexNormal(D3DXVECTOR3 *pd3dxvNormals);
	// ������ ���������� ����� ���
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
//	//������ ũ��(����: x-����, ����: z-����)�̴�.
//	int m_nWidth;
//	int m_nLength;
//	/*������ ������(����: x-����, ����: z-����, ����: y-����) �����̴�. ���� ���� �޽��� �� ������ x-��ǥ, y-��ǥ, z-��ǥ�� ������ ������ x-��ǥ, y-��ǥ, z-��ǥ�� ���� ���� ���´�. ��, ���� ������ x-�� ������ ������ 1�� �ƴ϶� ������ ������ x-��ǥ�� �ȴ�. �̷��� �ϸ� ���� ���ڸ� ����ϴ��� ū ���ڸ� ������ �� �ִ�.*/
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
//	//������ ����(����)�� ���̸� �����Ѵ�.
//	virtual float OnSetHeight(int x, int z, void *pContext);
//	//������ ����(����)�� ������ �����Ѵ�.
//	virtual D3DXCOLOR OnSetColor(int x, int z, void *pContext);
//
//	virtual void SetRasterizerState(ID3D11Device *pd3dDevice);
//	virtual void Render(ID3D11DeviceContext *pd3dImmediateDeviceContext);
//};
