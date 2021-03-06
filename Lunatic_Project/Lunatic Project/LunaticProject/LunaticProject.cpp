// LunaticProject.cpp : 응용 프로그램에 대한 진입점을 정의합니다.
//

#include "stdafx.h"
#include "LunaticProject.h"

CGameFramework gGameFramework;
CLobby gLobby;
CMainMenu gMainMenu;
CRoom gRoom;
HWND hChat{};

// 전역 변수:
HINSTANCE hInst;								// 현재 인스턴스입니다.

TCHAR szTitle[MAX_LOADSTRING];					// 제목 표시줄 텍스트입니다.
TCHAR szWindowClass[MAX_LOADSTRING];			// 기본 창 클래스 이름입니다.

WNDPROC wpOldEditProc;


// 이 코드 모듈에 들어 있는 함수의 정방향 선언입니다.
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

	// TODO: 여기에 코드를 입력합니다.
	MSG msg = { 0 };
	HACCEL hAccelTable;

	// 전역 문자열을 초기화합니다.
	LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadString(hInstance, IDC_LUNATICPROJECT, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// 응용 프로그램 초기화를 수행합니다.
	if (!InitInstance(hInstance, nCmdShow))
	{
		return FALSE;
	}

	hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_LUNATICPROJECT));

	// server
	wcout.imbue(locale("korean"));
	WSADATA	wsadata;
	WSAStartup(MAKEWORD(2, 2), &wsadata);

	g_mysocket = WSASocket(AF_INET, SOCK_STREAM, IPPROTO_TCP, NULL, 0, 0);



	SOCKADDR_IN ServerAddr;
	ZeroMemory(&ServerAddr, sizeof(SOCKADDR_IN));
	ServerAddr.sin_family = AF_INET;
	ServerAddr.sin_port = htons(MY_SERVER_PORT);

#ifdef _DEBUG
	ServerAddr.sin_addr.s_addr = inet_addr("127.0.0.1"); // 
#else
	char ipAddr[20];
	cout << "접속할 서버의 IP주소를 입력하세요 : ";
	cin >> ipAddr;
	ServerAddr.sin_addr.s_addr = inet_addr(ipAddr);
#endif



	int Result = WSAConnect(g_mysocket, (sockaddr *)&ServerAddr, sizeof(ServerAddr), NULL, NULL, NULL, NULL);
	WSAAsyncSelect(g_mysocket, g_hWnd, WM_SOCKET, FD_CLOSE | FD_READ);

	send_wsabuf.buf = send_buffer;
	send_wsabuf.len = BUF_SIZE;
	recv_wsabuf.buf = recv_buffer;
	recv_wsabuf.len = BUF_SIZE;

	// 기본 메시지 루프입니다.
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


	return (int)msg.wParam;
}



//
//  함수: MyRegisterClass()
//
//  목적: 창 클래스를 등록합니다.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_LUNATICPROJECT));
	wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName = NULL;
	wcex.lpszClassName = szWindowClass;
	wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	return RegisterClassEx(&wcex);
}

//
//   함수: InitInstance(HINSTANCE, int)
//
//   목적: 인스턴스 핸들을 저장하고 주 창을 만듭니다.
//
//   설명:
//
//        이 함수를 통해 인스턴스 핸들을 전역 변수에 저장하고
//        주 프로그램 창을 만든 다음 표시합니다.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
	hInst = hInstance; // 인스턴스 핸들을 전역 변수에 저장합니다.



	DWORD dwStyle = WS_OVERLAPPED | WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU | WS_BORDER;
	RECT rc = { 0, 0, FRAME_BUFFER_WIDTH, FRAME_BUFFER_HEIGHT };
	AdjustWindowRect(&rc, dwStyle, FALSE);

	g_hWnd = CreateWindow(szWindowClass, szTitle, dwStyle, CW_USEDEFAULT, CW_USEDEFAULT, rc.right - rc.left, rc.bottom - rc.top, NULL, NULL, hInstance, NULL);
	if (!g_hWnd)
		return FALSE;

	gGameFramework.Create(hInstance, g_hWnd);
	gMainMenu.Create(hInstance);
	gLobby.Create(hInstance);
	gRoom.Create(hInstance);

	ShowWindow(g_hWnd, nCmdShow);
	UpdateWindow(g_hWnd);

	return TRUE;
}

//
//  함수: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  목적:  주 창의 메시지를 처리합니다.
//
//  WM_COMMAND	- 응용 프로그램 메뉴를 처리합니다.
//  WM_PAINT	- 주 창을 그립니다.
//  WM_DESTROY	- 종료 메시지를 게시하고 반환합니다.
//
//

bool MouseInbox(int left, int top, int right, int bottom, int x, int y)
{
	if (left < x && right > x && top < y  && bottom > y)
		return true;
	else
		return false;
}
LRESULT CALLBACK EditProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	case WM_CHAR:
	{
		if (wParam == VK_RETURN && gGameFramework.ChangeScene == LOBBY)
		{
			if (!gLobby.RoomCreateWindow)
			{
				if (wcslen(gLobby.input))
				{
					cs_packet_lobbychat *my_packet = reinterpret_cast<cs_packet_lobbychat *>(send_buffer);
					my_packet->size = sizeof(cs_packet_lobbychat);
					send_wsabuf.len = sizeof(cs_packet_lobbychat);
					DWORD iobyte;
					my_packet->type = CS_LOBBY_CHAT;
					wcscpy_s(my_packet->message, gLobby.input);
					wcscpy_s(my_packet->id, gMainMenu.user_id);
					WSASend(g_mysocket, &send_wsabuf, 1, &iobyte, 0, NULL, NULL);

					SetWindowTextW(hWnd, '\0');
				}

				SetFocus(g_hWnd);
			}

			// Indicate that we processed the message.
			return 0;
		}
		else if (wParam == VK_RETURN && gGameFramework.ChangeScene == ROOM)
		{
			if (wcslen(gRoom.input))
			{
				cs_packet_roomchat *my_packet = reinterpret_cast<cs_packet_roomchat *>(send_buffer);
				my_packet->size = sizeof(cs_packet_roomchat);
				send_wsabuf.len = sizeof(cs_packet_roomchat);
				DWORD iobyte;
				my_packet->type = CS_ROOM_CHAT;
				wcscpy_s(my_packet->message, gRoom.input);
				wcscpy_s(my_packet->id, gMainMenu.user_id);
				my_packet->roomnumber = gRoom.RoomInfo.room_number;
				WSASend(g_mysocket, &send_wsabuf, 1, &iobyte, 0, NULL, NULL);

				memset(gRoom.input, 0, sizeof(gRoom.input));
				SetWindowTextW(hWnd, '\0');
			}

			SetFocus(g_hWnd);
		}

	}
	}

	// Pass the messages we don't process here on to the
	// original window procedure for default handling.
	return CallWindowProc(wpOldEditProc, hWnd, msg, wParam, lParam);
}


LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	int wmId, wmEvent;

	HDC hdc, memdc, memdc2;

	HFONT hFont, hOldFont;

	HBITMAP hBackBit, hOldBitmap;

	HPEN Pen, oldPen;
	PAINTSTRUCT ps;
	static HBITMAP bmp_loading; // bmp_lobby;
	static HBITMAP bmp_ending;  // bmp_ending

	static HBITMAP bmp_babarian;
	static HBITMAP bmp_healer;
	static HBITMAP bmp_knight;
	static HBITMAP bmp_magician;
	static HBITMAP bmp_swordman;
	static HBITMAP bmp_witch;


	static wchar_t s[5];
	switch (message)
	{
	case WM_CREATE:
#ifdef _DEBUG
#else
		SoundInit();
#endif
		bmp_loading = (HBITMAP)LoadBitmap(hInst, MAKEINTRESOURCE(IDB_LOADINGWINDOW));
		bmp_ending = (HBITMAP)LoadBitmap(hInst, MAKEINTRESOURCE(IDB_ENDING_BACK));

		bmp_babarian = (HBITMAP)LoadBitmap(hInst, MAKEINTRESOURCE(IDB_R_BABARIAN));
		bmp_healer = (HBITMAP)LoadBitmap(hInst, MAKEINTRESOURCE(IDB_R_HEALER));
		bmp_knight = (HBITMAP)LoadBitmap(hInst, MAKEINTRESOURCE(IDB_R_KNIGHT));
		bmp_magician = (HBITMAP)LoadBitmap(hInst, MAKEINTRESOURCE(IDB_R_MAGICIAN));
		bmp_swordman = (HBITMAP)LoadBitmap(hInst, MAKEINTRESOURCE(IDB_R_SWORDMAN));
		bmp_witch = (HBITMAP)LoadBitmap(hInst, MAKEINTRESOURCE(IDB_R_WITCH));
		

		// Chatting
		hChat = CreateWindow(L"edit", NULL, WS_CHILD | WS_VISIBLE | WS_BORDER |
			ES_AUTOHSCROLL, 2000, 2000, 200, 25, hWnd, (HMENU)ID_EDIT, hInst, NULL);


		PostMessage(hChat, EM_LIMITTEXT, (WPARAM)MAX_STR_SIZE - 1, 0);
		wpOldEditProc = (WNDPROC)SetWindowLongPtr(hChat,
			GWLP_WNDPROC, (LONG_PTR)EditProc);
		break;

	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);

		memdc = CreateCompatibleDC(hdc);
		hBackBit = CreateCompatibleBitmap(hdc, 1024, 768);
		hOldBitmap = (HBITMAP)SelectObject(memdc, hBackBit);


		memdc2 = CreateCompatibleDC(hdc);

		if (gGameFramework.ChangeScene == MAINMENU)
			gMainMenu.Draw(memdc, memdc2);

		else if (gGameFramework.ChangeScene == LOBBY)
		{
			if (gGameFramework.isEnding)
			{

				SelectObject(memdc2, bmp_ending);
				BitBlt(memdc, 0, 0, 1024, 768, memdc2, 0, 0, SRCCOPY);



				for (int i = 0; i <= 6; i += 2) // 결과창 아이콘 이미지
				{
					if (gGameFramework.m_pScene->pHeroObject[i]->m_HeroSelect == BABARIAN) SelectObject(memdc2, bmp_babarian);
					else if (gGameFramework.m_pScene->pHeroObject[i]->m_HeroSelect == HEALER) SelectObject(memdc2, bmp_healer);
					else if (gGameFramework.m_pScene->pHeroObject[i]->m_HeroSelect == MAGICIAN) SelectObject(memdc2, bmp_magician);
					else if (gGameFramework.m_pScene->pHeroObject[i]->m_HeroSelect == WITCH) SelectObject(memdc2, bmp_witch);
					else if (gGameFramework.m_pScene->pHeroObject[i]->m_HeroSelect == SWORDMAN) SelectObject(memdc2, bmp_swordman);
					else if (gGameFramework.m_pScene->pHeroObject[i]->m_HeroSelect == KNIGHT) SelectObject(memdc2, bmp_knight);

					if (gGameFramework.m_pScene->pHeroObject[i]->m_HeroSelect != 0)
						BitBlt(memdc, 83, gLobby.ending[i].yPos - 10, 60, 60, memdc2, 0, 0, SRCCOPY);
				}

				for (int i = 1; i < MAX_GAMER; i += 2)
				{
					if (gGameFramework.m_pScene->pHeroObject[i]->m_HeroSelect == BABARIAN) SelectObject(memdc2, bmp_babarian);
					else if (gGameFramework.m_pScene->pHeroObject[i]->m_HeroSelect == HEALER) SelectObject(memdc2, bmp_healer);
					else if (gGameFramework.m_pScene->pHeroObject[i]->m_HeroSelect == MAGICIAN) SelectObject(memdc2, bmp_magician);
					else if (gGameFramework.m_pScene->pHeroObject[i]->m_HeroSelect == WITCH) SelectObject(memdc2, bmp_witch);
					else if (gGameFramework.m_pScene->pHeroObject[i]->m_HeroSelect == SWORDMAN) SelectObject(memdc2, bmp_swordman);
					else if (gGameFramework.m_pScene->pHeroObject[i]->m_HeroSelect == KNIGHT) SelectObject(memdc2, bmp_knight);

					if (gGameFramework.m_pScene->pHeroObject[i]->m_HeroSelect != 0)
						BitBlt(memdc, 83, gLobby.ending[i].yPos + 6, 60, 60, memdc2, 0, 0, SRCCOPY);
				}




				hFont = CreateFont(24, 0, 0, 0, 0, 0, 0, 0, HANGEUL_CHARSET, 0, 0, 0, VARIABLE_PITCH | FF_ROMAN, TEXT("함초롱바탕"));
				hOldFont = (HFONT)SelectObject(memdc, hFont);

				SetTextColor(memdc, RGB(255, 255, 255));
				SetBkMode(memdc, TRANSPARENT);


				for (int i = 0; i < MAX_GAMER; i++)
				{

					// 스코어 REd 팀
					wsprintf(s, L"%d", g_B_Teamcount);
					TextOut(memdc, 450, 30, s, wcslen(s));

					// 스코어 BLUE 팀
					wsprintf(s, L"%d", g_A_Teamcount);
					TextOut(memdc, 550, 30, s, wcslen(s));


					// DB달고 여기에 아이디를 추가해야함


					TextOut(memdc, 200, gLobby.ending[i].yPos, (LPCWSTR)gRoom.RoomUI[i].ID, wcslen(gRoom.RoomUI[i].ID));


					wsprintf(s, L"%d", gGameFramework.m_pScene->pHeroObject[i]->m_killcount);
					TextOut(memdc, 410, gLobby.ending[i].yPos, s, wcslen(s)); // 방 제목 출력


					wsprintf(s, L"%d", gGameFramework.m_pScene->pHeroObject[i]->m_deathcount);
					TextOut(memdc, 510, gLobby.ending[i].yPos, s, wcslen(s)); // 방 제목 출력


					wsprintf(s, L"%d", gGameFramework.m_pScene->pHeroObject[i]->m_damageDealt);
					TextOut(memdc, 650, gLobby.ending[i].yPos, s, wcslen(s)); // 방 제목 출력

					wsprintf(s, L"%d", gGameFramework.m_pScene->pHeroObject[i]->m_hitAmount);
					TextOut(memdc, 780, gLobby.ending[i].yPos, s, wcslen(s)); // 방 제목 출력
				}




				SetTextColor(memdc, RGB(0, 0, 0));
			}
			else
			{
				gLobby.Draw(memdc, memdc2);
			}

		}


		else if (gGameFramework.ChangeScene == ROOM)
		{
			gRoom.Draw(memdc, memdc2);
		}

		else if (gGameFramework.ChangeScene == LOADING)
		{
			SelectObject(memdc2, bmp_loading);
			BitBlt(memdc, 0, 0, 1024, 768, memdc2, 0, 0, SRCCOPY);
		}


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

	case WM_MOUSEWHEEL:
		if (gGameFramework.ChangeScene == LOBBY)
		{
			gLobby.MouseWheel(wParam);
			InvalidateRect(hWnd, NULL, false);
		}
		else if (gGameFramework.ChangeScene == ROOM)
		{
			gRoom.MouseWheel(wParam);
			InvalidateRect(hWnd, NULL, false);
		}

		break;

	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case ID_EDIT:
			switch (HIWORD(wParam))
			{
			case EN_CHANGE:
				if (gGameFramework.ChangeScene == LOBBY)
				{
					if (!gLobby.RoomCreateWindow)
						GetWindowText(hChat, gLobby.input, sizeof(gLobby.input));
					else
					{
						if (gLobby.RoomCreateChat == gLobby.RNAME)
							GetWindowText(hChat, gLobby.RoomName, sizeof(gLobby.RoomName));
					}
				}
				else if (gGameFramework.ChangeScene == ROOM)
				{
					GetWindowText(hChat, gRoom.input, sizeof(gRoom.input));
				}


				break;
			}
		}
		InvalidateRect(g_hWnd, NULL, false);
		break;

	case WM_CHAR:
		if (gGameFramework.ChangeScene == MAINMENU)
		{
			gMainMenu.ID_PASSWORD_Input(wParam);
			InvalidateRect(hWnd, NULL, false);
		}

		else if (gGameFramework.ChangeScene == LOBBY)
		{
			gLobby.Password_Input(wParam);
			InvalidateRect(hWnd, NULL, false);
		}
		break;

	case WM_LBUTTONDOWN:
	{
		int mx = LOWORD(lParam);
		int my = HIWORD(lParam);

		if (gGameFramework.ChangeScene != GAME)
		{
			if (gGameFramework.ChangeScene == MAINMENU)
			{
				gMainMenu.L_ButtonDown(hChat, mx, my);
				InvalidateRect(hWnd, NULL, false);
			}


			else if (gGameFramework.ChangeScene == LOBBY)
			{
				if (gGameFramework.isEnding == true)
				{
					for (int i = 0; i < MAX_GAMER; ++i)
					{
						for (int i = 0; i < MAX_GAMER; ++i)
						{
							memset(gRoom.RoomUI[i].ID, 0, sizeof(gRoom.RoomUI[i].ID));
						}
						gGameFramework.m_pScene->pHeroObject[i]->HeroInit(); // Hero 정보 초기화
						gGameFramework.m_pScene->pHeroObject[i]->Rotate(0, 0, 0);
						gLobby.vOutPut.clear();
					}
						

					// server send (cs_packet_result_confirm complete)
					cs_packet_result_confirm *my_packet = reinterpret_cast<cs_packet_result_confirm *>(send_buffer);
					my_packet->size = sizeof(cs_packet_result_confirm);
					send_wsabuf.len = sizeof(cs_packet_result_confirm);
					DWORD iobyte;
					my_packet->type = CS_RESULTCONFIRM;
					my_packet->roomnumber = gGameFramework.m_pScene->MyRoomNumber;
					WSASend(g_mysocket, &send_wsabuf, 1, &iobyte, 0, NULL, NULL);
					//

					gGameFramework.isEnding = false;
					InvalidateRect(g_hWnd, NULL, false);
				}

				gLobby.L_ButtonDown(hWnd, hChat, mx, my);
				InvalidateRect(hWnd, NULL, false);
			}

			else if (gGameFramework.ChangeScene == ROOM)
			{
				gRoom.L_ButtonDown(mx, my);
				InvalidateRect(hWnd, NULL, false);
			}

			FMOD_System_PlaySound(g_System, FMOD_CHANNEL_REUSE, g_Sound[CLICK_SOUND], 0, &g_Channel[CLICK_SOUND]);
		}
	}

	break;

	case WM_KEYDOWN:

		switch (wParam)
		{
		case VK_RETURN:
			if (gGameFramework.ChangeScene == MAINMENU)
			{
				if (wcslen(gMainMenu.user_id) != 0 && wcslen(gMainMenu.user_password) != 0)
				{
					// server send (Ready)
					cs_packet_login *my_packet = reinterpret_cast<cs_packet_login *>(send_buffer);
					my_packet->size = sizeof(cs_packet_login);
					send_wsabuf.len = sizeof(cs_packet_login);
					DWORD iobyte;
					my_packet->type = CS_LOGIN;
					wcscpy_s(my_packet->id, gMainMenu.user_id);
					wcscpy_s(my_packet->password, gMainMenu.user_password);
					WSASend(g_mysocket, &send_wsabuf, 1, &iobyte, 0, NULL, NULL);
					//
				}
			}
			else if (gGameFramework.ChangeScene == LOBBY)
			{
				SetFocus(hChat);
				InvalidateRect(g_hWnd, NULL, false);
			}
			else if (gGameFramework.ChangeScene == ROOM)
			{
				SetFocus(hChat);
				InvalidateRect(g_hWnd, NULL, false);
			}
			break;
		}

	case WM_LBUTTONUP:
	case WM_RBUTTONDOWN:
	case WM_RBUTTONUP:
		break;
	case WM_MOUSEMOVE:
	{
		if (gGameFramework.ChangeScene == LOBBY)
		{
			int mx = LOWORD(lParam);
			int my = HIWORD(lParam);

			gLobby.MouseMove(mx, my);
			InvalidateRect(g_hWnd, NULL, false);
		}

		else if (gGameFramework.ChangeScene == ROOM)
		{
			int mx = LOWORD(lParam);
			int my = HIWORD(lParam);

			gLobby.MouseMove(mx, my);
			InvalidateRect(g_hWnd, NULL, false);
		}

		break;
	}

	case WM_KEYUP:
		switch (wParam)
		{
			//case VK_F1:
			//case VK_F2:
			//case VK_F3:

			//	gGameFramework.m_pPlayer->ChangeCamera(gGameFramework.Getpd3dDevice(), (wParam - VK_F1 + 1), gGameFramework.m_GameTimer.GetTimeElapsed());
			//	gGameFramework.m_pCamera = gGameFramework.m_pPlayer->GetCamera();
			//	// 씬에 현재 카메라를 설정한다.
			//	gGameFramework.m_pScene->SetCamera(gGameFramework.m_pCamera);
			//	break;
		case VK_ESCAPE:
			::PostQuitMessage(0);
			break;
		default:
			break;
		}
		break;
	case WM_SETFOCUS:
		activate = true;
		break;
	case WM_KILLFOCUS:
		activate = false;
		break;
	case WM_DESTROY:
		SetWindowLongPtr(hChat, GWLP_WNDPROC, (LONG_PTR)wpOldEditProc);
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

// 정보 대화 상자의 메시지 처리기입니다.
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
		// 접속
	case SC_ID:
	{
		sc_packet_id *my_packet = reinterpret_cast<sc_packet_id *>(ptr);

		memset(gMainMenu.user_password, 0, sizeof(gMainMenu.user_password));

		gMainMenu.IsLogin = true;
		gGameFramework.ChangeScene = LOBBY;
		InvalidateRect(g_hWnd, NULL, false);

		break;
	}
	case SC_LOGIN_FAILED:
	{
		sc_packet_loginfailed *my_packet = reinterpret_cast<sc_packet_loginfailed *>(ptr);

		gMainMenu.IsLogin = false;

		InvalidateRect(g_hWnd, NULL, false);
		break;
	}

	// 로비
	case SC_LOBBY_CHAT:
	{
		sc_packet_chat *my_packet = reinterpret_cast<sc_packet_chat *>(ptr);

		/*chat_id = L"client[" + to_wstring(id);
		chat_id += L"]: ";*/
		wstring s = my_packet->DB_id;
		s.push_back(' ');
		s.push_back(':');
		s.push_back(' ');

		gLobby.chat_id.assign(s.begin(), s.end());
		gLobby.chat_id += my_packet->message;

		gLobby.vOutPut.push_back(gLobby.chat_id);
		gLobby.chat_id.clear();

		if (gLobby.iLine >= 11)
		{
			++gLobby.iFrontRange;
		}
		else ++gLobby.iLine;


		if (gLobby.bScrool)
		{
			gLobby.iLastRange = 0;
			gLobby.iFrontRange = gLobby.vOutPut.size() - 11;
			gLobby.bScrool = false;
		}

		InvalidateRect(g_hWnd, NULL, false);
		break;
	}

	case SC_ROOM_INFO:
	{
		sc_packet_roominfo *my_packet = reinterpret_cast<sc_packet_roominfo *>(ptr);

		wcscpy_s(gLobby.room[my_packet->room_number].roomtitle, my_packet->roomtitle);
		gLobby.room[my_packet->room_number].roomstatus = my_packet->roomstatus;
		gLobby.room[my_packet->room_number].mode = my_packet->mode;
		gLobby.room[my_packet->room_number].playercount = my_packet->playercount;
		gLobby.room[my_packet->room_number]._private = my_packet->m_private;



		InvalidateRect(g_hWnd, NULL, false);

		break;
	}

	case SC_JOIN_ROOM:
	{
		sc_packet_join_room *my_packet = reinterpret_cast<sc_packet_join_room *>(ptr);

		gGameFramework.m_pScene->MyRoomNumber = my_packet->roomnumber;
		gGameFramework.m_pScene->myGame_id = my_packet->game_id;

		wcscpy_s(gRoom.RoomInfo.roomtitle, my_packet->roomtitle);//wcscpy_s(gRoom.RoomInfo.roomtitle, gLobby.room[gRoom.RoomInfo.room_number].roomtitle);
		gRoom.RoomInfo.mode = my_packet->mode;
		gRoom.RoomInfo.playercount = my_packet->playercount;
		gRoom.RoomInfo._private = my_packet->m_private;
		gRoom.RoomInfo.room_number = my_packet->roomnumber; // 룸안에서 방번호를 표시하기위해서 받아옴


		wcscpy_s(gRoom.RoomUI[gGameFramework.m_pScene->myGame_id].ID, gMainMenu.user_id); // 방에 입장하면 나의 아이디를 적용



		EnterRoom(); // 방에 입장한다.
		SetWindowTextW(hChat, '\0');

		InvalidateRect(g_hWnd, NULL, false);
		break;
	}

	case SC_JOIN_FAIL_EMPTY:
	{
		sc_packet_join_fail *my_packet = reinterpret_cast<sc_packet_join_fail *>(ptr);

		InvalidateRect(g_hWnd, NULL, false);
		break;
	}
	case SC_JOIN_FAIL_FULL:
	{
		sc_packet_join_fail *my_packet = reinterpret_cast<sc_packet_join_fail *>(ptr);

		InvalidateRect(g_hWnd, NULL, false);
		break;
	}

	case SC_JOIN_FAIL_INGAME:
	{
		sc_packet_join_fail *my_packet = reinterpret_cast<sc_packet_join_fail *>(ptr);

		InvalidateRect(g_hWnd, NULL, false);
		break;
	}

	case SC_QUICK_JOIN_FAIL:
	{
		sc_packet_join_fail *my_packet = reinterpret_cast<sc_packet_join_fail *>(ptr);

		InvalidateRect(g_hWnd, NULL, false);
		break;
	}

	case SC_QUICK_JOIN:
	{
		sc_packet_quick_join *my_packet = reinterpret_cast<sc_packet_quick_join *>(ptr);

		gGameFramework.m_pScene->MyRoomNumber = my_packet->roomnumber;
		gGameFramework.m_pScene->myGame_id = my_packet->game_id;

		wcscpy_s(gRoom.RoomInfo.roomtitle, my_packet->roomtitle);//wcscpy_s(gRoom.RoomInfo.roomtitle, gLobby.room[gRoom.RoomInfo.room_number].roomtitle);
		gRoom.RoomInfo.mode = my_packet->mode;
		gRoom.RoomInfo.playercount = my_packet->playercount;
		gRoom.RoomInfo._private = my_packet->m_private;
		gRoom.RoomInfo.room_number = my_packet->roomnumber; // 룸안에서 방번호를 표시하기위해서 받아옴


		wcscpy_s(gRoom.RoomUI[gGameFramework.m_pScene->myGame_id].ID, gMainMenu.user_id); // 방에 입장하면 나의 아이디를 적용


		EnterRoom(); // 방에 입장한다.

		InvalidateRect(g_hWnd, NULL, false);
		break;
	}




	// 방
	case SC_ROOM_CHAT:
	{
		cs_packet_roomchat *my_packet = reinterpret_cast<cs_packet_roomchat *>(ptr);

		/*chat_id = L"client[" + to_wstring(id);
		chat_id += L"]: ";*/
		wstring s = my_packet->id;
		s.push_back(' ');
		s.push_back(':');
		s.push_back(' ');

		gRoom.chat_id.assign(s.begin(), s.end());
		gRoom.chat_id += my_packet->message;

		gRoom.vOutPut.push_back(gRoom.chat_id);
		gRoom.chat_id.clear();

		if (gRoom.iLine >= 11)
		{
			++gRoom.iFrontRange;
		}
		else ++gRoom.iLine;


		if (gRoom.bScrool)
		{
			gRoom.iLastRange = 0;
			gRoom.iFrontRange = gRoom.vOutPut.size() - 11;
			gRoom.bScrool = false;
		}

		InvalidateRect(g_hWnd, NULL, false);
		break;
	}

	case SC_READY:
	{
		sc_packet_ready *my_packet = reinterpret_cast<sc_packet_ready *>(ptr);
		int id = my_packet->id;
		if (id == gGameFramework.m_pScene->myGame_id)
		{
			// cout << "SC_READY\n";
			// cout << "내 캐릭터 선택 : " << gGameFramework.m_pScene->pHeroObject[id]->m_HeroSelect << endl;

			if (id & 1)
			{
				gGameFramework.m_pScene->pHeroObject[id]->m_Team = B_TEAM; // 팀 선택

			}
			else
			{
				gGameFramework.m_pScene->pHeroObject[id]->m_Team = A_TEAM;

			}

			gRoom.RoomUI[id].Team = gGameFramework.m_pScene->pHeroObject[id]->m_Team; // 룸에 팀 전달

		}
		else
		{
			gGameFramework.m_pScene->pHeroObject[id]->m_HeroSelect = gRoom.RoomUI[id].HeroSelect = my_packet->hero_pick; // 캐릭터 선택

			if (id & 1)
			{
				gGameFramework.m_pScene->pHeroObject[id]->m_Team = B_TEAM; // 팀 선택


			}
			else
			{
				gGameFramework.m_pScene->pHeroObject[id]->m_Team = A_TEAM;

			}

			gRoom.RoomUI[id].Team = gGameFramework.m_pScene->pHeroObject[id]->m_Team; // 룸에 팀 전달
			gGameFramework.m_pScene->pHeroObject[id]->m_serverID = my_packet->serverID;
			gRoom.RoomUI[id].IsReady = true; // 내꺼
		}

		InvalidateRect(g_hWnd, NULL, false);
		break;
	}

	case SC_ALLREADY:
	{
		InvalidateRect(g_hWnd, NULL, false);

		gGameFramework.playercount = gRoom.RoomInfo.playercount;

		Sleep(3000);


		gGameFramework.ChangeScene = LOADING;
		InvalidateRect(g_hWnd, NULL, false);
		gGameFramework.LoadingScene = true;

		InvalidateRect(g_hWnd, NULL, false);

		for (int i = 0; i < MAX_GAMER; ++i)
		{
			// cout << i << ends << gGameFramework.m_pScene->pHeroObject[i]->m_Team << endl;
		}

		break;
	}

	case SC_ENTER_NEWPLAYER:
	{
		sc_packet_enter_newplayer *my_packet = reinterpret_cast<sc_packet_enter_newplayer *>(ptr);


		for (int i = 0; i < MAX_GAMER; ++i)
		{
			gRoom.RoomUI[i].IsReady = false;
			gRoom.RoomUI[i].HeroSelect = EMPTY; // 새로운 사용자가 들어오면 초기화
		}


		wcscpy_s(gRoom.RoomUI[my_packet->id].ID, my_packet->DB_id);

		InvalidateRect(g_hWnd, NULL, false);
		break;
	}


	// 인게임
	case SC_PUT_PLAYER:
	{
		sc_packet_put_player *my_packet = reinterpret_cast<sc_packet_put_player *>(ptr);
		int id = my_packet->id;
		gGameFramework.m_pScene->AteamScore(0);
		gGameFramework.m_pScene->BteamScore(0);

		gRoom.Init();

		gGameFramework.m_pScene->pHeroObject[id]->SetHp(my_packet->hp);
		gGameFramework.m_pScene->pHeroObject[id]->SetAttack(my_packet->att);

		if (id == gGameFramework.m_pScene->myGame_id) {
			// cout << "Hero Pos : \t " << id << endl;
			gGameFramework.m_pScene->pHeroObject[gGameFramework.m_pScene->myGame_id]->SetPosition(my_packet->x, my_packet->y, my_packet->z);
			gGameFramework.m_pPlayer->SetPosition(D3DXVECTOR3(my_packet->x, my_packet->y, my_packet->z));

			switch (gGameFramework.m_pScene->pHeroObject[id]->m_HeroSelect)
			{
			case BABARIAN:	gGameFramework.m_pScene->pHeroObject[gGameFramework.m_pScene->myGame_id]->Setmaxhp(BABARIAN_HP);
				gGameFramework.m_pScene->pHeroObject[gGameFramework.m_pScene->myGame_id]->SetRange(BABARIAN_RANGE); break;
			case KNIGHT:	gGameFramework.m_pScene->pHeroObject[gGameFramework.m_pScene->myGame_id]->Setmaxhp(KNIGHT_HP);
				gGameFramework.m_pScene->pHeroObject[gGameFramework.m_pScene->myGame_id]->SetRange(KNIGHT_RANGE); break;
			case SWORDMAN:	gGameFramework.m_pScene->pHeroObject[gGameFramework.m_pScene->myGame_id]->Setmaxhp(SWORDMAN_HP);
				gGameFramework.m_pScene->pHeroObject[gGameFramework.m_pScene->myGame_id]->SetRange(SWORDMAN_RANGE); break;
			case MAGICIAN:	gGameFramework.m_pScene->pHeroObject[gGameFramework.m_pScene->myGame_id]->Setmaxhp(MAGICIAN_HP);
				gGameFramework.m_pScene->pHeroObject[gGameFramework.m_pScene->myGame_id]->SetRange(MAGICIAN_RANGE); break;
			case ARCHER:	gGameFramework.m_pScene->pHeroObject[gGameFramework.m_pScene->myGame_id]->Setmaxhp(ARCHER_HP);
				gGameFramework.m_pScene->pHeroObject[gGameFramework.m_pScene->myGame_id]->SetRange(ARCHER_RANGE); break;
			case HEALER:	gGameFramework.m_pScene->pHeroObject[gGameFramework.m_pScene->myGame_id]->Setmaxhp(HEALER_HP);
				gGameFramework.m_pScene->pHeroObject[gGameFramework.m_pScene->myGame_id]->SetRange(HEALER_RANGE); break;
			case WITCH:		gGameFramework.m_pScene->pHeroObject[gGameFramework.m_pScene->myGame_id]->Setmaxhp(WITCH_HP);
				gGameFramework.m_pScene->pHeroObject[gGameFramework.m_pScene->myGame_id]->SetRange(WITCH_RANGE); break;
			}
			//SetWindowTextW(hChat, '\0');
			gGameFramework.ChangeScene = GAME;
			InvalidateRect(g_hWnd, NULL, false);
		}
		else {
			// cout << "Other Pos: \t " << id << endl;
			gGameFramework.m_pScene->pHeroObject[id]->SetPosition(my_packet->x, my_packet->y, my_packet->z);
			// cout << "초기좌표를 설정하는 다른 아이디" << ends << id << endl;

			switch (gGameFramework.m_pScene->pHeroObject[id]->m_HeroSelect)
			{
			case BABARIAN:	gGameFramework.m_pScene->pHeroObject[id]->Setmaxhp(BABARIAN_HP);  break;

			case KNIGHT:	gGameFramework.m_pScene->pHeroObject[id]->Setmaxhp(KNIGHT_HP);	  break;

			case SWORDMAN:	gGameFramework.m_pScene->pHeroObject[id]->Setmaxhp(SWORDMAN_HP);  break;

			case MAGICIAN:	gGameFramework.m_pScene->pHeroObject[id]->Setmaxhp(MAGICIAN_HP);  break;

			case ARCHER:	gGameFramework.m_pScene->pHeroObject[id]->Setmaxhp(ARCHER_HP);	  break;

			case HEALER:	gGameFramework.m_pScene->pHeroObject[id]->Setmaxhp(HEALER_HP);	  break;

			case WITCH:		gGameFramework.m_pScene->pHeroObject[id]->Setmaxhp(WITCH_HP);	  break;

			}
		}
		break;
	}

	case SC_POS:
	{
		//currentDateTime();
		//SYSTEMTIME st;
		//char currentTime[84] = "";
		//GetLocalTime(&st);
		//sprintf_s(currentTime, "%d/%d/%d %d:%d:%d %d", st.wDay, st.wMonth, st.wYear, st.wHour, st.wMinute, st.wSecond, st.wMilliseconds);
		//g_ping_recv = (st.wSecond * 1000) + st.wMilliseconds;
		//// cout << "PING : " << g_ping_recv - g_ping_send << endl;
		sc_packet_pos *my_packet = reinterpret_cast<sc_packet_pos *>(ptr);
		int id = my_packet->id;
		if (id == gGameFramework.m_pScene->myGame_id) {

			//// cout << "Hero Move\n";
			gGameFramework.dwDirection = my_packet->direction;
			// 각도 저장.
			if (my_packet->direction) gGameFramework.m_pScene->dwDirforCollision = my_packet->direction;

			gGameFramework.m_pScene->pHeroObject[gGameFramework.m_pScene->myGame_id]->SetPosition(my_packet->x, my_packet->y, my_packet->z);

			if (my_packet->direction != 0)
				gGameFramework.m_pScene->m_ppShaders[gGameFramework.m_pScene->myGame_id + 1]->GetFBXMesh->SetAnimation(ANI_RUN);
			else
				gGameFramework.m_pScene->m_ppShaders[gGameFramework.m_pScene->myGame_id + 1]->GetFBXMesh->SetAnimation(ANI_IDLE);
		}
		else {
			//// cout << "Other Move\n";
			gGameFramework.OtherDirection[id] = my_packet->direction;
			gGameFramework.m_pScene->pHeroObject[id]->SetPosition(my_packet->x, my_packet->y, my_packet->z);

			//// cout << "움직이는 중 " << endl;
			if (my_packet->direction != 0)
				gGameFramework.m_pScene->m_ppShaders[id + 1]->GetFBXMesh->SetAnimation(ANI_RUN);
			else
				gGameFramework.m_pScene->m_ppShaders[id + 1]->GetFBXMesh->SetAnimation(ANI_IDLE);



			//// cout << "Other id : " << id << endl;

		}
		break;
	}

	case SC_SKILL_DONE:
	{
		sc_packet_attack *my_packet = reinterpret_cast<sc_packet_attack *>(ptr);
		int id = my_packet->id;

		if (id == gGameFramework.m_pScene->myGame_id) {
			gGameFramework.m_pScene->m_ppShaders[id + 1]->GetFBXMesh->SetAnimation(ANI_IDLE);
			g_bDoing_Ani = false;	// 애니메이션 도중 키 입력 방지.
		}
		else {
			gGameFramework.m_pScene->m_ppShaders[id + 1]->GetFBXMesh->SetAnimation(ANI_IDLE);
		}

		break;
	}
	case SC_ATTACK:
	{
		sc_packet_attack *my_packet = reinterpret_cast<sc_packet_attack *>(ptr);
		int id = my_packet->id;
		if (id == gGameFramework.m_pScene->myGame_id) {
			// cout << "[My]SC_ATTACK_PACKET\n";
			gGameFramework.m_pScene->m_ppShaders[id + 1]->GetFBXMesh->SetAnimation(ANI_ATTACK);
			FMOD_System_PlaySound(g_System, FMOD_CHANNEL_REUSE, g_Sound[SWING_SOUND], 0, &g_Channel[SWING_SOUND]);
		}
		else {
			// cout << "[Other]SC_ATTACK_PACKET\n";
			gGameFramework.m_pScene->m_ppShaders[id + 1]->GetFBXMesh->SetAnimation(ANI_ATTACK);
			FMOD_System_PlaySound(g_System, FMOD_CHANNEL_REUSE, g_Sound[SWING_SOUND], 0, &g_Channel[SWING_SOUND]);
		}

		break;
	}

	case SC_SKILL_Q:
	{
		sc_packet_skillQ *my_packet = reinterpret_cast<sc_packet_skillQ *>(ptr);
		int id = my_packet->id;
		if (id == gGameFramework.m_pScene->myGame_id) {
			// cout << "[My]SC_Q_PACKET\n";
			gGameFramework.m_pScene->m_ppShaders[id + 1]->GetFBXMesh->SetAnimation(ANI_Q);

		}
		else {
			// cout << "[Other]SC_Q_PACKET\n";
			gGameFramework.m_pScene->m_ppShaders[id + 1]->GetFBXMesh->SetAnimation(ANI_Q);
		}

		break;
	}

	case SC_SKILL_W:
	{
		sc_packet_skillW *my_packet = reinterpret_cast<sc_packet_skillW *>(ptr);
		int id = my_packet->id;
		if (id == gGameFramework.m_pScene->myGame_id) {
			// cout << "[My]SC_W_PACKET\n";
			gGameFramework.m_pScene->m_ppShaders[id + 1]->GetFBXMesh->SetAnimation(ANI_W);
		}
		else {
			// cout << "[Other]SC_W_PACKET\n";
			gGameFramework.m_pScene->m_ppShaders[id + 1]->GetFBXMesh->SetAnimation(ANI_W);
		}

		break;
	}

	case SC_SKILL_E:
	{
		sc_packet_skillE *my_packet = reinterpret_cast<sc_packet_skillE *>(ptr);
		int id = my_packet->id;
		if (id == gGameFramework.m_pScene->myGame_id) {
			// cout << "[My]SC_E_PACKET\n";
			gGameFramework.m_pScene->m_ppShaders[id + 1]->GetFBXMesh->SetAnimation(ANI_E);
		}
		else {
			// cout << "[Other]SC_E_PACKET\n";
			gGameFramework.m_pScene->m_ppShaders[id + 1]->GetFBXMesh->SetAnimation(ANI_E);
		}

		break;
	}

	case SC_SKILL_R:
	{
		sc_packet_skillR *my_packet = reinterpret_cast<sc_packet_skillR *>(ptr);
		int id = my_packet->id;
		if (id == gGameFramework.m_pScene->myGame_id) {
			// cout << "[My]SC_R_PACKET\n";
			gGameFramework.m_pScene->m_ppShaders[id + 1]->GetFBXMesh->SetAnimation(ANI_R);
		}
		else {
			// cout << "[Other]SC_R_PACKET\n";
			gGameFramework.m_pScene->m_ppShaders[id + 1]->GetFBXMesh->SetAnimation(ANI_R);
		}

		break;
	}

	case SC_CHAR_COLL:
	{
		sc_packet_coll_char *my_packet = reinterpret_cast<sc_packet_coll_char *>(ptr);
		int id = my_packet->id;

		switch (my_packet->direction)
		{
		case CS_KEYDOWN_RIGHT:
			if (id == gGameFramework.m_pScene->myGame_id) {
				gGameFramework.m_pPlayer->SetPosition(D3DXVECTOR3(gGameFramework.m_pPlayer->GetPosition().x - gGameFramework.m_pScene->pHeroObject[gGameFramework.m_pScene->myGame_id]->GetSpeed(),
					gGameFramework.m_pPlayer->GetPosition().y,
					gGameFramework.m_pPlayer->GetPosition().z
				));
				gGameFramework.m_pScene->pHeroObject[gGameFramework.m_pScene->myGame_id]->SetPosition(gGameFramework.m_pPlayer->GetPosition());
			}
			else {
				gGameFramework.m_pScene->pHeroObject[id]->SetPosition(D3DXVECTOR3(gGameFramework.m_pScene->pHeroObject[id]->GetPosition().x - gGameFramework.m_pScene->pHeroObject[id]->GetSpeed(),
					gGameFramework.m_pScene->pHeroObject[id]->GetPosition().y,
					gGameFramework.m_pScene->pHeroObject[id]->GetPosition().z
				));
				gGameFramework.m_pScene->pHeroObject[id]->SetPosition(gGameFramework.m_pScene->pHeroObject[id]->GetPosition());

			}
			break;
		case CS_KEYDOWN_LEFT:
			if (id == gGameFramework.m_pScene->myGame_id) {
				gGameFramework.m_pPlayer->SetPosition(D3DXVECTOR3(gGameFramework.m_pPlayer->GetPosition().x + gGameFramework.m_pScene->pHeroObject[gGameFramework.m_pScene->myGame_id]->GetSpeed(),
					gGameFramework.m_pPlayer->GetPosition().y,
					gGameFramework.m_pPlayer->GetPosition().z
				));
				gGameFramework.m_pScene->pHeroObject[gGameFramework.m_pScene->myGame_id]->SetPosition(gGameFramework.m_pPlayer->GetPosition());
			}
			else {
				gGameFramework.m_pScene->pHeroObject[id]->SetPosition(D3DXVECTOR3(gGameFramework.m_pScene->pHeroObject[id]->GetPosition().x + gGameFramework.m_pScene->pHeroObject[id]->GetSpeed(),
					gGameFramework.m_pScene->pHeroObject[id]->GetPosition().y,
					gGameFramework.m_pScene->pHeroObject[id]->GetPosition().z
				));
				gGameFramework.m_pScene->pHeroObject[id]->SetPosition(gGameFramework.m_pScene->pHeroObject[id]->GetPosition());

			}
			break;
		case CS_KEYDOWN_UP:
			if (id == gGameFramework.m_pScene->myGame_id) {
				gGameFramework.m_pPlayer->SetPosition(D3DXVECTOR3(gGameFramework.m_pPlayer->GetPosition().x,
					gGameFramework.m_pPlayer->GetPosition().y,
					gGameFramework.m_pPlayer->GetPosition().z + gGameFramework.m_pScene->pHeroObject[gGameFramework.m_pScene->myGame_id]->GetSpeed()
				));
				gGameFramework.m_pScene->pHeroObject[gGameFramework.m_pScene->myGame_id]->SetPosition(gGameFramework.m_pPlayer->GetPosition());
			}
			else {
				gGameFramework.m_pScene->pHeroObject[id]->SetPosition(D3DXVECTOR3(gGameFramework.m_pScene->pHeroObject[id]->GetPosition().x,
					gGameFramework.m_pScene->pHeroObject[id]->GetPosition().y,
					gGameFramework.m_pScene->pHeroObject[id]->GetPosition().z + gGameFramework.m_pScene->pHeroObject[gGameFramework.m_pScene->myGame_id]->GetSpeed()
				));
				gGameFramework.m_pScene->pHeroObject[id]->SetPosition(gGameFramework.m_pScene->pHeroObject[id]->GetPosition());

			}
			break;
		case CS_KEYDOWN_DOWN:
			if (id == gGameFramework.m_pScene->myGame_id) {
				gGameFramework.m_pPlayer->SetPosition(D3DXVECTOR3(gGameFramework.m_pPlayer->GetPosition().x,
					gGameFramework.m_pPlayer->GetPosition().y,
					gGameFramework.m_pPlayer->GetPosition().z - gGameFramework.m_pScene->pHeroObject[gGameFramework.m_pScene->myGame_id]->GetSpeed()
				));
				gGameFramework.m_pScene->pHeroObject[gGameFramework.m_pScene->myGame_id]->SetPosition(gGameFramework.m_pPlayer->GetPosition());
			}
			else {
				gGameFramework.m_pScene->pHeroObject[id]->SetPosition(D3DXVECTOR3(gGameFramework.m_pScene->pHeroObject[id]->GetPosition().x,
					gGameFramework.m_pScene->pHeroObject[id]->GetPosition().y,
					gGameFramework.m_pScene->pHeroObject[id]->GetPosition().z - gGameFramework.m_pScene->pHeroObject[gGameFramework.m_pScene->myGame_id]->GetSpeed()
				));
				gGameFramework.m_pScene->pHeroObject[id]->SetPosition(gGameFramework.m_pScene->pHeroObject[id]->GetPosition());

			}
			break;
		}
		break;
	}
	// (HIT)
	case SC_ATTACK_HIT:
	{
		sc_packet_attack_hit *my_packet = reinterpret_cast<sc_packet_attack_hit *>(ptr);
		int clientid = my_packet->clientid;

		gGameFramework.m_pScene->pHeroObject[clientid]->SetHp(my_packet->hp);

		if (clientid == gGameFramework.m_pScene->myGame_id) {
			g_bDoing_Ani = true;	// 애니메이션 도중 키 입력 방지.
			gGameFramework.Scene_Animation = 1;
			gGameFramework.dwDirection = my_packet->direction;
		}
		else
		{
			gGameFramework.OtherDirection[clientid] = my_packet->direction;
		}
		
		if (gGameFramework.m_pScene->pHeroObject[clientid]->GetHp() > 0) {
			gGameFramework.m_pScene->pHeroObject[clientid]->bHeroHit = true;
			gGameFramework.m_pScene->pHeroObject[clientid]->bHeroRun = false;
			gGameFramework.m_pScene->m_ppShaders[clientid + 1]->GetFBXMesh->SetAnimation(ANI_HIT);
			FMOD_System_PlaySound(g_System, FMOD_CHANNEL_REUSE, g_Sound[HIT_SOUND], 0, &g_Channel[HIT_SOUND]);


		}


		// 플레이어들 체력 업데이트
		if (clientid == gGameFramework.m_pScene->myGame_id)
		{
			// 내 체력
			gGameFramework.m_pScene->pHpgaugeObject->SetEndPos((335.f / gGameFramework.m_pScene->pHeroObject[gGameFramework.m_pScene->myGame_id]->Getmaxhp()) * gGameFramework.m_pScene->pHeroObject[gGameFramework.m_pScene->myGame_id]->GetHp() + 50.f);



			// 상단에 있는 내체력
			if ((clientid % 2) == 0)
			{

				// Red Team
				if (gGameFramework.m_pScene->PlayerHpObject[clientid]->GetNum() == 0)
				{
					gGameFramework.m_pScene->PlayerHpObject[clientid]->SetEndPos(74.f / gGameFramework.m_pScene->pHeroObject[gGameFramework.m_pScene->myGame_id]->Getmaxhp() * gGameFramework.m_pScene->pHeroObject[gGameFramework.m_pScene->myGame_id]->GetHp()
						+ gGameFramework.m_pScene->PlayerHpObject[clientid]->GetNum() * 80 + 5 + 3);
				}
				else
				{
					gGameFramework.m_pScene->PlayerHpObject[clientid]->SetEndPos(74.f / gGameFramework.m_pScene->pHeroObject[gGameFramework.m_pScene->myGame_id]->Getmaxhp() * gGameFramework.m_pScene->pHeroObject[gGameFramework.m_pScene->myGame_id]->GetHp()
						+ (gGameFramework.m_pScene->PlayerHpObject[clientid]->GetNum() * 80) + (gGameFramework.m_pScene->PlayerHpObject[clientid]->GetNum() * 5) + 5 + 3);
				}

			}
			else
			{

				// Blue Team
				if (gGameFramework.m_pScene->PlayerHpObject[clientid]->GetNum() == 0)
				{
					gGameFramework.m_pScene->PlayerHpObject[clientid]->SetEndPos(74.f / gGameFramework.m_pScene->pHeroObject[gGameFramework.m_pScene->myGame_id]->Getmaxhp() * gGameFramework.m_pScene->pHeroObject[gGameFramework.m_pScene->myGame_id]->GetHp()
						+ 660 + (gGameFramework.m_pScene->PlayerHpObject[clientid]->GetNum() * 80) + 5 + 3);
				}
				else
				{
					gGameFramework.m_pScene->PlayerHpObject[clientid]->SetEndPos(74.f / gGameFramework.m_pScene->pHeroObject[gGameFramework.m_pScene->myGame_id]->Getmaxhp() * gGameFramework.m_pScene->pHeroObject[gGameFramework.m_pScene->myGame_id]->GetHp()
						+ 665 + (gGameFramework.m_pScene->PlayerHpObject[clientid]->GetNum() * 80) + gGameFramework.m_pScene->PlayerHpObject[clientid]->GetNum() * 5 + 3);
				}
			}

			gGameFramework.m_pScene->PlayerHpObject[clientid]->Update();
		}
		else
		{
			// 상단에 있는 다른 플레이어 체력
			if ((clientid % 2) == 0)
			{

				// Red Team
				if (gGameFramework.m_pScene->PlayerHpObject[clientid]->GetNum() == 0)
				{
					gGameFramework.m_pScene->PlayerHpObject[clientid]->SetEndPos(74.f / gGameFramework.m_pScene->pHeroObject[clientid]->Getmaxhp() * gGameFramework.m_pScene->pHeroObject[clientid]->GetHp()
						+ gGameFramework.m_pScene->PlayerHpObject[clientid]->GetNum() * 80 + 5 + 3);
				}
				else
				{
					gGameFramework.m_pScene->PlayerHpObject[clientid]->SetEndPos(74.f / gGameFramework.m_pScene->pHeroObject[clientid]->Getmaxhp() * gGameFramework.m_pScene->pHeroObject[clientid]->GetHp()
						+ (gGameFramework.m_pScene->PlayerHpObject[clientid]->GetNum() * 80) + (gGameFramework.m_pScene->PlayerHpObject[clientid]->GetNum() * 5) + 5 + 3);
				}
			}
			else
			{
				// Blue Team
				if (gGameFramework.m_pScene->PlayerHpObject[clientid]->GetNum() == 0)
				{
					gGameFramework.m_pScene->PlayerHpObject[clientid]->SetEndPos(74.f / gGameFramework.m_pScene->pHeroObject[clientid]->Getmaxhp() * gGameFramework.m_pScene->pHeroObject[clientid]->GetHp()
						+ 660 + (gGameFramework.m_pScene->PlayerHpObject[clientid]->GetNum() * 80) + 5 + 3);
				}
				else
				{
					gGameFramework.m_pScene->PlayerHpObject[clientid]->SetEndPos(74.f / gGameFramework.m_pScene->pHeroObject[clientid]->Getmaxhp() * gGameFramework.m_pScene->pHeroObject[clientid]->GetHp()
						+ 665 + (gGameFramework.m_pScene->PlayerHpObject[clientid]->GetNum() * 80) + gGameFramework.m_pScene->PlayerHpObject[clientid]->GetNum() * 5 + 3);
				}

			}
			gGameFramework.m_pScene->PlayerHpObject[clientid]->Update();
		}


		break;
	}

	// (DIE & RESPAWN)
	case SC_CHAR_DIE:
	{
		sc_char_die *my_packet = reinterpret_cast<sc_char_die *>(ptr);
		int clientid = my_packet->clientid;

		if (my_packet->team == A_TEAM)
		{
			gGameFramework.m_pScene->BteamScore(++g_A_Teamcount);

			// cout << "A_TEAM KILLED\n";
		}
		else
		{
			gGameFramework.m_pScene->AteamScore(++g_B_Teamcount);
			// cout << "B_TEAM KILLED\n";
		}

		gGameFramework.m_pScene->m_ppShaders[clientid + 1]->GetFBXMesh->SetAnimation(ANI_DIE);
		gGameFramework.m_pScene->pHeroObject[clientid]->bHeroDie = true;
		gGameFramework.m_pScene->pHeroObject[clientid]->bHeroRun = false;
		gGameFramework.m_pScene->pHeroObject[clientid]->bDeath = true;

		gGameFramework.m_pScene->pHeroObject[clientid]->SetHp(0);

		gGameFramework.m_pScene->pHeroObject[my_packet->clientid]->Rotate(0, 0, 0);
		

		break;
	}

	case SC_RESPAWN:
	{
		sc_respawn *my_packet = reinterpret_cast<sc_respawn *>(ptr);
		int id = my_packet->id;

		if (id == gGameFramework.m_pScene->myGame_id) {
			gGameFramework.m_pScene->pHeroObject[id]->SetPosition(my_packet->x, 0.f, my_packet->z);
			gGameFramework.m_pPlayer->SetPosition(gGameFramework.m_pScene->pHeroObject[id]->GetPosition());


			gGameFramework.m_pScene->pHeroObject[id]->SetHp(my_packet->hp);
			gGameFramework.m_pScene->pHpgaugeObject->SetEndPos((335.f / gGameFramework.m_pScene->pHeroObject[gGameFramework.m_pScene->myGame_id]->Getmaxhp()) * gGameFramework.m_pScene->pHeroObject[gGameFramework.m_pScene->myGame_id]->GetHp() + 50.f);

			// 상단에 있는 내체력
			if ((id % 2) == 0)
			{

				// Red Team
				if (gGameFramework.m_pScene->PlayerHpObject[id]->GetNum() == 0)
				{
					gGameFramework.m_pScene->PlayerHpObject[id]->SetEndPos(74.f / gGameFramework.m_pScene->pHeroObject[gGameFramework.m_pScene->myGame_id]->Getmaxhp() * gGameFramework.m_pScene->pHeroObject[gGameFramework.m_pScene->myGame_id]->GetHp()
						+ gGameFramework.m_pScene->PlayerHpObject[id]->GetNum() * 80 + 5 + 3);
				}
				else
				{
					gGameFramework.m_pScene->PlayerHpObject[id]->SetEndPos(74.f / gGameFramework.m_pScene->pHeroObject[gGameFramework.m_pScene->myGame_id]->Getmaxhp() * gGameFramework.m_pScene->pHeroObject[gGameFramework.m_pScene->myGame_id]->GetHp()
						+ (gGameFramework.m_pScene->PlayerHpObject[id]->GetNum() * 80) + (gGameFramework.m_pScene->PlayerHpObject[id]->GetNum() * 5) + 5 + 3);
				}

			}
			else
			{

				// Blue Team
				if (gGameFramework.m_pScene->PlayerHpObject[id]->GetNum() == 0)
				{
					gGameFramework.m_pScene->PlayerHpObject[id]->SetEndPos(74.f / gGameFramework.m_pScene->pHeroObject[gGameFramework.m_pScene->myGame_id]->Getmaxhp() * gGameFramework.m_pScene->pHeroObject[gGameFramework.m_pScene->myGame_id]->GetHp()
						+ 660 + (gGameFramework.m_pScene->PlayerHpObject[id]->GetNum() * 80) + 5 + 3);
				}
				else
				{
					gGameFramework.m_pScene->PlayerHpObject[id]->SetEndPos(74.f / gGameFramework.m_pScene->pHeroObject[gGameFramework.m_pScene->myGame_id]->Getmaxhp() * gGameFramework.m_pScene->pHeroObject[gGameFramework.m_pScene->myGame_id]->GetHp()
						+ 665 + (gGameFramework.m_pScene->PlayerHpObject[id]->GetNum() * 80) + gGameFramework.m_pScene->PlayerHpObject[id]->GetNum() * 5 + 3);
				}
			}

			gGameFramework.m_pScene->PlayerHpObject[id]->Update();
		}
		else
		{
			gGameFramework.m_pScene->pHeroObject[id]->SetPosition(my_packet->x, 0.f, my_packet->z);
			gGameFramework.m_pScene->pHeroObject[id]->SetHp(my_packet->hp);


			// 상단에 있는 다른 플레이어 체력
			if ((id % 2) == 0)
			{

				// Red Team
				if (gGameFramework.m_pScene->PlayerHpObject[id]->GetNum() == 0)
				{
					gGameFramework.m_pScene->PlayerHpObject[id]->SetEndPos(74.f / gGameFramework.m_pScene->pHeroObject[id]->Getmaxhp() * gGameFramework.m_pScene->pHeroObject[id]->GetHp()
						+ gGameFramework.m_pScene->PlayerHpObject[id]->GetNum() * 80 + 5 + 3);
				}
				else
				{
					gGameFramework.m_pScene->PlayerHpObject[id]->SetEndPos(74.f / gGameFramework.m_pScene->pHeroObject[id]->Getmaxhp() * gGameFramework.m_pScene->pHeroObject[id]->GetHp()
						+ (gGameFramework.m_pScene->PlayerHpObject[id]->GetNum() * 80) + (gGameFramework.m_pScene->PlayerHpObject[id]->GetNum() * 5) + 5 + 3);
				}
			}
			else
			{
				// Blue Team
				if (gGameFramework.m_pScene->PlayerHpObject[id]->GetNum() == 0)
				{
					gGameFramework.m_pScene->PlayerHpObject[id]->SetEndPos(74.f / gGameFramework.m_pScene->pHeroObject[id]->Getmaxhp() * gGameFramework.m_pScene->pHeroObject[id]->GetHp()
						+ 660 + (gGameFramework.m_pScene->PlayerHpObject[id]->GetNum() * 80) + 5 + 3);
				}
				else
				{
					gGameFramework.m_pScene->PlayerHpObject[id]->SetEndPos(74.f / gGameFramework.m_pScene->pHeroObject[id]->Getmaxhp() * gGameFramework.m_pScene->pHeroObject[id]->GetHp()
						+ 665 + (gGameFramework.m_pScene->PlayerHpObject[id]->GetNum() * 80) + gGameFramework.m_pScene->PlayerHpObject[id]->GetNum() * 5 + 3);
				}

			}
			gGameFramework.m_pScene->PlayerHpObject[id]->Update();
		}


		gGameFramework.m_pScene->m_ppShaders[id + 1]->GetFBXMesh->SetAnimation(ANI_IDLE);
		gGameFramework.m_pScene->pHeroObject[id]->bDeath = false;


		break;
	}
	
	case SC_RESULT:
	{	 
		sc_result *my_packet = reinterpret_cast<sc_result *>(ptr);
		//int id = my_packet->id;

		// cout << "나는 무스팀? " << gGameFramework.m_pScene->pHeroObject[gGameFramework.m_pScene->myGame_id]->m_Team << endl;

		if (gGameFramework.m_pScene->pHeroObject[gGameFramework.m_pScene->myGame_id]->m_Team == A_TEAM)
		{
			gGameFramework.m_pScene->ResultBackground->SetBackground(POINT{ 0, 0 }, POINT{ 1024, 768 });
			gGameFramework.m_pScene->ResultBackground->BackgroundUpdate();

			if (my_packet->IsAWin == true)		// A Team 승리
				gGameFramework.isWinResult = true;

			else
				gGameFramework.isLoseResult = true;
		}
		else if (gGameFramework.m_pScene->pHeroObject[gGameFramework.m_pScene->myGame_id]->m_Team == B_TEAM)
		{
			gGameFramework.m_pScene->ResultBackground->SetBackground(POINT{ 0, 0 }, POINT{ 1024, 768 });
			gGameFramework.m_pScene->ResultBackground->BackgroundUpdate();

			if (my_packet->IsAWin == true)		// A Team 승리
				gGameFramework.isLoseResult = true;

			else
				gGameFramework.isWinResult = true;
		}
		break;
	}

	case SC_ENDINGRESULT:
	{
		sc_packet_endingresult *my_packet = reinterpret_cast<sc_packet_endingresult *>(ptr);
		int id = my_packet->id;
		if (id == gGameFramework.m_pScene->myGame_id) {
			gGameFramework.m_pScene->pHeroObject[gGameFramework.m_pScene->myGame_id]->m_killcount = my_packet->killcount;
			gGameFramework.m_pScene->pHeroObject[gGameFramework.m_pScene->myGame_id]->m_deathcount = my_packet->deathcount;
			gGameFramework.m_pScene->pHeroObject[gGameFramework.m_pScene->myGame_id]->m_damageDealt = my_packet->deal;
			gGameFramework.m_pScene->pHeroObject[gGameFramework.m_pScene->myGame_id]->m_hitAmount = my_packet->hit;
		}
		else
		{
			gGameFramework.m_pScene->pHeroObject[id]->m_killcount = my_packet->killcount;
			gGameFramework.m_pScene->pHeroObject[id]->m_deathcount = my_packet->deathcount;
			gGameFramework.m_pScene->pHeroObject[id]->m_damageDealt = my_packet->deal;
			gGameFramework.m_pScene->pHeroObject[id]->m_hitAmount = my_packet->hit;
		}
		break;
	}

	case SC_REMOVE_PLAYER:
	{
		sc_packet_remove_player *my_packet = reinterpret_cast<sc_packet_remove_player *>(ptr);
		int id = my_packet->id;
		if (id != gGameFramework.m_pScene->myGame_id) {
			//gGameFramework.m_pScene->pOtherObject[id]->SetPosition(0.f,-3000.f,0.f);
		}
		break;
	}
	default:
		printf("Unknown PACKET type [%d]\n", ptr[1]);
	}
}

void SendReadyButton()
{

	//for (int i = 0; i < MAX_GAMER; ++i)
	//{
	//	if (i != GetMyGame_id())
	//	{
	//		if (gRoom.RoomUI[GetMyGame_id()].HeroSelect == gRoom.RoomUI[i].HeroSelect && gRoom.RoomUI[GetMyGame_id()].Team == gRoom.RoomUI[i].Team)
	//		{
	//			return;
	//		}
	//	}
	//	
	//}


	{
		// server send (Ready)
		cs_packet_ready *my_packet = reinterpret_cast<cs_packet_ready *>(send_buffer);
		my_packet->size = sizeof(cs_packet_ready);
		send_wsabuf.len = sizeof(cs_packet_ready);
		DWORD iobyte;
		my_packet->type = CS_READY;
		my_packet->hero_pick = gRoom.RoomUI[GetMyGame_id()].HeroSelect;
		my_packet->roomnumber = gGameFramework.m_pScene->MyRoomNumber;

		gGameFramework.m_pScene->pHeroObject[gGameFramework.m_pScene->myGame_id]->m_HeroSelect = gRoom.RoomUI[GetMyGame_id()].HeroSelect;

		WSASend(g_mysocket, &send_wsabuf, 1, &iobyte, 0, NULL, NULL);
		//
	}

	// cout << "캐릭터선택완료" << endl;
}


int GetMyGame_id()
{
	return gGameFramework.m_pScene->myGame_id;
}

void EnterRoom()
{
	gGameFramework.ChangeScene = ROOM; // 여기서 방정보가 전부 입력이 되지 않으면 못넘어가게 구현해야함

									   //gRoom.RoomInfo.roomstatus = gLobby.room[my_packet->room_number].roomstatus;

	SetFocus(g_hWnd);

	memset(gLobby.RoomName, 0, sizeof(gLobby.RoomName));
	memset(gLobby.input, 0, sizeof(gLobby.input));

	gLobby.RoomCreateWindow = false;
	gLobby.vOutPut.clear();
}

void SoundInit()
{
	FMOD_System_Create(&g_System); //사운드 시스템을 만들어주는 함수이다.


	FMOD_System_Init(g_System, 32, FMOD_INIT_NORMAL, NULL); //사운드 시스템을 초기화 해주는 함수다.

	FMOD_System_CreateSound(g_System, "Data/Sound/Lobby_sound.mp3", FMOD_LOOP_NORMAL, 0, &g_Sound[LOBBY_SOUND]); //사운드를 메모리로 읽어오는 함수 // 참고로 파일포멧은 mp3,wav,mid,plac,dls,asf,asx,fsb...  재생가능함
	FMOD_System_CreateSound(g_System, "Data/Sound/Click_sound.wav", FMOD_DEFAULT, 0, &g_Sound[CLICK_SOUND]); //사운드를 메모리로 읽어오는 함수 // 참고로 파일포멧은 mp3,wav,mid,plac,dls,asf,asx,fsb...  재생가능함
	FMOD_System_CreateSound(g_System, "Data/Sound/Swing_sound.wav", FMOD_DEFAULT, 0, &g_Sound[SWING_SOUND]); //사운드를 메모리로 읽어오는 함수 // 참고로 파일포멧은 mp3,wav,mid,plac,dls,asf,asx,fsb...  재생가능함
	FMOD_System_CreateSound(g_System, "Data/Sound/Hit_sound.wav", FMOD_DEFAULT, 0, &g_Sound[HIT_SOUND]); //사운드를 메모리로 읽어오는 함수 // 참고로 파일포멧은 mp3,wav,mid,plac,dls,asf,asx,fsb...  재생가능함

	FMOD_System_PlaySound(g_System, FMOD_CHANNEL_FREE, g_Sound[0], 0, &g_Channel[0]);
}