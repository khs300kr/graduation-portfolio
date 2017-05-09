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

	// 테스트빌딩
	pd3dsrvTexture = NULL;
	CTexture *pBuildingTexture = new CTexture(1, 1, 0, 0);
	D3DX11CreateShaderResourceViewFromFile(pd3dDevice, _T("../Data/Objects/house1.png"), NULL, NULL, &pd3dsrvTexture, NULL);
	pBuildingTexture->SetTexture(0, pd3dsrvTexture);
	pBuildingTexture->SetSampler(0, pd3dSamplerState);
	pd3dsrvTexture->Release();
	

	// ③ Object용 Material과 Shader를 생성 (Skybox는 쉐이더 내부에서 자체적으로 생성)
	CMaterial *pNormalMaterial = new CMaterial();
	pNormalMaterial->m_Material.m_d3dxcDiffuse = D3DXCOLOR(0.8f, 0.8f, 0.8f, 1.0f);
	pNormalMaterial->m_Material.m_d3dxcAmbient = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pNormalMaterial->m_Material.m_d3dxcSpecular = D3DXCOLOR(1.0f, 1.0f, 1.0f, 5.0f);
	pNormalMaterial->m_Material.m_d3dxcEmissive = D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f);

	// ④ 쉐이더에 적용할 메쉬(들) 생성	
	pHealerMeshA = new CFBXMesh(pd3dDevice, "../Data/Healer.data", 0.15f);
	pHealerMeshB = new CFBXMesh(pd3dDevice, "../Data/Healer.data", 0.15f);
	pSordManMeshA = new CFBXMesh(pd3dDevice, "../Data/SordMan.data", 0.1f);
	pSordManMeshB = new CFBXMesh(pd3dDevice, "../Data/SordMan.data", 0.1f);
	pBabarianMeshA = new CFBXMesh(pd3dDevice, "../Data/Babarian.data", 0.1f);
	pBabarianMeshB = new CFBXMesh(pd3dDevice, "../Data/Babarian.data", 0.1f);

	// map objects
	CMesh *pBuildingMesh = new CFBXMesh(pd3dDevice, "../Data/Objects/house1.data", 0.1f);

	
	// 일반 쉐이더 선언부
	/////////////////////////////////////////////////////////////////////////
	m_nShaders = 11;	// Skybox
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

		pMyObject = new CHeroManager(1);

		if (pMyObject->m_Team == A_TEAM)
		{
			if (pMyObject->m_CharSelect == SordMan)
			{
				pMyObject->SetMesh(pSordManMeshA);
				pMyObject->SetTexture(pSordManTexture);

			}
			else if (pMyObject->m_CharSelect == Healer)
			{
				pMyObject->SetMesh(pHealerMeshA);
				pMyObject->SetTexture(pHealerTexture);

			}
			else if (pMyObject->m_CharSelect == Babarian)
			{
				pMyObject->SetMesh(pBabarianMeshA);
				pMyObject->SetTexture(pBabarianTexture);
			}
		}

		else if (pMyObject->m_Team == B_TEAM)
		{
			if (pMyObject->m_CharSelect == SordMan)
			{
				pMyObject->SetMesh(pSordManMeshB);
				pMyObject->SetTexture(pSordManTexture);

			}
			else if (pMyObject->m_CharSelect == Healer)
			{
				pMyObject->SetMesh(pHealerMeshB);
				pMyObject->SetTexture(pHealerTexture);

			}
			else if (pMyObject->m_CharSelect == Babarian)
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
		for (int i = 0; i < MAX_USER; ++i)
		{
			m_ppShaders[i+2] = new CCharacterShader(1);
			m_ppShaders[i+2]->CreateShader(pd3dDevice);
			m_ppShaders[i+2]->BuildObjects(pd3dDevice);

			pOtherObject[i] = new CHeroManager(1);

			if (pOtherObject[i]->m_Team == A_TEAM)
			{
				if (pOtherObject[i]->m_CharSelect == SordMan)
				{
					pOtherObject[i]->SetMesh(pSordManMeshA);
					pOtherObject[i]->SetTexture(pSordManTexture);

				}
				else if (pOtherObject[i]->m_CharSelect == Healer)
				{
					pOtherObject[i]->SetMesh(pHealerMeshA);
					pOtherObject[i]->SetTexture(pHealerTexture);

				}
				else if (pOtherObject[i]->m_CharSelect == Babarian)
				{
					pOtherObject[i]->SetMesh(pBabarianMeshA);
					pOtherObject[i]->SetTexture(pBabarianTexture);
				}
			}

			else if (pOtherObject[i]->m_Team == B_TEAM)
			{
				if (pOtherObject[i]->m_CharSelect == SordMan)
				{
					pOtherObject[i]->SetMesh(pSordManMeshB);
					pOtherObject[i]->SetTexture(pSordManTexture);

				}
				else if (pOtherObject[i]->m_CharSelect == Healer)
				{
					pOtherObject[i]->SetMesh(pHealerMeshB);
					pOtherObject[i]->SetTexture(pHealerTexture);

				}
				else if (pOtherObject[i]->m_CharSelect == Babarian)
				{
					pOtherObject[i]->SetMesh(pBabarianMeshB);
					pOtherObject[i]->SetTexture(pBabarianTexture);
				}
			}
			pOtherObject[i]->SetMaterial(pNormalMaterial);
			pOtherObject[i]->SetPosition(0.0f, -3000.0f, 0.0f);
			pOtherObject[i]->Rotate(0.0f, 0.0f, 0.0f);

			m_ppShaders[i+2]->AddObject(pOtherObject[i]);
		}
		

		//for (int i = 0; i < MAX_USER; ++i)
		//{
		//	m_ppShaders[i+2] = new CCharacterShader(1);
		//	m_ppShaders[i+2]->CreateShader(pd3dDevice);
		//	m_ppShaders[i+2]->BuildObjects(pd3dDevice);
		//
		//	pOtherObject[i] = new CHeroManager(1);
		//	pOtherObject[i]->SetMesh(pHealerMesh);
		//	pOtherObject[i]->SetMaterial(pNormalMaterial);
		//	pOtherObject[i]->SetTexture(pHealerTexture);
		//	pOtherObject[i]->SetPosition(0.0f, -3000.0f, -10.0f);
		//	pOtherObject[i]->Rotate(0.0f, 0.0f, 0.0f);
		//	

		//	m_ppShaders[i+2]->AddObject(pOtherObject[i]);

		//}
		

		m_ppShaders[10] = new CTexturedIlluminatedShader(1);
		m_ppShaders[10]->CreateShader(pd3dDevice);
		m_ppShaders[10]->BuildObjects(pd3dDevice);

		CGameObject *pBuildingObject = new CGameObject(1);
		pBuildingObject->SetMesh(pBuildingMesh);
		pBuildingObject->SetMaterial(pNormalMaterial);
		pBuildingObject->SetTexture(pBuildingTexture);
		pBuildingObject->SetPosition(0.0f, 0.0f, 30.0f);
		pBuildingObject->Rotate(0.0f, 0.0f, 0.0f);
		
		m_ppShaders[10]->AddObject(pBuildingObject);
	}
	
	
	CreateShaderVariables(pd3dDevice);
}

void CScene::Create(CHeroManager* Object, int team, int hero)
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

		case 'Z':
			if (!bCharaterRun && !bCharaterPunch)
			{
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
		if (bCharaterRun)
		{
			m_ppShaders[1]->GetFBXMesh->SetAnimation(ANI_IDLE);
			bCharaterRun = false;
		}
		SendMovePacket(CS_KEYUP_UP);
	}	
	if (KEY_UP(VK_DOWN) && DownKeyDown)
	{
		DownKeyDown = false;
		if (bCharaterRun)
		{
			m_ppShaders[1]->GetFBXMesh->SetAnimation(ANI_IDLE);
			bCharaterRun = false;
		}
		SendMovePacket(CS_KEYUP_DOWN);
	}
	if (KEY_UP(VK_LEFT) && LeftKeyDown)
	{
		LeftKeyDown = false;
		if (bCharaterRun)
		{
			m_ppShaders[1]->GetFBXMesh->SetAnimation(ANI_IDLE);
			bCharaterRun = false;
		}
		SendMovePacket(CS_KEYUP_LEFT);
	}
	if (KEY_UP(VK_RIGHT) && RightKeyDown)
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
	//m_ppShaders[Healer]->GetFBXMesh->FBXFrameAdvance(fTimeElapsed); // Healer
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