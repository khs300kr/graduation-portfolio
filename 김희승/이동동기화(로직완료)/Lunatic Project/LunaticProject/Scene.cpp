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
	CTexture *pDrayerTexture = new CTexture(1, 1, 0, 0);
	D3DX11CreateShaderResourceViewFromFile(pd3dDevice, _T("../Data/Drayer_diffuse.png"), NULL, NULL, &pd3dsrvTexture, NULL);
	pDrayerTexture->SetTexture(0, pd3dsrvTexture);
	pDrayerTexture->SetSampler(0, pd3dSamplerState);
	pd3dsrvTexture->Release();
	pd3dsrvTexture = NULL;
	CTexture *pSordManTexture = new CTexture(1, 1, 0, 0);
	D3DX11CreateShaderResourceViewFromFile(pd3dDevice, _T("../Data/SordMan.png"), NULL, NULL, &pd3dsrvTexture, NULL);
	pSordManTexture->SetTexture(0, pd3dsrvTexture);
	pSordManTexture->SetSampler(0, pd3dSamplerState);
	pd3dsrvTexture->Release();
	// �׽�Ʈ����
	pd3dsrvTexture = NULL;
	CTexture *pBuildingTexture = new CTexture(1, 1, 0, 0);
	D3DX11CreateShaderResourceViewFromFile(pd3dDevice, _T("../Data/buildz4.jpg"), NULL, NULL, &pd3dsrvTexture, NULL);
	pBuildingTexture->SetTexture(0, pd3dsrvTexture);
	pBuildingTexture->SetSampler(0, pd3dSamplerState);
	pd3dsrvTexture->Release();
	


	// �� Object�� Material�� Shader�� ���� (Skybox�� ���̴� ���ο��� ��ü������ ����)
	CMaterial *pNormalMaterial = new CMaterial();
	pNormalMaterial->m_Material.m_d3dxcDiffuse = D3DXCOLOR(0.8f, 0.8f, 0.8f, 1.0f);
	pNormalMaterial->m_Material.m_d3dxcAmbient = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pNormalMaterial->m_Material.m_d3dxcSpecular = D3DXCOLOR(1.0f, 1.0f, 1.0f, 5.0f);
	pNormalMaterial->m_Material.m_d3dxcEmissive = D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f);

	// �� ���̴��� ������ �޽�(��) ����	
	CMesh *pTestMesh = new CFBXMesh(pd3dDevice, "../Data/man1.data", 0.1f);
	CMesh *pSordManMesh = new CFBXMesh(pd3dDevice, "../Data/drayer_animation.data", 0.01f);
	CMesh *pBuildingMesh = new CFBXMesh(pd3dDevice, "../Data/building-commercial_03.data", 1.0f);

	//
	// �Ϲ� ���̴� �����
	/////////////////////////////////////////////////////////////////////////
	m_nShaders = 4;	// Skybox
	m_ppShaders = new CShader*[m_nShaders];

	// �� SkyBox�� Shader�� ����
	m_ppShaders[0] = new CSkyBoxShader();
	m_ppShaders[0]->CreateShader(pd3dDevice);
	m_ppShaders[0]->BuildObjects(pd3dDevice);

	{
		m_ppShaders[1] = new CCharacterShader(1);
		m_ppShaders[1]->CreateShader(pd3dDevice);
		m_ppShaders[1]->BuildObjects(pd3dDevice);

		pTestObject = new CSordMan(1);
		pTestObject->SetMesh(pTestMesh);
		pTestObject->SetMaterial(pNormalMaterial);
		pTestObject->SetTexture(pSordManTexture);
		pTestObject->Rotate(0.0f, 0.0f, 0.0f);
		//pTestObject->SetPosition(20.0f, 0.0f, 0.0f);
		m_ppShaders[1]->AddObject(pTestObject);


		m_ppShaders[2] = new CCharacterShader(1);
		m_ppShaders[2]->CreateShader(pd3dDevice);
		m_ppShaders[2]->BuildObjects(pd3dDevice);

		pSordmanObject = new CSordMan(1);
		pSordmanObject->SetMesh(pSordManMesh);
		pSordmanObject->SetMaterial(pNormalMaterial);
		pSordmanObject->SetTexture(pDrayerTexture);
		pSordmanObject->Rotate(-90.0f, 0.0f, 0.0f);
		//pSordmanObject->SetPosition(0.0f, 0.0f, 0.0f);
		
		m_ppShaders[2]->AddObject(pSordmanObject);


		m_ppShaders[3] = new CTexturedIlluminatedShader(1);
		m_ppShaders[3]->CreateShader(pd3dDevice);
		m_ppShaders[3]->BuildObjects(pd3dDevice);

		CGameObject *pBuildingObject = new CGameObject(1);
		pBuildingObject->SetMesh(pBuildingMesh);
		pBuildingObject->SetMaterial(pNormalMaterial);
		pBuildingObject->SetTexture(pBuildingTexture);
		pBuildingObject->Rotate(0.0f, 0.0f, 0.0f);
		pBuildingObject->SetPosition(0.0f, 0.0f, 30.0f);
		m_ppShaders[3]->AddObject(pBuildingObject);
	}
	
	
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
	//{
	//	// server send(Move)
	//	if(KeyDownForServer == 0)
	//		KeyDownForServer = 1;
	//	DWORD dwDirection = 0;
	//	if (wParam == VK_LEFT && !LeftKeyDown) {
	//		LeftKeyDown = true; dwDirection |= DIR_LEFT;
	//	}
	//	if (wParam == VK_RIGHT && !RightKeyDown) {
	//		RightKeyDown = true; dwDirection |= DIR_RIGHT;
	//	}
	//	if (wParam == VK_UP && !UpKeyDown) {
	//		UpKeyDown = true; dwDirection |= DIR_BACK;
	//	}
	//	if (wParam == VK_DOWN && !DownKeyDown) {
	//		DownKeyDown = true; dwDirection |= DIR_FRONT;
	//	}
	//	cs_packet_up *my_packet = reinterpret_cast<cs_packet_up *>(send_buffer);
	//	my_packet->size = sizeof(my_packet);
	//	send_wsabuf.len = sizeof(my_packet);
	//	DWORD iobyte;

	//	switch (dwDirection)
	//	{
	//	case DIR_BACK:
	//		my_packet->type = CS_KEYDOWN_UP;
	//		break;
	//	case DIR_FRONT:
	//		my_packet->type = CS_KEYDOWN_DOWN;
	//		break;
	//	case DIR_LEFT:
	//		my_packet->type = CS_KEYDOWN_LEFT;
	//		break;
	//	case DIR_RIGHT:
	//		my_packet->type = CS_KEYDOWN_RIGHT;
	//		break;
	//	}
	//	if (KeyDownForServer == 1) {
	//		cout << "Send\n";
	//		WSASend(g_mysocket, &send_wsabuf, 1, &iobyte, 0, NULL, NULL);
	//		KeyDownForServer = 2;
	//	}
	//}	// Server KeyDown
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
				m_ppShaders[2]->GetFBXMesh->SetAnimation(2);
				
				bCharaterRun = false;
				bCharaterPunch = true;
			}
		default:
			break;
		}
		break;	// Client KeyDown


///////////////////////////////////////////////////////////////////////		


	case WM_KEYUP:
	//{
	//	// server send(Move)
	//	if (KeyDownForServer == 2)
	//		KeyDownForServer = 3;

	//	if (wParam == VK_LEFT && LeftKeyDown) {
	//		LeftKeyDown = false;
	//		if (bCharaterRun)
	//		{
	//			m_ppShaders[2]->GetFBXMesh->SetAnimation(0);
	//			bCharaterRun = false;
	//		}
	//	}
	//	if (wParam == VK_RIGHT && RightKeyDown) {
	//		RightKeyDown = false;
	//		if (bCharaterRun)
	//		{
	//			m_ppShaders[2]->GetFBXMesh->SetAnimation(0);
	//			bCharaterRun = false;
	//		}
	//	}
	//	if (wParam == VK_UP && UpKeyDown) {
	//		UpKeyDown = false;
	//		if (bCharaterRun)
	//		{
	//			m_ppShaders[2]->GetFBXMesh->SetAnimation(0);
	//			bCharaterRun = false;
	//		}
	//	}
	//	if (wParam == VK_DOWN && DownKeyDown) {
	//		DownKeyDown = false;
	//		if (bCharaterRun)
	//		{
	//			m_ppShaders[2]->GetFBXMesh->SetAnimation(0);
	//			bCharaterRun = false;
	//		}
	//	}
	//	cs_packet_up *my_packet = reinterpret_cast<cs_packet_up *>(send_buffer);
	//	my_packet->size = sizeof(my_packet);
	//	send_wsabuf.len = sizeof(my_packet);
	//	DWORD iobyte;
	//	my_packet->type = CS_KEYUP;

	//	if (KeyDownForServer == 3) {
	//		WSASend(g_mysocket, &send_wsabuf, 1, &iobyte, 0, NULL, NULL);
	//		KeyDownForServer = 0;
	//	}

	//} // Server KeyUp
		switch (wParam)
		{
		case 'Z':
			if (bCharaterRun)
			{
				/*m_ppShaders[2]->GetFBXMesh->SetAnimation(0);
				bCharaterRun = false;*/
			}
			break;
		case 'X':
			if (bCharaterPunch)
			{
				m_ppShaders[2]->GetFBXMesh->SetAnimation(0);
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
			m_ppShaders[2]->GetFBXMesh->SetAnimation(0);
			bCharaterRun = false;
		}
		//if(!DownKeyDown && !RightKeyDown && !LeftKeyDown)
			SendMovePacket(CS_KEYUP_UP);
	}	
	if (KEY_UP(VK_DOWN) && DownKeyDown)
	{
		DownKeyDown = false;
		if (bCharaterRun)
		{
			m_ppShaders[2]->GetFBXMesh->SetAnimation(0);
			bCharaterRun = false;
		}
		//if (!UpKeyDown && !RightKeyDown && !LeftKeyDown)
			SendMovePacket(CS_KEYUP_DOWN);
	}
	if (KEY_UP(VK_LEFT) && LeftKeyDown)
	{
		LeftKeyDown = false;
		if (bCharaterRun)
		{
			m_ppShaders[2]->GetFBXMesh->SetAnimation(0);
			bCharaterRun = false;
		}
		//if (!DownKeyDown && !RightKeyDown && !UpKeyDown)
		SendMovePacket(CS_KEYUP_LEFT);
	}
	if (KEY_UP(VK_RIGHT) && RightKeyDown)
	{
		RightKeyDown = false;
		if (bCharaterRun)
		{
			m_ppShaders[2]->GetFBXMesh->SetAnimation(0);
			bCharaterRun = false;
		}
		//if (!DownKeyDown && !UpKeyDown && !LeftKeyDown)
			SendMovePacket(CS_KEYUP_RIGHT);
	}


	//if (dwDirection)
	//{
	//	cs_packet_up *my_packet = reinterpret_cast<cs_packet_up *>(send_buffer);
	//	my_packet->size = sizeof(my_packet);
	//	send_wsabuf.len = sizeof(my_packet);
	//	DWORD iobyte;

	//	switch (dwDirection)
	//	{
	//	case DIR_BACK:
	//		my_packet->type = CS_KEYDOWN_UP;
	//		break;
	//	case DIR_FRONT:
	//		my_packet->type = CS_KEYDOWN_DOWN;
	//		break;
	//	case DIR_LEFT:
	//		my_packet->type = CS_KEYDOWN_LEFT;
	//		break;
	//	case DIR_RIGHT:
	//		my_packet->type = CS_KEYDOWN_RIGHT;
	//		break;
	//	}
	//	if (KeyDownForServer == 1) {
	//		WSASend(g_mysocket, &send_wsabuf, 1, &iobyte, 0, NULL, NULL);
	//		KeyDownForServer = 2;
	//	}
	//}
	//


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
	cs_packet_up *my_packet = reinterpret_cast<cs_packet_up *>(send_buffer);
	my_packet->size = sizeof(my_packet);
	send_wsabuf.len = sizeof(my_packet);
	DWORD iobyte;
	my_packet->type = type;
	
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

	
	m_ppShaders[1]->GetFBXMesh->FBXFrameAdvance(fTimeElapsed);
	m_ppShaders[2]->GetFBXMesh->FBXFrameAdvance(fTimeElapsed);


	if (LeftKeyDown || RightKeyDown || UpKeyDown || DownKeyDown)
	{
		if (LeftKeyDown && UpKeyDown)
		{
			pSordmanObject->Rotate(-90.0f, 135.0f, 0.0f);
			//pSordmanObject->m_d3dxvDirection = D3DXVECTOR3(-1.0f, 0.0f, 1.0f);
		}
		else if (LeftKeyDown && DownKeyDown)
		{
			pSordmanObject->Rotate(-90.0f, 45.0f, 0.0f);
			//pSordmanObject->m_d3dxvDirection = D3DXVECTOR3(-1.0f, 0.0f, -1.0f);
		}
		else if (RightKeyDown && UpKeyDown)
		{
			pSordmanObject->Rotate(-90.0f, -135.0f, 0.0f);
			//pSordmanObject->m_d3dxvDirection = D3DXVECTOR3(1.0f, 0.0f, 1.0f);
		}
		else if (RightKeyDown && DownKeyDown)
		{
			pSordmanObject->Rotate(-90.0f, -45.0f, 0.0f);
			//pSordmanObject->m_d3dxvDirection = D3DXVECTOR3(1.0f, 0.0f, -1.0f);
		}

		else
		{
			if (LeftKeyDown)
			{
				pSordmanObject->Rotate(-90.0f, 90.0f, 0.0f);
				//pSordmanObject->m_d3dxvDirection = D3DXVECTOR3(-1.0f, 0.0f, 0.0f);
			}
			else if (RightKeyDown)
			{
				pSordmanObject->Rotate(-90.0f, -90.0f, 0.0f);
				//pSordmanObject->m_d3dxvDirection = D3DXVECTOR3(1.0f, 0.0f, 0.0f);
			}
			else if (UpKeyDown)
			{
				pSordmanObject->Rotate(-90.0f, 180.0f, 0.0f);
				//pSordmanObject->m_d3dxvDirection = D3DXVECTOR3(0.0f, 0.0f, 1.0f);
			}
			else if (DownKeyDown)
			{
				pSordmanObject->Rotate(-90.0f, 0.0f, 0.0f);
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
	
	
	
	
	//m_pCamera->SetPosition(D3DXVECTOR3(pSordmanObject->GetPosition().x, pSordmanObject->GetPosition().y, pSordmanObject->GetPosition().z - 40));
	
	//cout << pSordmanObject->m_d3dxvDirection.x << ends << pSordmanObject->m_d3dxvDirection.y << ends << pSordmanObject->m_d3dxvDirection.z << endl;
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