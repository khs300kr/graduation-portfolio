#include "stdafx.h"
#include "Scene.h"
#include "CubeMesh.h"
#include "CubeObject.h"
#include "FBXMesh.h"
#include "CharacterShader.h"

static bool bCharaterRun = false;
static bool bCharaterPunch = false;
static bool bTest[8]{ false };

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

	//ColBox = false;
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
	CTexture *pHealerTexture = new CTexture(1, 1, 0, 0);
	D3DX11CreateShaderResourceViewFromFile(pd3dDevice, _T("../Data/Healer/Healer.png"), NULL, NULL, &pd3dsrvTexture, NULL);
	pHealerTexture->SetTexture(0, pd3dsrvTexture);
	pHealerTexture->SetSampler(0, pd3dSamplerState);
	pd3dsrvTexture->Release();
	pd3dsrvTexture = NULL;
	CTexture *pSordManTexture = new CTexture(1, 1, 0, 0);
	D3DX11CreateShaderResourceViewFromFile(pd3dDevice, _T("../Data/Sordman/Sordman.png"), NULL, NULL, &pd3dsrvTexture, NULL);
	pSordManTexture->SetTexture(0, pd3dsrvTexture);
	pSordManTexture->SetSampler(0, pd3dSamplerState);
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
	

	D3DXVECTOR3 d3dxvScale(8.0f, 2.0f, 8.0f);
	D3DXCOLOR d3dxColor(0.0f, 0.2f, 0.0f, 0.0f);
	/*지형을 높이 맵 이미지 파일을 사용하여 생성한다.
	높이 맵 이미지의 크기는 가로x세로(257x257)이고 격자 메쉬의 크기는 가로x세로(17x17)이다.
	지형 전체는 가로 방향으로 16개, 세로 방향으로 16의 격자 메쉬를 가진다.
	지형을 구성하는 격자 메쉬의 개수는 총 256(16x16)개가 된다.*/


	// ③ Object용 Material과 Shader를 생성 (Skybox는 쉐이더 내부에서 자체적으로 생성)
	CMaterial *pNormalMaterial = new CMaterial();
	pNormalMaterial->m_Material.m_d3dxcDiffuse = D3DXCOLOR(0.8f, 0.8f, 0.8f, 1.0f);
	pNormalMaterial->m_Material.m_d3dxcAmbient = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pNormalMaterial->m_Material.m_d3dxcSpecular = D3DXCOLOR(1.0f, 1.0f, 1.0f, 5.0f);
	pNormalMaterial->m_Material.m_d3dxcEmissive = D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f);

	// ④ 쉐이더에 적용할 메쉬(들) 생성	
	CMesh *pTestMesh = new CFBXMesh(pd3dDevice, "../Data/Healer/Healer.data", 0.1f);
	CMesh *pSordManMesh = new CFBXMesh(pd3dDevice, "../Data/Sordman/Sordman.data", 0.1f);

	//plane
	CMesh *pPlaneMesh = new CFBXMesh(pd3dDevice, "../Data/plane1.data", 1.f);

	CMesh *pBuilding1Mesh = new CFBXMesh(pd3dDevice, "../Data/building/building1/building1.data", 0.6f);
	CMesh *pBuilding2Mesh = new CFBXMesh(pd3dDevice, "../Data/building/building2/building2.data", 1.f);
	CMesh *pHouse1Mesh = new CFBXMesh(pd3dDevice, "../Data/building/house1/housetest.data", 1.0f);
	CMesh *pHouse2Mesh = new CFBXMesh(pd3dDevice, "../Data/building/house2/house2.data", 0.7f);
	CMesh *pCityhallMesh = new CFBXMesh(pd3dDevice, "../Data/building/cityhall/cityhall.data", 1.f);


	

	//
	// 일반 쉐이더 선언부
	/////////////////////////////////////////////////////////////////////////
	m_nShaders = 19;	// Skybox
	m_ppShaders = new CShader*[m_nShaders];

	// ⑤ SkyBox용 Shader를 생성
	m_ppShaders[0] = new CSkyBoxShader();
	m_ppShaders[0]->CreateShader(pd3dDevice);
	m_ppShaders[0]->BuildObjects(pd3dDevice);

	{
		m_ppShaders[1] = new CCharacterShader(1);
		m_ppShaders[1]->CreateShader(pd3dDevice);
		m_ppShaders[1]->BuildObjects(pd3dDevice);

		CGameObject *pTestObject = new CGameObject(1);
		pTestObject->SetMesh(pTestMesh);
		pTestObject->SetMaterial(pNormalMaterial);
		pTestObject->SetTexture(pHealerTexture);
		pTestObject->Rotate(0.0f, 0.0f, 0.0f);
		pTestObject->SetPosition(20.0f, 0.0f, -500.0f);
		m_ppShaders[1]->AddObject(pTestObject);


		m_ppShaders[2] = new CCharacterShader(1);
		m_ppShaders[2]->CreateShader(pd3dDevice);
		m_ppShaders[2]->BuildObjects(pd3dDevice);

		pSordmanObject = new CSordMan(1);
		pSordmanObject->SetMesh(pSordManMesh);
		pSordmanObject->SetMaterial(pNormalMaterial);
		pSordmanObject->SetTexture(pSordManTexture);
		pSordmanObject->Rotate(0.0f, 180.0f, 0.0f);
		pSordmanObject->SetPosition(0.0f, 0.0f, -500.0f);
		//pSordmanObject->SetOOBB(XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(1.f, 1.f, 1.f), XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f));
		pSordmanObject->SetOOBB(XMFLOAT3(pSordmanObject->GetPosition().x, pSordmanObject->GetPosition().y, pSordmanObject->GetPosition().z),
			XMFLOAT3(0.1f, 0.1f, 0.1f), XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f));
		
		m_ppShaders[2]->AddObject(pSordmanObject);

		CMesh *BoxMesh = BoundBox->CubeMesh(100, RED, 0);;

		BoundBox = new CGameObject(1);
		BoundBox->SetMesh(BoxMesh);
		BoundBox->SetPosition(0, 0, -500);


		//plane
		m_ppShaders[3] = new CTexturedIlluminatedShader(1);
		m_ppShaders[3]->CreateShader(pd3dDevice);
		m_ppShaders[3]->BuildObjects(pd3dDevice);

		CGameObject *pPlaneObject = new CGameObject(1);
		pPlaneObject->SetMesh(pPlaneMesh);
		pPlaneObject->SetMaterial(pNormalMaterial);
		pPlaneObject->SetTexture(pPlaneTexture);
		pPlaneObject->Rotate(0.0f, 0.0f, 0.0f);
		pPlaneObject->SetPosition(0.0f, 0.0f, 0.0f);
		m_ppShaders[3]->AddObject(pPlaneObject);

		//building2 (리스폰) (적군)
		m_ppShaders[4] = new CTexturedIlluminatedShader(1);
		m_ppShaders[4]->CreateShader(pd3dDevice);
		m_ppShaders[4]->BuildObjects(pd3dDevice);

		CGameObject *pBuilding2Object = new CGameObject(1);
		pBuilding2Object->SetMesh(pBuilding2Mesh);
		pBuilding2Object->SetMaterial(pNormalMaterial);
		pBuilding2Object->SetTexture(pBuilding2Texture);
		pBuilding2Object->Rotate(0.0f, 0.0f, 0.0f);
		pBuilding2Object->SetPosition(0.0f, 0.0f, 500.0f);
		m_ppShaders[4]->AddObject(pBuilding2Object);

		//building2 (리스폰) (아군)
		m_ppShaders[5] = new CTexturedIlluminatedShader(1);
		m_ppShaders[5]->CreateShader(pd3dDevice);
		m_ppShaders[5]->BuildObjects(pd3dDevice);

		CGameObject *pBuilding2Object2 = new CGameObject(1);
		pBuilding2Object2->SetMesh(pBuilding2Mesh);
		pBuilding2Object2->SetMaterial(pNormalMaterial);
		pBuilding2Object2->SetTexture(pBuilding2Texture);
		pBuilding2Object2->Rotate(0.0f, 180.0f, 0.0f);
		pBuilding2Object2->SetPosition(0.0f, -10.0f, -500.0f);
		m_ppShaders[5]->AddObject(pBuilding2Object2);


		// 건물 쉐이더 생성
		for (int i = 6; i < m_nShaders; ++i)
		{
			m_ppShaders[i] = new CTexturedIlluminatedShader(1);
			m_ppShaders[i]->CreateShader(pd3dDevice);
			m_ppShaders[i]->BuildObjects(pd3dDevice);

		}

		// 건물 오브젝트 생성
		//CGameObject *pHouse1Object[14]; 
		for(int i = 0; i < 14; ++i)
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
		for (int i = 6; i < m_nShaders; ++i)
		{
			m_ppShaders[i]->AddObject(pHouse1Object[i - 6]);
		}

		

		////house1
		//m_ppShaders[6] = new CTexturedIlluminatedShader(1);
		//m_ppShaders[6]->CreateShader(pd3dDevice);
		//m_ppShaders[6]->BuildObjects(pd3dDevice);

		//CGameObject *pHouse1Object = new CGameObject(1);
		//pHouse1Object->SetMesh(pHouse1Mesh);
		//pHouse1Object->SetMaterial(pNormalMaterial);
		//pHouse1Object->SetTexture(pHouse1Texture);
		//pHouse1Object->Rotate(0.0f, 0.0f, 0.0f);
		//pHouse1Object->SetPosition(50.0f, 0.0f, -300.0f);
		//m_ppShaders[6]->AddObject(pHouse1Object);

		////house1
		//m_ppShaders[7] = new CTexturedIlluminatedShader(1);
		//m_ppShaders[7]->CreateShader(pd3dDevice);
		//m_ppShaders[7]->BuildObjects(pd3dDevice);

		//CGameObject *pHouse1Object2 = new CGameObject(1);
		//pHouse1Object2->SetMesh(pHouse1Mesh);
		//pHouse1Object2->SetMaterial(pNormalMaterial);
		//pHouse1Object2->SetTexture(pHouse1Texture);
		//pHouse1Object2->Rotate(0.0f, 0.0f, 0.0f);
		//pHouse1Object2->SetPosition(-150.0f, 0.0f, -300.0f);
		//m_ppShaders[7]->AddObject(pHouse1Object2);

		////house1
		//m_ppShaders[8] = new CTexturedIlluminatedShader(1);
		//m_ppShaders[8]->CreateShader(pd3dDevice);
		//m_ppShaders[8]->BuildObjects(pd3dDevice);

		//CGameObject *pHouse1Object3 = new CGameObject(1);
		//pHouse1Object3->SetMesh(pHouse1Mesh);
		//pHouse1Object3->SetMaterial(pNormalMaterial);
		//pHouse1Object3->SetTexture(pHouse1Texture);
		//pHouse1Object3->Rotate(0.0f, 0.0f, 0.0f);
		//pHouse1Object3->SetPosition(250.0f, 0.0f, -300.0f);
		//m_ppShaders[8]->AddObject(pHouse1Object3);
	
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
		case VK_LEFT:
			if (!LeftKeyDown)
			{
				LeftKeyDown = true;
				
			}
			break;
		case VK_RIGHT:
			if (!RightKeyDown)
			{
				RightKeyDown = true;
			}
			break;
		case VK_UP:
			if (!UpKeyDown)
			{
				UpKeyDown = true;
			}
			break;
		case VK_DOWN:
			if (!DownKeyDown)
			{
				DownKeyDown = true;
			}
			break;

		case 'Z':
			if (!bCharaterRun && !bCharaterPunch)
			{
				/*m_ppShaders[2]->GetFBXMesh->SetAnimation(1);
				bCharaterRun = true;
				bCharaterPunch = false;*/
			}
			break;
		/*case 'X':
			if (!bCharaterPunch)
			{
				m_ppShaders[2]->GetFBXMesh->SetAnimation(2);

				bCharaterRun = false;
				bCharaterPunch = true;
			}*/
		case '1':
			if (!bTest[0] && !bTest[1] && !bTest[2] && !bTest[3] && !bTest[4] && !bTest[5] && !bTest[6] && !bTest[7] )
			{
				m_ppShaders[2]->GetFBXMesh->SetAnimation(2);
				
				for (int i = 0; i < 8; ++i)
				{
					if (i == 0)
						bTest[i] = true;
					else
						bTest[i] = false;

					cout << bTest[i];
				}
				cout << endl;
				
			}
		case '2':
			if (!bTest[0] && !bTest[1] && !bTest[2] && !bTest[3] && !bTest[4] && !bTest[5] && !bTest[6] && !bTest[7])
			{
				m_ppShaders[2]->GetFBXMesh->SetAnimation(3);

				for (int i = 0; i < 8; ++i)
				{
					if(i == 1)
						bTest[i] = true;
					else
						bTest[i] = false;
					cout << bTest[i]  ;
				}
				cout << endl;
			}
		case '3':
			if (!bTest[0] && !bTest[1] && !bTest[2] && !bTest[3] && !bTest[4] && !bTest[5] && !bTest[6] && !bTest[7])
			{
				m_ppShaders[2]->GetFBXMesh->SetAnimation(4);

				for (int i = 0; i < 8; ++i)
				{
					if (i == 2)
						bTest[i] = true;
					else
						bTest[i] = false;
					cout << bTest[i];
				}
				cout << endl;
			}
		case '4':
			if (!bTest[0] && !bTest[1] && !bTest[2] && !bTest[3] && !bTest[4] && !bTest[5] && !bTest[6] && !bTest[7])
			{
				m_ppShaders[2]->GetFBXMesh->SetAnimation(5);

				for (int i = 0; i < 8; ++i)
				{
					if (i == 3)
						bTest[i] = true;
					else
						bTest[i] = false;
					cout << bTest[i];
				}
				cout << endl;
			}
		case '5':
			if (!bTest[0] && !bTest[1] && !bTest[2] && !bTest[3] && !bTest[4] && !bTest[5] && !bTest[6] && !bTest[7])
			{
				m_ppShaders[2]->GetFBXMesh->SetAnimation(6);

				for (int i = 0; i < 8; ++i)
				{
					if (i == 4)
						bTest[i] = true;
					else
						bTest[i] = false;
					cout << bTest[i];
				}
				cout << endl;
			}
		case '6':
			if (!bTest[0] && !bTest[1] && !bTest[2] && !bTest[3] && !bTest[4] && !bTest[5] && !bTest[6] && !bTest[7])
			{
				m_ppShaders[2]->GetFBXMesh->SetAnimation(7);

				for (int i = 0; i < 8; ++i)
				{
					if (i == 5)
						bTest[i] = true;
					else
						bTest[i] = false;
					cout << bTest[i];
				}
				cout << endl;
			}
		case '7':
			if (!bTest[0] && !bTest[1] && !bTest[2] && !bTest[3] && !bTest[4] && !bTest[5] && !bTest[6] && !bTest[7])
			{
				m_ppShaders[2]->GetFBXMesh->SetAnimation(8);

				for (int i = 0; i < 8; ++i)
				{
					if (i == 6)
						bTest[i] = true;
					else
						bTest[i] = false;
					cout << bTest[i];
				}
				cout << endl;
			}
		case '8':
			if (!bTest[0] && !bTest[1] && !bTest[2] && !bTest[3] && !bTest[4] && !bTest[5] && !bTest[6] && !bTest[7])
			{
				m_ppShaders[2]->GetFBXMesh->SetAnimation(9);

				for (int i = 0; i < 8; ++i)
				{
					if (i == 7)
						bTest[i] = true;
					else
						bTest[i] = false;
					cout << bTest[i];
				}
				cout << endl;
			}
		default:
			break;
		}
		break;
	case WM_KEYUP:
		switch (wParam)
		{
		case VK_LEFT:
			if (LeftKeyDown)
			{
				LeftKeyDown = false;
				if (bCharaterRun)
				{
					m_ppShaders[2]->GetFBXMesh->SetAnimation(0);
					bCharaterRun = false;
				}
			}
			break;
		case VK_RIGHT:
			if (RightKeyDown)
			{
				RightKeyDown = false;
				if (bCharaterRun)
				{
					m_ppShaders[2]->GetFBXMesh->SetAnimation(0);
					bCharaterRun = false;
				}
			}
			break;
		case VK_UP:
			if (UpKeyDown)
			{
				UpKeyDown = false;
				if (bCharaterRun)
				{
					m_ppShaders[2]->GetFBXMesh->SetAnimation(0);
					bCharaterRun = false;
				}
			}
			break;
		case VK_DOWN:
			if (DownKeyDown)
			{
				DownKeyDown = false;
				if (bCharaterRun)
				{
					m_ppShaders[2]->GetFBXMesh->SetAnimation(0);
					bCharaterRun = false;
				}
			}
			break;
		case 'Z':
			if (bCharaterRun)
			{
				/*m_ppShaders[2]->GetFBXMesh->SetAnimation(0);
				bCharaterRun = false;*/
			}
			break;
		/*case 'X':
			if (bCharaterPunch)
			{
				m_ppShaders[2]->GetFBXMesh->SetAnimation(0);
				bCharaterPunch = false;
			}*/
		case '1':
			if (bTest[0])
			{
				m_ppShaders[2]->GetFBXMesh->SetAnimation(0);

				bTest[0] = false;
			}
		case '2':
			if (bTest[1])
			{
				m_ppShaders[2]->GetFBXMesh->SetAnimation(0);

				bTest[1] = false;
			}
		case '3':
			if (bTest[2])
			{
				m_ppShaders[2]->GetFBXMesh->SetAnimation(0);

				bTest[2] = false;
			}
		case '4':
			if (bTest[3])
			{
				m_ppShaders[2]->GetFBXMesh->SetAnimation(0);

				bTest[3] = false;
			}
		case '5':
			if (bTest[4])
			{
				m_ppShaders[2]->GetFBXMesh->SetAnimation(0);

				bTest[4] = false;
			}
		case '6':
			if (bTest[5])
			{
				m_ppShaders[2]->GetFBXMesh->SetAnimation(0);

				bTest[5] = false;
			}
		case '7':
			if (bTest[6])
			{
				m_ppShaders[2]->GetFBXMesh->SetAnimation(0);

				bTest[6] = false;
			}
		case '8':
			if (bTest[7])
			{
				m_ppShaders[2]->GetFBXMesh->SetAnimation(0);

				bTest[7] = false;
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


bool CScene::ProcessInput()
{
	return false;
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

void CScene::UpdateShaderVariable(ID3D11DeviceContext *pd3dDeviceContext, LIGHTS *pLights)
{
	D3D11_MAPPED_SUBRESOURCE d3dMappedResource;
	pd3dDeviceContext->Map(m_pd3dcbLights, 0, D3D11_MAP_WRITE_DISCARD, 0, &d3dMappedResource);
	LIGHTS *pcbLight = (LIGHTS *)d3dMappedResource.pData;
	memcpy(pcbLight, pLights, sizeof(LIGHTS));
	pd3dDeviceContext->Unmap(m_pd3dcbLights, 0);
	pd3dDeviceContext->PSSetConstantBuffers(PS_SLOT_LIGHT, 1, &m_pd3dcbLights);
}

void CScene::CollisionCheck(int i)
{
	if (pHouse1Object[i]->m_xmOOBB.Intersects(pSordmanObject->m_xmOOBB))
	{
		pHouse1Object[i]->m_pCollider = pSordmanObject;
		pSordmanObject->m_pCollider = pHouse1Object[i];

		pHouse1Object[i]->SetColCheck(true);
		//ColBox = true;
	}
	if (!(pHouse1Object[i]->m_xmOOBB.Intersects(pSordmanObject->m_xmOOBB)))
	{
		pHouse1Object[i]->SetColCheck(false);
		//ColBox = false;
	}
}

void CScene::AnimateObjects(float fTimeElapsed)
{
	pSordmanObject->SetOOBB(XMFLOAT3(pSordmanObject->GetPosition().x, pSordmanObject->GetPosition().y, pSordmanObject->GetPosition().z),
		XMFLOAT3(1.f, 1.f, 1.f), XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f));
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

	
	m_ppShaders[1]->GetFBXMesh->FBXFrameAdvance(fTimeElapsed);
	m_ppShaders[2]->GetFBXMesh->FBXFrameAdvance(fTimeElapsed);


	if (LeftKeyDown || RightKeyDown || UpKeyDown || DownKeyDown)
	{
		if (LeftKeyDown && UpKeyDown)
		{
			pSordmanObject->Rotate(0.0f, 135.0f, 0.0f);
			//pSordmanObject->m_d3dxvDirection = D3DXVECTOR3(-1.0f, 0.0f, 1.0f);
		}
		else if (LeftKeyDown && DownKeyDown)
		{
			pSordmanObject->Rotate(0.0f, 45.0f, 0.0f);
			//pSordmanObject->m_d3dxvDirection = D3DXVECTOR3(-1.0f, 0.0f, -1.0f);
		}
		else if (RightKeyDown && UpKeyDown)
		{
			pSordmanObject->Rotate(0.0f, -135.0f, 0.0f);
			//pSordmanObject->m_d3dxvDirection = D3DXVECTOR3(1.0f, 0.0f, 1.0f);
		}
		else if (RightKeyDown && DownKeyDown)
		{
			pSordmanObject->Rotate(0.0f, -45.0f, 0.0f);
			//pSordmanObject->m_d3dxvDirection = D3DXVECTOR3(1.0f, 0.0f, -1.0f);
		}

		else
		{
			if (LeftKeyDown)
			{
				pSordmanObject->Rotate(0.0f, 90.0f, 0.0f);
				//pSordmanObject->m_d3dxvDirection = D3DXVECTOR3(-1.0f, 0.0f, 0.0f);
			}
			else if (RightKeyDown)
			{
				pSordmanObject->Rotate(0.0f, -90.0f, 0.0f);
				//pSordmanObject->m_d3dxvDirection = D3DXVECTOR3(1.0f, 0.0f, 0.0f);
			}
			else if (UpKeyDown)
			{
				pSordmanObject->Rotate(0.0f, 180.0f, 0.0f);
				//pSordmanObject->m_d3dxvDirection = D3DXVECTOR3(0.0f, 0.0f, 1.0f);
			}
			else if (DownKeyDown)
			{
				pSordmanObject->Rotate(0.0f, 0.0f, 0.0f);
				//pSordmanObject->m_d3dxvDirection = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
			}
		}

		if (!bCharaterRun && !bCharaterPunch)
		{
			m_ppShaders[2]->GetFBXMesh->SetAnimation(1);
			bCharaterRun = true;
			bCharaterPunch = false;
		}
	}
	/*for (int i = 0; i < 14; i++) pHouse1Object[i]->m_pCollider = NULL;
	for (int i = 0; i < 14; ++i)
	{
		if (pHouse1Object[i]->m_xmOOBB.Intersects(pSordmanObject->m_xmOOBB))
		{
			pHouse1Object[i]->m_pCollider = pSordmanObject;
			pSordmanObject->m_pCollider = pHouse1Object[i];
			
			pSordmanObject->SetSpeed(pSordmanObject->GetSpeed() * -1);
		}
		if (!(pHouse1Object[i]->m_xmOOBB.Intersects(pSordmanObject->m_xmOOBB)))
		{
			pSordmanObject->SetSpeed(pSordmanObject->GetSpeed() * -1);
		}
	}*/



	CollisionCheck(0);
	CollisionCheck(1);

	/*if (pHouse1Object[0]->m_xmOOBB.Intersects(pSordmanObject->m_xmOOBB))
	{
		pHouse1Object[0]->m_pCollider = pSordmanObject;
		pSordmanObject->m_pCollider = pHouse1Object[0];

		ColBox = true;
	}
	if (!(pHouse1Object[0]->m_xmOOBB.Intersects(pSordmanObject->m_xmOOBB)))
	{
		ColBox = false;
	}*/


	//cout << ColBox << endl;
	//m_pCamera->SetPosition(D3DXVECTOR3(pSordmanObject->GetPosition().x, pSordmanObject->GetPosition().y, pSordmanObject->GetPosition().z - 40));
	
	//cout << pSordmanObject->m_d3dxvDirection.x << ends << pSordmanObject->m_d3dxvDirection.y << ends << pSordmanObject->m_d3dxvDirection.z << endl;
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
