#pragma once
#include "Mesh.h"
#include "Camera.h"

struct MATERIAL
{
	D3DXCOLOR m_d3dxcAmbient;
	D3DXCOLOR m_d3dxcDiffuse;
	D3DXCOLOR m_d3dxcSpecular; //(r,g,b,a=power)
	D3DXCOLOR m_d3dxcEmissive;
};


class CMaterial
{
public:
	CMaterial();
	virtual ~CMaterial();

private:
	int m_nReferences;

public:
	void AddRef() { m_nReferences++; }
	void Release() { if (--m_nReferences <= 0) delete this; }

	MATERIAL m_Material;
};


class CTexture
{
public:
	CTexture(int nTextures = 1, int nSamplers = 1, int nTextureStartSlot = 0, int nSamplerStartSlot = 0);
	virtual ~CTexture();

private:
	int m_nReferences;

public:
	void AddRef() { m_nReferences++; }
	void Release() { if (--m_nReferences <= 0) delete this; }

private:
	// 텍스쳐 리소스의 개수
	int m_nTextures;
	ID3D11ShaderResourceView **m_ppd3dsrvTextures;
	// 텍스쳐 리소스를 연결할 시작 슬롯
	int m_nTextureStartSlot;
	// 샘플러 상태 객체의 개수
	int m_nSamplers;
	ID3D11SamplerState **m_ppd3dSamplerStates;
	// 샘플러 상태 객체를 연결할 시작 슬롯
	int m_nSamplerStartSlot;

public:
	void SetTexture(int nIndex, ID3D11ShaderResourceView *pd3dsrvTexture);
	void SetSampler(int nIndex, ID3D11SamplerState *pd3dSamplerState);
	// 텍스쳐 리소스와 샘플러 상태 객체에 대한 쉐이더 변수를 변경
	void UpdateShaderVariable(ID3D11DeviceContext *pd3dDeviceContext);
	// 텍스쳐 리소스에 대한 쉐이더 변수를 변경
	void UpdateTextureShaderVariable(ID3D11DeviceContext *pd3dDeviceContext, int nIndex = 0, int nSlot = 0);
	// 샘플러 상태 객체에 대한 쉐이더 변수를 변경
	void UpdateSamplerShaderVariable(ID3D11DeviceContext *pd3dDeviceContext, int nIndex = 0, int nSlot = 0);
};


class CGameObject
{
private:
	int m_nReferences;

protected:
	float m_fRotationSpeed;
	bool m_bActive;

public:
	CGameObject();
	CGameObject(int nMeshes);
	virtual ~CGameObject();

	void AddRef();
	void Release();

	D3DXMATRIX m_d3dxmtxWorld;
	D3DXMATRIX m_d3dxmtxRotate;
	D3DXMATRIX m_d3dxmtxScale;
	D3DXMATRIX m_d3dxmtxTranlate;

	D3DXVECTOR3 m_d3dxvDirection;

	float m_fRotationAngle;

	CMesh **m_ppMeshes;
	int m_nMeshes;
	CMaterial *m_pMaterial;
	CTexture *m_pTexture;

	// 객체가 가지는 메쉬 전체에 대한 바운딩 박스
	AABB m_bcMeshBoundingCube;
	// OOBB 충돌 체크
	BoundingOrientedBox		m_xmOOBB;
	BoundingOrientedBox		m_xmOOBBTransformed;
	CGameObject				*m_pCollider;
	void SetOOBB(XMFLOAT3& xmCenter, XMFLOAT3& xmExtents, XMFLOAT4& xmOrientation) { m_xmOOBBTransformed = m_xmOOBB = BoundingOrientedBox(xmCenter, xmExtents, xmOrientation); }



	void SetMaterial(CMaterial *pMaterial);
	void SetTexture(CTexture *pTexture);
	virtual void SetMesh(CMesh *pMesh, int nIndex = 0);
	virtual void Animate(float fTimeElapsed);
	virtual void Render(ID3D11DeviceContext *pd3dDeviceContext, CCamera *pCamera);

	virtual void SetPosition(float x, float y, float z);
	virtual void SetPosition(D3DXVECTOR3 d3dxvPosition);

	D3DXVECTOR3 GetPosition();

	void MoveRight(float fDistance = 1.0f);
	void MoveUp(float fDistance = 1.0f);
	void MoveForward(float fDistance = 1.0f);

	void Rotate(float fPitch = 10.0f, float fYaw = 10.0f, float fRoll = 10.0f);
	void Rotate(D3DXVECTOR3* pd3dAxis, float fAngle);

	D3DXVECTOR3 GetLookAt();
	D3DXVECTOR3 GetUp();
	D3DXVECTOR3 GetRight();
	CMesh *GetMesh(int nIndex = 0) { return(m_ppMeshes[nIndex]); }

	bool IsVisible(CCamera *pCamera = NULL);
	void SetActive(bool bActive = false) { m_bActive = bActive; }

	// 객체 렌더링 전에 호출할 함수
	virtual void OnPrepareRender() {}

	//
	CMesh *CubeMesh(float fSize, DWORD dwColor, int Type);
};

///////////// Terrain
//
//class CHeightMap
//{
//private:
//	//높이 맵 이미지 픽셀(8-비트)들의 이차원 배열이다. 각 픽셀은 0~255의 값을 갖는다.
//	BYTE *m_pHeightMapImage;
//	//높이 맵 이미지의 가로와 세로 크기이다.
//	int m_nWidth;
//	int m_nLength;
//	//높이 맵을 실제로 몇 배 확대하여 사용할 것인가를 나타내는 스케일 벡터이다.
//	D3DXVECTOR3 m_d3dxvScale;
//
//public:
//	CHeightMap(LPCTSTR pFileName, int nWidth, int nLength, D3DXVECTOR3 d3dxvScale);
//	~CHeightMap(void);
//
//	//높이 맵 이미지에서 (x, z) 위치의 픽셀 값에 기반한 지형의 높이를 반환한다.
//	float GetHeight(float x, float z, bool bReverseQuad = false);
//	//높이 맵 이미지에서 (x, z) 위치의 법선 벡터를 반환한다.
//	D3DXVECTOR3 GetHeightMapNormal(int x, int z);
//	BYTE *GetHeightMapImage() { return(m_pHeightMapImage); }
//	D3DXVECTOR3 GetScale() { return(m_d3dxvScale); }
//	int GetHeightMapWidth() { return(m_nWidth); }
//	int GetHeightMapLength() { return(m_nLength); }
//};
//
//class CHeightMapTerrain 
//{
//public:
//	CHeightMapTerrain(ID3D11Device *pd3dDevice, LPCTSTR pFileName, int nWidth, int nLength, int nBlockWidth, int nBlockLength, D3DXVECTOR3 d3dxvScale, D3DXCOLOR d3dxColor);
//	~CHeightMapTerrain();
//
//	//지형의 높이 맵으로 사용할 이미지이다.
//	CHeightMap *m_pHeightMap;
//
//	//지형의 가로와 세로 크기이다.
//	int m_nWidth;
//	int m_nLength;
//
//	//지형을 실제로 몇 배 확대할 것인가를 나타내는 스케일 벡터이다.
//	D3DXVECTOR3 m_d3dxvScale;
//	//지형 전체가 어떤 위치와 방향을 갖는 가를 나타내는 월드 변환 행렬이다.
//	D3DXMATRIX m_d3dxmtxWorld;
//
//	//지형을 구성하는 격자 메쉬들의 배열과 메쉬들의 개수이다.
//	CHeightMapGridMesh **m_ppMesh;
//	int m_nMeshes;
//
//	void Render(ID3D11DeviceContext *pd3dImmediateDeviceContext, CCamera *pCamera = NULL);
//};