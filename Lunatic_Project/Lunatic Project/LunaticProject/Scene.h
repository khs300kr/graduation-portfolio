#pragma once
#include "Camera.h"
#include "Shader.h"
#include "SkyBoxShader.h"
#include "ParticleSystem.h"

#include "HeroManager.h"
#include "UIObject.h"

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

	CParticleSystem *m_pParticleSystem;
	float m_fGametime;

	CInstancingShader **m_ppInstancingShaders;

	CCamera *m_pCamera;

	LIGHTS *m_pLights;
	ID3D11Buffer *m_pd3dcbLights;

	bool LeftKeyDown;
	bool RightKeyDown;
	bool UpKeyDown;
	bool DownKeyDown;

	bool DKeyDown;
	bool QKeyDown;
	bool WKeyDown;
	bool EKeyDown;
	bool RKeyDown;

	int KeyDownForServer;
	DWORD dwDirection;



public:
	CScene();
	~CScene();

	//CHeroManager* pMyObject;

	//CHeroManager* pOtherObject[MAX_GAMER];

	CHeroManager* pHeroObject[MAX_GAMER];
	CGameObject *pTeamViewObject[MAX_GAMER];
	CGameObject *pOtherHpbarObject[MAX_GAMER];


	CGameObject *pHouse1Object[13];
	CGameObject *pWallObject[64];

	int myGame_id = -1;
	int MyRoomNumber = -1;

	// 스코어 UI
	CTexture *pScoreNum[10];
	CUIObject *pAteam[2][10]; // 0: 1의 자리 1: 10의 자리
	CUIObject *pBteam[2][10];

	CMaterial *pNormalMaterial;

	CTexture *pHealerTexture;
	CTexture *pSwordmanTexture;
	CTexture *pBabarianTexture;


	CTexture *pArcherTexture;
	CTexture *pMagicianTexture;
	CTexture *pWitchTexture;
	CTexture *pKnightTexture;

	CTexture *pTestTexture;

	CTexture *pRedTeamTexture;
	CTexture *pBlueTeamTexture;

	CMesh *pSwordmanMeshA;
	CMesh *pSwordmanMeshB;
	CMesh *pHealerMeshA;
	CMesh *pHealerMeshB;
	CMesh *pBabarianMeshA;
	CMesh *pBabarianMeshB;

	CMesh *pKnightMeshA;
	CMesh *pKnightMeshB;
	CMesh *pMagicianMeshA;
	CMesh *pMagicianMeshB;
	CMesh *pArcherMeshA;
	CMesh *pArcherMeshB;

	CMesh *pWitchMeshA;
	CMesh *pWitchMeshB;

	CMesh *pTestMesh;

	CShader **m_ppShaders;

	bool OnProcessingMouseMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam);
	bool OnProcessingKeyboardMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam);

	void BuildObjects(ID3D11Device *pd3dDevice, int playercount);
	void ReleaseObjects();


	void ProcessInput();
	void AnimateObjects(float fTimeElapsed);
	void Render(ID3D11DeviceContext*pd3dDeviceContext, CCamera *pCamera);

	void SetCamera(CCamera *pCamera) { m_pCamera = pCamera; }

	void CreateShaderVariables(ID3D11Device *pd3dDevice);
	void UpdateShaderVariable(ID3D11DeviceContext *pd3dDeviceContext, LIGHTS *pLights);
	void ReleaseShaderVariables();

	//bool GetCol() { return ColBox; }

	bool Leftcollision(CHeroManager* Object1, CHeroManager* Object2, float sizeX1, float sizeZ1, float sizeX2, float sizeZ2);
	bool Rightcollision(CHeroManager* Object1, CHeroManager* Object2, float sizeX1, float sizeZ1, float sizeX2, float sizeZ2);
	bool Upcollision(CHeroManager* Object1, CHeroManager* Object2, float sizeX1, float sizeZ1, float sizeX2, float sizeZ2);
	bool Downcollision(CHeroManager* Object1, CHeroManager* Object2, float sizeX1, float sizeZ1, float sizeX2, float sizeZ2);

	bool Leftcollision(CHeroManager* Object1, CGameObject* Object2, float sizeX1, float sizeZ1, float sizeX2, float sizeZ2);
	bool Rightcollision(CHeroManager* Object1, CGameObject* Object2, float sizeX1, float sizeZ1, float sizeX2, float sizeZ2);
	bool Upcollision(CHeroManager* Object1, CGameObject* Object2, float sizeX1, float sizeZ1, float sizeX2, float sizeZ2);
	bool Downcollision(CHeroManager* Object1, CGameObject* Object2, float sizeX1, float sizeZ1, float sizeX2, float sizeZ2);



	bool Sectorcollision(CHeroManager * Object1, CHeroManager * Object2, DWORD dir, float sizeXZ, float range);


	CUIObject *gUIObject;

	// A,B팀 플레이어 상단에 보이기
	// 이미지 테두리
	CUIObject *pTeamObject[MAX_GAMER];

	// A,B팀
	CUIObject *GamerImage[MAX_GAMER];

	// hp gauge
	CUIObject *pHpgaugeObject;

	// 모든 플레이어 hp
	CUIObject *PlayerHpObject[MAX_GAMER];

	// 스킬 UI
	CUIObject *pAttackUpObject;
	CUIObject *pAttackDownObject;
	CUIObject *pSwingUpObject;
	CUIObject *pSwingDownObject;

public:


	// server
	DWORD Animation_number[MAX_GAMER] = {};
	void SendMovePacket(BYTE type);
	DWORD dwDirforCollision = DIR_FRONT;


	// UI
	CUIManager *m_pMyHpUIManager[2]; //hpbar 1 + bpgauge 1 = 2
	CUIManager *m_pSkillUIManager[4];  // 약공, 강공 up & down

	CUIManager *m_pScoreManager[41]; // scoreboard 1 + number(0~9) 10 x 2 (A,B팀 스코어 2개 필요) + 1의자리 10의자리 때매 A,B팀 각각 10개 추가 = 41

	CUIManager *m_TeamGamerManager[MAX_GAMER];
	CUIManager *m_TeamHPManager[MAX_GAMER];
	CUIManager *pTeamEdge[8]; // 이미지 테두리

							  // Score
	void AteamScore(int _score);
	void BteamScore(int _score);
};