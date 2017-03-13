#include "GraphicsClass.h"



CGraphicsClass::CGraphicsClass()
{
	m_D3D = 0;
}

CGraphicsClass::CGraphicsClass(const CGraphicsClass& other)
{
}

CGraphicsClass::~CGraphicsClass()
{
}

bool CGraphicsClass::Initialize(int screenWidth, int screenHeight, HWND hwnd)
{
	bool result;

	// Direct3D 객체를 생성한다.
	m_D3D = new CD3DClass;
	if (!m_D3D)
	{
		return false;
	}

	// Direct3D 객체를 초기화 한다.
	result = m_D3D->Initialize(screenWidth, screenHeight, VSYNC_ENABLED, hwnd, FULL_SCREEN, SCREEN_DEPTH, SCREEN_NEAR);
	if (!result)
	{
		MessageBox(hwnd, L"Cuold not initialize Direct3D", L"Error", MB_OK);
		return false;
	}

	return true;
}

void CGraphicsClass::Shutdown()
{
	// D3D 객체를 반환한다.
	if (m_D3D)
	{
		m_D3D->Shutdown();
		delete m_D3D;
		m_D3D = 0;
	}
	return;
}

bool CGraphicsClass::Frame()
{
	bool result;

	// 그래픽 렌더링을 수행한다.
	result = Render();
	if (!result)
	{
		return false;
	}

	return true;
}

bool CGraphicsClass::Render()
{
	// 씬 그리기를 시작하기 위해 버퍼의 내용을 지운다.
	m_D3D->BeginScene(1.0f, 1.f, 0.f, 1.0f);

	// 버퍼에 그려진 씬을 화면에 표시한다.
	m_D3D->EndScene();

	return true;
}





















