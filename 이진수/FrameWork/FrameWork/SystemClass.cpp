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

	// 함수에 높이와 너비를 전달하기 전에 변수를 0으로 초기화 한다.
	screenWidth		= 0;
	screenHeight	= 0;

	// 윈도우즈api를 사용하여 초기화 한다.
	InitializeWindows(screenWidth, screenHeight);



	// Input 객체를 생성한다. 이 객체는 유저로 부터 들어오는 키보드 입력을 처리하기위해 사용된다.
	m_Input = new CInputClass;
	if (!m_Input)
	{
		return false;
	}
	// Input 객체를 초기화 한다.
	m_Input->Initialize();



	// graphics 객체를 생성한다. 이 객체는 이 어플리케이션의 모든 그래픽 요소를 그리는 일을 한다.
	m_Graphics = new CGraphicsClass;

	if (!m_Graphics)
	{
		return false;
	}
	// Graphics 객체를 초기화 한다.
	result = m_Graphics->Initialize(screenWidth, screenHeight, m_hwnd);

	if (!result)
	{
		return false;
	}

	return true;
}


void CSystemClass::Shutdown()
{
	// Graphics 객체를 반환한다.
	if (m_Graphics)
	{
		m_Graphics->Shutdown();
		delete m_Graphics;
		m_Graphics = 0;
	}

	// Input 객체를 반환한다.
	if (m_Input)
	{
		delete m_Input;
		m_Input = 0;
	}

	// 창을 종료한다.
	ShutdownWindows();

	return;
}

void CSystemClass::Run()
{
	MSG msg;
	bool done;
	bool result;

	// 메세지 구조체를 초기화 한다.
	ZeroMemory(&msg, sizeof(MSG));

	// 유저로부터 종료 메세지를 받을 때까지 루프를 돈다.
	done = false;
	while (!done)
	{
		// 윈도우 메세지를 처리한다.
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		// 윈도우에서 어플리케이션의 종료를 요청하는 경우 빠져나간다.
		if (msg.message == WM_QUIT)
		{
			done = true;
		}
		else
		{
			// 그 외에는 Frame 함수를 처리한다.
			// Frame 함수는 어플리케이션의 모든 작업이 처리되는 곳이다.
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

	// 유저가 Esc키를 눌러 어플리케이션을 종료하기를 원하는지 확인한다.
	if (m_Input->IsKeyDown(VK_ESCAPE))
	{
		return false;
	}

	// graphics 객체의 작업을 처리한다.
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
	// 키보드가 키가 눌렸는지 확인한다.
	case WM_KEYDOWN:
	{
		// 키가 눌렸다면 input 객체에 이 사실을 전달하여 기록하도록 한다.
		m_Input->KeyDown((unsigned int)wparam);
		return 0;
	}

	// 키보드가 눌린 키가 떼어졌는지 확인한다.
	case WM_KEYUP:
	{
		// 키가 떼어졌다면 input 객체에 이 사실을 전달하여 해제하도록 한다.
		m_Input->KeyUp((unsigned int)wparam);
		return 0;
	}

	// 다른 메세지들은 사용하지 않으므로 기본 메세지 처리기에 전달한다.
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

	// 외부 포인터를 이 객체로 설정한다.
	ApplicationHandle = this;

	// 이 어플리케이션의 인스턴스를 가져온다.
	m_hinstance = GetModuleHandle(NULL);

	// 어플리케이션의 이름을 설정한다.
	m_applicationName = L"Engine";

	// 윈도우 클래스를 기본 설정으로 맞춘다.
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
	
	// 윈도우 클래스를 등록합니다.
	RegisterClassEx(&wc); 
	
	// 모니터 화면의 해상도를 알아옵니다. 
	screenWidth = GetSystemMetrics(SM_CXSCREEN);
	screenHeight = GetSystemMetrics(SM_CYSCREEN); 
	
	// 풀스크린 모드 변수의 값에 따라 화면 설정을 합니다. 
	if(FULL_SCREEN) 
	{ 
		// 만약 풀스크린 모드라면 화면 크기를 데스크톱 크기에 맞추고 색상을 32bit로 합니다.
		memset(&dmScreenSettings, 0, sizeof(dmScreenSettings)); 
		dmScreenSettings.dmSize = sizeof(dmScreenSettings);
		dmScreenSettings.dmPelsWidth = (unsigned long)screenWidth; 
		dmScreenSettings.dmPelsHeight = (unsigned long)screenHeight; 
		dmScreenSettings.dmBitsPerPel = 32;
		dmScreenSettings.dmFields = DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT;

		// 풀스크린에 맞는 디스플레이 설정을 합니다. 
		ChangeDisplaySettings(&dmScreenSettings, CDS_FULLSCREEN);
		
		// 윈도우의 위치를 화면의 왼쪽 위로 맞춥니다. 
		posX = posY = 0; 
	} 
	else 
	{ 
		// 윈도우 모드라면 800x600의 크기를 가지게 합니다.
		screenWidth = 800; 
		screenHeight = 600; 
		
		// 창을 모니터의 중앙에 오도록 합니다.
		posX = (GetSystemMetrics(SM_CXSCREEN) - screenWidth) / 2; 
		posY = (GetSystemMetrics(SM_CYSCREEN) - screenHeight) / 2; 
	} 
	
	// 설정한 것을 가지고 창을 만들고 그 핸들을 가져옵니다.
	m_hwnd = CreateWindowEx(WS_EX_APPWINDOW, m_applicationName, m_applicationName, WS_CLIPSIBLINGS | WS_CLIPCHILDREN | WS_POPUP, 
		posX, posY, screenWidth, screenHeight, NULL, NULL, m_hinstance, NULL); 

	// 윈도우를 화면에 표시하고 포커스를 줍니다. 
	ShowWindow(m_hwnd, SW_SHOW); 
	SetForegroundWindow(m_hwnd); 
	SetFocus(m_hwnd); 
	
	// 마우스 커서를 표시하지 않습니다.
	ShowCursor(false);

	return; 
}

void CSystemClass::ShutdownWindows()
{
	// 마우스 커서를 표시한다.
	ShowCursor(true);

	// 풀스크린 모드를 빠져나올 떄 디스플레이 설정을 바꾼다.
	if (FULL_SCREEN)
	{
		ChangeDisplaySettings(NULL, 0);
	}

	// 창을 제거한다.
	DestroyWindow(m_hwnd);
	m_hwnd = NULL;

	// 어플리케이션 인스턴스를 제거한다.
	UnregisterClass(m_applicationName, m_hinstance);
	m_hinstance = NULL;

	// 이 클래스에 대한 외부 포인터 참조를 제거한다.
	ApplicationHandle = NULL;

	return;
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT umessage, WPARAM wparam, LPARAM lparam) 
{
	switch (umessage) 
	{ 
	// 윈도우가 제거되었는지 확인합니다. 
	case WM_DESTROY: 
	{ 
		PostQuitMessage(0); 
		return 0; 
	} 
	// 윈도우가 닫히는지 확인합니다. 
	case WM_CLOSE: 
	{ 
		PostQuitMessage(0); 
		return 0; 
	} 
	// 다른 모든 메세지들은 system 클래스의 메세지 처리기에 전달합니다.
	default: 
	{ 
		return ApplicationHandle->MessageHandler(hwnd, umessage, wparam, lparam);
	} 

	} 
}

	