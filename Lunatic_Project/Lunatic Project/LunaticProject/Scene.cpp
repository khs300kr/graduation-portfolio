#include "stdafx.h"
#include "Scene.h"
#include "CubeMesh.h"
#include "CubeObject.h"
#include "FBXMesh.h"
#include "CharacterShader.h"


CScene::CScene()
{
	m_nShaders = 0;
	m_nInstancingShaders = 0;

	m_ppShaders = NULL;
	m_ppInstancingShaders = NULL;

	m_pCamera = NULL;
	m_pLights = NULL;
	m_pd3dcbLights = NULL;


	LeftKeyDown = false;
	RightKeyDown = false;
	UpKeyDown = false;
	DownKeyDown = false;

	KeyDownForServer = 0;
	DWORD dwDirection = 0;

	pMyObject = new CHeroManager(1);
	pMyObject->m_Team = A_TEAM;
	pMyObject->SetPosition(0.0f, -3000.f, 0.0f);

	for(int i = 0; i < MAX_USER; ++i)
	{
		pOtherObject[i] = new CHeroManager(1);
		
	}



}


CScene::~CScene()
{
}

void CScene::BuildObjects(ID3D11Device *pd3dDevice)
{

	// �� �ؽ��� ���ο� ����� ���÷� ���� ��ü�� ����
	ID3D11SamplerState *pd3dSamplerState = NULL;
	D3D11_SAMPLER_DESC d3dSamplerDesc;
	ZeroMemory(&d3dSamplerDesc, sizeof(D3D11_SAMPLER_DESC));
	d3dSamplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	d3dSamplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	d3dSamplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	d3dSamplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	d3dSamplerDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
	d3dSamplerDesc.MinLOD = 0;
	d3dSamplerDesc.MaxLOD = 0;
	pd3dDevice->CreateSamplerState(&d3dSamplerDesc, &pd3dSamplerState);

	// �� �ؽ��� ���ҽ��� ����
	ID3D11ShaderResourceView *pd3dsrvTexture = NULL;

	pHealerTexture = new CTexture(1, 1, 0, 0);
	D3DX11CreateShaderResourceViewFromFile(pd3dDevice, _T("../Data/Healer.png"), NULL, NULL, &pd3dsrvTexture, NULL);
	pHealerTexture->SetTexture(0, pd3dsrvTexture);
	pHealerTexture->SetSampler(0, pd3dSamplerState);
	pd3dsrvTexture->Release();
	pd3dsrvTexture = NULL;

	pSordManTexture = new CTexture(1, 1, 0, 0);
	D3DX11CreateShaderResourceViewFromFile(pd3dDevice, _T("../Data/SordMan.png"), NULL, NULL, &pd3dsrvTexture, NULL);
	pSordManTexture->SetTexture(0, pd3dsrvTexture);
	pSordManTexture->SetSampler(0, pd3dSamplerState);
	pd3dsrvTexture->Release();

	pBabarianTexture = new CTexture(1, 1, 0, 0);
	D3DX11CreateShaderResourceViewFromFile(pd3dDevice, _T("../Data/Babarian.png"), NULL, NULL, &pd3dsrvTexture, NULL);
	pBabarianTexture->SetTexture(0, pd3dsrvTexture);
	pBabarianTexture->SetSampler(0, pd3dSamplerState);
	pd3dsrvTexture->Release();

	//�ٴ�
	pd3dsrvTexture = NULL;
	CTexture *pPlaneTexture = new CTexture(1, 1, 0, 0);
	D3DX11CreateShaderResourceViewFromFile(pd3dDevice, _T("../Data/plane.png"), NULL, NULL, &pd3dsrvTexture, NULL);
	pPlaneTexture->SetTexture(0, pd3dsrvTexture);
	pPlaneTexture->SetSampler(0, pd3dSamplerState);
	pd3dsrvTexture->Release();

	// �׽�Ʈ����1
	pd3dsrvTexture = NULL;
	CTexture *pBuilding1Texture = new CTexture(1, 1, 0, 0);
	D3DX11CreateShaderResourceViewFromFile(pd3dDevice, _T("../Data/building/building1/building1.png"), NULL, NULL, &pd3dsrvTexture, NULL);
	pBuilding1Texture->SetTexture(0, pd3dsrvTexture);
	pBuilding1Texture->SetSampler(0, pd3dSamplerState);
	pd3dsrvTexture->Release();
	// ����2
	pd3dsrvTexture = NULL;
	CTexture *pBuilding2Texture = new CTexture(1, 1, 0, 0);
	D3DX11CreateShaderResourceViewFromFile(pd3dDevice, _T("../Data/building/building2/building2.png"), NULL, NULL, &pd3dsrvTexture, NULL);
	pBuilding2Texture->SetTexture(0, pd3dsrvTexture);
	pBuilding2Texture->SetSampler(0, pd3dSamplerState);
	pd3dsrvTexture->Release();
	// �Ͽ콺1
	pd3dsrvTexture = NULL;
	CTexture *pHouse1Texture = new CTexture(1, 1, 0, 0);
	D3DX11CreateShaderResourceViewFromFile(pd3dDevice, _T("../Data/building/house1/house1.png"), NULL, NULL, &pd3dsrvTexture, NULL);
	pHouse1Texture->SetTexture(0, pd3dsrvTexture);
	pHouse1Texture->SetSampler(0, pd3dSamplerState);
	pd3dsrvTexture->Release();
	//�Ͽ콺2
	pd3dsrvTexture = NULL;
	CTexture *pHouse2Texture = new CTexture(1, 1, 0, 0);
	D3DX11CreateShaderResourceViewFromFile(pd3dDevice, _T("../Data/building/house2/house2.png"), NULL, NULL, &pd3dsrvTexture, NULL);
	pHouse2Texture->SetTexture(0, pd3dsrvTexture);
	pHouse2Texture->SetSampler(0, pd3dSamplerState);
	pd3dsrvTexture->Release();
	//��ƼȦ
	pd3dsrvTexture = NULL;
	CTexture *pCityhallTexture = new CTexture(1, 1, 0, 0);
	D3DX11CreateShaderResourceViewFromFile(pd3dDevice, _T("../Data/building/cityhall/cityhall.png"), NULL, NULL, &pd3dsrvTexture, NULL);
	pCityhallTexture->SetTexture(0, pd3dsrvTexture);
	pCityhallTexture->SetSampler(0, pd3dSamplerState);
	pd3dsrvTexture->Release();
	

	// �� Object�� Material�� Shader�� ���� (Skybox�� ���̴� ���ο��� ��ü������ ����)
	pNormalMaterial = new CMaterial();
	pNormalMaterial->m_Material.m_d3dxcDiffuse = D3DXCOLOR(0.8f, 0.8f, 0.8f, 1.0f);
	pNormalMaterial->m_Material.m_d3dxcAmbient = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pNormalMaterial->m_Material.m_d3dxcSpecular = D3DXCOLOR(1.0f, 1.0f, 1.0f, 5.0f);
	pNormalMaterial->m_Material.m_d3dxcEmissive = D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f);

	// �� ���̴��� ������ �޽�(��) ����	
	pHealerMeshA = new CFBXMesh(pd3dDevice, "../Data/Healer.data", 0.1f);
	pHealerMeshB = new CFBXMesh(pd3dDevice, "../Data/Healer.data", 0.1f);
	pSordManMeshA = new CFBXMesh(pd3dDevice, "../Data/SordMan.data", 0.1f);
	pSordManMeshB = new CFBXMesh(pd3dDevice, "../Data/SordMan.data", 0.1f);
	pBabarianMeshA = new CFBXMesh(pd3dDevice, "../Data/Babarian.data", 0.1f);
	pBabarianMeshB = new CFBXMesh(pd3dDevice, "../Data/Babarian.data", 0.1f);

	// map objects
	CMesh *pPlaneMesh = new CFBXMesh(pd3dDevice, "../Data/plane1.data", 1.f);

	CMesh *pBuilding1Mesh = new CFBXMesh(pd3dDevice, "../Data/building/building1/building1.data", 0.6f);
	CMesh *pBuilding2Mesh = new CFBXMesh(pd3dDevice, "../Data/building/building2/building2.data", 1.f);
	CMesh *pHouse1Mesh = new CFBXMesh(pd3dDevice, "../Data/building/house1/house1.data", 0.7f);
	CMesh *pHouse2Mesh = new CFBXMesh(pd3dDevice, "../Data/building/house2/house2.data", 0.7f);
	CMesh *pCityhallMesh = new CFBXMesh(pd3dDevice, "../Data/building/cityhall/cityhall.data", 1.f);

	
	// �Ϲ� ���̴� �����
	/////////////////////////////////////////////////////////////////////////

	m_nShaders = 10;   // Skybox����
	m_ppShaders = new CShader*[m_nShaders];

	// �� SkyBox�� Shader�� ����
	m_ppShaders[0] = new CSkyBoxShader();
	m_ppShaders[0]->CreateShader(pd3dDevice);
	m_ppShaders[0]->BuildObjects(pd3dDevice);


	{		

		// My
		m_ppShaders[1] = new CCharacterShader(1);
		m_ppShaders[1]->CreateShader(pd3dDevice);
		m_ppShaders[1]->BuildObjects(pd3dDevice);

		//pMyObject = new CHeroManager(1);
		//pMyObject->m_HeroSelect = Babarian;

		if (pMyObject->m_Team == A_TEAM)
		{
			if (pMyObject->m_HeroSelect == SordMan)
			{
				pMyObject->SetMesh(pSordManMeshA);
				pMyObject->SetTexture(pSordManTexture);

			}
			else if (pMyObject->m_HeroSelect == Healer)
			{
				pMyObject->SetMesh(pHealerMeshA);
				pMyObject->SetTexture(pHealerTexture);

			}
			else if (pMyObject->m_HeroSelect == Babarian)
			{
				pMyObject->SetMesh(pBabarianMeshA);
				pMyObject->SetTexture(pBabarianTexture);
			}
		}

		/*else if (pMyObject->m_Team == B_TEAM)
		{
			if (pMyObject->m_HeroSelect == SordMan)
			{
				pMyObject->SetMesh(pSordManMeshB);
				pMyObject->SetTexture(pSordManTexture);

			}
			else if (pMyObject->m_HeroSelect == Healer)
			{
				pMyObject->SetMesh(pHealerMeshB);
				pMyObject->SetTexture(pHealerTexture);

			}
			else if (pMyObject->m_HeroSelect == Babarian)
			{
				pMyObject->SetMesh(pBabarianMeshB);
				pMyObject->SetTexture(pBabarianTexture);
			}
		}*/
		pMyObject->SetMaterial(pNormalMaterial);
		//pMyObject->SetPosition(0.0f, -3000.0f, 0.0f);
		//pMyObject->Rotate(0.0f, 0.0f, 0.0f);

		m_ppShaders[1]->AddObject(pMyObject);
		

		for (int i = 0; i <= 7; ++i)
		{
			m_ppShaders[i + 2] = new CCharacterShader(1);
			m_ppShaders[i + 2]->CreateShader(pd3dDevice);
			m_ppShaders[i + 2]->BuildObjects(pd3dDevice);

		}

		for (int i = 0; i < MAX_USER; ++i)
		{
			pOtherObject[i]->SetMesh(pSordManMeshB);
			pOtherObject[i]->SetTexture(pSordManTexture);
			pOtherObject[i]->SetMaterial(pNormalMaterial);
			pOtherObject[i]->SetPosition(0.0f, -3000.0f, 0.0f);
			//pOtherObject[i]->Rotate(0.0f, 0.0f, 0.0f);

			m_ppShaders[i + 2]->AddObject(pOtherObject[i]);
		}


	
   }
   //m_pTerrain = new CHeightMapTerrain(pd3dDevice, _T("Data\\HeightMap.raw"), 257, 257, 17, 17, d3dxvScale, d3dxColor);
   //m_pTerrain->SetMaterial(pNormalMaterial);

	CreateShaderVariables(pd3dDevice);
}

void CScene::SetHero()
{
	for (int i = 0; i < MAX_USER; ++i)
	{
		if (i != g_myid)
		{
			if (pOtherObject[i]->m_Team == A_TEAM)
			{
				if (pOtherObject[i]->m_HeroSelect == Babarian && pOtherObject[i]->GetMesh() != pBabarianMeshA)
				{
					pOtherObject[i]->SetMesh(pBabarianMeshA);
					pOtherObject[i]->SetTexture(pBabarianTexture);

				}
				else if (pOtherObject[i]->m_HeroSelect == Healer && pOtherObject[i]->GetMesh() != pHealerMeshA)
				{
					pOtherObject[i]->SetMesh(pHealerMeshA);
					pOtherObject[i]->SetTexture(pHealerTexture);
				}
				else if (pOtherObject[i]->m_HeroSelect == SordMan && pOtherObject[i]->GetMesh() != pSordManMeshA)
				{
					pOtherObject[i]->SetMesh(pSordManMeshA);
					pOtherObject[i]->SetTexture(pSordManTexture);
				}
				else
					continue;

			}
		}

	}
}

void CScene::ChangeMesh_Texture()
{

}

void CScene::ReleaseObjects()
{
	ReleaseShaderVariables();

	for (int j = 0; j < m_nShaders; j++)
	{
		if (m_ppShaders[j]) m_ppShaders[j]->ReleaseObjects();
		if (m_ppShaders[j]) delete m_ppShaders[j];
	}
	if (m_ppShaders) delete[] m_ppShaders;

	for (int j = 0; j < m_nInstancingShaders; j++)
	{
		if (m_ppInstancingShaders[j]) m_ppInstancingShaders[j]->ReleaseObjects();
		if (m_ppInstancingShaders[j]) delete m_ppInstancingShaders[j];
	}
	if (m_ppInstancingShaders) delete[] m_ppInstancingShaders;
}




bool CScene::OnProcessingMouseMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam)
{
	return false;
}


bool CScene::OnProcessingKeyboardMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam)
{
	switch (nMessageID)
	{
	case WM_KEYDOWN:

		switch (wParam)
		{
		case 'A':
			pMyObject->SetMesh(pBabarianMeshA, 0);
			pMyObject->SetTexture(pBabarianTexture);
			break;
		case 'B':
			pMyObject->SetMesh(pHealerMeshA, 0);
			pMyObject->SetTexture(pHealerTexture);
			break;
		case 'C':
			pMyObject->SetMesh(pSordManMeshA, 0);
			pMyObject->SetTexture(pSordManTexture);
			break;
		case 'Z':
			if (!bHeroAttack)
			{
				
				/*m_ppShaders[2]->GetFBXMesh->SetAnimation(1);
				bHeroRun = true;
				bHeroPunch = false;*/
			}
			break;
		case 'X':
			//if (!bHeroAttack)
			//{
			//	m_ppShaders[1]->GetFBXMesh->SetAnimation(ANI_ATTACK);
			//	
			//	bHeroRun = false;
			//	bHeroAttack = true;
			//}
		default:
			break;
		}
		break;	// Client KeyDown


///////////////////////////////////////////////////////////////////////		


	case WM_KEYUP:

		switch (wParam)
		{
		case 'Z':
			break;
		case 'X':
			//if (bHeroAttack)
			//{
			//	m_ppShaders[1]->GetFBXMesh->SetAnimation(ANI_IDLE);
			//	bHeroAttack = false;
			//}
		default:
			break;
		}
		break;
	default:
		break;
	}
	return false;
}


void CScene::ProcessInput()
{
	if (activate)
	{
		if (KEY_DOWN(VK_UP) && !UpKeyDown) {
			UpKeyDown = true;
			dwDirection |= DIR_BACK;

			if (dwDirection) SendMovePacket(CS_KEYDOWN_UP);
		}
		if (KEY_DOWN(VK_DOWN) && !DownKeyDown) {
			DownKeyDown = true;
			dwDirection |= DIR_FRONT;
			if (dwDirection) SendMovePacket(CS_KEYDOWN_DOWN);
		}
		if (KEY_DOWN(VK_LEFT) && !LeftKeyDown) {
			LeftKeyDown = true;
			dwDirection |= DIR_LEFT;
			if (dwDirection) SendMovePacket(CS_KEYDOWN_LEFT);
		}
		if (KEY_DOWN(VK_RIGHT) && !RightKeyDown) {
			RightKeyDown = true;
			dwDirection |= DIR_RIGHT;
			if (dwDirection) SendMovePacket(CS_KEYDOWN_RIGHT);
		}


		if (KEY_UP(VK_UP) && UpKeyDown)
		{
			UpKeyDown = false;
			if (bHeroRun && !bHeroAttack)
			{
				m_ppShaders[1]->GetFBXMesh->SetAnimation(ANI_IDLE);
				bHeroRun = false;
			}
			SendMovePacket(CS_KEYUP_UP);
		}
		if (KEY_UP(VK_DOWN) && DownKeyDown)
		{
			DownKeyDown = false;
			if (bHeroRun && !bHeroAttack)
			{
				m_ppShaders[1]->GetFBXMesh->SetAnimation(ANI_IDLE);
				bHeroRun = false;
			}
			SendMovePacket(CS_KEYUP_DOWN);
		}
		if (KEY_UP(VK_LEFT) && LeftKeyDown)
		{
			LeftKeyDown = false;
			if (bHeroRun && !bHeroAttack)
			{
				m_ppShaders[1]->GetFBXMesh->SetAnimation(ANI_IDLE);
				bHeroRun = false;
			}
			SendMovePacket(CS_KEYUP_LEFT);
		}
		if (KEY_UP(VK_RIGHT) && RightKeyDown)
		{
			RightKeyDown = false;
			if (bHeroRun && !bHeroAttack)
			{
				m_ppShaders[1]->GetFBXMesh->SetAnimation(ANI_IDLE);
				bHeroRun = false;
			}
			SendMovePacket(CS_KEYUP_RIGHT);
		}


		if (KEY_DOWN('S'))
		{
			if (!bHeroAttack)
			{
				m_ppShaders[1]->GetFBXMesh->SetAnimation(ANI_ATTACK);
				pMyObject->SetSpeed(0);
				bHeroAttack = true;
				bHeroRun = false;
			}
		}
		
	}
	


}


void CScene::CreateShaderVariables(ID3D11Device *pd3dDevice)
{
	m_pLights = new LIGHTS;
	ZeroMemory(m_pLights, sizeof(LIGHTS));
	// ���� ���� ��ü�� ���ߴ� �ֺ������� �����Ѵ�.
	m_pLights->m_d3dxcGlobalAmbient = D3DXCOLOR(0.1f, 0.1f, 0.1f, 1.0f);

	// 3���� ����(�� ����, ���� ����, ���⼺ ����)�� �����Ѵ�.

	m_pLights->m_pLights[0].m_bEnable = 1.0f;
	m_pLights->m_pLights[0].m_nType = POINT_LIGHT;
	m_pLights->m_pLights[0].m_fRange = 300.0f;
	m_pLights->m_pLights[0].m_d3dxcAmbient = D3DXCOLOR(0.1f, 0.0f, 0.0f, 1.0f);
	m_pLights->m_pLights[0].m_d3dxcDiffuse = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
	m_pLights->m_pLights[0].m_d3dxcSpecular = D3DXCOLOR(0.1f, 0.1f, 0.1f, 0.0f);
	m_pLights->m_pLights[0].m_d3dxvPosition = D3DXVECTOR3(300.0f, 300.0f, 300.0f);
	m_pLights->m_pLights[0].m_d3dxvDirection = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_pLights->m_pLights[0].m_d3dxvAttenuation = D3DXVECTOR3(1.0f, 0.001f, 0.0001f);
	m_pLights->m_pLights[1].m_bEnable = 1.0f;
	m_pLights->m_pLights[1].m_nType = SPOT_LIGHT;
	m_pLights->m_pLights[1].m_fRange = 100.0f;
	m_pLights->m_pLights[1].m_d3dxcAmbient = D3DXCOLOR(0.1f, 0.1f, 0.1f, 1.0f);
	m_pLights->m_pLights[1].m_d3dxcDiffuse = D3DXCOLOR(0.3f, 0.3f, 0.3f, 1.0f);
	m_pLights->m_pLights[1].m_d3dxcSpecular = D3DXCOLOR(0.1f, 0.1f, 0.1f, 0.0f);
	m_pLights->m_pLights[1].m_d3dxvPosition = D3DXVECTOR3(500.0f, 300.0f, 500.0f);
	m_pLights->m_pLights[1].m_d3dxvDirection = D3DXVECTOR3(0.0f, 0.0f, 1.0f);
	m_pLights->m_pLights[1].m_d3dxvAttenuation = D3DXVECTOR3(1.0f, 0.01f, 0.0001f);
	m_pLights->m_pLights[1].m_fFalloff = 8.0f;
	m_pLights->m_pLights[1].m_fPhi = (float)cos(D3DXToRadian(40.0f));
	m_pLights->m_pLights[1].m_fTheta = (float)cos(D3DXToRadian(20.0f));
	m_pLights->m_pLights[2].m_bEnable = 1.0f;
	m_pLights->m_pLights[2].m_nType = DIRECTIONAL_LIGHT;
	m_pLights->m_pLights[2].m_d3dxcAmbient = D3DXCOLOR(0.1f, 0.1f, 0.1f, 1.0f);
	m_pLights->m_pLights[2].m_d3dxcDiffuse = D3DXCOLOR(0.4f, 0.4f, 0.4f, 1.0f);
	m_pLights->m_pLights[2].m_d3dxcSpecular = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);
	m_pLights->m_pLights[2].m_d3dxvDirection = D3DXVECTOR3(0.0f, -1.0f, 0.0f);
	m_pLights->m_pLights[3].m_bEnable = 1.0f;
	m_pLights->m_pLights[3].m_nType = SPOT_LIGHT;
	m_pLights->m_pLights[3].m_fRange = 60.0f;
	m_pLights->m_pLights[3].m_d3dxcAmbient = D3DXCOLOR(0.1f, 0.1f, 0.1f, 1.0f);
	m_pLights->m_pLights[3].m_d3dxcDiffuse = D3DXCOLOR(0.2f, 0.2f, 0.2f, 1.0f);
	m_pLights->m_pLights[3].m_d3dxcSpecular = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);
	m_pLights->m_pLights[3].m_d3dxvPosition = D3DXVECTOR3(500.0f, 300.0f, 500.0f);
	m_pLights->m_pLights[3].m_d3dxvDirection = D3DXVECTOR3(0.0f, -1.0f, 0.0f);
	m_pLights->m_pLights[3].m_d3dxvAttenuation = D3DXVECTOR3(1.0f, 0.01f, 0.001f);
	m_pLights->m_pLights[3].m_fFalloff = 20.0f;
	m_pLights->m_pLights[3].m_fPhi = (float)cos(D3DXToRadian(40.0f));
	m_pLights->m_pLights[3].m_fTheta = (float)cos(D3DXToRadian(15.0f));

	D3D11_BUFFER_DESC d3dBufferDesc;
	ZeroMemory(&d3dBufferDesc, sizeof(d3dBufferDesc));
	d3dBufferDesc.Usage = D3D11_USAGE_DYNAMIC; 
	d3dBufferDesc.ByteWidth = sizeof(LIGHTS);
	d3dBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	d3dBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	D3D11_SUBRESOURCE_DATA d3dBufferData;
	ZeroMemory(&d3dBufferData, sizeof(D3D11_SUBRESOURCE_DATA));
	d3dBufferData.pSysMem = m_pLights;
	pd3dDevice->CreateBuffer(&d3dBufferDesc, &d3dBufferData, &m_pd3dcbLights);
}

void CScene::ReleaseShaderVariables()
{
	if (m_pLights) delete m_pLights;
	if (m_pd3dcbLights) m_pd3dcbLights->Release();
}

void CScene::SendMovePacket(BYTE type)
{
	cs_packet_pos *my_packet = reinterpret_cast<cs_packet_pos *>(send_buffer);
	my_packet->size = sizeof(cs_packet_pos);
	send_wsabuf.len = sizeof(cs_packet_pos);
	DWORD iobyte;
	my_packet->type = type;
	my_packet->x = pMyObject->GetPosition().x;
	my_packet->y = pMyObject->GetPosition().y;
	my_packet->z = pMyObject->GetPosition().z;

	
	WSASend(g_mysocket, &send_wsabuf, 1, &iobyte, 0, NULL, NULL);
}

void CScene::UpdateShaderVariable(ID3D11DeviceContext *pd3dDeviceContext, LIGHTS *pLights)
{
	D3D11_MAPPED_SUBRESOURCE d3dMappedResource;
	pd3dDeviceContext->Map(m_pd3dcbLights, 0, D3D11_MAP_WRITE_DISCARD, 0, &d3dMappedResource);
	LIGHTS *pcbLight = (LIGHTS *)d3dMappedResource.pData;
	memcpy(pcbLight, pLights, sizeof(LIGHTS));
	pd3dDeviceContext->Unmap(m_pd3dcbLights, 0);
	pd3dDeviceContext->PSSetConstantBuffers(PS_SLOT_LIGHT, 1, &m_pd3dcbLights);
}

void CScene::AnimateObjects(float fTimeElapsed)
{
	CPlayer *pPlayer = m_pCamera->GetPlayer();

	if (m_pLights && m_pd3dcbLights)
	{
		// �� �ӽ��ּ�
		// ���� ī�޶��� ��ġ ���͸� ������ ��Ÿ���� ��� ���ۿ� �����Ѵ�.
		D3DXVECTOR3 d3dxvCameraPosition = m_pCamera->GetPosition();
		m_pLights->m_d3dxvCameraPosition = D3DXVECTOR4(d3dxvCameraPosition, 1.0f);

		//// �� ������ ������ �߾��� �߽����� ȸ���ϵ��� �����Ѵ�.
		//CHeightMapTerrain *pTerrain = GetTerrain();
		//static D3DXVECTOR3 d3dxvRotated = D3DXVECTOR3(pTerrain->GetWidth()*0.3f, 0.0f, 0.0f);
		//D3DXMATRIX d3dxmtxRotate;
		//D3DXMatrixRotationYawPitchRoll(&d3dxmtxRotate, (float)D3DXToRadian(30.0f*fTimeElapsed), 0.0f, 0.0f);
		//D3DXVec3TransformCoord(&d3dxvRotated, &d3dxvRotated, &d3dxmtxRotate);
		//D3DXVECTOR3 d3dxvTerrainCenter = D3DXVECTOR3(pTerrain->GetWidth()*0.5f, pTerrain->GetPeakHeight() + 10.0f, pTerrain->GetLength()*0.5f);
		//m_pLights->m_pLights[0].m_d3dxvPosition = d3dxvTerrainCenter + d3dxvRotated;
		//m_pLights->m_pLights[0].m_fRange = pTerrain->GetPeakHeight();

		/*�� ��° ������ �÷��̾ ������ �ִ� ������(���� ����)�̴�. �׷��Ƿ� �÷��̾��� ��ġ�� ������ �ٲ�� ���� �÷��̾��� ��ġ�� z-�� ���� ���͸� ���� ������ ��ġ�� �������� �����Ѵ�.*/
		
		m_pLights->m_pLights[1].m_d3dxvPosition = pPlayer->GetPosition();
		m_pLights->m_pLights[1].m_d3dxvDirection = pPlayer->GetLookVector();
		

		m_pLights->m_pLights[3].m_d3dxvPosition = pPlayer->GetPosition() + D3DXVECTOR3(0.0f, 40.0f, 0.0f);
	}
	for (int i = 0; i < m_nShaders; i++) m_ppShaders[i]->AnimateObjects(fTimeElapsed);
	for (int i = 0; i < m_nInstancingShaders; i++) m_ppInstancingShaders[i]->AnimateObjects(fTimeElapsed);

	//cout << m_ppShaders[2]->GetFBXMesh->GetFBXAnimationNum() << " " << m_ppShaders[2]->GetFBXMesh->GetFBXMaxFrameNum() << endl;

	
	if (bHeroAttack)
	{
		if (m_ppShaders[1]->GetFBXMesh->GetFBXNowFrameNum() == m_ppShaders[1]->GetFBXMesh->GetFBXMaxFrameNum() - 1)
		{
			if (bHeroRun || LeftKeyDown || RightKeyDown || UpKeyDown || DownKeyDown)
			{
				m_ppShaders[1]->GetFBXMesh->SetAnimation(ANI_RUN);
			}
			else
			{
				m_ppShaders[1]->GetFBXMesh->SetAnimation(ANI_IDLE);
			}

			pMyObject->SetSpeed(pMyObject->GetNormalSpeed());
			bHeroAttack = false;
		}

	}

	
	//for (int i = 0; i < MAX_USER; ++i)
	//{
	m_ppShaders[1]->GetFBXMesh->FBXFrameAdvance(fTimeElapsed);
	m_ppShaders[2]->GetFBXMesh->FBXFrameAdvance(fTimeElapsed);
	m_ppShaders[3]->GetFBXMesh->FBXFrameAdvance(fTimeElapsed);
	m_ppShaders[4]->GetFBXMesh->FBXFrameAdvance(fTimeElapsed);
		//m_ppShaders[2]->GetFBXMesh->FBXFrameAdvance(fTimeElapsed);
		//m_ppShaders[3]->GetFBXMesh->FBXFrameAdvance(fTimeElapsed);
	//}
	
	//cout << m_ppShaders[1]->GetFBXMesh->GetFBXNowFrameNum() << ends << m_ppShaders[1]->GetFBXMesh->GetFBXMaxFrameNum() << endl;
	


}

void CScene::Render(ID3D11DeviceContext*pd3dDeviceContext, CCamera *pCamera)
{
	if (m_pLights && m_pd3dcbLights) UpdateShaderVariable(pd3dDeviceContext, m_pLights);
	
	for (int i = 0; i < m_nShaders; i++)
	{
		if (i == 1 || i == 2) // i��° ������ ��� ��������.
			m_ppShaders[i]->GetFBXMesh->UpdateBoneTransform(pd3dDeviceContext, m_ppShaders[i]->GetFBXMesh->GetFBXAnimationNum(), m_ppShaders[i]->GetFBXMesh->GetFBXNowFrameNum());
		m_ppShaders[i]->Render(pd3dDeviceContext, pCamera);
	}
	for (int i = 0; i < m_nInstancingShaders; i++) m_ppInstancingShaders[i]->Render(pd3dDeviceContext, pCamera);
}