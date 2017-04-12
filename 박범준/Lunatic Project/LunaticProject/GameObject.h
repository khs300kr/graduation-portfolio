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
	CGameObject(int nMeshes);
	virtual ~CGameObject();

	void AddRef();
	void Release();

	D3DXMATRIX m_d3dxmtxWorld;
	D3DXMATRIX m_d3dxmtxRotate;
	D3DXMATRIX m_d3dxmtxScale;
	D3DXMATRIX m_d3dxmtxTranlate;

	float m_fRotationAngle;

	CMesh **m_ppMeshes;
	int m_nMeshes;
	CMaterial *m_pMaterial;
	CTexture *m_pTexture;

	// 객체가 가지는 메쉬 전체에 대한 바운딩 박스
	AABB m_bcMeshBoundingCube;

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
};