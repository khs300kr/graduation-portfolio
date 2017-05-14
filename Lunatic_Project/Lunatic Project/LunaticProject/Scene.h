#pragma once
#include "Camera.h"
#include "Shader.h"
#include "SkyBoxShader.h"

#include "HeroManager.h"

#define MAX_LIGHTS		4 
#define POINT_LIGHT		1.0f
#define SPOT_LIGHT		2.0f
#define DIRECTIONAL_LIGHT	3.0f

// 1개의 조명을 표현하는 구조체이다. 
struct LIGHT
{
	D3DXCOLOR m_d3dxcAmbient;
	D3DXCOLOR m_d3dxcDiffuse;
	D3DXCOLOR m_d3dxcSpecular;
	D3DXVECTOR3 m_d3dxvPosition;
	float m_fRange;
	D3DXVECTOR3 m_d3dxvDirection;
	float m_nType;
	D3DXVECTOR3 m_d3dxvAttenuation;
	float m_fFalloff;
	float m_fTheta; //cos(m_fTheta)
	float m_fPhi; //cos(m_fPhi)
	float m_bEnable;
	float padding;
};

// 상수 버퍼는 크기가 반드시 16 바이트의 배수가 되어야 한다. 
struct LIGHTS
{
	LIGHT m_pLights[MAX_LIGHTS];
	D3DXCOLOR m_d3dxcGlobalAmbient;
	D3DXVECTOR4 m_d3dxvCameraPosition;
};

class CScene
{
private:
	int m_nShaders;
	int m_nInstancingShaders;

	
	CInstancingShader **m_ppInstancingShaders;

	CCamera *m_pCamera;

	LIGHTS *m_pLights;
	ID3D11Buffer *m_pd3dcbLights;

	bool LeftKeyDown;
	bool RightKeyDown;
	bool UpKeyDown;
	bool DownKeyDown;

	int KeyDownForServer;
	DWORD dwDirection;



public:
	CScene();
	~CScene();

	//CHeroManager* pMyObject;

	//CHeroManager* pOtherObject[MAX_USER];

	CHeroManager* pHeroObject[MAX_USER];
	
	CMaterial *pNormalMaterial;

	CTexture *pHealerTexture;
	CTexture *pSordManTexture;
	CTexture *pBabarianTexture;
	CTexture *pTestTexture;

	CMesh *pSordManMeshA;
	CMesh *pSordManMeshB;
	CMesh *pHealerMeshA;
	CMesh *pHealerMeshB;
	CMesh *pBabarianMeshA;
	CMesh *pBabarianMeshB;

	CMesh *pTestMesh;

	CShader **m_ppShaders;

	void SetHero();
	void ChangeMesh_Texture();
	
	bool OnProcessingMouseMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam);
	bool OnProcessingKeyboardMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam);

	void BuildObjects(ID3D11Device *pd3dDevice);
	void ReleaseObjects();


	void ProcessInput();
	void AnimateObjects(float fTimeElapsed);
	void Render(ID3D11DeviceContext*pd3dDeviceContext, CCamera *pCamera);

	void SetCamera(CCamera *pCamera) { m_pCamera = pCamera; }

	void CreateShaderVariables(ID3D11Device *pd3dDevice);
	void UpdateShaderVariable(ID3D11DeviceContext *pd3dDeviceContext, LIGHTS *pLights);
	void ReleaseShaderVariables();

public:

	
	// server
	DWORD Animation_number[MAX_USER] = {};
	void SendMovePacket(BYTE type);

};