// LunaticProject.cpp : ���� ���α׷��� ���� �������� �����մϴ�.
//

#include "stdafx.h"
#include "LunaticProject.h"
#include "GameFramework.h"

#define MAX_LOADSTRING 100

// ���� ����:
HINSTANCE hInst;								// ���� �ν��Ͻ��Դϴ�.

TCHAR szTitle[MAX_LOADSTRING];					// ���� ǥ���� �ؽ�Ʈ�Դϴ�.
TCHAR szWindowClass[MAX_LOADSTRING];			// �⺻ â Ŭ���� �̸��Դϴ�.

CGameFramework gGameFramework;

// �� �ڵ� ��⿡ ��� �ִ� �Լ��� ������ �����Դϴ�.
ATOM				MyRegisterClass(HINSTANCE hInstance);
BOOL				InitInstance(HINSTANCE, int);
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK	About(HWND, UINT, WPARAM, LPARAM);

int APIENTRY _tWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPTSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

 	// TODO: ���⿡ �ڵ带 �Է��մϴ�.
	MSG msg = { 0 };
	HACCEL hAccelTable;

	// ���� ���ڿ��� �ʱ�ȭ�մϴ�.
	LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadString(hInstance, IDC_LUNATICPROJECT, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// ���� ���α׷� �ʱ�ȭ�� �����մϴ�.
	if (!InitInstance (hInstance, nCmdShow))
	{
		return FALSE;
	}

	hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_LUNATICPROJECT));

	// server
	WSADATA	wsadata;
	WSAStartup(MAKEWORD(2, 2), &wsadata);

	g_mysocket = WSASocket(AF_INET, SOCK_STREAM, IPPROTO_TCP, NULL, 0, 0);

	char ipAddr[20];
	cout << "������ ������ IP�ּҸ� �Է��ϼ��� : ";
	cin >> ipAddr;

	SOCKADDR_IN ServerAddr;
	ZeroMemory(&ServerAddr, sizeof(SOCKADDR_IN));
	ServerAddr.sin_family = AF_INET;
	ServerAddr.sin_port = htons(MY_SERVER_PORT);
	//ServerAddr.sin_addr.s_addr = inet_addr("127.0.0.1");
	ServerAddr.sin_addr.s_addr = inet_addr(ipAddr);

	int Result = WSAConnect(g_mysocket, (sockaddr *)&ServerAddr, sizeof(ServerAddr), NULL, NULL, NULL, NULL);
	WSAAsyncSelect(g_mysocket, g_hWnd, WM_SOCKET, FD_CLOSE | FD_READ);

	send_wsabuf.buf = send_buffer;
	send_wsabuf.len = BUF_SIZE;
	recv_wsabuf.buf = recv_buffer;
	recv_wsabuf.len = BUF_SIZE;

	// �⺻ �޽��� �����Դϴ�.
	while (WM_QUIT != msg.message)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else
		{
			gGameFramework.FrameAdvance();
		}
	}
	gGameFramework.Destroy();


	return (int) msg.wParam;
}



//
//  �Լ�: MyRegisterClass()
//
//  ����: â Ŭ������ ����մϴ�.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style			= CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc	= WndProc;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= hInstance;
	wcex.hIcon			= LoadIcon(hInstance, MAKEINTRESOURCE(IDI_LUNATICPROJECT));
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW+1);
	wcex.lpszMenuName	= NULL;	
	wcex.lpszClassName	= szWindowClass;
	wcex.hIconSm		= LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	return RegisterClassEx(&wcex);
}

//
//   �Լ�: InitInstance(HINSTANCE, int)
//
//   ����: �ν��Ͻ� �ڵ��� �����ϰ� �� â�� ����ϴ�.
//
//   ����:
//
//        �� �Լ��� ���� �ν��Ͻ� �ڵ��� ���� ������ �����ϰ�
//        �� ���α׷� â�� ���� ���� ǥ���մϴ�.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
	hInst = hInstance; // �ν��Ͻ� �ڵ��� ���� ������ �����մϴ�.

	DWORD dwStyle = WS_OVERLAPPED | WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU | WS_BORDER;
	RECT rc = { 0, 0, FRAME_BUFFER_WIDTH, FRAME_BUFFER_HEIGHT };
	AdjustWindowRect(&rc, dwStyle, FALSE);

	g_hWnd = CreateWindow(szWindowClass, szTitle, dwStyle, CW_USEDEFAULT, CW_USEDEFAULT, rc.right - rc.left, rc.bottom - rc.top, NULL, NULL, hInstance, NULL);
	if (!g_hWnd)
		return FALSE;

	gGameFramework.Create(hInstance, g_hWnd);

	ShowWindow(g_hWnd, nCmdShow);
	UpdateWindow(g_hWnd);

	return TRUE;
}

//
//  �Լ�: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  ����:  �� â�� �޽����� ó���մϴ�.
//
//  WM_COMMAND	- ���� ���α׷� �޴��� ó���մϴ�.
//  WM_PAINT	- �� â�� �׸��ϴ�.
//  WM_DESTROY	- ���� �޽����� �Խ��ϰ� ��ȯ�մϴ�.
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	int wmId, wmEvent;

	HDC hdc, memdc, memdc2;

	HBITMAP hBackBit, hOldBitmap;

	HPEN Pen, oldPen;
	PAINTSTRUCT ps;
	static HBITMAP bmp_room, bmp_loading, bmp_mainmenu, bmp_lobby;

	switch (message)
	{
	case WM_CREATE:
		bmp_room = (HBITMAP)LoadBitmap(hInst, MAKEINTRESOURCE(IDB_BITMAP1));
		bmp_loading = (HBITMAP)LoadBitmap(hInst, MAKEINTRESOURCE(IDB_BITMAP2));
		bmp_mainmenu = (HBITMAP)LoadBitmap(hInst, MAKEINTRESOURCE(IDB_BITMAP3));
		bmp_lobby = (HBITMAP)LoadBitmap(hInst, MAKEINTRESOURCE(IDB_BITMAP4));
		break;

	case WM_PAINT:


		hdc = BeginPaint(hWnd, &ps);

		memdc = CreateCompatibleDC(hdc);
		hBackBit = CreateCompatibleBitmap(hdc, 1024, 768);
		hOldBitmap = (HBITMAP)SelectObject(memdc, hBackBit);

		Pen = CreatePen(PS_SOLID, 5, RGB(255, 0, 0));
		oldPen = (HPEN)SelectObject(memdc, Pen);

		memdc2 = CreateCompatibleDC(hdc);



		if (gGameFramework.ChangeScene == MAINMENU)
		{
			SelectObject(memdc2, bmp_mainmenu);
			BitBlt(memdc, 0, 0, 1024, 768, memdc2, 0, 0, SRCCOPY);
		}

		else if (gGameFramework.ChangeScene == LOBBY)
		{
			SelectObject(memdc2, bmp_lobby);
			
			SelectObject(memdc, GetStockObject(NULL_BRUSH));
			TextOut(memdc2, 420, 120, L"���� ����", 5);
			BitBlt(memdc, 0, 0, 1024, 768, memdc2, 0, 0, SRCCOPY);

			
			
		}

		else if (gGameFramework.ChangeScene == ROOM)
		{

			SelectObject(memdc2, bmp_room);
			BitBlt(memdc, 0, 0, 1024, 768, memdc2, 0, 0, SRCCOPY);

			SelectObject(memdc, GetStockObject(NULL_BRUSH));
			Rectangle(memdc, 229 + ((gGameFramework.SelectCount - 1) * 199), 130, 370 + ((gGameFramework.SelectCount - 1) * 199), 270);

		}

		if (gGameFramework.LoadingScene)
		{
			SelectObject(memdc2, bmp_loading);
			BitBlt(memdc, 0, 0, 1024, 768, memdc2, 0, 0, SRCCOPY);
		}


		SelectObject(memdc, oldPen);
		DeleteObject(Pen);
		DeleteDC(memdc2);

		BitBlt(hdc, 0, 0, 1024, 768, memdc, 0, 0, SRCCOPY);

		SelectObject(memdc, hOldBitmap);
		DeleteObject(hBackBit);
		DeleteDC(memdc);

		EndPaint(hWnd, &ps);

		break;
	// server
	case WM_SOCKET:
	{
		if (WSAGETSELECTERROR(lParam)) {
			closesocket((SOCKET)wParam);
			exit(-1);
			break;
		}
		switch (WSAGETSELECTEVENT(lParam)) {
		case FD_READ:
			ReadPacket((SOCKET)wParam);
			break;
		case FD_CLOSE:
			closesocket((SOCKET)wParam);
			exit(-1);
			break;
		}
	}
	case WM_SIZE:
		break;
	case WM_LBUTTONDOWN:
	case WM_LBUTTONUP:
	case WM_RBUTTONDOWN:
	case WM_RBUTTONUP:
	case WM_MOUSEMOVE:
	case WM_KEYDOWN:
	case WM_KEYUP:
		if(activate)
			gGameFramework.OnProcessingWindowMessage(hWnd, message, wParam, lParam);
		break;
	case WM_SETFOCUS:
		activate = true;
		break;
	case WM_KILLFOCUS:
		activate = false;
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

// ���� ��ȭ ������ �޽��� ó�����Դϴ�.
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
		return (INT_PTR)TRUE;

	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
		{
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		break;
	}
	return (INT_PTR)FALSE;
}

// server
void ReadPacket(SOCKET sock)
{
	DWORD iobyte, ioflag = 0;

	int ret = WSARecv(sock, &recv_wsabuf, 1, &iobyte, &ioflag, NULL, NULL);
	if (ret) {
		int err_code = WSAGetLastError();
		printf("Recv Error [%d]\n", err_code);
	}

	BYTE *ptr = reinterpret_cast<BYTE *>(recv_buffer);

	while (0 != iobyte) {
		if (0 == in_packet_size) in_packet_size = ptr[0];
		if (iobyte + saved_packet_size >= in_packet_size) {
			memcpy(packet_buffer + saved_packet_size, ptr, in_packet_size - saved_packet_size);
			ProcessPacket(packet_buffer);
			ptr += in_packet_size - saved_packet_size;
			iobyte -= in_packet_size - saved_packet_size;
			in_packet_size = 0;
			saved_packet_size = 0;
		}

		else {
			memcpy(packet_buffer + saved_packet_size, ptr, iobyte);
			saved_packet_size += iobyte;
			iobyte = 0;
		}
	}
}

void ProcessPacket(char * ptr)
{
	static bool first_time = true;
	switch (ptr[1])
	{
	// ����
	case SC_ID:
	{
		sc_packet_id *my_packet = reinterpret_cast<sc_packet_id *>(ptr);
		int id = my_packet->id;
		if (first_time) {
			first_time = false;
			g_myid = id;
		}
		cout << "���Ƶ� : " << g_myid << endl;
		break;
	}

	// ��
	case SC_READY:
	{
		sc_packet_ready *my_packet = reinterpret_cast<sc_packet_ready *>(ptr);
		int id = my_packet->id;
		if (id == g_myid) {
			cout << "SC_READY\n";
			cout << "�� ĳ���� ���� : " << (int)my_packet->hero_pick << endl;
			
			//gGameFramework.m_pScene->pHeroObject[g_myid]->m_HeroSelect =  SC_BABARIAN - 10;

		}
		else {
			gGameFramework.m_pScene->pHeroObject[id]->m_HeroSelect = my_packet->hero_pick;
			gGameFramework.m_pScene->pHeroObject[id]->m_Team = A_TEAM;
		}
		break;
	}
	case SC_ALLREADY:

		Sleep(3000);
		gGameFramework.LoadingScene = true;
		InvalidateRect(g_hWnd, NULL, false);
		cout << "�÷���\n";
		//gGameFramework.ChangeScene = GAME;

		break;

	// �ΰ���
	case SC_PUT_PLAYER:
	{
		sc_packet_put_player *my_packet = reinterpret_cast<sc_packet_put_player *>(ptr);
		int id = my_packet->id;
		if (id == g_myid) {
			cout << "Hero Pos\n";
			gGameFramework.m_pScene->pHeroObject[g_myid]->SetPosition(my_packet->x, my_packet->y, my_packet->z);
			gGameFramework.m_pPlayer->Move(D3DXVECTOR3(my_packet->x, my_packet->y, my_packet->z));
		}
		else {
			cout << "Other Pos\n";
			gGameFramework.m_pScene->pHeroObject[id]->SetPosition(my_packet->x, my_packet->y, my_packet->z);
		}
		break;	
	}

	case SC_POS:
	{
		sc_packet_pos *my_packet = reinterpret_cast<sc_packet_pos *>(ptr);
		int id = my_packet->id;
		if (id == g_myid) {
			//cout << "Hero Move\n";
			gGameFramework.dwDirection = my_packet->direction;
			gGameFramework.m_pScene->pHeroObject[g_myid]->SetPosition(my_packet->x, my_packet->y, my_packet->z);

			if (my_packet->direction != 0)
				gGameFramework.m_pScene->m_ppShaders[g_myid+1]->GetFBXMesh->SetAnimation(ANI_RUN);
			else
				gGameFramework.m_pScene->m_ppShaders[g_myid+1]->GetFBXMesh->SetAnimation(ANI_IDLE);
		}
		else {
			//cout << "Other Move\n";
			gGameFramework.OtherDirection[id] = my_packet->direction;
			gGameFramework.m_pScene->pHeroObject[id]->SetPosition(my_packet->x, my_packet->y, my_packet->z);

			
			if (my_packet->direction != 0)
				gGameFramework.m_pScene->m_ppShaders[id + 1]->GetFBXMesh->SetAnimation(ANI_RUN);
			else
				gGameFramework.m_pScene->m_ppShaders[id + 1]->GetFBXMesh->SetAnimation(ANI_IDLE);
		
			
		
			cout << "Other id : " << id << endl;

		}
		break;
	}

	case SC_ATTACK:
	{
		sc_packet_attack *my_packet = reinterpret_cast<sc_packet_attack *>(ptr);
		int id = my_packet->id;
		if (id == g_myid) {
			cout << "[My]SC_ATTACK_PACKET\n";
		}
		else {
			cout << "[Other]SC_ATTACK_PACKET\n";
			gGameFramework.m_pScene->m_ppShaders[id + 1]->GetFBXMesh->SetAnimation(ANI_ATTACK);
		}

		break;
	}

	case SC_SKILL_Q:
	{
		sc_packet_skillQ *my_packet = reinterpret_cast<sc_packet_skillQ *>(ptr);
		int id = my_packet->id;
		if (id == g_myid) {
			cout << "[My]SC_Q_PACKET\n";

		}
		else {
			cout << "[Other]SC_Q_PACKET\n";
			gGameFramework.m_pScene->m_ppShaders[id + 1]->GetFBXMesh->SetAnimation(ANI_Q);
		}

		break;
	}

	case SC_SKILL_W:
	{
		sc_packet_skillW *my_packet = reinterpret_cast<sc_packet_skillW *>(ptr);
		int id = my_packet->id;
		if (id == g_myid) {
			cout << "[My]SC_W_PACKET\n";
		}
		else {
			cout << "[Other]SC_W_PACKET\n";
			gGameFramework.m_pScene->m_ppShaders[id + 1]->GetFBXMesh->SetAnimation(ANI_W);
		}

		break;
	}

	case SC_SKILL_E:
	{
		sc_packet_skillE *my_packet = reinterpret_cast<sc_packet_skillE *>(ptr);
		int id = my_packet->id;
		if (id == g_myid) {
			cout << "[My]SC_E_PACKET\n";
		}
		else {
			cout << "[Other]SC_E_PACKET\n";
			gGameFramework.m_pScene->m_ppShaders[id + 1]->GetFBXMesh->SetAnimation(ANI_E);
		}

		break;
	}

	case SC_SKILL_R:
	{
		sc_packet_skillR *my_packet = reinterpret_cast<sc_packet_skillR *>(ptr);
		int id = my_packet->id;
		if (id == g_myid) {
			cout << "[My]SC_R_PACKET\n";
		}
		else {
			cout << "[Other]SC_R_PACKET\n";
			gGameFramework.m_pScene->m_ppShaders[id + 1]->GetFBXMesh->SetAnimation(ANI_R);
		}

		break;
	}



	case SC_REMOVE_PLAYER:
	{
		sc_packet_remove_player *my_packet = reinterpret_cast<sc_packet_remove_player *>(ptr);
		int id = my_packet->id;
		if (id != g_myid) {
			//gGameFramework.m_pScene->pOtherObject[id]->SetPosition(0.f,-3000.f,0.f);
		}
		break;
	}
	default:
		printf("Unknown PACKET type [%d]\n", ptr[1]);
	}
}