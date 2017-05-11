#include "stdafx.h"
#include "Scene.h"
#include "CubeMesh.h"
#include "CubeObject.h"
#include "FBXMesh.h"
#include "CharacterShader.h"

static bool bCharaterRun = false;
static bool bCharaterPunch = false;

CScene::CScene()
{
	m_nShaders = 0;
	m_nInstancingShaders = 0;

	m_ppShaders = NULL;
	m_ppInstancingShaders = NULL;

	m_pCamera = NULL;
	m_pLights = NULL;
	m_pd3dcbLights = NULL;

	pMyObject = new CHeroManager(1);


	LeftKeyDown = false;
	RightKeyDown = false;
	UpKeyDown = false;
	DownKeyDown = false;

	KeyDownForServer = 0;
	DWORD dwDirection = 0;
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
	

	// ③ Object용 Material과 Shader를 생성 (Skybox는 쉐이더 내부에서 자체적으로 생성)
	CMaterial *pNormalMaterial = new CMaterial();
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

	// map objects
	CMesh *pPlaneMesh = new CFBXMesh(pd3dDevice, "../Data/plane1.data", 1.f);

	CMesh *pBuilding1Mesh = new CFBXMesh(pd3dDevice, "../Data/building/building1/building1.data", 0.6f);
	CMesh *pBuilding2Mesh = new CFBXMesh(pd3dDevice, "../Data/building/building2/building2.data", 1.f);
	CMesh *pHouse1Mesh = new CFBXMesh(pd3dDevice, "../Data/building/house1/house1.data", 0.7f);
	CMesh *pHouse2Mesh = new CFBXMesh(pd3dDevice, "../Data/building/house2/house2.data", 0.7f);
	CMesh *pCityhallMesh = new CFBXMesh(pd3dDevice, "../Data/building/cityhall/cityhall.data", 1.f);

	
	// 일반 쉐이더 선언부
	/////////////////////////////////////////////////////////////////////////

	m_nShaders = 10;   // Skybox\포함
	m_ppShaders = new CShader*[m_nShaders];

	// ⑤ SkyBox용 Shader를 생성
	m_ppShaders[0] = new CSkyBoxShader();
	m_ppShaders[0]->CreateShader(pd3dDevice);
	m_ppShaders[0]->BuildObjects(pd3dDevice);


	{
		// My
		m_ppShaders[1] = new CCharacterShader(1);
		m_ppShaders[1]->CreateShader(pd3dDevice);
		m_ppShaders[1]->BuildObjects(pd3dDevice);

		

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

		else if (pMyObject->m_Team == B_TEAM)
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
		}
		pMyObject->SetMaterial(pNormalMaterial);
		pMyObject->SetPosition(0.0f, -3000.0f, 0.0f);
		pMyObject->Rotate(0.0f, 0.0f, 0.0f);

		m_ppShaders[1]->AddObject(pMyObject);


		//Other

		m_ppShaders[2] = new CCharacterShader(1);
		m_ppShaders[2]->CreateShader(pd3dDevice);
		m_ppShaders[2]->BuildObjects(pd3dDevice);

		pOtherObject[0] = new CHeroManager(1);
		if (pOtherObject[0]->m_HeroSelect == Babarian)
		{
			pOtherObject[0]->SetMesh(pBabarianMeshA);
			pOtherObject[0]->SetTexture(pBabarianTexture);
		}
		else if (pOtherObject[0]->m_HeroSelect == Healer)
		{
			pOtherObject[0]->SetMesh(pHealerMeshA);
			pOtherObject[0]->SetTexture(pHealerTexture);
		}

		else if (pOtherObject[0]->m_HeroSelect == SordMan)
		{
			pOtherObject[0]->SetMesh(pSordManMeshA);
			pOtherObject[0]->SetTexture(pSordManTexture);
		}
		pOtherObject[0]->SetMaterial(pNormalMaterial);
		pOtherObject[0]->SetPosition(0.0f, -3000.0f, 0.0f);
		pOtherObject[0]->Rotate(0.0f, 0.0f, 0.0f);

		m_ppShaders[2]->AddObject(pOtherObject[0]);


		m_ppShaders[3] = new CCharacterShader(1);
		m_ppShaders[3]->CreateShader(pd3dDevice);
		m_ppShaders[3]->BuildObjects(pd3dDevice);

		pOtherObject[1] = new CHeroManager(1);
		if (pOtherObject[1]->m_HeroSelect == Babarian)
		{
			pOtherObject[1]->SetMesh(pBabarianMeshA);
			pOtherObject[1]->SetTexture(pBabarianTexture);
		}
		else if (pOtherObject[1]->m_HeroSelect == Healer)
		{
			pOtherObject[1]->SetMesh(pHealerMeshA);
			pOtherObject[1]->SetTexture(pHealerTexture);
		}

		else if (pOtherObject[1]->m_HeroSelect == SordMan)
		{
			pOtherObject[1]->SetMesh(pSordManMeshA);
			pOtherObject[1]->SetTexture(pSordManTexture);
		}
		pOtherObject[1]->SetMaterial(pNormalMaterial);
		pOtherObject[1]->SetPosition(0.0f, -3000.0f, 0.0f);
		pOtherObject[1]->Rotate(0.0f, 0.0f, 0.0f);

		m_ppShaders[3]->AddObject(pOtherObject[1]);


		for (int i = 0; i <= 5; ++i)
		{
			m_ppShaders[i + 4] = new CCharacterShader(1);
			m_ppShaders[i + 4]->CreateShader(pd3dDevice);
			m_ppShaders[i + 4]->BuildObjects(pd3dDevice);

		}

		for (int i = 0; i <= 5; ++i)
		{
			pOtherObject[i + 2] = new CHeroManager(1);
			pOtherObject[i + 2]->SetMesh(pSordManMeshB);
			pOtherObject[i + 2]->SetTexture(pSordManTexture);
			pOtherObject[i + 2]->SetMaterial(pNormalMaterial);
			pOtherObject[i + 2]->SetPosition(0.0f, -3000.0f, 0.0f);
			pOtherObject[i + 2]->Rotate(0.0f, 0.0f, 0.0f);

			m_ppShaders[i + 4]->AddObject(pOtherObject[i + 2]);

		}




		//for (int i = 0; i < MAX_USER; ++i)
		//{
		//   m_ppShaders[i+2] = new CCharacterShader(1);
		//   m_ppShaders[i+2]->CreateShader(pd3dDevice);
		//   m_ppShaders[i+2]->BuildObjects(pd3dDevice);

		//   pOtherObject[i] = new CHeroManager(1);
		//   pOtherObject[i]->SetMesh(pSordManMeshB);
		//   pOtherObject[i]->SetTexture(pSordManTexture);
		//   /*if (pOtherObject[i]->m_Team == A_TEAM)
		//   {
		//      if (pOtherObject[i]->m_CharSelect == SordMan)
		//      {
		//         pOtherObject[i]->SetMesh(pSordManMeshA);
		//         pOtherObject[i]->SetTexture(pSordManTexture);

		//      }
		//      else if (pOtherObject[i]->m_CharSelect == Healer)
		//      {
		//         pOtherObject[i]->SetMesh(pHealerMeshA);
		//         pOtherObject[i]->SetTexture(pHealerTexture);

		//      }
		//      else if (pOtherObject[i]->m_CharSelect == Babarian)
		//      {
		//         pOtherObject[i]->SetMesh(pBabarianMeshA);
		//         pOtherObject[i]->SetTexture(pBabarianTexture);
		//      }
		//   }

		//   else if (pOtherObject[i]->m_Team == B_TEAM)
		//   {
		//      if (pOtherObject[i]->m_CharSelect == SordMan)
		//      {
		//         pOtherObject[i]->SetMesh(pSordManMeshB);
		//         pOtherObject[i]->SetTexture(pSordManTexture);

		//      }
		//      else if (pOtherObject[i]->m_CharSelect == Healer)
		//      {
		//         pOtherObject[i]->SetMesh(pHealerMeshB);
		//         pOtherObject[i]->SetTexture(pHealerTexture);

		//      }
		//      else if (pOtherObject[i]->m_CharSelect == Babarian)
		//      {
		//         pOtherObject[i]->SetMesh(pBabarianMeshB);
		//         pOtherObject[i]->SetTexture(pBabarianTexture);
		//      }
		//   }*/
		//   pOtherObject[i]->SetMaterial(pNormalMaterial);
		//   pOtherObject[i]->SetPosition(0.0f, -3000.0f, 0.0f);
		//   pOtherObject[i]->Rotate(0.0f, 0.0f, 0.0f);

		//   m_ppShaders[i+2]->AddObject(pOtherObject[i]);
		//}


		//for (int i = 0; i < MAX_USER; ++i)
		//{
		//   m_ppShaders[i+2] = new CCharacterShader(1);
		//   m_ppShaders[i+2]->CreateShader(pd3dDevice);
		//   m_ppShaders[i+2]->BuildObjects(pd3dDevice);
		//
		//   pOtherObject[i] = new CHeroManager(1);
		//   pOtherObject[i]->SetMesh(pHealerMesh);
		//   pOtherObject[i]->SetMaterial(pNormalMaterial);
		//   pOtherObject[i]->SetTexture(pHealerTexture);
		//   pOtherObject[i]->SetPosition(0.0f, -3000.0f, -10.0f);
		//   pOtherObject[i]->Rotate(0.0f, 0.0f, 0.0f);
		//   

		//   m_ppShaders[i+2]->AddObject(pOtherObject[i]);

		//}


		////building1 (왼쪽)
		//m_ppShaders[10] = new CTexturedIlluminatedShader(1);
		//m_ppShaders[10]->CreateShader(pd3dDevice);
		//m_ppShaders[10]->BuildObjects(pd3dDevice);

		//CGameObject *pBuilding1Object = new CGameObject(1);
		//pBuilding1Object->SetMesh(pBuilding1Mesh);
		//pBuilding1Object->SetMaterial(pNormalMaterial);
		//pBuilding1Object->SetTexture(pBuilding1Texture);
		//pBuilding1Object->Rotate(0.0f, -90.0f, 0.0f);
		//pBuilding1Object->SetPosition(-300.0f, 25.0f, -350.0f);
		//m_ppShaders[10]->AddObject(pBuilding1Object);

		////building2 (리스폰) (정면)
		//m_ppShaders[11] = new CTexturedIlluminatedShader(1);
		//m_ppShaders[11]->CreateShader(pd3dDevice);
		//m_ppShaders[11]->BuildObjects(pd3dDevice);

		//CGameObject *pBuilding2Object = new CGameObject(1);
		//pBuilding2Object->SetMesh(pBuilding2Mesh);
		//pBuilding2Object->SetMaterial(pNormalMaterial);
		//pBuilding2Object->SetTexture(pBuilding2Texture);
		//pBuilding2Object->Rotate(0.0f, 0.0f, 0.0f);
		//pBuilding2Object->SetPosition(0.0f, 0.0f, 500.0f);
		//m_ppShaders[11]->AddObject(pBuilding2Object);

		////house1
		//m_ppShaders[12] = new CTexturedIlluminatedShader(1);
		//m_ppShaders[12]->CreateShader(pd3dDevice);
		//m_ppShaders[12]->BuildObjects(pd3dDevice);

		//CGameObject *pHouse1Object = new CGameObject(1);
		//pHouse1Object->SetMesh(pHouse1Mesh);
		//pHouse1Object->SetMaterial(pNormalMaterial);
		//pHouse1Object->SetTexture(pHouse1Texture);
		//pHouse1Object->Rotate(0.0f, 0.0f, 0.0f);
		//pHouse1Object->SetPosition(300.0f, 0.0f, 300.0f);
		//m_ppShaders[12]->AddObject(pHouse1Object);

		////house2
		//m_ppShaders[13] = new CTexturedIlluminatedShader(1);
		//m_ppShaders[13]->CreateShader(pd3dDevice);
		//m_ppShaders[13]->BuildObjects(pd3dDevice);

		//CGameObject *pHouse2Object = new CGameObject(1);
		//pHouse2Object->SetMesh(pHouse2Mesh);
		//pHouse2Object->SetMaterial(pNormalMaterial);
		//pHouse2Object->SetTexture(pHouse2Texture);
		//pHouse2Object->Rotate(0.0f, 90.0f, 0.0f);
		//pHouse2Object->SetPosition(400.0f, 60.0f, 600.0f);
		//m_ppShaders[13]->AddObject(pHouse2Object);

		////cityhall
		//m_ppShaders[14] = new CTexturedIlluminatedShader(1);
		//m_ppShaders[14]->CreateShader(pd3dDevice);
		//m_ppShaders[14]->BuildObjects(pd3dDevice);

		//CGameObject *pCityhallObject = new CGameObject(1);
		//pCityhallObject->SetMesh(pCityhallMesh);
		//pCityhallObject->SetMaterial(pNormalMaterial);
		//pCityhallObject->SetTexture(pCityhallTexture);
		//pCityhallObject->Rotate(0.0f, -90.0f, 0.0f);
		//pCityhallObject->SetPosition(-300.0f, 100.0f, 0.0f);
		//m_ppShaders[14]->AddObject(pCityhallObject);

		////plane
		//m_ppShaders[15] = new CTexturedIlluminatedShader(1);
		//m_ppShaders[15]->CreateShader(pd3dDevice);
		//m_ppShaders[15]->BuildObjects(pd3dDevice);

		//CGameObject *pPlaneObject = new CGameObject(1);
		//pPlaneObject->SetMesh(pPlaneMesh);
		//pPlaneObject->SetMaterial(pNormalMaterial);
		//pPlaneObject->SetTexture(pPlaneTexture);
		//pPlaneObject->Rotate(0.0f, 0.0f, 0.0f);
		//pPlaneObject->SetPosition(0.0f, 0.0f, 0.0f);
		//m_ppShaders[15]->AddObject(pPlaneObject);

		////시티홀 왼쪽으로 3개 house
		////house1
		//m_ppShaders[16] = new CTexturedIlluminatedShader(1);
		//m_ppShaders[16]->CreateShader(pd3dDevice);
		//m_ppShaders[16]->BuildObjects(pd3dDevice);

		//CGameObject *pHouse1Object1 = new CGameObject(1);
		//pHouse1Object1->SetMesh(pHouse1Mesh);
		//pHouse1Object1->SetMaterial(pNormalMaterial);
		//pHouse1Object1->SetTexture(pHouse1Texture);
		//pHouse1Object1->Rotate(0.0f, 0.0f, 0.0f);
		//pHouse1Object1->SetPosition(-200.0f, 0.0f, 300.0f);
		//m_ppShaders[16]->AddObject(pHouse1Object1);

		////house1
		//m_ppShaders[17] = new CTexturedIlluminatedShader(1);
		//m_ppShaders[17]->CreateShader(pd3dDevice);
		//m_ppShaders[17]->BuildObjects(pd3dDevice);

		//CGameObject *pHouse1Object2 = new CGameObject(1);
		//pHouse1Object2->SetMesh(pHouse1Mesh);
		//pHouse1Object2->SetMaterial(pNormalMaterial);
		//pHouse1Object2->SetTexture(pHouse1Texture);
		//pHouse1Object2->Rotate(0.0f, 0.0f, 0.0f);
		//pHouse1Object2->SetPosition(-300.0f, 0.0f, 300.0f);
		//m_ppShaders[17]->AddObject(pHouse1Object2);

		////house1
		//m_ppShaders[18] = new CTexturedIlluminatedShader(1);
		//m_ppShaders[18]->CreateShader(pd3dDevice);
		//m_ppShaders[18]->BuildObjects(pd3dDevice);

		//CGameObject *pHouse1Object3 = new CGameObject(1);
		//pHouse1Object3->SetMesh(pHouse1Mesh);
		//pHouse1Object3->SetMaterial(pNormalMaterial);
		//pHouse1Object3->SetTexture(pHouse1Texture);
		//pHouse1Object3->Rotate(0.0f, 0.0f, 0.0f);
		//pHouse1Object3->SetPosition(-400.0f, 0.0f, 300.0f);
		//m_ppShaders[18]->AddObject(pHouse1Object3);

		////building2 (리스폰) (뒷면)
		//m_ppShaders[19] = new CTexturedIlluminatedShader(1);
		//m_ppShaders[19]->CreateShader(pd3dDevice);
		//m_ppShaders[19]->BuildObjects(pd3dDevice);

		//CGameObject *pBuilding2Object2 = new CGameObject(1);
		//pBuilding2Object2->SetMesh(pBuilding2Mesh);
		//pBuilding2Object2->SetMaterial(pNormalMaterial);
		//pBuilding2Object2->SetTexture(pBuilding2Texture);
		//pBuilding2Object2->Rotate(0.0f, 180.0f, 0.0f);
		//pBuilding2Object2->SetPosition(0.0f, 0.0f, -500.0f);
		//m_ppShaders[19]->AddObject(pBuilding2Object2);

		////house2 (오른쪽)
		//m_ppShaders[20] = new CTexturedIlluminatedShader(1);
		//m_ppShaders[20]->CreateShader(pd3dDevice);
		//m_ppShaders[20]->BuildObjects(pd3dDevice);

		//CGameObject *pHouse2Object2 = new CGameObject(1);
		//pHouse2Object2->SetMesh(pHouse2Mesh);
		//pHouse2Object2->SetMaterial(pNormalMaterial);
		//pHouse2Object2->SetTexture(pHouse2Texture);
		//pHouse2Object2->Rotate(0.0f, 0.0f, 0.0f);
		//pHouse2Object2->SetPosition(0.0f, 60.0f, 0.0f);
		//m_ppShaders[20]->AddObject(pHouse2Object2);

	//	cout << pCityhallObject->GetPosition().x << ", " << pCityhallObject->GetPosition().y << ", " << pCityhallObject->GetPosition().z << endl;
   }
   //m_pTerrain = new CHeightMapTerrain(pd3dDevice, _T("Data\\HeightMap.raw"), 257, 257, 17, 17, d3dxvScale, d3dxColor);
   //m_pTerrain->SetMaterial(pNormalMaterial);



	
	CreateShaderVariables(pd3dDevice);
}

void CScene::SetHero()
{
	pMyObject->SetMesh(pBabarianMeshA);
	pMyObject->SetTexture(pBabarianTexture);
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

		case 'Z':
			if (!bCharaterRun && !bCharaterPunch)
			{
				cout << "Z " << pMyObject->m_HeroSelect << endl;
				/*m_ppShaders[2]->GetFBXMesh->SetAnimation(1);
				bCharaterRun = true;
				bCharaterPunch = false;*/
			}
			break;
		case 'X':
			if (!bCharaterPunch)
			{
				m_ppShaders[1]->GetFBXMesh->SetAnimation(ANI_ATTACK);
				
				bCharaterRun = false;
				bCharaterPunch = true;
			}
		default:
			break;
		}
		break;	// Client KeyDown


///////////////////////////////////////////////////////////////////////		


	case WM_KEYUP:

		switch (wParam)
		{
		case 'Z':
			if (bCharaterRun)
			{

			}
			break;
		case 'X':
			if (bCharaterPunch)
			{
				m_ppShaders[1]->GetFBXMesh->SetAnimation(ANI_IDLE);
				bCharaterPunch = false;
			}
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
	if (KEY_DOWN(VK_UP) && !UpKeyDown && activate) {
		UpKeyDown = true;
		dwDirection |= DIR_BACK;

		if (dwDirection) SendMovePacket(CS_KEYDOWN_UP);
	}
	if (KEY_DOWN(VK_DOWN) && !DownKeyDown && activate) {
		DownKeyDown = true;
		dwDirection |= DIR_FRONT;
		if (dwDirection) SendMovePacket(CS_KEYDOWN_DOWN);
	}
	if (KEY_DOWN(VK_LEFT) && !LeftKeyDown && activate) {
		LeftKeyDown = true;
		dwDirection |= DIR_LEFT;
		if (dwDirection) SendMovePacket(CS_KEYDOWN_LEFT);
	}
	if (KEY_DOWN(VK_RIGHT) && !RightKeyDown && activate) {
		RightKeyDown = true;
		dwDirection |= DIR_RIGHT;
		if (dwDirection) SendMovePacket(CS_KEYDOWN_RIGHT);
	}


	if (KEY_UP(VK_UP) && UpKeyDown && activate)
	{
		UpKeyDown = false;
		if (bCharaterRun)
		{
			m_ppShaders[1]->GetFBXMesh->SetAnimation(ANI_IDLE);
			bCharaterRun = false;
		}
		SendMovePacket(CS_KEYUP_UP);
	}	
	if (KEY_UP(VK_DOWN) && DownKeyDown && activate)
	{
		DownKeyDown = false;
		if (bCharaterRun)
		{
			m_ppShaders[1]->GetFBXMesh->SetAnimation(ANI_IDLE);
			bCharaterRun = false;
		}
		SendMovePacket(CS_KEYUP_DOWN);
	}
	if (KEY_UP(VK_LEFT) && LeftKeyDown && activate)
	{
		LeftKeyDown = false;
		if (bCharaterRun)
		{
			m_ppShaders[1]->GetFBXMesh->SetAnimation(ANI_IDLE);
			bCharaterRun = false;
		}
		SendMovePacket(CS_KEYUP_LEFT);
	}
	if (KEY_UP(VK_RIGHT) && RightKeyDown && activate)
	{
		RightKeyDown = false;
		if (bCharaterRun)
		{
			m_ppShaders[1]->GetFBXMesh->SetAnimation(ANI_IDLE);
			bCharaterRun = false;
		}
		SendMovePacket(CS_KEYUP_RIGHT);
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

	
	m_ppShaders[1]->GetFBXMesh->FBXFrameAdvance(fTimeElapsed); // SordMan
	m_ppShaders[2]->GetFBXMesh->FBXFrameAdvance(fTimeElapsed); // Healer
	//m_ppShaders[Babarian]->GetFBXMesh->FBXFrameAdvance(fTimeElapsed); // Babarian
	


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