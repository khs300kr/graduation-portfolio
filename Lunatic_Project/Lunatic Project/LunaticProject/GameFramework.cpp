﻿#include "stdafx.h"
#include "GameFramework.h"
#include "Player.h"
#include "Scene.h"

CGameFramework::CGameFramework()
{
	m_pd3dDevice = NULL;
	m_pDXGISwapChain = NULL;
	m_pd3dRenderTargetView = NULL;
	m_pd3dDeviceContext = NULL;

	m_nWndClientWidth = FRAME_BUFFER_WIDTH;
	m_nWndClientHeight = FRAME_BUFFER_HEIGHT;

	m_pScene = NULL;

	_tcscpy_s(m_pszBuffer, _T("Lunatic Project ("));

	m_pPlayer = NULL;

	m_pd3dDepthStencilBuffer = NULL;
	m_pd3dDepthStencilView = NULL;
	m_pPlayerShader = NULL;

	m_pCamera = NULL;
	dwDirection = 0;


	isEnding = false; // 엔딩
	isResult = false;

	isWinResult = false;
	isLoseResult = false;

	ResultTimer = 0;

	for (int i = 0; i < MAX_GAMER; ++i)
		OtherDirection[i] = 0;

	LoadingScene = false;
	ChangeScene = MAINMENU;

	Scene_Animation = 0;
	Scene_count = 0;

	WinTimer = 0;
	LoseTimer = 0;
	WinCount = 0;
	LoseCount = 0;
}

CGameFramework::~CGameFramework()
{
}

//다음 함수는 응용 프로그램이 실행되면 호출된다는 것에 유의하라. 
bool CGameFramework::Create(HINSTANCE hInstance, HWND hMainWnd)
{
	m_hInstance = hInstance;
	m_hWnd = hMainWnd;
	//Direct3D 디바이스, 디바이스 컨텍스트, 스왑 체인 등을 생성하는 함수를 호출한다. 
	if (!CreateDirect3DDisplay()) return(false);

	//렌더링할 객체(게임 월드 객체)를 생성한다. 

	BuildObjects();
	m_pScene = new CScene();

	m_pScene->SetCamera(m_pCamera);


	return(true);
}

bool CGameFramework::CreateRenderTargetDepthStencilView()
{
	HRESULT hResult = S_OK;
	ID3D11Texture2D *pd3dBackBuffer;
	if (FAILED(hResult = m_pDXGISwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID *)&pd3dBackBuffer))) return(false);
	if (FAILED(hResult = m_pd3dDevice->CreateRenderTargetView(pd3dBackBuffer, NULL, &m_pd3dRenderTargetView))) return(false);
	if (pd3dBackBuffer) pd3dBackBuffer->Release();

	//렌더 타겟과 같은 크기의 깊이 버퍼(Depth Buffer)를 생성한다.
	D3D11_TEXTURE2D_DESC d3dDepthStencilBufferDesc;
	ZeroMemory(&d3dDepthStencilBufferDesc, sizeof(D3D11_TEXTURE2D_DESC));
	d3dDepthStencilBufferDesc.Width = m_nWndClientWidth;
	d3dDepthStencilBufferDesc.Height = m_nWndClientHeight;
	d3dDepthStencilBufferDesc.MipLevels = 1;
	d3dDepthStencilBufferDesc.ArraySize = 1;
	d3dDepthStencilBufferDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	d3dDepthStencilBufferDesc.SampleDesc.Count = 1;
	d3dDepthStencilBufferDesc.SampleDesc.Quality = 0;
	d3dDepthStencilBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	d3dDepthStencilBufferDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	d3dDepthStencilBufferDesc.CPUAccessFlags = 0;
	d3dDepthStencilBufferDesc.MiscFlags = 0;
	if (FAILED(hResult = m_pd3dDevice->CreateTexture2D(&d3dDepthStencilBufferDesc, NULL, &m_pd3dDepthStencilBuffer))) return(false);

	//생성한 깊이 버퍼(Depth Buffer)에 대한 뷰를 생성한다.
	D3D11_DEPTH_STENCIL_VIEW_DESC d3dViewDesc;
	ZeroMemory(&d3dViewDesc, sizeof(D3D11_DEPTH_STENCIL_VIEW_DESC));
	d3dViewDesc.Format = d3dDepthStencilBufferDesc.Format;
	d3dViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	d3dViewDesc.Texture2D.MipSlice = 0;
	if (FAILED(hResult = m_pd3dDevice->CreateDepthStencilView(m_pd3dDepthStencilBuffer, &d3dViewDesc, &m_pd3dDepthStencilView))) return(false);

	m_pd3dDeviceContext->OMSetRenderTargets(1, &m_pd3dRenderTargetView, m_pd3dDepthStencilView);

	return(true);
}


bool CGameFramework::CreateDirect3DDisplay()
{
	RECT rcClient;
	::GetClientRect(m_hWnd, &rcClient);
	m_nWndClientWidth = rcClient.right - rcClient.left;
	m_nWndClientHeight = rcClient.bottom - rcClient.top;

	UINT dwCreateDeviceFlags = 0;
#ifdef _DEBUG
	dwCreateDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

	//디바이스를 생성하기 위하여 시도할 드라이버 유형의 순서를 나타낸다.
	D3D_DRIVER_TYPE d3dDriverTypes[] =
	{
		D3D_DRIVER_TYPE_HARDWARE,
		D3D_DRIVER_TYPE_WARP,
		D3D_DRIVER_TYPE_REFERENCE
	};
	UINT nDriverTypes = sizeof(d3dDriverTypes) / sizeof(D3D10_DRIVER_TYPE);

	//디바이스를 생성하기 위하여 시도할 특성 레벨의 순서를 나타낸다.
	D3D_FEATURE_LEVEL d3dFeatureLevels[] =
	{
		D3D_FEATURE_LEVEL_11_0,
		D3D_FEATURE_LEVEL_10_1,
		D3D_FEATURE_LEVEL_10_0
	};
	UINT nFeatureLevels = sizeof(d3dFeatureLevels) / sizeof(D3D_FEATURE_LEVEL);

	//생성할 스왑 체인을 서술하는 구조체이다.
	DXGI_SWAP_CHAIN_DESC dxgiSwapChainDesc;
	::ZeroMemory(&dxgiSwapChainDesc, sizeof(dxgiSwapChainDesc));
	dxgiSwapChainDesc.BufferCount = 1;
	dxgiSwapChainDesc.BufferDesc.Width = m_nWndClientWidth;
	dxgiSwapChainDesc.BufferDesc.Height = m_nWndClientHeight;
	dxgiSwapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	dxgiSwapChainDesc.BufferDesc.RefreshRate.Numerator = 60;
	dxgiSwapChainDesc.BufferDesc.RefreshRate.Denominator = 1;
	dxgiSwapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	dxgiSwapChainDesc.OutputWindow = m_hWnd;
	dxgiSwapChainDesc.SampleDesc.Count = 1;
	dxgiSwapChainDesc.SampleDesc.Quality = 0;
	dxgiSwapChainDesc.Windowed = TRUE;

	D3D_DRIVER_TYPE nd3dDriverType = D3D_DRIVER_TYPE_NULL;
	D3D_FEATURE_LEVEL nd3dFeatureLevel = D3D_FEATURE_LEVEL_11_0;

	HRESULT hResult = S_OK;
	//디바이스의 드라이버 유형과 특성 레벨을 지원하는 디바이스와 스왑 체인을 생성한다. 고수준의 디바이스 생성을 시도하고 실패하면 다음 수준의 디바이스를 생성한다.
	for (UINT i = 0; i < nDriverTypes; i++)
	{
		nd3dDriverType = d3dDriverTypes[i];
		if (SUCCEEDED(hResult = D3D11CreateDeviceAndSwapChain(NULL, nd3dDriverType, NULL, dwCreateDeviceFlags, d3dFeatureLevels, nFeatureLevels, D3D11_SDK_VERSION, &dxgiSwapChainDesc, &m_pDXGISwapChain, &m_pd3dDevice, &nd3dFeatureLevel, &m_pd3dDeviceContext))) break;
	}
	if (!m_pDXGISwapChain || !m_pd3dDevice || !m_pd3dDeviceContext) return(false);

	//렌더 타겟 뷰를 생성하는 함수를 호출한다.
	//CreateRenderTargetView() 함수 호출 다음에 SetViewport() 함수를 호출한다.
	if (!CreateRenderTargetDepthStencilView()) return(false);

	return(true);
}

void CGameFramework::OnProcessingMouseMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam)
{
	switch (nMessageID)
	{
	case WM_LBUTTONDOWN:
	case WM_RBUTTONDOWN:
		//마우스 캡쳐를 하고 현재 마우스 위치를 가져온다.
		SetCapture(hWnd);
		GetCursorPos(&m_ptOldCursorPos);
		break;
	case WM_LBUTTONUP:
	case WM_RBUTTONUP:
		// 마우스 캡쳐를 해제한다.
		ReleaseCapture();
		break;
	case WM_MOUSEMOVE:
		break;
	default:
		break;
	}
}

void CGameFramework::OnProcessingKeyboardMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam)
{
	//if (m_pScene) m_pScene->OnProcessingKeyboardMessage(hWnd, nMessageID, wParam, lParam);

	switch (nMessageID)
	{

	}
}

LRESULT CALLBACK CGameFramework::OnProcessingWindowMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam)
{
	switch (nMessageID)
	{
		/*윈도우의 크기가 변경될 때(현재는 “Alt+Enter“ 전체 화면 모드와 윈도우 모드로 전환될 때) 스왑 체인의 후면버퍼 크기를 조정하고 후면버퍼에 대한 렌더 타겟 뷰를 다시 생성한다. */
	case WM_SIZE:
	{
		m_nWndClientWidth = LOWORD(lParam);
		m_nWndClientHeight = HIWORD(lParam);

		m_pd3dDeviceContext->OMSetRenderTargets(0, NULL, NULL);

		if (m_pd3dRenderTargetView) m_pd3dRenderTargetView->Release();
		if (m_pd3dDepthStencilBuffer) m_pd3dDepthStencilBuffer->Release();
		if (m_pd3dDepthStencilView) m_pd3dDepthStencilView->Release();

		m_pDXGISwapChain->ResizeBuffers(1, 0, 0, DXGI_FORMAT_R8G8B8A8_UNORM, 0);

		CreateRenderTargetDepthStencilView();

		CCamera *pCamera = m_pPlayer->GetCamera();
		if (pCamera) pCamera->SetViewport(m_pd3dDeviceContext, 0, 0, m_nWndClientWidth, m_nWndClientHeight, 0.0f, 1.0f);
		break;
	}

	case WM_LBUTTONDOWN:
	case WM_RBUTTONDOWN:
	case WM_LBUTTONUP:
	case WM_RBUTTONUP:
	case WM_MOUSEMOVE:
		OnProcessingMouseMessage(hWnd, nMessageID, wParam, lParam);
		break;
	case WM_KEYDOWN:
	case WM_KEYUP:
		OnProcessingKeyboardMessage(hWnd, nMessageID, wParam, lParam);
		break;
	}
	return(0);
}

//void CGameFramework::char_collision_send()
//{
//	// server send (cs_packet_char_collision)
//	cs_packet_char_collision *my_packet = reinterpret_cast<cs_packet_char_collision *>(send_buffer);
//	my_packet->size = sizeof(cs_packet_char_collision);
//	send_wsabuf.len = sizeof(cs_packet_char_collision);
//	DWORD iobyte;
//	my_packet->type = CS_CHAR_COLLISION;
//	my_packet->roomnumber = m_pScene->MyRoomNumber;
//
//
//	WSASend(g_mysocket, &send_wsabuf, 1, &iobyte, 0, NULL, NULL);
//	//
//}




//다음 함수는 응용 프로그램이 종료될 때 호출된다는 것에 유의하라. 
void CGameFramework::Destroy()
{
	WSACleanup();

	ReleaseObjects();

	if (m_pd3dDeviceContext) m_pd3dDeviceContext->ClearState();
	if (m_pd3dRenderTargetView) m_pd3dRenderTargetView->Release();
	if (m_pd3dDepthStencilBuffer) m_pd3dDepthStencilBuffer->Release();
	if (m_pd3dDepthStencilView) m_pd3dDepthStencilView->Release();
	if (m_pDXGISwapChain) m_pDXGISwapChain->Release();
	if (m_pd3dDeviceContext) m_pd3dDeviceContext->Release();
	if (m_pd3dDevice) m_pd3dDevice->Release();
}

void CGameFramework::BuildObjects()
{
	//CShader 클래스의 정적(static) 멤버 변수로 선언된 상수 버퍼를 생성한다.
	CShader::CreateShaderVariables(m_pd3dDevice);
	CIlluminatedShader::CreateShaderVariables(m_pd3dDevice);


	m_pPlayerShader = new CPlayerShader();
	m_pPlayerShader->CreateShader(m_pd3dDevice);
	m_pPlayerShader->BuildObjects(m_pd3dDevice);
	m_pPlayer = m_pPlayerShader->GetPlayer();

	m_pCamera = m_pPlayer->GetCamera();
	m_pCamera->SetViewport(m_pd3dDeviceContext, 0, 0, FRAME_BUFFER_WIDTH, FRAME_BUFFER_HEIGHT, 0.0f, 1.0f);
	m_pCamera->GenerateViewMatrix();

	m_pPlayer->ChangeCamera(m_pd3dDevice, (112 - 0x70 + 1), m_GameTimer.GetTimeElapsed());
	m_pCamera = m_pPlayer->GetCamera();
	// 씬에 현재 카메라를 설정한다.


}

void CGameFramework::ReleaseObjects()
{
	//CShader 클래스의 정적(static) 멤버 변수로 선언된 상수 버퍼를 반환한다.
	CShader::ReleaseShaderVariables();
	CIlluminatedShader::ReleaseShaderVariables();

	if (m_pScene) m_pScene->ReleaseObjects();
	if (m_pScene) delete m_pScene;

	if (m_pPlayerShader) m_pPlayerShader->ReleaseObjects();
	if (m_pPlayerShader) delete m_pPlayerShader;
}

void CGameFramework::ProcessInput()
{
	bool bProcessedByScene = false;
	m_pScene->ProcessInput();
	if (!bProcessedByScene)
	{
		static UCHAR pKeyBuffer[256];

		// 키보드의 상태 정보를 반환한다. 
		// 방향키와 PageUp/Down 에 대한 정보
		//if (GetAsyncKeyState(VK_UP) & 0x8000) dwDirection |= DIR_BACK;
		//if (GetAsyncKeyState(VK_DOWN) & 0x8000) dwDirection |= DIR_FRONT;
		//if (GetAsyncKeyState(VK_LEFT) & 0x8000) dwDirection |= DIR_LEFT;
		//if (GetAsyncKeyState(VK_RIGHT) & 0x8000) dwDirection |= DIR_RIGHT;

		//if (GetAsyncKeyState(VK_PRIOR) & 0x8000) dwDirection |= DIR_UP;
		//if (GetAsyncKeyState(VK_NEXT) & 0x8000) dwDirection |= DIR_DOWN;

		float cxDelta = 0.0f, cyDelta = 0.0f;
		POINT ptCursorPos;

		// 마우스를 캡쳐했으면 마우스가 얼마만큼 이동하였는 가를 계산한다.
		// 마우스 왼쪽 또는 오른쪽 버튼이 눌러질 때의 메시지(WM_LBUTTONDOWN, WM_RBUTTONDOWN)를 처리할 때 마우스를 캡쳐
		// 그러므로 마우스가 캡쳐된 것은 마우스 버튼이 눌려진 상태를 의미한다. 마우스를 좌우 또는 상하로 움직이면 플레이어를 x-축 또는 y-축으로 회전한다.
		if (GetCapture() == m_hWnd)
		{
			// 마우스 커서를 화면에서 없앤다(보이지 않게 한다).
			SetCursor(NULL);
			// 현재 마우스 커서의 위치를 가져온다.
			GetCursorPos(&ptCursorPos);
			// 마우스 버튼이 눌린 채로 이전 위치에서 현재 마우스 커서의 위치까지 움직인 양을 구한다.
			cxDelta = (float)(ptCursorPos.x - m_ptOldCursorPos.x) / 3.0f;
			cyDelta = (float)(ptCursorPos.y - m_ptOldCursorPos.y) / 3.0f;
			SetCursorPos(m_ptOldCursorPos.x, m_ptOldCursorPos.y);
		}
		float RotY_Hero = 0;
		// 플레이어를 이동하거나(dwDirection) 회전한다(cxDelta 또는 cyDelta).
		if ((dwDirection != 0) || (cxDelta != 0.0f) || (cyDelta != 0.0f))
		{
			if (cxDelta || cyDelta)
			{
				// cxDelta는 y-축의 회전을 나타내고 cyDelta는 x-축의 회전을 나타낸다. 오른쪽 마우스 버튼이 눌려진 경우 cxDelta는 z-축의 회전을 나타낸다.
				if (pKeyBuffer[VK_RBUTTON] & 0xF0)
					m_pPlayer->Rotate(cyDelta, 0.0f, -cxDelta);
				else
					m_pPlayer->Rotate(cyDelta, cxDelta, 0.0f);
			}
			// 플레이어를 dwDirection 방향으로 이동한다(실제로는 속도 벡터를 변경한다). 
			// 이동 거리는 시간에 비례하도록 한다. 플레이어의 이동 속력은 (50/초)로 가정한다. 만약 플레이어의 이동 속력이 있다면 그 값을 사용한다.
			if (dwDirection && ChangeScene == GAME)
			{
				if (dwDirection == DIR_LEFT_BACK || dwDirection == DIR_LEFT_FRONT || dwDirection == DIR_RIGHT_BACK || dwDirection == DIR_RIGHT_FRONT) // 대각선 이동 이동속도 구현
				{
					if (dwDirection == DIR_LEFT_BACK) RotY_Hero = 135.0f;
					else if (dwDirection == DIR_LEFT_FRONT) RotY_Hero = 45.0f;
					else if (dwDirection == DIR_RIGHT_BACK) RotY_Hero = -135.0f;
					else if (dwDirection == DIR_RIGHT_FRONT) RotY_Hero = -45.0f;
					m_pScene->pHeroObject[m_pScene->myGame_id]->SetSpeed(m_pScene->pHeroObject[m_pScene->myGame_id]->GetRootSpeed());

				}
				else
				{
					if (dwDirection == DIR_LEFT) RotY_Hero = 90.0f;
					else if (dwDirection == DIR_RIGHT) RotY_Hero = -90.0f;
					else if (dwDirection == DIR_FRONT) RotY_Hero = 0.0f;
					else if (dwDirection == DIR_BACK) RotY_Hero = 180.0f;

					m_pScene->pHeroObject[m_pScene->myGame_id]->SetSpeed(m_pScene->pHeroObject[m_pScene->myGame_id]->GetNormalSpeed());
				}


				//		if (!m_pScene->pHeroObject[m_pScene->myGame_id]->bHeroAttack && !m_pScene->pHeroObject[m_pScene->myGame_id]->bHeroQ && !m_pScene->pHeroObject[m_pScene->myGame_id]->bHeroW && !m_pScene->pHeroObject[m_pScene->myGame_id]->bHeroE && !m_pScene->pHeroObject[m_pScene->myGame_id]->bHeroR)
				//		{

				// 애니메이션 중 일 때 움직이지 못하고 방향에 따라서 움직임
				m_pPlayer->Move(dwDirection, m_pScene->pHeroObject[m_pScene->myGame_id]->GetSpeed(), true);
				m_pScene->pHeroObject[m_pScene->myGame_id]->SetPosition(m_pPlayer->GetPosition());
				m_pScene->pHeroObject[m_pScene->myGame_id]->Rotate(0, RotY_Hero, 0);
				//		}



				// 클라이언트 캐릭터 충돌부분!! size 3.0 <- 요거 나중에 Define해야함
				for (int i = 0; i < MAX_GAMER; ++i)
				{
					if (i != m_pScene->myGame_id)
					{
						if (m_pScene->Rightcollision(m_pScene->pHeroObject[m_pScene->myGame_id], m_pScene->pHeroObject[i], 3.0f, 3.0f, 3.0f, 3.0f))
						{
							cs_packet_char_coll *my_packet = reinterpret_cast<cs_packet_char_coll *>(send_buffer);
							my_packet->size = sizeof(cs_packet_char_coll);
							send_wsabuf.len = sizeof(cs_packet_char_coll);
							DWORD iobyte;
							my_packet->type = CS_CHAR_COLL;
							my_packet->roomnumber = m_pScene->MyRoomNumber;
							my_packet->direction = CS_KEYDOWN_RIGHT;

							WSASend(g_mysocket, &send_wsabuf, 1, &iobyte, 0, NULL, NULL);
							break;
						}
						else if (m_pScene->Leftcollision(m_pScene->pHeroObject[m_pScene->myGame_id], m_pScene->pHeroObject[i], 3.0f, 3.0f, 3.0f, 3.0f))
						{
							cs_packet_char_coll *my_packet = reinterpret_cast<cs_packet_char_coll *>(send_buffer);
							my_packet->size = sizeof(cs_packet_char_coll);
							send_wsabuf.len = sizeof(cs_packet_char_coll);
							DWORD iobyte;
							my_packet->type = CS_CHAR_COLL;
							my_packet->roomnumber = m_pScene->MyRoomNumber;
							my_packet->direction = CS_KEYDOWN_LEFT;

							WSASend(g_mysocket, &send_wsabuf, 1, &iobyte, 0, NULL, NULL);
							break;
						}
						else if (m_pScene->Upcollision(m_pScene->pHeroObject[m_pScene->myGame_id], m_pScene->pHeroObject[i], 3.0f, 3.0f, 3.0f, 3.0f))
						{
							cs_packet_char_coll *my_packet = reinterpret_cast<cs_packet_char_coll *>(send_buffer);
							my_packet->size = sizeof(cs_packet_char_coll);
							send_wsabuf.len = sizeof(cs_packet_char_coll);
							DWORD iobyte;
							my_packet->type = CS_CHAR_COLL;
							my_packet->roomnumber = m_pScene->MyRoomNumber;
							my_packet->direction = CS_KEYDOWN_UP;

							WSASend(g_mysocket, &send_wsabuf, 1, &iobyte, 0, NULL, NULL);
							break;
						}
						else if (m_pScene->Downcollision(m_pScene->pHeroObject[m_pScene->myGame_id], m_pScene->pHeroObject[i], 3.0f, 3.0f, 3.0f, 3.0f))
						{
							cs_packet_char_coll *my_packet = reinterpret_cast<cs_packet_char_coll *>(send_buffer);
							my_packet->size = sizeof(cs_packet_char_coll);
							send_wsabuf.len = sizeof(cs_packet_char_coll);
							DWORD iobyte;
							my_packet->type = CS_CHAR_COLL;
							my_packet->roomnumber = m_pScene->MyRoomNumber;
							my_packet->direction = CS_KEYDOWN_DOWN;

							WSASend(g_mysocket, &send_wsabuf, 1, &iobyte, 0, NULL, NULL);
							break;
						}


					}

				}

				for (int i = 1; i < 13; ++i) // house1 <-> Hero 건물 충돌체크 // 아래, 위 해야함
				{
					if (m_pScene->Rightcollision(m_pScene->pHeroObject[m_pScene->myGame_id], m_pScene->pHouse1Object[i], 3.0f, 3.0f, 110.0f, 32.0f))
					{
						cs_packet_building_coll *my_packet = reinterpret_cast<cs_packet_building_coll *>(send_buffer);
						my_packet->size = sizeof(cs_packet_char_coll);
						send_wsabuf.len = sizeof(cs_packet_char_coll);
						DWORD iobyte;
						my_packet->type = CS_BUILDING_COLL;
						my_packet->roomnumber = m_pScene->MyRoomNumber;
						my_packet->direction = CS_KEYDOWN_RIGHT;

						WSASend(g_mysocket, &send_wsabuf, 1, &iobyte, 0, NULL, NULL);
						break;
					}

					else if (m_pScene->Leftcollision(m_pScene->pHeroObject[m_pScene->myGame_id], m_pScene->pHouse1Object[i], 3.0f, 3.0f, -2.0f, 32.0f))
					{
						cs_packet_building_coll *my_packet = reinterpret_cast<cs_packet_building_coll *>(send_buffer);
						my_packet->size = sizeof(cs_packet_char_coll);
						send_wsabuf.len = sizeof(cs_packet_char_coll);
						DWORD iobyte;
						my_packet->type = CS_BUILDING_COLL;
						my_packet->roomnumber = m_pScene->MyRoomNumber;
						my_packet->direction = CS_KEYDOWN_LEFT;

						WSASend(g_mysocket, &send_wsabuf, 1, &iobyte, 0, NULL, NULL);
						break;
					}

					else if (m_pScene->Upcollision(m_pScene->pHeroObject[m_pScene->myGame_id], m_pScene->pHouse1Object[i], 3.0f, 3.0f, 55.0f, 32.0f))
					{
						cs_packet_building_coll *my_packet = reinterpret_cast<cs_packet_building_coll *>(send_buffer);
						my_packet->size = sizeof(cs_packet_char_coll);
						send_wsabuf.len = sizeof(cs_packet_char_coll);
						DWORD iobyte;
						my_packet->type = CS_BUILDING_COLL;
						my_packet->roomnumber = m_pScene->MyRoomNumber;
						my_packet->direction = CS_KEYDOWN_UP;

						WSASend(g_mysocket, &send_wsabuf, 1, &iobyte, 0, NULL, NULL);
						break;
					}
					else if (m_pScene->Downcollision(m_pScene->pHeroObject[m_pScene->myGame_id], m_pScene->pHouse1Object[i], 3.0f, 3.0f, 55.0f, 32.0f))
					{
						cs_packet_building_coll *my_packet = reinterpret_cast<cs_packet_building_coll *>(send_buffer);
						my_packet->size = sizeof(cs_packet_char_coll);
						send_wsabuf.len = sizeof(cs_packet_char_coll);
						DWORD iobyte;
						my_packet->type = CS_BUILDING_COLL;
						my_packet->roomnumber = m_pScene->MyRoomNumber;
						my_packet->direction = CS_KEYDOWN_DOWN;

						WSASend(g_mysocket, &send_wsabuf, 1, &iobyte, 0, NULL, NULL);
						break;
					}
				}

				if (m_pScene->pHeroObject[m_pScene->myGame_id]->GetPosition().x > 403.f) // 오른쪽 벽 통과못하게
				{
					cs_packet_building_coll *my_packet = reinterpret_cast<cs_packet_building_coll *>(send_buffer);
					my_packet->size = sizeof(cs_packet_char_coll);
					send_wsabuf.len = sizeof(cs_packet_char_coll);
					DWORD iobyte;
					my_packet->type = CS_BUILDING_COLL;
					my_packet->roomnumber = m_pScene->MyRoomNumber;
					my_packet->direction = CS_KEYDOWN_RIGHT;

					WSASend(g_mysocket, &send_wsabuf, 1, &iobyte, 0, NULL, NULL);
				}
				else if (m_pScene->pHeroObject[m_pScene->myGame_id]->GetPosition().x < -403.f) // 왼쪽 벽 통과못하게
				{
					cs_packet_building_coll *my_packet = reinterpret_cast<cs_packet_building_coll *>(send_buffer);
					my_packet->size = sizeof(cs_packet_char_coll);
					send_wsabuf.len = sizeof(cs_packet_char_coll);
					DWORD iobyte;
					my_packet->type = CS_BUILDING_COLL;
					my_packet->roomnumber = m_pScene->MyRoomNumber;
					my_packet->direction = CS_KEYDOWN_LEFT;

					WSASend(g_mysocket, &send_wsabuf, 1, &iobyte, 0, NULL, NULL);
				}

				else if (m_pScene->pHeroObject[m_pScene->myGame_id]->GetPosition().x < -100.f && m_pScene->pHeroObject[m_pScene->myGame_id]->GetPosition().z < -495.f) //리스폰 왼쪽 밑
				{
					cs_packet_building_coll *my_packet = reinterpret_cast<cs_packet_building_coll *>(send_buffer);
					my_packet->size = sizeof(cs_packet_char_coll);
					send_wsabuf.len = sizeof(cs_packet_char_coll);
					DWORD iobyte;
					my_packet->type = CS_BUILDING_COLL;
					my_packet->roomnumber = m_pScene->MyRoomNumber;
					my_packet->direction = CS_KEYDOWN_UP;

					WSASend(g_mysocket, &send_wsabuf, 1, &iobyte, 0, NULL, NULL);

				}

				else if (m_pScene->pHeroObject[m_pScene->myGame_id]->GetPosition().x > 100.f && m_pScene->pHeroObject[m_pScene->myGame_id]->GetPosition().z < -495.f) //리스폰 오른쪽 밑
				{
					cs_packet_building_coll *my_packet = reinterpret_cast<cs_packet_building_coll *>(send_buffer);
					my_packet->size = sizeof(cs_packet_char_coll);
					send_wsabuf.len = sizeof(cs_packet_char_coll);
					DWORD iobyte;
					my_packet->type = CS_BUILDING_COLL;
					my_packet->roomnumber = m_pScene->MyRoomNumber;
					my_packet->direction = CS_KEYDOWN_UP;

					WSASend(g_mysocket, &send_wsabuf, 1, &iobyte, 0, NULL, NULL);
				}

				// 리스폰 지역 부분 충돌 처리

				// 리스폰안에서 아래쪽으로
				if (m_pScene->pHeroObject[m_pScene->myGame_id]->GetPosition().x > -80.0f && m_pScene->pHeroObject[m_pScene->myGame_id]->GetPosition().x < 80.0f && m_pScene->pHeroObject[m_pScene->myGame_id]->GetPosition().z < -574.0f)
				{
					cs_packet_building_coll *my_packet = reinterpret_cast<cs_packet_building_coll *>(send_buffer);
					my_packet->size = sizeof(cs_packet_char_coll);
					send_wsabuf.len = sizeof(cs_packet_char_coll);
					DWORD iobyte;
					my_packet->type = CS_BUILDING_COLL;
					my_packet->roomnumber = m_pScene->MyRoomNumber;
					my_packet->direction = CS_KEYDOWN_UP;

					WSASend(g_mysocket, &send_wsabuf, 1, &iobyte, 0, NULL, NULL);
				}

				// 리스폰 안에서 오른쪽으로
				if (m_pScene->pHeroObject[m_pScene->myGame_id]->GetPosition().x > 63.0f && m_pScene->pHeroObject[m_pScene->myGame_id]->GetPosition().x < 65.0f && m_pScene->pHeroObject[m_pScene->myGame_id]->GetPosition().z < -413.0f && m_pScene->pHeroObject[m_pScene->myGame_id]->GetPosition().z > -580.0f)
				{
					cs_packet_building_coll *my_packet = reinterpret_cast<cs_packet_building_coll *>(send_buffer);
					my_packet->size = sizeof(cs_packet_char_coll);
					send_wsabuf.len = sizeof(cs_packet_char_coll);
					DWORD iobyte;
					my_packet->type = CS_BUILDING_COLL;
					my_packet->roomnumber = m_pScene->MyRoomNumber;
					my_packet->direction = CS_KEYDOWN_RIGHT;

					WSASend(g_mysocket, &send_wsabuf, 1, &iobyte, 0, NULL, NULL);
				}

				//리스폰 안에서 왼쪽으로
				if (m_pScene->pHeroObject[m_pScene->myGame_id]->GetPosition().x < -63.0f && m_pScene->pHeroObject[m_pScene->myGame_id]->GetPosition().x > -65.0f && m_pScene->pHeroObject[m_pScene->myGame_id]->GetPosition().z < -413.0f && m_pScene->pHeroObject[m_pScene->myGame_id]->GetPosition().z > -580.0f)
				{
					cs_packet_building_coll *my_packet = reinterpret_cast<cs_packet_building_coll *>(send_buffer);
					my_packet->size = sizeof(cs_packet_char_coll);
					send_wsabuf.len = sizeof(cs_packet_char_coll);
					DWORD iobyte;
					my_packet->type = CS_BUILDING_COLL;
					my_packet->roomnumber = m_pScene->MyRoomNumber;
					my_packet->direction = CS_KEYDOWN_LEFT;

					WSASend(g_mysocket, &send_wsabuf, 1, &iobyte, 0, NULL, NULL);
				}

				// 리스폰 밖에서 오른쪽으로
				if (m_pScene->pHeroObject[m_pScene->myGame_id]->GetPosition().x < -104.0f && m_pScene->pHeroObject[m_pScene->myGame_id]->GetPosition().x > -106.0f && m_pScene->pHeroObject[m_pScene->myGame_id]->GetPosition().z < -450.0f && m_pScene->pHeroObject[m_pScene->myGame_id]->GetPosition().z > -580.0f)
				{
					cs_packet_building_coll *my_packet = reinterpret_cast<cs_packet_building_coll *>(send_buffer);
					my_packet->size = sizeof(cs_packet_char_coll);
					send_wsabuf.len = sizeof(cs_packet_char_coll);
					DWORD iobyte;
					my_packet->type = CS_BUILDING_COLL;
					my_packet->roomnumber = m_pScene->MyRoomNumber;
					my_packet->direction = CS_KEYDOWN_RIGHT;

					WSASend(g_mysocket, &send_wsabuf, 1, &iobyte, 0, NULL, NULL);
				}

				//리스폰 밖에서 왼쪽으로
				if (m_pScene->pHeroObject[m_pScene->myGame_id]->GetPosition().x > 104.0f && m_pScene->pHeroObject[m_pScene->myGame_id]->GetPosition().x < 106.0f && m_pScene->pHeroObject[m_pScene->myGame_id]->GetPosition().z < -450.0f && m_pScene->pHeroObject[m_pScene->myGame_id]->GetPosition().z > -580.0f)
				{
					cs_packet_building_coll *my_packet = reinterpret_cast<cs_packet_building_coll *>(send_buffer);
					my_packet->size = sizeof(cs_packet_char_coll);
					send_wsabuf.len = sizeof(cs_packet_char_coll);
					DWORD iobyte;
					my_packet->type = CS_BUILDING_COLL;
					my_packet->roomnumber = m_pScene->MyRoomNumber;
					my_packet->direction = CS_KEYDOWN_LEFT;

					WSASend(g_mysocket, &send_wsabuf, 1, &iobyte, 0, NULL, NULL);
				}







			}


		}

		float RotY[MAX_GAMER] = {};
		for (int i = 0; i < MAX_GAMER; ++i)
		{
			if (OtherDirection[i] && ChangeScene == GAME && i != m_pScene->myGame_id)
			{
				if (OtherDirection[i] == DIR_LEFT_BACK || OtherDirection[i] == DIR_LEFT_FRONT || OtherDirection[i] == DIR_RIGHT_BACK || OtherDirection[i] == DIR_RIGHT_FRONT) // 대각선 이동 이동속도 구현
				{
					if (OtherDirection[i] == DIR_LEFT_BACK) RotY[i] = 135.0f;
					else if (OtherDirection[i] == DIR_LEFT_FRONT) RotY[i] = 45.0f;
					else if (OtherDirection[i] == DIR_RIGHT_BACK) RotY[i] = -135.0f;
					else if (OtherDirection[i] == DIR_RIGHT_FRONT) RotY[i] = -45.0f;

					m_pScene->pHeroObject[i]->SetSpeed(m_pScene->pHeroObject[i]->GetRootSpeed());

				}
				else
				{
					if (OtherDirection[i] == DIR_LEFT) RotY[i] = 90.0f;
					else if (OtherDirection[i] == DIR_RIGHT) RotY[i] = -90.0f;
					else if (OtherDirection[i] == DIR_FRONT) RotY[i] = 0.0f;
					else if (OtherDirection[i] == DIR_BACK) RotY[i] = 180.0f;

					m_pScene->pHeroObject[i]->SetSpeed(m_pScene->pHeroObject[i]->GetNormalSpeed());
				}



				//	if (!m_pScene->pHeroObject[m_pScene->myGame_id]->bHeroAttack && !m_pScene->pHeroObject[m_pScene->myGame_id]->bHeroQ && !m_pScene->pHeroObject[m_pScene->myGame_id]->bHeroW && !m_pScene->pHeroObject[m_pScene->myGame_id]->bHeroE && !m_pScene->pHeroObject[m_pScene->myGame_id]->bHeroR)
				//	{
				if (OtherDirection[i] & DIR_FRONT) m_pScene->pHeroObject[i]->SetPosition(D3DXVECTOR3(m_pScene->pHeroObject[i]->GetPosition().x, m_pScene->pHeroObject[i]->GetPosition().y, m_pScene->pHeroObject[i]->GetPosition().z - m_pScene->pHeroObject[i]->GetSpeed()));
				if (OtherDirection[i] & DIR_BACK) m_pScene->pHeroObject[i]->SetPosition(D3DXVECTOR3(m_pScene->pHeroObject[i]->GetPosition().x, m_pScene->pHeroObject[i]->GetPosition().y, m_pScene->pHeroObject[i]->GetPosition().z + m_pScene->pHeroObject[i]->GetSpeed()));
				if (OtherDirection[i] & DIR_LEFT) m_pScene->pHeroObject[i]->SetPosition(D3DXVECTOR3(m_pScene->pHeroObject[i]->GetPosition().x - m_pScene->pHeroObject[i]->GetSpeed(), m_pScene->pHeroObject[i]->GetPosition().y, m_pScene->pHeroObject[i]->GetPosition().z));
				if (OtherDirection[i] & DIR_RIGHT) m_pScene->pHeroObject[i]->SetPosition(D3DXVECTOR3(m_pScene->pHeroObject[i]->GetPosition().x + m_pScene->pHeroObject[i]->GetSpeed(), m_pScene->pHeroObject[i]->GetPosition().y, m_pScene->pHeroObject[i]->GetPosition().z));





				m_pScene->pHeroObject[i]->Rotate(0, RotY[i], 0);
				//	}

				//if (!m_pScene->pHeroObject[m_pScene->myGame_id]->bHeroAttack && !m_pScene->pHeroObject[m_pScene->myGame_id]->bHeroQ && !m_pScene->pHeroObject[m_pScene->myGame_id]->bHeroW && !m_pScene->pHeroObject[m_pScene->myGame_id]->bHeroE && !m_pScene->pHeroObject[m_pScene->myGame_id]->bHeroR)
				//{
				//	if (OtherDirection[i] == DIR_LEFT)
				//		m_pScene->pHeroObject[i]->SetPosition(D3DXVECTOR3(m_pScene->pHeroObject[i]->GetPosition().x + m_pScene->pHeroObject[i]->GetSpeed(), m_pScene->pHeroObject[i]->GetPosition().y, m_pScene->pHeroObject[i]->GetPosition().z));
				//	if (OtherDirection[i] & DIR_RIGHT)
				//		m_pScene->pHeroObject[i]->SetPosition(D3DXVECTOR3(m_pScene->pHeroObject[i]->GetPosition().x - m_pScene->pHeroObject[i]->GetSpeed(), m_pScene->pHeroObject[i]->GetPosition().y, m_pScene->pHeroObject[i]->GetPosition().z));
				//	if (OtherDirection[i] & DIR_FRONT)
				//		m_pScene->pHeroObject[i]->SetPosition(D3DXVECTOR3(m_pScene->pHeroObject[i]->GetPosition().x, m_pScene->pHeroObject[i]->GetPosition().y, m_pScene->pHeroObject[i]->GetPosition().z + m_pScene->pHeroObject[i]->GetSpeed()));
				//	if (OtherDirection[i] & DIR_BACK)
				//		m_pScene->pHeroObject[i]->SetPosition(D3DXVECTOR3(m_pScene->pHeroObject[i]->GetPosition().x, m_pScene->pHeroObject[i]->GetPosition().y, m_pScene->pHeroObject[i]->GetPosition().z - m_pScene->pHeroObject[i]->GetSpeed()));


				//}


				//m_pScene->pOtherObject[i]->SetPosition(m_pScene->pOtherObject[i]->GetPosition().x + 1.0f, m_pScene->pOtherObject[i]->GetPosition().y, m_pScene->pOtherObject[i]->GetPosition().z);
				//m_pScene->pOtherObject[i]->Move(m_pScene->pOtherObject[i]->GetPosition(), OtherDirection[i], m_pScene->pOtherObject[i]->GetSpeed(), true);
			}
		}


	}
	// 플레이어를 실제로 이동하고 카메라를 갱신한다. 중력과 마찰력의 영향을 속도 벡터에 적용한다.
	m_pPlayer->Update(m_GameTimer.GetTimeElapsed());
}

void CGameFramework::AnimateObjects()
{
	if (m_pScene) m_pScene->AnimateObjects(m_GameTimer.GetTimeElapsed());
}

void CGameFramework::FrameAdvance()
{
	m_GameTimer.Tick(60);

	if (LoadingScene == true)
	{

		LoadingScene = false;
		m_pScene->BuildObjects(m_pd3dDevice, playercount);


		// server send (loading complete)
		cs_packet_LoadingComplete *my_packet = reinterpret_cast<cs_packet_LoadingComplete *>(send_buffer);
		my_packet->size = sizeof(cs_packet_LoadingComplete);
		send_wsabuf.len = sizeof(cs_packet_LoadingComplete);
		DWORD iobyte;
		my_packet->type = CS_LOADCOMPLETE;
		my_packet->roomnumber = m_pScene->MyRoomNumber;
		WSASend(g_mysocket, &send_wsabuf, 1, &iobyte, 0, NULL, NULL);
		//
	}

	if (ChangeScene == GAME)
	{
		ProcessInput();
		AnimateObjects();

		float fClearColor[4] = { 0.0f, 0.125f, 0.3f, 1.0f };
		if (m_pd3dRenderTargetView) m_pd3dDeviceContext->ClearRenderTargetView(m_pd3dRenderTargetView, fClearColor);
		if (m_pd3dDepthStencilView) m_pd3dDeviceContext->ClearDepthStencilView(m_pd3dDepthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);

		if (m_pPlayer) {
			m_pPlayer->UpdateShaderVariables(m_pd3dDeviceContext);
		}
		if (m_pScene) m_pScene->Render(m_pd3dDeviceContext, m_pCamera);
		//if (m_pPlayerShader) m_pPlayerShader->Render(m_pd3dDeviceContext, m_pCamera);

		if (Scene_Animation != 0)
			VibeCamera(1.5f);

		m_pDXGISwapChain->Present(0, 0);


		// win & lose
		if (isWinResult)
		{
			if (ResultTimer == 0)
			{
				ResultTimer = GetTickCount();
			}
			//
			else if (GetTickCount() - ResultTimer > 5000) // 5초 경과
			{
				isWinResult = false;
				ResultTimer = 0;

				ChangeScene = LOBBY;
				isEnding = true;

				InvalidateRect(g_hWnd, NULL, false);

			}


			else
			{
				if (WinTimer == 0)
					WinTimer = GetTickCount();
				else if (GetTickCount() - WinTimer > 100)
				{
					

					if (WinCount == 0)
					{
						m_pScene->WinObject[WinCount]->SetResult(POINT{ 400, 350 }, POINT{ 600, 450 });
						m_pScene->WinObject[WinCount]->ResultUpdate();
					}
					else if (WinCount < 14)
					{
						m_pScene->WinObject[WinCount - 1]->SetResult(POINT{ -400, -350 }, POINT{ -600, -450 });
						m_pScene->WinObject[WinCount - 1]->ResultUpdate();

						m_pScene->WinObject[WinCount]->SetResult(POINT{ 400, 350 }, POINT{ 600, 450 });
						m_pScene->WinObject[WinCount]->ResultUpdate();
					}
					WinTimer = 0;

					++WinCount;
				}
			}
		}

		else if (isLoseResult)
		{
			if (ResultTimer == 0)
			{
				ResultTimer = GetTickCount();
			}
			//
			else if (GetTickCount() - ResultTimer > 5000) // 5초 경과
			{
				isLoseResult = false;
				ResultTimer = 0;

				ChangeScene = LOBBY;

				isEnding = true;

				InvalidateRect(g_hWnd, NULL, false);

			}
			else
			{
				if (LoseTimer == 0)
					LoseTimer = GetTickCount();
				else if (GetTickCount() - LoseTimer > 100)
				{
					

					if (LoseCount == 0)
					{
						m_pScene->LoseObject[LoseCount]->SetResult(POINT{ 400, 350 }, POINT{ 600, 450 });
						m_pScene->LoseObject[LoseCount]->ResultUpdate();
					}
					else if (LoseCount < 18)
					{
						m_pScene->LoseObject[LoseCount - 1]->SetResult(POINT{ -400, -350 }, POINT{ -600, -450 });
						m_pScene->LoseObject[LoseCount - 1]->ResultUpdate();

						m_pScene->LoseObject[LoseCount]->SetResult(POINT{ 400, 350 }, POINT{ 600, 450 });
						m_pScene->LoseObject[LoseCount]->ResultUpdate();
					}
					LoseTimer = 0;

					++LoseCount;
				}
			}
		}

	}


	m_GameTimer.GetFrameRate(m_pszBuffer + 16, 33);
	::SetWindowText(m_hWnd, m_pszBuffer);
}

void CGameFramework::VibeCamera(float VibeSpeed)
{
	if (Scene_count == 30)
	{
		Scene_count = 0;
		Scene_Animation = 0;
	}

	else if (Scene_Animation == 1)
	{
		m_pCamera->Rotate(m_pCamera->GetPitch() + VibeSpeed, m_pCamera->GetYaw() + VibeSpeed, m_pCamera->GetRoll() + VibeSpeed);
		Scene_Animation = 2;

		++Scene_count;
	}
	else if (Scene_Animation == 2)
	{
		m_pCamera->Rotate(m_pCamera->GetPitch() - VibeSpeed, m_pCamera->GetYaw() - VibeSpeed, m_pCamera->GetRoll() - VibeSpeed);
		Scene_Animation = 1;

		++Scene_count;
	}


}