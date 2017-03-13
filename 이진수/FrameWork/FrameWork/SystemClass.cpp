#include "SystemClass.h"



CSystemClass::CSystemClass()
{
	m_Input		= 0;
	m_Graphics	= 0;
}

CSystemClass::CSystemClass(const CSystemClass& ohter)
{

}

CSystemClass::~CSystemClass()
{
} 

bool CSystemClass::Initialize()
{
	int		screenWidth;
	int		screenHeight;
	bool	result;

	// �Լ��� ���̿� �ʺ� �����ϱ� ���� ������ 0���� �ʱ�ȭ �Ѵ�.
	screenWidth		= 0;
	screenHeight	= 0;

	// ��������api�� ����Ͽ� �ʱ�ȭ �Ѵ�.
	InitializeWindows(screenWidth, screenHeight);



	// Input ��ü�� �����Ѵ�. �� ��ü�� ������ ���� ������ Ű���� �Է��� ó���ϱ����� ���ȴ�.
	m_Input = new CInputClass;
	if (!m_Input)
	{
		return false;
	}
	// Input ��ü�� �ʱ�ȭ �Ѵ�.
	m_Input->Initialize();



	// graphics ��ü�� �����Ѵ�. �� ��ü�� �� ���ø����̼��� ��� �׷��� ��Ҹ� �׸��� ���� �Ѵ�.
	m_Graphics = new CGraphicsClass;

	if (!m_Graphics)
	{
		return false;
	}
	// Graphics ��ü�� �ʱ�ȭ �Ѵ�.
	result = m_Graphics->Initialize(screenWidth, screenHeight, m_hwnd);

	if (!result)
	{
		return false;
	}

	return true;
}


void CSystemClass::Shutdown()
{
	// Graphics ��ü�� ��ȯ�Ѵ�.
	if (m_Graphics)
	{
		m_Graphics->Shutdown();
		delete m_Graphics;
		m_Graphics = 0;
	}

	// Input ��ü�� ��ȯ�Ѵ�.
	if (m_Input)
	{
		delete m_Input;
		m_Input = 0;
	}

	// â�� �����Ѵ�.
	ShutdownWindows();

	return;
}

void CSystemClass::Run()
{
	MSG msg;
	bool done;
	bool result;

	// �޼��� ����ü�� �ʱ�ȭ �Ѵ�.
	ZeroMemory(&msg, sizeof(MSG));

	// �����κ��� ���� �޼����� ���� ������ ������ ����.
	done = false;
	while (!done)
	{
		// ������ �޼����� ó���Ѵ�.
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		// �����쿡�� ���ø����̼��� ���Ḧ ��û�ϴ� ��� ����������.
		if (msg.message == WM_QUIT)
		{
			done = true;
		}
		else
		{
			// �� �ܿ��� Frame �Լ��� ó���Ѵ�.
			// Frame �Լ��� ���ø����̼��� ��� �۾��� ó���Ǵ� ���̴�.
			result = Frame();
			if (!result)
			{
				done = true;
			}
		}
	}

	return;
}

bool CSystemClass::Frame()
{
	bool result;

	// ������ EscŰ�� ���� ���ø����̼��� �����ϱ⸦ ���ϴ��� Ȯ���Ѵ�.
	if (m_Input->IsKeyDown(VK_ESCAPE))
	{
		return false;
	}

	// graphics ��ü�� �۾��� ó���Ѵ�.
	result = m_Graphics->Frame();
	if (!result)
	{
		return false;
	}

	return true;
}

LRESULT CALLBACK CSystemClass::MessageHandler(HWND hwnd, UINT umsg, WPARAM wparam, LPARAM lparam)
{
	switch (umsg)
	{
	// Ű���尡 Ű�� ���ȴ��� Ȯ���Ѵ�.
	case WM_KEYDOWN:
	{
		// Ű�� ���ȴٸ� input ��ü�� �� ����� �����Ͽ� ����ϵ��� �Ѵ�.
		m_Input->KeyDown((unsigned int)wparam);
		return 0;
	}

	// Ű���尡 ���� Ű�� ���������� Ȯ���Ѵ�.
	case WM_KEYUP:
	{
		// Ű�� �������ٸ� input ��ü�� �� ����� �����Ͽ� �����ϵ��� �Ѵ�.
		m_Input->KeyUp((unsigned int)wparam);
		return 0;
	}

	// �ٸ� �޼������� ������� �����Ƿ� �⺻ �޼��� ó���⿡ �����Ѵ�.
	default:
	{
		return DefWindowProc(hwnd, umsg, wparam, lparam);
	}

	}
}

void CSystemClass::InitializeWindows(int& screenWidth, int& screenHeight)
{
	WNDCLASSEX wc;
	DEVMODE dmScreenSettings;
	int posX;
	int posY;

	// �ܺ� �����͸� �� ��ü�� �����Ѵ�.
	ApplicationHandle = this;

	// �� ���ø����̼��� �ν��Ͻ��� �����´�.
	m_hinstance = GetModuleHandle(NULL);

	// ���ø����̼��� �̸��� �����Ѵ�.
	m_applicationName = L"Engine";

	// ������ Ŭ������ �⺻ �������� �����.
	wc.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC; 
	wc.lpfnWndProc = WndProc; 
	wc.cbClsExtra = 0; 
	wc.cbWndExtra = 0; 
	wc.hInstance = m_hinstance; 
	wc.hIcon = LoadIcon(NULL, IDI_WINLOGO); 
	wc.hIconSm = wc.hIcon; 
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH); 
	wc.lpszMenuName = NULL; wc.lpszClassName = m_applicationName; 
	wc.cbSize = sizeof(WNDCLASSEX); 
	
	// ������ Ŭ������ ����մϴ�.
	RegisterClassEx(&wc); 
	
	// ����� ȭ���� �ػ󵵸� �˾ƿɴϴ�. 
	screenWidth = GetSystemMetrics(SM_CXSCREEN);
	screenHeight = GetSystemMetrics(SM_CYSCREEN); 
	
	// Ǯ��ũ�� ��� ������ ���� ���� ȭ�� ������ �մϴ�. 
	if(FULL_SCREEN) 
	{ 
		// ���� Ǯ��ũ�� ����� ȭ�� ũ�⸦ ����ũ�� ũ�⿡ ���߰� ������ 32bit�� �մϴ�.
		memset(&dmScreenSettings, 0, sizeof(dmScreenSettings)); 
		dmScreenSettings.dmSize = sizeof(dmScreenSettings);
		dmScreenSettings.dmPelsWidth = (unsigned long)screenWidth; 
		dmScreenSettings.dmPelsHeight = (unsigned long)screenHeight; 
		dmScreenSettings.dmBitsPerPel = 32;
		dmScreenSettings.dmFields = DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT;

		// Ǯ��ũ���� �´� ���÷��� ������ �մϴ�. 
		ChangeDisplaySettings(&dmScreenSettings, CDS_FULLSCREEN);
		
		// �������� ��ġ�� ȭ���� ���� ���� ����ϴ�. 
		posX = posY = 0; 
	} 
	else 
	{ 
		// ������ ����� 800x600�� ũ�⸦ ������ �մϴ�.
		screenWidth = 800; 
		screenHeight = 600; 
		
		// â�� ������� �߾ӿ� ������ �մϴ�.
		posX = (GetSystemMetrics(SM_CXSCREEN) - screenWidth) / 2; 
		posY = (GetSystemMetrics(SM_CYSCREEN) - screenHeight) / 2; 
	} 
	
	// ������ ���� ������ â�� ����� �� �ڵ��� �����ɴϴ�.
	m_hwnd = CreateWindowEx(WS_EX_APPWINDOW, m_applicationName, m_applicationName, WS_CLIPSIBLINGS | WS_CLIPCHILDREN | WS_POPUP, 
		posX, posY, screenWidth, screenHeight, NULL, NULL, m_hinstance, NULL); 

	// �����츦 ȭ�鿡 ǥ���ϰ� ��Ŀ���� �ݴϴ�. 
	ShowWindow(m_hwnd, SW_SHOW); 
	SetForegroundWindow(m_hwnd); 
	SetFocus(m_hwnd); 
	
	// ���콺 Ŀ���� ǥ������ �ʽ��ϴ�.
	ShowCursor(false);

	return; 
}

void CSystemClass::ShutdownWindows()
{
	// ���콺 Ŀ���� ǥ���Ѵ�.
	ShowCursor(true);

	// Ǯ��ũ�� ��带 �������� �� ���÷��� ������ �ٲ۴�.
	if (FULL_SCREEN)
	{
		ChangeDisplaySettings(NULL, 0);
	}

	// â�� �����Ѵ�.
	DestroyWindow(m_hwnd);
	m_hwnd = NULL;

	// ���ø����̼� �ν��Ͻ��� �����Ѵ�.
	UnregisterClass(m_applicationName, m_hinstance);
	m_hinstance = NULL;

	// �� Ŭ������ ���� �ܺ� ������ ������ �����Ѵ�.
	ApplicationHandle = NULL;

	return;
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT umessage, WPARAM wparam, LPARAM lparam) 
{
	switch (umessage) 
	{ 
	// �����찡 ���ŵǾ����� Ȯ���մϴ�. 
	case WM_DESTROY: 
	{ 
		PostQuitMessage(0); 
		return 0; 
	} 
	// �����찡 �������� Ȯ���մϴ�. 
	case WM_CLOSE: 
	{ 
		PostQuitMessage(0); 
		return 0; 
	} 
	// �ٸ� ��� �޼������� system Ŭ������ �޼��� ó���⿡ �����մϴ�.
	default: 
	{ 
		return ApplicationHandle->MessageHandler(hwnd, umessage, wparam, lparam);
	} 

	} 
}

	