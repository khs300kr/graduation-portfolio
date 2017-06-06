// LunaticProject.cpp : 응용 프로그램에 대한 진입점을 정의합니다.
//

#include "stdafx.h"
#include "LunaticProject.h"
#include "GameFramework.h"

#define MAX_LOADSTRING 100
#define CHAT_LENGTH 50
#define ID_EDIT 100
#define MAX_CHAT_LINE 30

// 전역 변수:
HINSTANCE hInst;								// 현재 인스턴스입니다.

TCHAR szTitle[MAX_LOADSTRING];					// 제목 표시줄 텍스트입니다.
TCHAR szWindowClass[MAX_LOADSTRING];			// 기본 창 클래스 이름입니다.
WNDPROC wpOldEditProc;
WCHAR output[MAX_CHAT_LINE][CHAT_LENGTH];
WCHAR input[MAX_STR_SIZE];
wstring chat_id;
vector<wstring> vOutPut;

int iLine = 0;
int iFrontRange = 0;
int iLastRange = 0;
bool bScrool = false;


bool RoomCreateWindow = false; // 방만들기 윈도우 활성화
enum { RNOPE, RNAME, RPASSWORD };
int RoomCreateChat = RNOPE; // 0 아무것도아님 1 Name입력 2 Password입력

WCHAR RoomName[MAX_STR_SIZE]; // 방만들기 방제목
char RoomPassword[16]; // 방만들기 비밀번호

CGameFramework gGameFramework;

// 이 코드 모듈에 들어 있는 함수의 정방향 선언입니다.
ATOM				MyRegisterClass(HINSTANCE hInstance);
BOOL				InitInstance(HINSTANCE, int);
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK	EditProc(HWND, UINT, WPARAM, LPARAM);
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
	if (!InitInstance (hInstance, nCmdShow))
	{
		return FALSE;
	}

	hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_LUNATICPROJECT));

	// server
	WSADATA	wsadata;
	WSAStartup(MAKEWORD(2, 2), &wsadata);

	g_mysocket = WSASocket(AF_INET, SOCK_STREAM, IPPROTO_TCP, NULL, 0, 0);



	SOCKADDR_IN ServerAddr;
	ZeroMemory(&ServerAddr, sizeof(SOCKADDR_IN));
	ServerAddr.sin_family = AF_INET;
	ServerAddr.sin_port = htons(MY_SERVER_PORT);

#ifdef _DEBUG
	ServerAddr.sin_addr.s_addr = inet_addr("127.0.0.1");
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


	return (int) msg.wParam;
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

	//g_hWnd = CreateWindow(szWindowClass, szTitle, dwStyle, CW_USEDEFAULT, CW_USEDEFAULT, rc.right - rc.left, rc.bottom - rc.top, NULL, NULL, hInstance, NULL);
	g_hWnd = CreateWindow(szWindowClass, szTitle, dwStyle, 100, 30, rc.right - rc.left, rc.bottom - rc.top, NULL, NULL, hInstance, NULL);
	if (!g_hWnd)
		return FALSE;

	gGameFramework.Create(hInstance, g_hWnd);

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
typedef struct Room
{
	int xPos; // 방 xPos
	int yPos; // 방 yPos
	int roomInfo; // 방 정보
	int people; // 유저가 방에 몇명 들어왔는지?
}Room;

bool MouseInbox(int left, int top, int right, int bottom, int x, int y)
{
	if (left < x && right > x && top < y  && bottom > y)
		return true;
	else
		return false;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	int wmId, wmEvent;

	HDC hdc, memdc, memdc2;

	HBITMAP hBackBit, hOldBitmap;
	HFONT hFont, hOldFont;

	HPEN Pen, oldPen;
	HBRUSH Brush, oldBrush;
	PAINTSTRUCT ps;
	static HBITMAP bmp_background, bmp_loading, bmp_mainmenu;//, bmp_lobby;
	static HBITMAP bmp_chatwindow, bmp_create, bmp_quickjoin, bmp_whojoin, bmp_roombackground, bmp_createwindow, bmp_room;
	//HBITMAP
	//
	static HWND hChat{};
	static Room room[3][2];
	SIZE size{};
	
	static bool IsPassword;
	static int GameMode;


	
	switch (message)
	{
	case WM_CREATE:
		wcout.imbue(locale("korean"));


		bmp_background = (HBITMAP)LoadBitmap(hInst, MAKEINTRESOURCE(IDB_BITMAP1));
		bmp_loading = (HBITMAP)LoadBitmap(hInst, MAKEINTRESOURCE(IDB_LOADINGWINDOW));
		bmp_mainmenu = (HBITMAP)LoadBitmap(hInst, MAKEINTRESOURCE(IDB_MAINMENU));

		bmp_create = (HBITMAP)LoadBitmap(hInst, MAKEINTRESOURCE(IDB_CREATE));
		bmp_quickjoin = (HBITMAP)LoadBitmap(hInst, MAKEINTRESOURCE(IDB_QUICKJOIN));
		bmp_whojoin = (HBITMAP)LoadBitmap(hInst, MAKEINTRESOURCE(IDB_WHOJOIN));
		bmp_roombackground = (HBITMAP)LoadBitmap(hInst, MAKEINTRESOURCE(IDB_ROOMBG));
		bmp_room = (HBITMAP)LoadBitmap(hInst, MAKEINTRESOURCE(IDB_ROOM));
		bmp_createwindow = (HBITMAP)LoadBitmap(hInst, MAKEINTRESOURCE(IDB_CREATEWINDOW));
		bmp_chatwindow = (HBITMAP)LoadBitmap(hInst, MAKEINTRESOURCE(IDB_CHATWINDOW));

		//AddFontResourceA("../data/Fonts/HoonWhitecatR.ttf");


		// room
		for (int i = 0; i < 3; ++i) 
		{
			for (int j = 0; j < 2; ++j)
			{
				room[i][j].xPos = 30 + (j * 400); // 방의 X
				room[i][j].yPos = 160 + (i * 120); // 방의 Y
				room[i][j].roomInfo = Empty; // 방 정보
				room[i][j].people = 0; // 유저가 몇명 방에 들어왔는지?
			}
		}
		
		GameMode = DEATHMATCH; //DeathMatch
		IsPassword = false;


		// Chatting
		hChat = CreateWindow(L"edit", NULL, WS_CHILD | WS_VISIBLE | WS_BORDER |
			ES_AUTOHSCROLL, 2000, 2000, 200, 25, hWnd, (HMENU)ID_EDIT, hInst, NULL);

		
		PostMessage(hChat, EM_LIMITTEXT, (WPARAM)MAX_STR_SIZE - 1, 0);
		wpOldEditProc = (WNDPROC)SetWindowLongPtr(hChat,
			GWLP_WNDPROC,
			(LONG_PTR)EditProc);

		break;
	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case ID_EDIT:
			switch (HIWORD(wParam))
			{
			case EN_CHANGE:
				if(!RoomCreateWindow)
					GetWindowText(hChat, input, sizeof(input));
				else
				{
					if(RoomCreateChat == RNAME)
						GetWindowText(hChat, RoomName, sizeof(RoomName));
				}
				
				break;
			}
		}
		InvalidateRect(g_hWnd, NULL, false);
		break;

	case WM_CHAR: //비밀번호 입력

		if (IsPassword && RoomCreateChat == RPASSWORD)
		{
			int len = strlen(RoomPassword);


			if (wParam == VK_BACK)
			{
				if (len > 0)
				{
					len--;
					RoomPassword[len] = '\0';
				}

			}

			else if (len > 14)
			{
				break;
			}

			else if (wParam == VK_RETURN)
				break;
			else
			{
				RoomPassword[len++] = wParam;
				RoomPassword[len] = '\0';
				
			}


			InvalidateRect(hWnd, NULL, false);
		}

		break;

	case WM_PAINT:


		hdc = BeginPaint(hWnd, &ps);

		memdc = CreateCompatibleDC(hdc);
		hBackBit = CreateCompatibleBitmap(hdc, 1024, 768);
		hOldBitmap = (HBITMAP)SelectObject(memdc, hBackBit);

		memdc2 = CreateCompatibleDC(hdc);


		if (gGameFramework.ChangeScene == MAINMENU)
		{
			DrawBitmap(memdc, memdc2, bmp_mainmenu, 0, 0, FRAME_BUFFER_WIDTH, FRAME_BUFFER_HEIGHT);
		}

		else if (gGameFramework.ChangeScene == LOBBY)
		{
			DrawBitmap(memdc, memdc2, bmp_create, 0, 0, 400, 140); // 방만들기
			DrawBitmap(memdc, memdc2, bmp_quickjoin, 400, 0, 400, 140); // 빠른 참여
			DrawBitmap(memdc, memdc2, bmp_whojoin, 800, 0, 400, 768); // 접속자

			DrawBitmap(memdc, memdc2, bmp_roombackground, 0, 140, 800, 406); // room background
			DrawBitmap(memdc, memdc2, bmp_chatwindow, 0, 518, 800, 250); // chatting
			Rectangle(memdc, 0, 738, 800, 768);


			hFont = CreateFont(30, 0, 0, 0, 0, 0, 0, 0, HANGEUL_CHARSET, 0, 0, 0, VARIABLE_PITCH | FF_ROMAN, TEXT("함초롱바탕"));
			hOldFont = (HFONT)SelectObject(memdc, hFont);

			SetBkMode(memdc, TRANSPARENT);



			for (int i = 0; i < 3; ++i)
			{
				for (int j = 0; j < 2; ++j)
				{
					DrawBitmap(memdc, memdc2, bmp_room, room[i][j].xPos, room[i][j].yPos, 335, 102); // 방 박스 출력

					switch (room[i][j].roomInfo)
					{
					case Empty:
						TextOut(memdc, room[i][j].xPos + 30, room[i][j].yPos + 30, L"생성가능", 4);
						break;
					case Comein:
						TextOut(memdc, room[i][j].xPos + 30, room[i][j].yPos + 30, L"입장가능", 4);
						break;
					case Full:
						TextOut(memdc, room[i][j].xPos + 30, room[i][j].yPos + 30, L"입장불가", 4);
						break;
					case InGame:
						TextOut(memdc, room[i][j].xPos + 30, room[i][j].yPos + 30, L"게임중", 3);
						break;
					}

					wchar_t s[6];
					wsprintf(s, L"(%d/8)", room[i][j].people);
					TextOut(memdc, room[i][j].xPos + 260, room[i][j].yPos + 68, s, 5); // (0/8) 인원수 출력
				}
			}




			

			if (RoomCreateWindow) //방만들기 눌렀을 때
			{
				hFont = CreateFont(30, 0, 0, 0, 0, 0, 0, 0, HANGEUL_CHARSET, 0, 0, 0, VARIABLE_PITCH | FF_ROMAN, TEXT("함초롱바탕"));
				hOldFont = (HFONT)SelectObject(memdc, hFont);
				SetBkMode(memdc, TRANSPARENT);

				DrawBitmap(memdc, memdc2, bmp_createwindow, 240, 170, 544, 408);
				

				Rectangle(memdc, 370, 300, 720, 340); // 방제목 입력
				Rectangle(memdc, 370, 343, 720, 383); // 비밀번호 입력


				Pen = CreatePen(PS_SOLID, 4, RGB(0, 0, 0));
				oldPen = (HPEN)SelectObject(memdc, Pen);

				SelectObject(memdc, GetStockObject(NULL_BRUSH));

				if(RoomCreateChat == RNAME)
					Rectangle(memdc, 370, 300, 720, 340); // 선택된 입력박스
				else if(RoomCreateChat == RPASSWORD)
					Rectangle(memdc, 370, 343, 720, 383); // 선택된 입력박스

				SelectObject(memdc, oldPen);
				DeleteObject(Pen);
				SelectObject(memdc, GetStockObject(WHITE_BRUSH));


				Rectangle(memdc, 730, 343, 770, 383); // 비밀번호 체크박스
				

				Rectangle(memdc, 520, 397, 550, 427); //데스매치 체크박스
				Rectangle(memdc, 670, 397, 700, 427); //점령전 체크박스


				Pen = CreatePen(PS_SOLID, 3, RGB(255, 0, 0));
				oldPen = (HPEN)SelectObject(memdc, Pen);

				SelectObject(memdc, GetStockObject(NULL_BRUSH));

				if(IsPassword)
					Ellipse(memdc, 735, 348, 765, 378);

				if (GameMode == DEATHMATCH)
					Ellipse(memdc, 522, 399, 548, 425);
				else
					Ellipse(memdc, 672, 399, 698, 425);

				SelectObject(memdc, oldPen);
				DeleteObject(Pen); 

				TextOut(memdc, 370, 305, RoomName, wcslen(RoomName));

				for (int i = 0; i < strlen(RoomPassword); ++i)
				{
					TextOut(memdc, 372 + (i*15), 343, L"*", 1);
				}
				

				if (!IsPassword)
				{
					Brush = CreateSolidBrush(RGB(128, 128, 128));
					oldBrush = (HBRUSH)SelectObject(memdc, Brush);

					Rectangle(memdc, 372, 343, 718, 383); // 비밀번호 체크박스

					SelectObject(memdc, oldBrush);
					DeleteObject(Brush);
				}

				SelectObject(memdc2, hFont);
				DeleteObject(hFont);

			}



			hFont = CreateFont(15, 0, 0, 0, 0, 0, 0, 0, HANGEUL_CHARSET, 0, 0, 0, VARIABLE_PITCH | FF_ROMAN, TEXT("함초롱바탕"));
			hOldFont = (HFONT)SelectObject(memdc, hFont);


			vector<wstring>::iterator iter = vOutPut.begin() + iFrontRange;
			for (int i = 0; iter != vOutPut.end() + iLastRange;++i,++iter)
			{
				TextOut(memdc, 0, 518 + (i*20), iter->c_str(), wcslen(iter->c_str()));
			}

			//for (auto& d : vOutPut)
			//{
			//	TextOut(memdc, 0, 20, d.c_str(), wcslen(d.c_str()));
			//}

			//채팅창 출력
			GetTextExtentPoint(memdc, input, wcslen(input), &size);
			TextOut(memdc, 0, 740, input, wcslen(input));



			//BitBlt(memdc, 0, 0, 1024, 768, memdc2, 0, 0, SRCCOPY);

			SelectObject(memdc2, hFont);
			DeleteObject(hFont);

		}


		else if (gGameFramework.ChangeScene == ROOM)
		{

			SelectObject(memdc2, bmp_background);
			BitBlt(memdc, 0, 0, 1024, 768, memdc2, 0, 0, SRCCOPY);

			Pen = CreatePen(PS_SOLID, 5, RGB(255, 0, 0));
			oldPen = (HPEN)SelectObject(memdc, Pen);

			SelectObject(memdc, GetStockObject(NULL_BRUSH));
			Rectangle(memdc, 229 + ((gGameFramework.SelectCount - 1) * 199), 130, 370 + ((gGameFramework.SelectCount - 1) * 199), 270);

			SelectObject(memdc, oldPen);
			DeleteObject(Pen);

		}

		if (gGameFramework.LoadingScene)
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
			if ((short)HIWORD(wParam) < 0)
			{
				if (vOutPut.size() - 11 > iFrontRange && iLine >= 11)
				{
					++iFrontRange;
					++iLastRange;
					bScrool = true;
				}
			}
			else
			{
				if (iFrontRange > 0)
				{
					--iFrontRange;
					--iLastRange;
					bScrool = true;
				}
			}
			InvalidateRect(hWnd, NULL, false);
		}
		break;


	case WM_LBUTTONDOWN:
	{
		if (gGameFramework.ChangeScene == LOBBY)
		{
			int mx = LOWORD(lParam);
			int my = HIWORD(lParam);

			if (!RoomCreateWindow && MouseInbox(0, 0, 400, 140, mx, my)) // 방만들기
			{
				RoomCreateWindow = true;
				
				SetFocus(hWnd);
				memset(input, '\0', sizeof(input));
				SetWindowTextW(hChat, '\0');
			}

			else if (RoomCreateWindow) // 방만들기 윈도우가 활성화 되어있을 때
			{
				if (!MouseInbox(240, 170, 784, 578, mx, my) || MouseInbox(536, 473, 696, 541, mx, my)) //취소를 누르거나 방만들기윈도우 밖을 누르면 방만들기윈도우를 제거한다.
				{
					if (IsPassword)
						IsPassword = false;

					RoomCreateWindow = false; // 방만들기 윈도우 헤제
					
					SetFocus(hWnd);
					memset(RoomName, '\0', sizeof(RoomName));
					SetWindowTextW(hChat, '\0');
				}
					

				else if (MouseInbox(325, 473, 484, 541, mx, my)) // 확인을 누르면 방에 입장
				{
					if (IsPassword)
						IsPassword = false;

					SetFocus(g_hWnd);
					EnterRoom();

				}

				else if (GameMode != DEATHMATCH && MouseInbox(378, 397, 548, 425, mx, my)) // 데스매치 클릭
				{
					GameMode = DEATHMATCH;
				}

				else if (GameMode != TERRITORY && MouseInbox(567, 397, 698, 425, mx, my)) // 점령전 클릭
				{
					GameMode = TERRITORY;
				}

				else if (MouseInbox(370, 300, 720, 340, mx, my)) // 방제목 활성화
				{
					RoomCreateChat = RNAME;
					SetFocus(hChat); // 방제목 활성화
				}

				else if (MouseInbox(370, 343, 720, 383, mx, my)) // 비밀번호 활성화
				{
					if (IsPassword)
					{
						RoomCreateChat = RPASSWORD;
						SetFocus(g_hWnd);
					}

				}

				else if (MouseInbox(730, 343, 770, 383, mx, my)) // 비밀번호 체크박스 활성화
				{
					IsPassword = !IsPassword; // 비밀번호 체크박스 활성화

					if (IsPassword)
					{
						RoomCreateChat = RPASSWORD;
						SetFocus(g_hWnd);
					}
						
					else
						RoomCreateChat = RNOPE;

						
				}
				else //창을 누르면
				{
					RoomCreateChat = RNOPE;
				}
			}

			if (!RoomCreateWindow && MouseInbox(400, 0, 800, 140, mx, my)) // 빠른참여
			{
				cout << "빠른 참여" << endl;
			}

			
			

			cout << mx << ends << my << endl;


			InvalidateRect(hWnd, NULL, false);
		}
		
	}
		break;


	case WM_RBUTTONDOWN:
		break;

	case WM_LBUTTONUP:
	case WM_RBUTTONUP:
		break;
	case WM_MOUSEMOVE:
		break;

	case WM_KEYDOWN:

		switch (wParam)
		{
		case VK_ESCAPE:
			if (gGameFramework.ChangeScene == LOBBY && RoomCreateWindow) //방만들기 윈도우가 열려있을 경우
			{
				RoomCreateWindow = false;
				InvalidateRect(hWnd, NULL, false);
			}
			break;

		case VK_SPACE:
			if (gGameFramework.ChangeScene == MAINMENU)
			{
				gGameFramework.ChangeScene = LOBBY;
				InvalidateRect(g_hWnd, NULL, false);
			}

			else if (gGameFramework.ChangeScene == ROOM)
			{
				{
					// server send (Ready)
					cs_packet_ready *my_packet = reinterpret_cast<cs_packet_ready *>(send_buffer);
					my_packet->size = sizeof(cs_packet_ready);
					send_wsabuf.len = sizeof(cs_packet_ready);
					DWORD iobyte;
					my_packet->type = CS_READY;
					my_packet->hero_pick = gGameFramework.SelectCount;

					WSASend(g_mysocket, &send_wsabuf, 1, &iobyte, 0, NULL, NULL);
					//
				}
				gGameFramework.m_pScene->pHeroObject[g_myid]->m_HeroSelect = gGameFramework.SelectCount;
				gGameFramework.m_pScene->pHeroObject[g_myid]->m_Team = A_TEAM;
				cout << "캐릭터선택완료" << endl;


				//LoadingScene = true;
				//InvalidateRect(m_hWnd, NULL, false);

			}
			break;

		case VK_RETURN:
			if (gGameFramework.ChangeScene == MAINMENU)
			{
				gGameFramework.ChangeScene = LOBBY;
			}

			else if (gGameFramework.ChangeScene == LOBBY)
			{
				if(!RoomCreateWindow) // 방만들기 윈도우가 없을 때
					SetFocus(hChat);
			}

			InvalidateRect(g_hWnd, NULL, false);
			break;

		case VK_LEFT:
			if (gGameFramework.ChangeScene == ROOM)
			{
				if (gGameFramework.SelectCount == 1)
					gGameFramework.SelectCount = 3;
				else
					gGameFramework.SelectCount--;
				InvalidateRect(g_hWnd, NULL, false);
			}
			break;
		case VK_RIGHT:
			if (gGameFramework.ChangeScene == ROOM)
			{
				if (gGameFramework.SelectCount == 3)
					gGameFramework.SelectCount = 1;
				else
					gGameFramework.SelectCount++;
				InvalidateRect(g_hWnd, NULL, false);
			}
			break;

		}


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
		SetWindowLongPtr(hChat, GWLP_WNDPROC, (LONG_PTR)wpOldEditProc);
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}
LRESULT CALLBACK EditProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	case WM_CHAR:
	{
		if (wParam == VK_RETURN && gGameFramework.ChangeScene == LOBBY)
		{
			

			if (!RoomCreateWindow)
			{
				SetFocus(g_hWnd);

				if (wcslen(input))
				{
					cs_packet_chat *my_packet = reinterpret_cast<cs_packet_chat *>(send_buffer);
					my_packet->size = sizeof(cs_packet_chat);
					send_wsabuf.len = sizeof(cs_packet_chat);
					DWORD iobyte;
					my_packet->type = CS_CHAT;
					wcscpy_s(my_packet->message, input);
					WSASend(g_mysocket, &send_wsabuf, 1, &iobyte, 0, NULL, NULL);

					memset(input, '\0', sizeof(input));
					SetWindowTextW(hWnd, '\0');
				}
			}
			




			//if (wcslen(str[index]))
			//{
			//	if (index <= MAX_CHAT_LINE)
			//		index++;

			//	else
			//	{
			//		index = 0;
			//	}

			//	if (index > scrollmax)
			//	{
			//		page++;
			//		maxpage++;
			//	}
			//	cs_packet_chat *my_packet = reinterpret_cast<cs_packet_chat *>(send_buffer);
			//	my_packet->size = sizeof(cs_packet_chat);
			//	send_wsabuf.len = sizeof(cs_packet_chat);
			//	DWORD iobyte;
			//	my_packet->type = CS_CHAT;
			//	my_packet->message = str[0];

			//}
			
			// Indicate that we processed the message.
			return 0;
			}

		}
	}

	// Pass the messages we don't process here on to the
	// original window procedure for default handling.
	return CallWindowProc(wpOldEditProc, hWnd, msg, wParam, lParam);
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
		int id = my_packet->id;
		if (first_time) {
			first_time = false;
			g_myid = id;
		}
		cout << "내아디 : " << g_myid << endl;
		break;
	}
	// 로비
	case SC_CHAT:
	{
		sc_packet_chat *my_packet = reinterpret_cast<sc_packet_chat *>(ptr);
		int id = my_packet->id;


		chat_id = L"client[" + to_wstring(id);
		chat_id += L"]: ";
		chat_id += my_packet->message;

		vOutPut.push_back(chat_id);
		chat_id.clear();

		if (iLine >= 11)
		{
			++iFrontRange;
		}
		else ++iLine;


		if (bScrool)
		{
			iLastRange = 0;
			iFrontRange = vOutPut.size() - 11;
			bScrool = false;
		}



		InvalidateRect(g_hWnd, NULL, false);
		break;
	}
	// 방
	case SC_READY:
	{
		sc_packet_ready *my_packet = reinterpret_cast<sc_packet_ready *>(ptr);
		int id = my_packet->id;
		if (id == g_myid) {
			cout << "SC_READY\n";
			cout << "내 캐릭터 선택 : " << (int)my_packet->hero_pick << endl;
			
			//gGameFramework.m_pScene->pHeroObject[g_myid]->m_HeroSelect =  SC_BABARIAN - 10;

		}
		else {
			gGameFramework.m_pScene->pHeroObject[id]->m_HeroSelect = my_packet->hero_pick;
			gGameFramework.m_pScene->pHeroObject[id]->m_Team = A_TEAM;
		}
		break;
	}
	case SC_ALLREADY:
	{
		Sleep(3000);
		gGameFramework.LoadingScene = true;
		InvalidateRect(g_hWnd, NULL, false);
		// server send (loading complete)
		cs_packet_LoadingComplete *my_packet = reinterpret_cast<cs_packet_LoadingComplete *>(send_buffer);
		my_packet->size = sizeof(cs_packet_LoadingComplete);
		send_wsabuf.len = sizeof(cs_packet_LoadingComplete);
		DWORD iobyte;
		my_packet->type = CS_LOADCOMPLETE;

		WSASend(g_mysocket, &send_wsabuf, 1, &iobyte, 0, NULL, NULL);
		//
		cout << "올레디\n";
		//gGameFramework.ChangeScene = GAME;

		break;
	}


	// 인게임
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
			cout << "초기좌표를 설정하는 다른 아이디" << ends << id << endl;
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

			cout << "움직이는 중 " << endl;
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


void DrawBitmap(HDC memdc, HDC memdc2, HBITMAP bitmap, int x, int y, int sizeX, int sizeY)
{
	SelectObject(memdc2, bitmap);
	BitBlt(memdc, x, y, sizeX, sizeY, memdc2, 0, 0, SRCCOPY);
}

void EnterRoom()
{
	gGameFramework.ChangeScene = ROOM; // 여기서 방정보가 전부 입력이 되지 않으면 못넘어가게 구현해야함
	vOutPut.clear();
}