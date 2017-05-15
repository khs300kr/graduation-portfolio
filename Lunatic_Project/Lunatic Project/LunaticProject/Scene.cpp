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

	for(int i = 0; i < MAX_USER; ++i)
		pHeroObject[i] = NULL;
	for (int i = 0; i < 13; ++i)
		pHouse1Object[i] = NULL;

	pNormalMaterial = NULL;
	pHealerTexture = NULL;
	pSordManTexture = NULL;
	pBabarianTexture = NULL;
	pTestTexture = NULL;
	
	pSordManMeshA = NULL;
	pSordManMeshB = NULL;
	pHealerMeshA = NULL;
	pHealerMeshB = NULL;
	pBabarianMeshA = NULL;
	pBabarianMeshB = NULL;
	pTestMesh = NULL;




	//pHeroObject[g_myid] = new CHeroManager(1);
	//pHeroObject[g_myid]->m_Team = A_TEAM;
	//pHeroObject[g_myid]->SetPosition(0.0f, -3000.f, 0.0f);

	for(int i = 0; i < MAX_USER; ++i)
	{
		pHeroObject[i] = new CHeroManager(1);
		pHeroObject[i]->SetPosition(0.0f, -3000.0f, 0.0f);
	}


	ColBox = false;
}


CScene::~CScene()
{
}

void CScene::BuildObjects(ID3D11Device *pd3dDevice)
{

	// ① 텍스쳐 맵핑에 사용할 샘플러 상태 객체를 생성
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

	// ② 텍스쳐 리소스를 생성
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

	//바닥
	pd3dsrvTexture = NULL;
	CTexture *pPlaneTexture = new CTexture(1, 1, 0, 0);
	D3DX11CreateShaderResourceViewFromFile(pd3dDevice, _T("../Data/plane.png"), NULL, NULL, &pd3dsrvTexture, NULL);
	pPlaneTexture->SetTexture(0, pd3dsrvTexture);
	pPlaneTexture->SetSampler(0, pd3dSamplerState);
	pd3dsrvTexture->Release();

	// 테스트빌딩1
	pd3dsrvTexture = NULL;
	CTexture *pBuilding1Texture = new CTexture(1, 1, 0, 0);
	D3DX11CreateShaderResourceViewFromFile(pd3dDevice, _T("../Data/building/building1/building1.png"), NULL, NULL, &pd3dsrvTexture, NULL);
	pBuilding1Texture->SetTexture(0, pd3dsrvTexture);
	pBuilding1Texture->SetSampler(0, pd3dSamplerState);
	pd3dsrvTexture->Release();
	// 빌딩2
	pd3dsrvTexture = NULL;
	CTexture *pBuilding2Texture = new CTexture(1, 1, 0, 0);
	D3DX11CreateShaderResourceViewFromFile(pd3dDevice, _T("../Data/building/building2/building2.png"), NULL, NULL, &pd3dsrvTexture, NULL);
	pBuilding2Texture->SetTexture(0, pd3dsrvTexture);
	pBuilding2Texture->SetSampler(0, pd3dSamplerState);
	pd3dsrvTexture->Release();
	// 하우스1
	pd3dsrvTexture = NULL;
	CTexture *pHouse1Texture = new CTexture(1, 1, 0, 0);
	D3DX11CreateShaderResourceViewFromFile(pd3dDevice, _T("../Data/building/house1/house1.png"), NULL, NULL, &pd3dsrvTexture, NULL);
	pHouse1Texture->SetTexture(0, pd3dsrvTexture);
	pHouse1Texture->SetSampler(0, pd3dSamplerState);
	pd3dsrvTexture->Release();
	//하우스2
	pd3dsrvTexture = NULL;
	CTexture *pHouse2Texture = new CTexture(1, 1, 0, 0);
	D3DX11CreateShaderResourceViewFromFile(pd3dDevice, _T("../Data/building/house2/house2.png"), NULL, NULL, &pd3dsrvTexture, NULL);
	pHouse2Texture->SetTexture(0, pd3dsrvTexture);
	pHouse2Texture->SetSampler(0, pd3dSamplerState);
	pd3dsrvTexture->Release();

	//시티홀
	pd3dsrvTexture = NULL;
	CTexture *pCityhallTexture = new CTexture(1, 1, 0, 0);
	D3DX11CreateShaderResourceViewFromFile(pd3dDevice, _T("../Data/building/cityhall/cityhall.png"), NULL, NULL, &pd3dsrvTexture, NULL);
	pCityhallTexture->SetTexture(0, pd3dsrvTexture);
	pCityhallTexture->SetSampler(0, pd3dSamplerState);
	pd3dsrvTexture->Release();

	//테스트
	pd3dsrvTexture = NULL;
	pTestTexture = new CTexture(1, 1, 0, 0);
	D3DX11CreateShaderResourceViewFromFile(pd3dDevice, _T("../Data/testbox.png"), NULL, NULL, &pd3dsrvTexture, NULL);
	pCityhallTexture->SetTexture(0, pd3dsrvTexture);
	pCityhallTexture->SetSampler(0, pd3dSamplerState);
	pd3dsrvTexture->Release();
	

	// ③ Object용 Material과 Shader를 생성 (Skybox는 쉐이더 내부에서 자체적으로 생성)
	pNormalMaterial = new CMaterial();
	pNormalMaterial->m_Material.m_d3dxcDiffuse = D3DXCOLOR(0.8f, 0.8f, 0.8f, 1.0f);
	pNormalMaterial->m_Material.m_d3dxcAmbient = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pNormalMaterial->m_Material.m_d3dxcSpecular = D3DXCOLOR(1.0f, 1.0f, 1.0f, 5.0f);
	pNormalMaterial->m_Material.m_d3dxcEmissive = D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f);

	// ④ 쉐이더에 적용할 메쉬(들) 생성	
	pHealerMeshA = new CFBXMesh(pd3dDevice, "../Data/Healer.data", 0.1f);
	pHealerMeshB = new CFBXMesh(pd3dDevice, "../Data/Healer.data", 0.1f);
	pSordManMeshA = new CFBXMesh(pd3dDevice, "../Data/SordMan.data", 0.1f);
	pSordManMeshB = new CFBXMesh(pd3dDevice, "../Data/SordMan.data", 0.1f);
	pBabarianMeshA = new CFBXMesh(pd3dDevice, "../Data/Babarian.data", 0.1f);
	pBabarianMeshB = new CFBXMesh(pd3dDevice, "../Data/Babarian.data", 0.1f);
	pTestMesh = new CFBXMesh(pd3dDevice, "../Data/testbox.data", 0.1f);

	// map objects
	CMesh *pPlaneMesh = new CFBXMesh(pd3dDevice, "../Data/plane1.data", 1.f);

	CMesh *pBuilding1Mesh = new CFBXMesh(pd3dDevice, "../Data/building/building1/building1.data", 0.6f);
	CMesh *pBuilding2Mesh = new CFBXMesh(pd3dDevice, "../Data/building/building2/building2.data", 1.f);
	CMesh *pHouse1Mesh = new CFBXMesh(pd3dDevice, "../Data/building/house1/house1.data", 1.0f);
	CMesh *pHouse2Mesh = new CFBXMesh(pd3dDevice, "../Data/building/house2/house2.data", 0.7f);
	CMesh *pCityhallMesh = new CFBXMesh(pd3dDevice, "../Data/building/cityhall/cityhall.data", 1.f);

	
	// 일반 쉐이더 선언부
	/////////////////////////////////////////////////////////////////////////

	m_nShaders = 25;   // Skybox포함
	m_ppShaders = new CShader*[m_nShaders];

	// ⑤ SkyBox용 Shader를 생성
	m_ppShaders[0] = new CSkyBoxShader();
	m_ppShaders[0]->CreateShader(pd3dDevice);
	m_ppShaders[0]->BuildObjects(pd3dDevice);


	{		
		

		for (int i = 1; i <= 8; ++i)
		{
			m_ppShaders[i] = new CCharacterShader(1);
			m_ppShaders[i]->CreateShader(pd3dDevice);
			m_ppShaders[i]->BuildObjects(pd3dDevice);

		}

		pHeroObject[g_myid]->SetOOBB(XMFLOAT3(pHeroObject[g_myid]->GetPosition().x, pHeroObject[g_myid]->GetPosition().y, pHeroObject[g_myid]->GetPosition().z),
			XMFLOAT3(0.1f, 0.1f, 0.1f), XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f));

		for (int i = 0; i < MAX_USER; ++i)
		{
			if (pHeroObject[i]->m_Team == A_TEAM)
			{
				if (pHeroObject[i]->m_HeroSelect == SordMan)
				{
					pHeroObject[i]->SetMesh(pSordManMeshA);
					pHeroObject[i]->SetTexture(pSordManTexture);

				}
				else if (pHeroObject[i]->m_HeroSelect == Healer)
				{
					pHeroObject[i]->SetMesh(pHealerMeshA);
					pHeroObject[i]->SetTexture(pHealerTexture);

				}
				else if (pHeroObject[i]->m_HeroSelect == Babarian)
				{
					pHeroObject[i]->SetMesh(pBabarianMeshA);
					pHeroObject[i]->SetTexture(pBabarianTexture);
				}
			}
			else
			{
				pHeroObject[i]->SetMesh(pTestMesh);
				pHeroObject[i]->SetTexture(pTestTexture);
			}
			pHeroObject[i]->SetMaterial(pNormalMaterial);

			m_ppShaders[i + 1]->AddObject(pHeroObject[i]);
		}


		m_ppShaders[9] = new CTexturedIlluminatedShader(1);
		m_ppShaders[9]->CreateShader(pd3dDevice);
		m_ppShaders[9]->BuildObjects(pd3dDevice);

		CGameObject *pPlaneObject = new CGameObject(1);
		pPlaneObject->SetMesh(pPlaneMesh);
		pPlaneObject->SetMaterial(pNormalMaterial);
		pPlaneObject->SetTexture(pPlaneTexture);
		pPlaneObject->Rotate(0.0f, 0.0f, 0.0f);
		pPlaneObject->SetPosition(0.0f, 0.0f, 0.0f);
		m_ppShaders[9]->AddObject(pPlaneObject);

		//building2 (리스폰) (적군)
		m_ppShaders[10] = new CTexturedIlluminatedShader(1);
		m_ppShaders[10]->CreateShader(pd3dDevice);
		m_ppShaders[10]->BuildObjects(pd3dDevice);

		CGameObject *pBuilding2Object = new CGameObject(1);
		pBuilding2Object->SetMesh(pBuilding2Mesh);
		pBuilding2Object->SetMaterial(pNormalMaterial);
		pBuilding2Object->SetTexture(pBuilding2Texture);
		pBuilding2Object->Rotate(0.0f, 0.0f, 0.0f);
		pBuilding2Object->SetPosition(0.0f, 0.0f, 500.0f);
		m_ppShaders[10]->AddObject(pBuilding2Object);

		//building2 (리스폰) (아군)
		m_ppShaders[11] = new CTexturedIlluminatedShader(1);
		m_ppShaders[11]->CreateShader(pd3dDevice);
		m_ppShaders[11]->BuildObjects(pd3dDevice);

		CGameObject *pBuilding2Object2 = new CGameObject(1);
		pBuilding2Object2->SetMesh(pBuilding2Mesh);
		pBuilding2Object2->SetMaterial(pNormalMaterial);
		pBuilding2Object2->SetTexture(pBuilding2Texture);
		pBuilding2Object2->Rotate(0.0f, 180.0f, 0.0f);
		pBuilding2Object2->SetPosition(0.0f, -10.0f, -500.0f);
		m_ppShaders[11]->AddObject(pBuilding2Object2);


		// 건물 쉐이더 생성
		for (int i = 12; i < m_nShaders; ++i)
		{
			m_ppShaders[i] = new CTexturedIlluminatedShader(1);
			m_ppShaders[i]->CreateShader(pd3dDevice);
			m_ppShaders[i]->BuildObjects(pd3dDevice);

		}

		// 건물 오브젝트 생성
		//CGameObject *pHouse1Object[14]; 
		for (int i = 0; i < 13; ++i)
		{
			pHouse1Object[i] = new CGameObject(1);
			pHouse1Object[i]->SetMesh(pHouse1Mesh);
			pHouse1Object[i]->SetMaterial(pNormalMaterial);
			pHouse1Object[i]->SetTexture(pHouse1Texture);
			pHouse1Object[i]->Rotate(0.0f, 0.0f, 0.0f);
			//pHouse1Object[i]->SetOOBB(XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(100.f, 100.f, 100.f), XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f));
		}
		// 건물 위치 설정
		pHouse1Object[0]->SetPosition(50.0f, 0.0f, -300.0f);
		pHouse1Object[0]->SetOOBB(XMFLOAT3(-50.0f, 0.0f, -300.0f), XMFLOAT3(90.f, 100.f, 50.f), XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f));


		pHouse1Object[1]->SetPosition(250.0f, 0.0f, -300.0f);
		pHouse1Object[2]->SetPosition(-150.0f, 0.0f, -300.0f);

		pHouse1Object[3]->SetPosition(150.0f, 0.0f, -150.0f);
		pHouse1Object[4]->SetPosition(-50.0f, 0.0f, -150.0f);

		pHouse1Object[5]->SetPosition(50.0f, 0.0f, 0.0f);
		pHouse1Object[6]->SetPosition(250.0f, 0.0f, 0.0f);
		pHouse1Object[7]->SetPosition(-150.0f, 0.0f, 0.0f);

		pHouse1Object[8]->SetPosition(150.0f, 0.0f, 150.0f);
		pHouse1Object[9]->SetPosition(-50.0f, 0.0f, 150.0f);

		pHouse1Object[10]->SetPosition(50.0f, 0.0f, 300.0f);
		pHouse1Object[11]->SetPosition(250.0f, 0.0f, 300.0f);
		pHouse1Object[12]->SetPosition(-150.0f, 0.0f, 300.0f);


		// 쉐이더에 저장
		for (int i = 12; i < m_nShaders; ++i)
		{
			m_ppShaders[i]->AddObject(pHouse1Object[i - 12]);
		}
	
   }
   //m_pTerrain = new CHeightMapTerrain(pd3dDevice, _T("Data\\HeightMap.raw"), 257, 257, 17, 17, d3dxvScale, d3dxColor);
   //m_pTerrain->SetMaterial(pNormalMaterial);

	CreateShaderVariables(pd3dDevice);
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
			cout << "my " << g_myid << ends<< pHeroObject[g_myid]->m_Team << endl;
			for (int i = 0; i < MAX_USER; ++i)
			{
				cout << i << ends << pHeroObject[i]->GetPosition().y << endl;
			}
			break;

		case 'Z':
			if (!pHeroObject[g_myid]->bHeroAttack)
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
		if (!pHeroObject[g_myid]->bHeroAttack)
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
		}



		if (KEY_UP(VK_UP) && UpKeyDown)
		{
			UpKeyDown = false;
			if (pHeroObject[g_myid]->bHeroRun)
			{
				m_ppShaders[g_myid+1]->GetFBXMesh->SetAnimation(ANI_IDLE);
				pHeroObject[g_myid]->bHeroRun = false;
			}
			SendMovePacket(CS_KEYUP_UP);
		}
		if (KEY_UP(VK_DOWN) && DownKeyDown)
		{
			DownKeyDown = false;
			if (pHeroObject[g_myid]->bHeroRun)
			{
				m_ppShaders[g_myid+1]->GetFBXMesh->SetAnimation(ANI_IDLE);
				pHeroObject[g_myid]->bHeroRun = false;
			}
			SendMovePacket(CS_KEYUP_DOWN);
		}
		if (KEY_UP(VK_LEFT) && LeftKeyDown)
		{
			LeftKeyDown = false;
			if (pHeroObject[g_myid]->bHeroRun)
			{
				m_ppShaders[g_myid+1]->GetFBXMesh->SetAnimation(ANI_IDLE);
				pHeroObject[g_myid]->bHeroRun = false;
			}
			SendMovePacket(CS_KEYUP_LEFT);
		}
		if (KEY_UP(VK_RIGHT) && RightKeyDown)
		{
			RightKeyDown = false;
			if (pHeroObject[g_myid]->bHeroRun)
			{
				m_ppShaders[g_myid+1]->GetFBXMesh->SetAnimation(ANI_IDLE);
				pHeroObject[g_myid]->bHeroRun = false;
			}
			SendMovePacket(CS_KEYUP_RIGHT);
		}


		if (KEY_DOWN('D'))
		{
			if (!pHeroObject[g_myid]->bHeroAttack)
			{
				m_ppShaders[g_myid+1]->GetFBXMesh->SetAnimation(ANI_ATTACK);
	
				pHeroObject[g_myid]->bHeroAttack = true;
				pHeroObject[g_myid]->bHeroRun = false;

				cs_packet_attack *my_packet = reinterpret_cast<cs_packet_attack *>(send_buffer);
				my_packet->size = sizeof(cs_packet_attack);
				send_wsabuf.len = sizeof(cs_packet_attack);
				DWORD iobyte;
				my_packet->type = CS_ATTACK;

				WSASend(g_mysocket, &send_wsabuf, 1, &iobyte, 0, NULL, NULL);
			}

			
		}

		else if (KEY_DOWN('Q'))
		{
			if (!pHeroObject[g_myid]->bHeroQ)
			{
				m_ppShaders[g_myid+1]->GetFBXMesh->SetAnimation(ANI_Q);

				pHeroObject[g_myid]->bHeroQ = true;
				pHeroObject[g_myid]->bHeroRun = false;

				cs_packet_skillQ *my_packet = reinterpret_cast<cs_packet_skillQ *>(send_buffer);
				my_packet->size = sizeof(cs_packet_skillQ);
				send_wsabuf.len = sizeof(cs_packet_skillQ);
				DWORD iobyte;
				my_packet->type = CS_SKILL_Q;

				WSASend(g_mysocket, &send_wsabuf, 1, &iobyte, 0, NULL, NULL);
			}
		}
		else if (KEY_DOWN('W'))
		{
			if (!pHeroObject[g_myid]->bHeroW)
			{
				m_ppShaders[g_myid+1]->GetFBXMesh->SetAnimation(ANI_W);
				
				pHeroObject[g_myid]->bHeroW = true;
				pHeroObject[g_myid]->bHeroRun = false;

				cs_packet_skillW *my_packet = reinterpret_cast<cs_packet_skillW *>(send_buffer);
				my_packet->size = sizeof(cs_packet_skillW);
				send_wsabuf.len = sizeof(cs_packet_skillW);
				DWORD iobyte;
				my_packet->type = CS_SKILL_W;

				WSASend(g_mysocket, &send_wsabuf, 1, &iobyte, 0, NULL, NULL);
			}
		}
		else if (KEY_DOWN('E'))
		{
			if (!pHeroObject[g_myid]->bHeroE)
			{
				m_ppShaders[g_myid+1]->GetFBXMesh->SetAnimation(ANI_E);

				pHeroObject[g_myid]->bHeroE = true;
				pHeroObject[g_myid]->bHeroRun = false;

				cs_packet_skillE *my_packet = reinterpret_cast<cs_packet_skillE *>(send_buffer);
				my_packet->size = sizeof(cs_packet_skillE);
				send_wsabuf.len = sizeof(cs_packet_skillE);
				DWORD iobyte;
				my_packet->type = CS_SKILL_E;

				WSASend(g_mysocket, &send_wsabuf, 1, &iobyte, 0, NULL, NULL);
			}
		}
		else if (KEY_DOWN('R'))
		{
			if (!pHeroObject[g_myid]->bHeroR)
			{
				m_ppShaders[g_myid+1]->GetFBXMesh->SetAnimation(ANI_R);

				pHeroObject[g_myid]->bHeroR = true;
				pHeroObject[g_myid]->bHeroRun = false;

				cs_packet_skillR *my_packet = reinterpret_cast<cs_packet_skillR *>(send_buffer);
				my_packet->size = sizeof(cs_packet_skillR);
				send_wsabuf.len = sizeof(cs_packet_skillR);
				DWORD iobyte;
				my_packet->type = CS_SKILL_R;

				WSASend(g_mysocket, &send_wsabuf, 1, &iobyte, 0, NULL, NULL);
			}
		}

		else if (KEY_DOWN('1'))
		{
			if (!pHeroObject[g_myid]->bHeroHit)
			{
				m_ppShaders[g_myid+1]->GetFBXMesh->SetAnimation(ANI_HIT);

				pHeroObject[g_myid]->bHeroHit = true;
				pHeroObject[g_myid]->bHeroRun = false;
			}
		}
		else if (KEY_DOWN('2'))
		{
			if (!pHeroObject[g_myid]->bHeroStun)
			{
				m_ppShaders[g_myid+1]->GetFBXMesh->SetAnimation(ANI_STUN);

				pHeroObject[g_myid]->bHeroStun = true;
				pHeroObject[g_myid]->bHeroRun = false;
			}
		}
		else if (KEY_DOWN('3'))
		{
			if (!pHeroObject[g_myid]->bHeroDie)
			{
				m_ppShaders[g_myid+1]->GetFBXMesh->SetAnimation(ANI_DIE);

				pHeroObject[g_myid]->bHeroDie = true;
				pHeroObject[g_myid]->bHeroRun = false;
			}
		}

		
	}
	


}


void CScene::CreateShaderVariables(ID3D11Device *pd3dDevice)
{
	m_pLights = new LIGHTS;
	ZeroMemory(m_pLights, sizeof(LIGHTS));
	// 게임 월드 전체를 비추는 주변조명을 설정한다.
	m_pLights->m_d3dxcGlobalAmbient = D3DXCOLOR(0.1f, 0.1f, 0.1f, 1.0f);

	// 3개의 조명(점 광원, 스팟 광원, 방향성 광원)을 설정한다.

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
	my_packet->x = pHeroObject[g_myid]->GetPosition().x;
	my_packet->y = pHeroObject[g_myid]->GetPosition().y;
	my_packet->z = pHeroObject[g_myid]->GetPosition().z;

	
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
		// ● 임시주석
		// 현재 카메라의 위치 벡터를 조명을 나타내는 상수 버퍼에 설정한다.
		D3DXVECTOR3 d3dxvCameraPosition = m_pCamera->GetPosition();
		m_pLights->m_d3dxvCameraPosition = D3DXVECTOR4(d3dxvCameraPosition, 1.0f);

		//// 점 조명이 지형의 중앙을 중심으로 회전하도록 설정한다.
		//CHeightMapTerrain *pTerrain = GetTerrain();
		//static D3DXVECTOR3 d3dxvRotated = D3DXVECTOR3(pTerrain->GetWidth()*0.3f, 0.0f, 0.0f);
		//D3DXMATRIX d3dxmtxRotate;
		//D3DXMatrixRotationYawPitchRoll(&d3dxmtxRotate, (float)D3DXToRadian(30.0f*fTimeElapsed), 0.0f, 0.0f);
		//D3DXVec3TransformCoord(&d3dxvRotated, &d3dxvRotated, &d3dxmtxRotate);
		//D3DXVECTOR3 d3dxvTerrainCenter = D3DXVECTOR3(pTerrain->GetWidth()*0.5f, pTerrain->GetPeakHeight() + 10.0f, pTerrain->GetLength()*0.5f);
		//m_pLights->m_pLights[0].m_d3dxvPosition = d3dxvTerrainCenter + d3dxvRotated;
		//m_pLights->m_pLights[0].m_fRange = pTerrain->GetPeakHeight();

		/*두 번째 조명은 플레이어가 가지고 있는 손전등(스팟 조명)이다. 그러므로 플레이어의 위치와 방향이 바뀌면 현재 플레이어의 위치와 z-축 방향 벡터를 스팟 조명의 위치와 방향으로 설정한다.*/
		
		m_pLights->m_pLights[1].m_d3dxvPosition = pPlayer->GetPosition();
		m_pLights->m_pLights[1].m_d3dxvDirection = pPlayer->GetLookVector();
		

		m_pLights->m_pLights[3].m_d3dxvPosition = pPlayer->GetPosition() + D3DXVECTOR3(0.0f, 40.0f, 0.0f);
	}
	for (int i = 0; i < m_nShaders; i++) m_ppShaders[i]->AnimateObjects(fTimeElapsed);
	for (int i = 0; i < m_nInstancingShaders; i++) m_ppInstancingShaders[i]->AnimateObjects(fTimeElapsed);

	//cout << m_ppShaders[2]->GetFBXMesh->GetFBXAnimationNum() << " " << m_ppShaders[2]->GetFBXMesh->GetFBXMaxFrameNum() << endl;

	
	if (pHeroObject[g_myid]->bHeroAttack || pHeroObject[g_myid]->bHeroQ || pHeroObject[g_myid]->bHeroW || pHeroObject[g_myid]->bHeroE || pHeroObject[g_myid]->bHeroR)
	{
		if (m_ppShaders[g_myid+1]->GetFBXMesh->GetFBXNowFrameNum() == m_ppShaders[g_myid+1]->GetFBXMesh->GetFBXMaxFrameNum() - 1)
		{
			if (pHeroObject[g_myid]->bHeroRun || LeftKeyDown || RightKeyDown || UpKeyDown || DownKeyDown)
			{
				m_ppShaders[g_myid+1]->GetFBXMesh->SetAnimation(ANI_RUN);
			}
			else
			{
				m_ppShaders[g_myid+1]->GetFBXMesh->SetAnimation(ANI_IDLE);
			}

			if(pHeroObject[g_myid]->bHeroAttack) pHeroObject[g_myid]->bHeroAttack = false;
			if (pHeroObject[g_myid]->bHeroQ) pHeroObject[g_myid]->bHeroQ = false;
			if (pHeroObject[g_myid]->bHeroW) pHeroObject[g_myid]->bHeroW = false;
			if (pHeroObject[g_myid]->bHeroE) pHeroObject[g_myid]->bHeroE = false;
			if (pHeroObject[g_myid]->bHeroR) pHeroObject[g_myid]->bHeroR = false;

		}
	}

	

	//for (int i = 0; i < MAX_USER; ++i)
	//{
	//	if (i != g_myid)
	//	{
	//		if (m_ppShaders[i + 1]->GetFBXMesh->GetFBXNowFrameNum() == m_ppShaders[i + 1]->GetFBXMesh->GetFBXMaxFrameNum() - 1)
	//		{
	//			m_ppShaders[i + 1]->GetFBXMesh->SetAnimation(ANI_IDLE);	
	//		}
	//	}

	//}


	//if ((m_ppShaders[i + 1]->GetFBXMesh->GetFBXNowFrameNum() == m_ppShaders[i + 1]->GetFBXMesh->GetFBXMaxFrameNum() - 1))
	//{
	//	cs_packet_attack *my_packet = reinterpret_cast<cs_packet_attack *>(send_buffer);
	//	my_packet->size = sizeof(cs_packet_attack);
	//	send_wsabuf.len = sizeof(cs_packet_attack);
	//	DWORD iobyte;
	//	my_packet->type = CS_ATTACK;

	//	WSASend(g_mysocket, &send_wsabuf, 1, &iobyte, 0, NULL, NULL);
	//}

	
	for (int i = 1; i < MAX_USER+1; ++i)
	{
		m_ppShaders[i]->GetFBXMesh->FBXFrameAdvance(fTimeElapsed);

	}


	
	pHeroObject[g_myid]->SetOOBB(XMFLOAT3(pHeroObject[g_myid]->GetPosition().x, pHeroObject[g_myid]->GetPosition().y, pHeroObject[g_myid]->GetPosition().z),
		XMFLOAT3(0.1f, 0.1f, 0.1f), XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f));

	if (pHouse1Object[0]->m_xmOOBB.Intersects(pHeroObject[g_myid]->m_xmOOBB))
	{
		pHouse1Object[0]->m_pCollider = pHeroObject[g_myid];
		pHeroObject[g_myid]->m_pCollider = pHouse1Object[0];

		//pHouse1Object[0]->SetColCheck(true);
		ColBox = true;
	}
	else
	{
		ColBox = false;
	}

}

void CScene::Render(ID3D11DeviceContext*pd3dDeviceContext, CCamera *pCamera)
{
	if (m_pLights && m_pd3dcbLights) UpdateShaderVariable(pd3dDeviceContext, m_pLights);
	
	for (int i = 0; i < m_nShaders; i++)
	{
		if (i == 1 || i == 2) // i번째 뼈대의 행렬 변경하자.
			m_ppShaders[i]->GetFBXMesh->UpdateBoneTransform(pd3dDeviceContext, m_ppShaders[i]->GetFBXMesh->GetFBXAnimationNum(), m_ppShaders[i]->GetFBXMesh->GetFBXNowFrameNum());
		m_ppShaders[i]->Render(pd3dDeviceContext, pCamera);
	}
	for (int i = 0; i < m_nInstancingShaders; i++) m_ppInstancingShaders[i]->Render(pd3dDeviceContext, pCamera);
}