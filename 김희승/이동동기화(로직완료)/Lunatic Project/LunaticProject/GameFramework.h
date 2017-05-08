#pragma once
#include "GameTimer.h"
#include "Scene.h"

class CGameFramework
{
private:
	HINSTANCE m_hInstance;
	HWND m_hWnd;

	int m_nWndClientWidth;
	int m_nWndClientHeight;

	// 디바이스 인터페이스에 대한 포인터이다. 주로 리소스를 생성하기 위하여 필요하다.
	ID3D11Device *m_pd3dDevice;
	// 스왑 체인 인터페이스에 대한 포인터이다. 주로 디스플레이를 제어하기 위하여 필요하다.
	IDXGISwapChain *m_pDXGISwapChain;
	// 디바이스 컨텍스트에 대한 포인터이다. 주로 파이프라인 설정을 하기 위하여 필요하다.
	ID3D11DeviceContext *m_pd3dDeviceContext;
	// 렌더 타겟 뷰 인터페이스에 대한 포인터이다. 
	ID3D11RenderTargetView *m_pd3dRenderTargetView;

	// 게임 프레임워크에서 사용할 타이머
	CGameTimer m_GameTimer;
	// 게임의 장면(Scene)을 관리하는 객체에 대한 포인터
	// 프레임 레이트를 주 윈도우의 캡션에 출력하기 위한 문자열
	_TCHAR m_pszBuffer[60];

	// 플레이어 멤버 변수
	

	// 상수 버퍼
	ID3D11Buffer *m_pd3dcbColor;
		
	// 깊이 버퍼(Depth Buffer) 객체 인터페이스와 뷰에 대한 포인터이다.
	ID3D11Texture2D *m_pd3dDepthStencilBuffer;
	ID3D11DepthStencilView *m_pd3dDepthStencilView;

	// 마지막으로 마우스 버튼을 클릭할 때의 마우스 커서의 위치이다.
	POINT	m_ptOldCursorPos;

	CCamera *m_pCamera;

public:
	CGameFramework();
	~CGameFramework();
	CPlayer *m_pPlayer;
	CScene *m_pScene;

	CPlayerShader *m_pPlayerShader;
	
	bool Create(HINSTANCE hInstance, HWND hMainWnd);
	void Destroy();

	//디바이스, 스왑 체인, 디바이스 컨텍스트, 디바이스와 관련된 뷰를 생성하는 함수이다. 
	bool CreateRenderTargetDepthStencilView();
	bool CreateDirect3DDisplay();

	//렌더링할 메쉬, 객체를 생성하고 소멸하는 함수이다. 
	void BuildObjects();
	void ReleaseObjects();

	//프레임워크의 핵심(사용자 입력, 애니메이션, 렌더링)을 구성하는 함수이다. 
	void ProcessInput();
	void AnimateObjects();
	void FrameAdvance();

	//윈도우의 메시지(키보드, 마우스 입력)를 처리하는 함수이다. 
	void OnProcessingMouseMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam);
	void OnProcessingKeyboardMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam);
	LRESULT CALLBACK OnProcessingWindowMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam);

	// Server Func

	DWORD dwDirection = 0;
public:
	void SetHeroPosition(float x, float y, float z);
	void SetOtherPosition(float x, float y, float z);

};
