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

	for(int i = 0; i < MAX_GAMER; ++i)
		pHeroObject[i] = NULL;

	for (int i = 0; i < 13; ++i)
		pHouse1Object[i] = NULL;

	pNormalMaterial = NULL;
	
	pHealerTexture = NULL;
	pSwordmanTexture = NULL;
	pBabarianTexture = NULL;
	pArcherTexture = NULL;
	pMagicianTexture = NULL;
	pKnightTexture = NULL;
	pWitchTexture = NULL;


	pTestTexture = NULL;
	
	pSwordmanMeshA = NULL;
	pSwordmanMeshB = NULL;
	pHealerMeshA = NULL;
	pHealerMeshB = NULL;
	pBabarianMeshA = NULL;
	pBabarianMeshB = NULL;

	pArcherMeshA = NULL;
	pArcherMeshB = NULL;
	pMagicianMeshA = NULL;
	pMagicianMeshB = NULL;
	pKnightMeshA = NULL;
	pKnightMeshB = NULL;
	pWitchMeshA = NULL;
	pWitchMeshB = NULL;


	pTestMesh = NULL;

	for(int i = 0; i < MAX_GAMER; ++i)
	{
		pHeroObject[i] = new CHeroManager(1);
		pHeroObject[i]->SetPosition(0.0f, -3000.0f, 0.0f);
	}


}


CScene::~CScene()
{
}

void CScene::BuildObjects(ID3D11Device *pd3dDevice, int playercount)
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
	//바닥
	pd3dsrvTexture = NULL;
	CTexture *pPlaneTexture = new CTexture(1, 1, 0, 0);
	D3DX11CreateShaderResourceViewFromFile(pd3dDevice, _T("../Data/texture/plane.png"), NULL, NULL, &pd3dsrvTexture, NULL);
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

	// 벽
	pd3dsrvTexture = NULL;
	CTexture *pWallTexture = new CTexture(1, 1, 0, 0);
	D3DX11CreateShaderResourceViewFromFile(pd3dDevice, _T("../Data/building/wall/Wall.png"), NULL, NULL, &pd3dsrvTexture, NULL);
	pWallTexture->SetTexture(0, pd3dsrvTexture);
	pWallTexture->SetSampler(0, pd3dSamplerState);
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



	pTestMesh = new CFBXMesh(pd3dDevice, "../Data/testbox.data", 0.1f);

	// map objects
	CMesh *pPlaneMesh = new CFBXMesh(pd3dDevice, "../Data/plane1.data", 1.f);

	CMesh *pBuilding1Mesh = new CFBXMesh(pd3dDevice, "../Data/building/building1/building1.data", 0.6f);
	CMesh *pBuilding2Mesh = new CFBXMesh(pd3dDevice, "../Data/building/building2/building2.data", 1.f);
	CMesh *pHouse1Mesh = new CFBXMesh(pd3dDevice, "../Data/building/house1/house1.data", 1.0f);
	CMesh *pHouse2Mesh = new CFBXMesh(pd3dDevice, "../Data/building/house2/house2.data", 0.7f);
	CMesh *pCityhallMesh = new CFBXMesh(pd3dDevice, "../Data/building/cityhall/cityhall.data", 1.f);
	CMesh *pWallMesh = new CFBXMesh(pd3dDevice, "../Data/building/wall/Wall.data", 5.f);

	// 일반 쉐이더 선언부
	/////////////////////////////////////////////////////////////////////////

	m_nShaders = 24 + 24 + 40; //24 + 24   // Skybox 포함 + 16 // playercount + 17  = 24 // + 벽 12 + 12 + 40
	m_ppShaders = new CShader*[m_nShaders];

	// ⑤ SkyBox용 Shader를 생성
	m_ppShaders[0] = new CSkyBoxShader();
	m_ppShaders[0]->CreateShader(pd3dDevice);
	m_ppShaders[0]->BuildObjects(pd3dDevice);


	{

		//pHeroObject[myroom_id]->SetOOBB(XMFLOAT3(pHeroObject[myroom_id]->GetPosition().x, pHeroObject[myroom_id]->GetPosition().y, pHeroObject[myroom_id]->GetPosition().z),
		//	XMFLOAT3(0.1f, 0.1f, 0.1f), XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f));


		for (int i = 0; i < MAX_GAMER; ++i)
		{
			m_ppShaders[i + 1] = new CCharacterShader(1);
			m_ppShaders[i + 1]->CreateShader(pd3dDevice);
			m_ppShaders[i + 1]->BuildObjects(pd3dDevice);

			if (pHeroObject[i]->m_HeroSelect == SWORDMAN)
			{
				pd3dsrvTexture = NULL;
				pSwordmanTexture = new CTexture(1, 1, 0, 0);
				D3DX11CreateShaderResourceViewFromFile(pd3dDevice, _T("../Data/texture/Swordman.png"), NULL, NULL, &pd3dsrvTexture, NULL);
				pSwordmanTexture->SetTexture(0, pd3dsrvTexture);
				pSwordmanTexture->SetSampler(0, pd3dSamplerState);
				pd3dsrvTexture->Release();

				if (pHeroObject[i]->m_Team == A_TEAM)
				{
					pSwordmanMeshA = new CFBXMesh(pd3dDevice, "../Data/data/SwordMan.data", 0.1f);
					pHeroObject[i]->SetMesh(pSwordmanMeshA);
					pHeroObject[i]->SetTexture(pSwordmanTexture);
				}

				else if (pHeroObject[i]->m_Team == B_TEAM)
				{
					pSwordmanMeshB = new CFBXMesh(pd3dDevice, "../Data/data/SwordMan.data", 0.1f);
					pHeroObject[i]->SetMesh(pSwordmanMeshB);
					pHeroObject[i]->SetTexture(pSwordmanTexture);
				}

			}

			else if (pHeroObject[i]->m_HeroSelect == HEALER)
			{
				pd3dsrvTexture = NULL;
				pHealerTexture = new CTexture(1, 1, 0, 0);
				D3DX11CreateShaderResourceViewFromFile(pd3dDevice, _T("../Data/texture/Healer.png"), NULL, NULL, &pd3dsrvTexture, NULL);
				pHealerTexture->SetTexture(0, pd3dsrvTexture);
				pHealerTexture->SetSampler(0, pd3dSamplerState);
				pd3dsrvTexture->Release();

				if (pHeroObject[i]->m_Team == A_TEAM)
				{
					pHealerMeshA = new CFBXMesh(pd3dDevice, "../Data/data/Healer.data", 0.1f);
					pHeroObject[i]->SetMesh(pHealerMeshA);
					pHeroObject[i]->SetTexture(pHealerTexture);
				}

				else if (pHeroObject[i]->m_Team == B_TEAM)
				{
					pHealerMeshB = new CFBXMesh(pd3dDevice, "../Data/data/Healer.data", 0.1f);
					pHeroObject[i]->SetMesh(pHealerMeshB);
					pHeroObject[i]->SetTexture(pHealerTexture);
				}
			}

			else if (pHeroObject[i]->m_HeroSelect == BABARIAN)
			{
				pd3dsrvTexture = NULL;
				pBabarianTexture = new CTexture(1, 1, 0, 0);
				D3DX11CreateShaderResourceViewFromFile(pd3dDevice, _T("../Data/texture/Babarian.png"), NULL, NULL, &pd3dsrvTexture, NULL);
				pBabarianTexture->SetTexture(0, pd3dsrvTexture);
				pBabarianTexture->SetSampler(0, pd3dSamplerState);
				pd3dsrvTexture->Release();

				if (pHeroObject[i]->m_Team == A_TEAM)
				{
					pBabarianMeshA = new CFBXMesh(pd3dDevice, "../Data/data/Babarian.data", 0.1f);
					pHeroObject[i]->SetMesh(pBabarianMeshA);
					pHeroObject[i]->SetTexture(pBabarianTexture);
				}

				else if (pHeroObject[i]->m_Team == B_TEAM)
				{
					pBabarianMeshB = new CFBXMesh(pd3dDevice, "../Data/data/Babarian.data", 0.1f);
					pHeroObject[i]->SetMesh(pBabarianMeshB);
					pHeroObject[i]->SetTexture(pBabarianTexture);
				}
			}

			else if (pHeroObject[i]->m_HeroSelect == KNIGHT)
			{
				pd3dsrvTexture = NULL;
				pKnightTexture = new CTexture(1, 1, 0, 0);
				D3DX11CreateShaderResourceViewFromFile(pd3dDevice, _T("../Data/texture/Knight.png"), NULL, NULL, &pd3dsrvTexture, NULL);
				pKnightTexture->SetTexture(0, pd3dsrvTexture);
				pKnightTexture->SetSampler(0, pd3dSamplerState);
				pd3dsrvTexture->Release();

				if (pHeroObject[i]->m_Team == A_TEAM)
				{
					pKnightMeshA = new CFBXMesh(pd3dDevice, "../Data/data/Knight.data", 0.1f);
					pHeroObject[i]->SetMesh(pKnightMeshA);
					pHeroObject[i]->SetTexture(pKnightTexture);
				}

				else if (pHeroObject[i]->m_Team == B_TEAM)
				{
					pKnightMeshB = new CFBXMesh(pd3dDevice, "../Data/data/Knight.data", 0.1f);
					pHeroObject[i]->SetMesh(pKnightMeshB);
					pHeroObject[i]->SetTexture(pKnightTexture);
				}
			}

			else if (pHeroObject[i]->m_HeroSelect == ARCHER)
			{
				pd3dsrvTexture = NULL;
				pArcherTexture = new CTexture(1, 1, 0, 0);
				D3DX11CreateShaderResourceViewFromFile(pd3dDevice, _T("../Data/texture/Archer.png"), NULL, NULL, &pd3dsrvTexture, NULL);
				pArcherTexture->SetTexture(0, pd3dsrvTexture);
				pArcherTexture->SetSampler(0, pd3dSamplerState);
				pd3dsrvTexture->Release();

				if (pHeroObject[i]->m_Team == A_TEAM)
				{
					pArcherMeshA = new CFBXMesh(pd3dDevice, "../Data/data/Archer.data", 0.1f);
					pHeroObject[i]->SetMesh(pArcherMeshA);
					pHeroObject[i]->SetTexture(pArcherTexture);
				}

				else if (pHeroObject[i]->m_Team == B_TEAM)
				{
					pArcherMeshB = new CFBXMesh(pd3dDevice, "../Data/data/Archer.data", 0.1f);
					pHeroObject[i]->SetMesh(pArcherMeshB);
					pHeroObject[i]->SetTexture(pArcherTexture);
				}
			}

			else if (pHeroObject[i]->m_HeroSelect == WITCH)
			{
				pd3dsrvTexture = NULL;
				pWitchTexture = new CTexture(1, 1, 0, 0);
				D3DX11CreateShaderResourceViewFromFile(pd3dDevice, _T("../Data/texture/Witch.png"), NULL, NULL, &pd3dsrvTexture, NULL);
				pWitchTexture->SetTexture(0, pd3dsrvTexture);
				pWitchTexture->SetSampler(0, pd3dSamplerState);
				pd3dsrvTexture->Release();

				if (pHeroObject[i]->m_Team == A_TEAM)
				{
					pWitchMeshA = new CFBXMesh(pd3dDevice, "../Data/data/Witch.data", 0.1f);
					pHeroObject[i]->SetMesh(pWitchMeshA);
					pHeroObject[i]->SetTexture(pWitchTexture);
				}

				else if (pHeroObject[i]->m_Team == B_TEAM)
				{
					pWitchMeshB = new CFBXMesh(pd3dDevice, "../Data/data/Witch.data", 0.1f);
					pHeroObject[i]->SetMesh(pWitchMeshB);
					pHeroObject[i]->SetTexture(pWitchTexture);
				}
			}

			else if (pHeroObject[i]->m_HeroSelect == MAGICIAN)
			{
				pd3dsrvTexture = NULL;
				pMagicianTexture = new CTexture(1, 1, 0, 0);
				D3DX11CreateShaderResourceViewFromFile(pd3dDevice, _T("../Data/texture/Magician.png"), NULL, NULL, &pd3dsrvTexture, NULL);
				pMagicianTexture->SetTexture(0, pd3dsrvTexture);
				pMagicianTexture->SetSampler(0, pd3dSamplerState);
				pd3dsrvTexture->Release();

				if (pHeroObject[i]->m_Team == A_TEAM)
				{
					pMagicianMeshA = new CFBXMesh(pd3dDevice, "../Data/data/Magician.data", 0.1f);
					pHeroObject[i]->SetMesh(pMagicianMeshA);
					pHeroObject[i]->SetTexture(pMagicianTexture);
				}

				else if (pHeroObject[i]->m_Team == B_TEAM)
				{
					pMagicianMeshB = new CFBXMesh(pd3dDevice, "../Data/data/Magician.data", 0.1f);
					pHeroObject[i]->SetMesh(pMagicianMeshB);
					pHeroObject[i]->SetTexture(pMagicianTexture);
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
			//pHouse1Object[i]->Rotate(0.0f, 0.0f, 0.0f);
			//pHouse1Object[i]->SetOOBB(XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(100.f, 100.f, 100.f), XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f));
		}
		// 건물 위치 설정


		//pHouse1Object[0]->SetPosition(50.0f, 0.0f, -300.0f);
		//pHouse1Object[0]->SetOOBB(XMFLOAT3(-50.0f, 0.0f, -300.0f), XMFLOAT3(90.f, 100.f, 50.f), XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f));


		pHouse1Object[1]->SetPosition(250.0f, 0.0f, -300.0f); // 바로 앞에 있는 건물 두개 왼쪽
		pHouse1Object[2]->SetPosition(-150.0f, 0.0f, -300.0f); // 바로 앞에 있는 건물 두개 오른쪽

		pHouse1Object[3]->SetPosition(150.0f, 0.0f, -150.0f);
		pHouse1Object[4]->SetPosition(-50.0f, 0.0f, -150.0f);

		pHouse1Object[5]->SetPosition(150.0f, 0.0f, 150.0f);
		pHouse1Object[6]->SetPosition(-50.0f, 0.0f, 150.0f);

		pHouse1Object[7]->SetPosition(50.0f, 0.0f, 300.0f);
		pHouse1Object[8]->SetPosition(250.0f, 0.0f, 300.0f);
		pHouse1Object[9]->SetPosition(-150.0f, 0.0f, 300.0f);

		pHouse1Object[10]->SetPosition(-300.0f, 0.0f, -100.0f);
		pHouse1Object[10]->Rotate(0.0f, 90.0f, 0.0f);
		pHouse1Object[11]->SetPosition(250.0f, 0.0f, 100.0f);
		pHouse1Object[11]->Rotate(0.0f, -90.0f, 0.0f);
		//pHouse1Object[7]->SetPosition(-150.0f, 0.0f, 0.0f);

		// 쉐이더에 저장
		for (int i = 12; i < m_nShaders - 64; ++i) // 24
		{
			m_ppShaders[i]->AddObject(pHouse1Object[i - 12]);
		}

		//// 벽 쉐이더 생성
		//for (int i = 24; i < m_nShaders; ++i)
		//{
		//	m_ppShaders[i] = new CTexturedIlluminatedShader(1);
		//	m_ppShaders[i]->CreateShader(pd3dDevice);
		//	m_ppShaders[i]->BuildObjects(pd3dDevice);

		//}

		// 벽 오브젝트 생성

		for (int i = 0; i < 64; ++i)
		{
			pWallObject[i] = new CGameObject(1);
			pWallObject[i]->SetMesh(pWallMesh);
			pWallObject[i]->SetMaterial(pNormalMaterial);
			pWallObject[i]->SetTexture(pWallTexture);
			pWallObject[i]->Rotate(-90.0f, 0.0f, 0.0f);

		}
		// 벽 위치 설정
		// 아군 리스폰 오른쪽 벽
		pWallObject[0]->SetPosition(135.0f, 0.0f, -500.0f);
		pWallObject[1]->SetPosition(185.0f, 0.0f, -500.0f);
		pWallObject[2]->SetPosition(235.0f, 0.0f, -500.0f);
		pWallObject[3]->SetPosition(285.0f, 0.0f, -500.0f);
		pWallObject[4]->SetPosition(335.0f, 0.0f, -500.0f);
		pWallObject[5]->SetPosition(385.0f, 0.0f, -500.0f);

		// 아군 리스폰 왼쪽 벽
		pWallObject[6]->SetPosition(-135.0f, 0.0f, -500.0f);
		pWallObject[7]->SetPosition(-185.0f, 0.0f, -500.0f);
		pWallObject[8]->SetPosition(-235.0f, 0.0f, -500.0f);
		pWallObject[9]->SetPosition(-285.0f, 0.0f, -500.0f);
		pWallObject[10]->SetPosition(-335.0f, 0.0f, -500.0f);
		pWallObject[11]->SetPosition(-385.0f, 0.0f, -500.0f);

		// 적군 리스폰 오른쪽 벽
		pWallObject[12]->SetPosition(135.0f, 0.0f, 500.0f);
		pWallObject[13]->SetPosition(185.0f, 0.0f, 500.0f);
		pWallObject[14]->SetPosition(235.0f, 0.0f, 500.0f);
		pWallObject[15]->SetPosition(285.0f, 0.0f, 500.0f);
		pWallObject[16]->SetPosition(335.0f, 0.0f, 500.0f);
		pWallObject[17]->SetPosition(385.0f, 0.0f, 500.0f);

		// 적군 리스폰 왼쪽 벽
		pWallObject[18]->SetPosition(-135.0f, 0.0f, 500.0f);
		pWallObject[19]->SetPosition(-185.0f, 0.0f, 500.0f);
		pWallObject[20]->SetPosition(-235.0f, 0.0f, 500.0f);
		pWallObject[21]->SetPosition(-285.0f, 0.0f, 500.0f);
		pWallObject[22]->SetPosition(-335.0f, 0.0f, 500.0f);
		pWallObject[23]->SetPosition(-385.0f, 0.0f, 500.0f);

		// 오른쪽 벽
		/*pWallObject[24]->SetPosition(435.0f, 0.0f, -450.0f);
		pWallObject[24]->Rotate(0.0f, 90.0f, 0.0f);
		pWallObject[25]->SetPosition(435.0f, 0.0f, -450.0f);
		pWallObject[25]->Rotate(0.0f, 90.0f, 0.0f);
		pWallObject[26]->SetPosition(435.0f, 0.0f, -450.0f);
		pWallObject[26]->Rotate(0.0f, 90.0f, 0.0f);
		pWallObject[27]->SetPosition(435.0f, 0.0f, -450.0f);
		pWallObject[27]->Rotate(0.0f, 90.0f, 0.0f);
		pWallObject[28]->SetPosition(435.0f, 0.0f, -450.0f);
		pWallObject[28]->Rotate(0.0f, 90.0f, 0.0f);
		pWallObject[29]->SetPosition(435.0f, 0.0f, -450.0f);
		pWallObject[29]->Rotate(0.0f, 90.0f, 0.0f);
		pWallObject[30]->SetPosition(435.0f, 0.0f, -450.0f);
		pWallObject[30]->Rotate(0.0f, 90.0f, 0.0f);
		pWallObject[31]->SetPosition(435.0f, 0.0f, -450.0f);
		pWallObject[31]->Rotate(0.0f, 90.0f, 0.0f);*/


		int j = 0;
		for (int i = 24; i < 24 + 20; ++i)
		{
			pWallObject[i]->SetPosition(415.0f, 0.0f, -475.0f + j);
			pWallObject[i]->Rotate(-90.0f, 90.0f, 0.0f);
			j += 50;
		}
		// 왼쪽 벽
		int k = 0;
		for (int i = 44; i < 44 + 20; ++i)
		{
			pWallObject[i]->SetPosition(-415.0f, 0.0f, -475.0f + k);
			pWallObject[i]->Rotate(-90.0f, 90.0f, 0.0f);
			k += 50;
		}

		// 쉐이더에 저장
		for (int i = 24; i < m_nShaders; ++i) // 88
		{
			m_ppShaders[i]->AddObject(pWallObject[i - 24]);
		}
	}
	//m_pTerrain = new CHeightMapTerrain(pd3dDevice, _T("Data\\HeightMap.raw"), 257, 257, 17, 17, d3dxvScale, d3dxColor);
	//m_pTerrain->SetMaterial(pNormalMaterial);

	CreateShaderVariables(pd3dDevice);

	///////////////////////////////////////////////////////////////////////////
	// UI

	//hpbar ui
	pd3dsrvTexture = NULL;
	CTexture *pHpbar = new CTexture(1, 1, 0, 0);
	D3DX11CreateShaderResourceViewFromFile(pd3dDevice, _T("../Data/UI/hpbar.png"), NULL, NULL, &pd3dsrvTexture, NULL);
	pHpbar->SetTexture(0, pd3dsrvTexture);
	pHpbar->SetSampler(0, pd3dSamplerState);
	pd3dsrvTexture->Release();

	m_pUIManager[0] = new CUIManager();
	m_pUIManager[0]->Initialize(pd3dDevice);

	CUIObject *pHpbarObject = new CUIObject(pd3dDevice);
	pHpbarObject->SetMaterial(pHpbar);
	pHpbarObject->Initialize(pd3dDevice, POINT{ 0, 668 }, POINT{ 400, 768 }, 0.5f);
	m_pUIManager[0]->AddUIObject(pHpbarObject);

	//hp gauge
	CTexture *pHpgauge = new CTexture(1, 1, 0, 0);
	D3DX11CreateShaderResourceViewFromFile(pd3dDevice, _T("../Data/UI/gauge.png"), NULL, NULL, &pd3dsrvTexture, NULL);
	pHpgauge->SetTexture(0, pd3dsrvTexture);
	pHpgauge->SetSampler(0, pd3dSamplerState);
	pd3dsrvTexture->Release();

	m_pUIManager[1] = new CUIManager();
	m_pUIManager[1]->Initialize(pd3dDevice);

	CUIObject *pHpgaugeObject = new CUIObject(pd3dDevice);
	pHpgaugeObject->SetMaterial(pHpgauge);
	pHpgaugeObject->Initialize(pd3dDevice, POINT{ 50, 720 }, POINT{ 385, 745 }, 0.0f);
	m_pUIManager[1]->AddUIObject(pHpgaugeObject);


	///////////skill box/////////////////////
	//skill box Q
	CTexture *pSkillboxQ = new CTexture(1, 1, 0, 0);
	D3DX11CreateShaderResourceViewFromFile(pd3dDevice, _T("../Data/UI/SkillBox.png"), NULL, NULL, &pd3dsrvTexture, NULL);
	pSkillboxQ->SetTexture(0, pd3dsrvTexture);
	pSkillboxQ->SetSampler(0, pd3dSamplerState);
	pd3dsrvTexture->Release();

	m_pUIManager[2] = new CUIManager();
	m_pUIManager[2]->Initialize(pd3dDevice);

	CUIObject *pSkillboxQObject = new CUIObject(pd3dDevice);
	pSkillboxQObject->SetMaterial(pSkillboxQ);
	pSkillboxQObject->Initialize(pd3dDevice, POINT{ 600, 668 }, POINT{ 700, 768 }, 0.5f);
	m_pUIManager[2]->AddUIObject(pSkillboxQObject);
	//skill box W
	CTexture *pSkillboxW = new CTexture(1, 1, 0, 0);
	D3DX11CreateShaderResourceViewFromFile(pd3dDevice, _T("../Data/UI/SkillBox.png"), NULL, NULL, &pd3dsrvTexture, NULL);
	pSkillboxW->SetTexture(0, pd3dsrvTexture);
	pSkillboxW->SetSampler(0, pd3dSamplerState);
	pd3dsrvTexture->Release();

	m_pUIManager[3] = new CUIManager();
	m_pUIManager[3]->Initialize(pd3dDevice);

	CUIObject *pSkillboxWObject = new CUIObject(pd3dDevice);
	pSkillboxWObject->SetMaterial(pSkillboxW);
	pSkillboxWObject->Initialize(pd3dDevice, POINT{ 700, 668 }, POINT{ 800, 768 }, 0.5f);
	m_pUIManager[3]->AddUIObject(pSkillboxWObject);
	//skill box E
	CTexture *pSkillboxE = new CTexture(1, 1, 0, 0);
	D3DX11CreateShaderResourceViewFromFile(pd3dDevice, _T("../Data/UI/SkillBox.png"), NULL, NULL, &pd3dsrvTexture, NULL);
	pSkillboxE->SetTexture(0, pd3dsrvTexture);
	pSkillboxE->SetSampler(0, pd3dSamplerState);
	pd3dsrvTexture->Release();

	m_pUIManager[4] = new CUIManager();
	m_pUIManager[4]->Initialize(pd3dDevice);

	CUIObject *pSkillboxEObject = new CUIObject(pd3dDevice);
	pSkillboxEObject->SetMaterial(pSkillboxE);
	pSkillboxEObject->Initialize(pd3dDevice, POINT{ 800, 668 }, POINT{ 900, 768 }, 0.5f);
	m_pUIManager[4]->AddUIObject(pSkillboxEObject);
	//skill box R
	CTexture *pSkillboxR = new CTexture(1, 1, 0, 0);
	D3DX11CreateShaderResourceViewFromFile(pd3dDevice, _T("../Data/UI/SkillBox.png"), NULL, NULL, &pd3dsrvTexture, NULL);
	pSkillboxR->SetTexture(0, pd3dsrvTexture);
	pSkillboxR->SetSampler(0, pd3dSamplerState);
	pd3dsrvTexture->Release();

	m_pUIManager[5] = new CUIManager();
	m_pUIManager[5]->Initialize(pd3dDevice);

	CUIObject *pSkillboxRObject = new CUIObject(pd3dDevice);
	pSkillboxRObject->SetMaterial(pSkillboxR);
	pSkillboxRObject->Initialize(pd3dDevice, POINT{ 900, 668 }, POINT{ 1000, 768 }, 0.5f);
	m_pUIManager[5]->AddUIObject(pSkillboxRObject);
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

			break;

		case 'Z':
			if (!pHeroObject[myGame_id]->bHeroAttack)
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
		if (!pHeroObject[myGame_id]->bHeroAttack && !pHeroObject[myGame_id]->bHeroQ && !pHeroObject[myGame_id]->bHeroW && !pHeroObject[myGame_id]->bHeroE && !pHeroObject[myGame_id]->bHeroR)
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
			if (pHeroObject[myGame_id]->bHeroRun)
			{
				m_ppShaders[myGame_id + 1]->GetFBXMesh->SetAnimation(ANI_IDLE);
				pHeroObject[myGame_id]->bHeroRun = false;
			}
			SendMovePacket(CS_KEYUP_UP);
		}
		if (KEY_UP(VK_DOWN) && DownKeyDown)
		{
			DownKeyDown = false;
			if (pHeroObject[myGame_id]->bHeroRun)
			{
				m_ppShaders[myGame_id + 1]->GetFBXMesh->SetAnimation(ANI_IDLE);
				pHeroObject[myGame_id]->bHeroRun = false;
			}
			SendMovePacket(CS_KEYUP_DOWN);
		}
		if (KEY_UP(VK_LEFT) && LeftKeyDown)
		{
			LeftKeyDown = false;
			if (pHeroObject[myGame_id]->bHeroRun)
			{
				m_ppShaders[myGame_id +1]->GetFBXMesh->SetAnimation(ANI_IDLE);
				pHeroObject[myGame_id]->bHeroRun = false;
			}
			SendMovePacket(CS_KEYUP_LEFT);
		}
		if (KEY_UP(VK_RIGHT) && RightKeyDown)
		{
			RightKeyDown = false;
			if (pHeroObject[myGame_id]->bHeroRun)
			{
				m_ppShaders[myGame_id +1]->GetFBXMesh->SetAnimation(ANI_IDLE);
				pHeroObject[myGame_id]->bHeroRun = false;
			}
			SendMovePacket(CS_KEYUP_RIGHT);
		}


		if (KEY_DOWN('D'))
		{
			if (!pHeroObject[myGame_id]->bHeroAttack)
			{
				pHeroObject[myGame_id]->bHeroAttack = true;
				pHeroObject[myGame_id]->bHeroRun = false;

				cs_packet_attack *my_packet = reinterpret_cast<cs_packet_attack *>(send_buffer);
				my_packet->size = sizeof(cs_packet_attack);
				send_wsabuf.len = sizeof(cs_packet_attack);
				DWORD iobyte;
				my_packet->type = CS_ATTACK;
				my_packet->roomnumber = MyRoomNumber;

				WSASend(g_mysocket, &send_wsabuf, 1, &iobyte, 0, NULL, NULL);
			}

			
		}

		else if (KEY_DOWN('Q'))
		{
			if (!pHeroObject[myGame_id]->bHeroQ)
			{
				pHeroObject[myGame_id]->bHeroQ = true;
				pHeroObject[myGame_id]->bHeroRun = false;

				cs_packet_skillQ *my_packet = reinterpret_cast<cs_packet_skillQ *>(send_buffer);
				my_packet->size = sizeof(cs_packet_skillQ);
				send_wsabuf.len = sizeof(cs_packet_skillQ);
				DWORD iobyte;
				my_packet->type = CS_SKILL_Q;
				my_packet->roomnumber = MyRoomNumber;

				WSASend(g_mysocket, &send_wsabuf, 1, &iobyte, 0, NULL, NULL);
			}
		}
		else if (KEY_DOWN('W'))
		{
			if (!pHeroObject[myGame_id]->bHeroW)
			{
				pHeroObject[myGame_id]->bHeroW = true;
				pHeroObject[myGame_id]->bHeroRun = false;

				cs_packet_skillW *my_packet = reinterpret_cast<cs_packet_skillW *>(send_buffer);
				my_packet->size = sizeof(cs_packet_skillW);
				send_wsabuf.len = sizeof(cs_packet_skillW);
				DWORD iobyte;
				my_packet->type = CS_SKILL_W;
				my_packet->roomnumber = MyRoomNumber;

				WSASend(g_mysocket, &send_wsabuf, 1, &iobyte, 0, NULL, NULL);
			}
		}
		else if (KEY_DOWN('E'))
		{
			if (!pHeroObject[myGame_id]->bHeroE)
			{

				pHeroObject[myGame_id]->bHeroE = true;
				pHeroObject[myGame_id]->bHeroRun = false;

				cs_packet_skillE *my_packet = reinterpret_cast<cs_packet_skillE *>(send_buffer);
				my_packet->size = sizeof(cs_packet_skillE);
				send_wsabuf.len = sizeof(cs_packet_skillE);
				DWORD iobyte;
				my_packet->type = CS_SKILL_E;
				my_packet->roomnumber = MyRoomNumber;

				WSASend(g_mysocket, &send_wsabuf, 1, &iobyte, 0, NULL, NULL);
			}
		}
		else if (KEY_DOWN('R'))
		{
			if (!pHeroObject[myGame_id]->bHeroR)
			{
				
				pHeroObject[myGame_id]->bHeroR = true;
				pHeroObject[myGame_id]->bHeroRun = false;

				cs_packet_skillR *my_packet = reinterpret_cast<cs_packet_skillR *>(send_buffer);
				my_packet->size = sizeof(cs_packet_skillR);
				send_wsabuf.len = sizeof(cs_packet_skillR);
				DWORD iobyte;
				my_packet->type = CS_SKILL_R;
				my_packet->roomnumber = MyRoomNumber;

				WSASend(g_mysocket, &send_wsabuf, 1, &iobyte, 0, NULL, NULL);
			}
		}

		else if (KEY_DOWN('1'))
		{
			if (!pHeroObject[myGame_id]->bHeroHit)
			{
				m_ppShaders[myGame_id +1]->GetFBXMesh->SetAnimation(ANI_HIT);

				pHeroObject[myGame_id]->bHeroHit = true;
				pHeroObject[myGame_id]->bHeroRun = false;
			}
		}
		else if (KEY_DOWN('2'))
		{
			if (!pHeroObject[myGame_id]->bHeroStun)
			{
				m_ppShaders[myGame_id +1]->GetFBXMesh->SetAnimation(ANI_STUN);

				pHeroObject[myGame_id]->bHeroStun = true;
				pHeroObject[myGame_id]->bHeroRun = false;
			}
		}
		else if (KEY_DOWN('3'))
		{
			if (!pHeroObject[myGame_id]->bHeroDie)
			{
				m_ppShaders[myGame_id +1]->GetFBXMesh->SetAnimation(ANI_DIE);

				pHeroObject[myGame_id]->bHeroDie = true;
				pHeroObject[myGame_id]->bHeroRun = false;
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
	my_packet->x = pHeroObject[myGame_id]->GetPosition().x;
	my_packet->y = pHeroObject[myGame_id]->GetPosition().y;
	my_packet->z = pHeroObject[myGame_id]->GetPosition().z;
	my_packet->roomnumber = MyRoomNumber;

	//currentDateTime();
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

	
	if (pHeroObject[myGame_id]->bHeroAttack || pHeroObject[myGame_id]->bHeroQ || pHeroObject[myGame_id]->bHeroW || pHeroObject[myGame_id]->bHeroE || pHeroObject[myGame_id]->bHeroR)
	{
		if (m_ppShaders[myGame_id + 1]->GetFBXMesh->GetFBXNowFrameNum() == m_ppShaders[myGame_id + 1]->GetFBXMesh->GetFBXMaxFrameNum() - 1)
		{

			// server send (Ready)
			cs_packet_skill_done *my_packet = reinterpret_cast<cs_packet_skill_done *>(send_buffer); //스킬이 종료되면 받는다.
			my_packet->size = sizeof(cs_packet_skill_done);
			send_wsabuf.len = sizeof(cs_packet_skill_done);
			DWORD iobyte;
			my_packet->type = CS_SKILL_DONE;
			
			WSASend(g_mysocket, &send_wsabuf, 1, &iobyte, 0, NULL, NULL);
			//

			if(pHeroObject[myGame_id]->bHeroAttack) pHeroObject[myGame_id]->bHeroAttack = false;
			if (pHeroObject[myGame_id]->bHeroQ) pHeroObject[myGame_id]->bHeroQ = false;
			if (pHeroObject[myGame_id]->bHeroW) pHeroObject[myGame_id]->bHeroW = false;
			if (pHeroObject[myGame_id]->bHeroE) pHeroObject[myGame_id]->bHeroE = false;
			if (pHeroObject[myGame_id]->bHeroR) pHeroObject[myGame_id]->bHeroR = false;

		}

	}

	

	//for (int i = 0; i < MAX_GAMER; ++i)
	//{
	//	if (i != myroom_id)
	//	{
	//		if (m_ppShaders[i + 1]->GetFBXMesh->GetFBXNowFrameNum() == m_ppShaders[i + 1]->GetFBXMesh->GetFBXMaxFrameNum() - 1)
	//		{
	//			m_ppShaders[i + 1]->GetFBXMesh->SetAnimation(ANI_IDLE);	
	//		}
	//	}

	//}

	
	for (int i = 1; i < MAX_GAMER+1; ++i)
	{
		m_ppShaders[i]->GetFBXMesh->FBXFrameAdvance(fTimeElapsed);

	}


	
	//pHeroObject[myroom_id]->SetOOBB(XMFLOAT3(pHeroObject[myroom_id]->GetPosition().x, pHeroObject[myroom_id]->GetPosition().y, pHeroObject[myroom_id]->GetPosition().z),
	//	XMFLOAT3(0.1f, 0.1f, 0.1f), XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f));

	//if (pHouse1Object[0]->m_xmOOBB.Intersects(pHeroObject[myroom_id]->m_xmOOBB))
	//{
	//	pHouse1Object[0]->m_pCollider = pHeroObject[myroom_id];
	//	pHeroObject[myroom_id]->m_pCollider = pHouse1Object[0];

	//	//pHouse1Object[0]->SetColCheck(true);
	//	ColBox = true;
	//}
	//else
	//{
	//	ColBox = false;
	//}

}

void CScene::Render(ID3D11DeviceContext*pd3dDeviceContext, CCamera *pCamera)
{
	if (m_pLights && m_pd3dcbLights) UpdateShaderVariable(pd3dDeviceContext, m_pLights);
	
	for (int i = 0; i < m_nShaders; ++i)
	{
		
		if (i == 1 || i == 2 || i == 3 || i == 4 || i == 5 || i == 6 || i == 7 || i == 8) // i번째 뼈대의 행렬 변경하자.
			m_ppShaders[i]->GetFBXMesh->UpdateBoneTransform(pd3dDeviceContext, m_ppShaders[i]->GetFBXMesh->GetFBXAnimationNum(), m_ppShaders[i]->GetFBXMesh->GetFBXNowFrameNum());

		m_ppShaders[i]->Render(pd3dDeviceContext, pCamera);
	}
	for (int i = 0; i < m_nInstancingShaders; i++) m_ppInstancingShaders[i]->Render(pd3dDeviceContext, pCamera);

	for(int i = 0; i < 6; ++i)
		m_pUIManager[i]->RenderAll(pd3dDeviceContext);
}

bool CScene::Rightcollision(CHeroManager* Object1, CHeroManager* Object2, float sizeX1, float sizeZ1, float sizeX2, float sizeZ2)
{
	float Left = (Object1->GetPosition().x - sizeX1);
	float Top = (Object1->GetPosition().z - sizeZ1);
	float Right = (Object1->GetPosition().x + sizeX1);
	float Bottom = (Object1->GetPosition().z + sizeZ1);

	float Left2 = (Object2->GetPosition().x - sizeX2);
	float Top2 = (Object2->GetPosition().z - sizeZ2);
	float Right2 = (Object2->GetPosition().x - sizeX2);
	float Bottom2 = (Object2->GetPosition().z + sizeZ2);

	if (Left < Right2 && Right > Left2 && Top < Bottom2 && Bottom > Top2)
		return true;
	else
		return false;
}


bool CScene::Leftcollision(CHeroManager* Object1, CHeroManager* Object2, float sizeX1, float sizeZ1, float sizeX2, float sizeZ2)
{

	float Left = (Object1->GetPosition().x - sizeX1);
	float Top = (Object1->GetPosition().z - sizeZ1);
	float Right = (Object1->GetPosition().x + sizeX1);
	float Bottom = (Object1->GetPosition().z + sizeZ1);

	float Left2 = (Object2->GetPosition().x + sizeX2);
	float Top2 = (Object2->GetPosition().z - sizeZ2);
	float Right2 = (Object2->GetPosition().x + sizeX2);
	float Bottom2 = (Object2->GetPosition().z + sizeZ2);

	if (Left < Right2 && Right > Left2 && Top < Bottom2 && Bottom > Top2)
		return true;
	else
		return false;
}

bool CScene::Downcollision(CHeroManager* Object1, CHeroManager* Object2, float sizeX1, float sizeZ1, float sizeX2, float sizeZ2)
{
	float Left = (Object1->GetPosition().x - sizeX1);
	float Top = (Object1->GetPosition().z - sizeZ1);
	float Right = (Object1->GetPosition().x + sizeX1);
	float Bottom = (Object1->GetPosition().z + sizeZ1);

	float Left2 = (Object2->GetPosition().x - sizeX2);
	float Top2 = (Object2->GetPosition().z - sizeZ2);
	float Right2 = (Object2->GetPosition().x + sizeX2);
	float Bottom2 = (Object2->GetPosition().z - sizeZ2);

	if (Left < Right2 && Right > Left2 && Top < Bottom2 && Bottom > Top2)
		return true;
	else
		return false;
}

bool CScene::Upcollision(CHeroManager* Object1, CHeroManager* Object2, float sizeX1, float sizeZ1, float sizeX2, float sizeZ2)
{
	float Left = (Object1->GetPosition().x - sizeX1);
	float Top = (Object1->GetPosition().z - sizeZ1);
	float Right = (Object1->GetPosition().x + sizeX1);
	float Bottom = (Object1->GetPosition().z + sizeZ1);

	float Left2 = (Object2->GetPosition().x - sizeX2);
	float Top2 = (Object2->GetPosition().z + sizeZ2);
	float Right2 = (Object2->GetPosition().x + sizeX2);
	float Bottom2 = (Object2->GetPosition().z + sizeZ2);

	if (Left < Right2 && Right > Left2 && Top < Bottom2 && Bottom > Top2)
		return true;
	else
		return false;
}



bool CScene::Rightcollision(CHeroManager* Object1, CGameObject* Object2, float sizeX1, float sizeZ1, float sizeX2, float sizeZ2)
{
	float Left = (Object1->GetPosition().x - sizeX1);
	float Top = (Object1->GetPosition().z - sizeZ1);
	float Right = (Object1->GetPosition().x + sizeX1);
	float Bottom = (Object1->GetPosition().z + sizeZ1);

	float Left2 = (Object2->GetPosition().x - sizeX2);
	float Top2 = (Object2->GetPosition().z - sizeZ2);
	float Right2 = (Object2->GetPosition().x - sizeX2);
	float Bottom2 = (Object2->GetPosition().z + sizeZ2);

	if (Left < Right2 && Right > Left2 && Top < Bottom2 && Bottom > Top2)
		return true;
	else
		return false;
}


bool CScene::Leftcollision(CHeroManager* Object1, CGameObject* Object2, float sizeX1, float sizeZ1, float sizeX2, float sizeZ2)
{

	float Left = (Object1->GetPosition().x - sizeX1);
	float Top = (Object1->GetPosition().z - sizeZ1);
	float Right = (Object1->GetPosition().x + sizeX1);
	float Bottom = (Object1->GetPosition().z + sizeZ1);

	float Left2 = (Object2->GetPosition().x + sizeX2);
	float Top2 = (Object2->GetPosition().z - sizeZ2);
	float Right2 = (Object2->GetPosition().x + sizeX2);
	float Bottom2 = (Object2->GetPosition().z + sizeZ2);

	if (Left < Right2 && Right > Left2 && Top < Bottom2 && Bottom > Top2)
		return true;
	else
		return false;
}

bool CScene::Downcollision(CHeroManager* Object1, CGameObject* Object2, float sizeX1, float sizeZ1, float sizeX2, float sizeZ2)
{
	float Left = (Object1->GetPosition().x - sizeX1);
	float Top = (Object1->GetPosition().z - sizeZ1);
	float Right = (Object1->GetPosition().x + sizeX1);
	float Bottom = (Object1->GetPosition().z + sizeZ1);

	float Left2 = (Object2->GetPosition().x - sizeX2 - sizeX2);
	float Top2 = (Object2->GetPosition().z - sizeZ2);
	float Right2 = (Object2->GetPosition().x - 15.0f);
	float Bottom2 = (Object2->GetPosition().z - sizeZ2);

	if (Left < Right2 && Right > Left2 && Top < Bottom2 && Bottom > Top2)
		return true;
	else
		return false;
}

bool CScene::Upcollision(CHeroManager* Object1, CGameObject* Object2, float sizeX1, float sizeZ1, float sizeX2, float sizeZ2)
{
	float Left = (Object1->GetPosition().x - sizeX1);
	float Top = (Object1->GetPosition().z - sizeZ1);
	float Right = (Object1->GetPosition().x + sizeX1);
	float Bottom = (Object1->GetPosition().z + sizeZ1);

	float Left2 = (Object2->GetPosition().x - sizeX2 - sizeX2);
	float Top2 = (Object2->GetPosition().z + sizeZ2);
	float Right2 = (Object2->GetPosition().x - 15.0f);
	float Bottom2 = (Object2->GetPosition().z + sizeZ2);

	if (Left < Right2 && Right > Left2 && Top < Bottom2 && Bottom > Top2)
		return true;
	else
		return false;
}