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

	// Direct3D ��ü�� �����Ѵ�.
	m_D3D = new CD3DClass;
	if (!m_D3D)
	{
		return false;
	}

	// Direct3D ��ü�� �ʱ�ȭ �Ѵ�.
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
	// D3D ��ü�� ��ȯ�Ѵ�.
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

	// �׷��� �������� �����Ѵ�.
	result = Render();
	if (!result)
	{
		return false;
	}

	return true;
}

bool CGraphicsClass::Render()
{
	// �� �׸��⸦ �����ϱ� ���� ������ ������ �����.
	m_D3D->BeginScene(1.0f, 1.f, 0.f, 1.0f);

	// ���ۿ� �׷��� ���� ȭ�鿡 ǥ���Ѵ�.
	m_D3D->EndScene();

	return true;
}





















