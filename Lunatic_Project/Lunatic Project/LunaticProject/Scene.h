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

// 1���� ������ ǥ���ϴ� ����ü�̴�. 
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

// ��� ���۴� ũ�Ⱑ �ݵ�� 16 ����Ʈ�� ����� �Ǿ�� �Ѵ�. 
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

	// ���ھ� UI
	CTexture *pScoreNum[10];
	CUIObject *pAteam[2][10]; // 0: 1�� �ڸ� 1: 10�� �ڸ�
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

	// A,B�� �÷��̾� ��ܿ� ���̱�
	// �̹��� �׵θ�
	CUIObject *pTeamObject[MAX_GAMER];

	// A,B��
	CUIObject *GamerImage[MAX_GAMER];

	// hp gauge
	CUIObject *pHpgaugeObject;

	// ��� �÷��̾� hp
	CUIObject *PlayerHpObject[MAX_GAMER];

	// ��ų UI
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
	CUIManager *m_pSkillUIManager[4];  // ���, ���� up & down

	CUIManager *m_pScoreManager[41]; // scoreboard 1 + number(0~9) 10 x 2 (A,B�� ���ھ� 2�� �ʿ�) + 1���ڸ� 10���ڸ� ���� A,B�� ���� 10�� �߰� = 41

	CUIManager *m_TeamGamerManager[MAX_GAMER];
	CUIManager *m_TeamHPManager[MAX_GAMER];
	CUIManager *pTeamEdge[8]; // �̹��� �׵θ�

							  // Score
	void AteamScore(int _score);
	void BteamScore(int _score);
};