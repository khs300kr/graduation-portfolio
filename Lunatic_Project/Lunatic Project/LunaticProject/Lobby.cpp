#include "stdafx.h"
#include "Lobby.h"



CLobby::CLobby()
{
	// 로비에서 보이는 룸

	int k = 0;

	for (int i = 0; i < 3; ++i)
	{
		for (int j = 0; j < 2; ++j)
		{
			room[k].xPos = 30 + (j * 400); // 방의 X
			room[k].yPos = 160 + (i * 120); // 방의 Y
			room[k].roomstatus = EMPTY; // 방 정보
			room[k++].playercount = 0; // 유저가 몇명 방에 들어왔는지?
		}
	}

	whatclick = -1;

}


CLobby::~CLobby()
{
}

void CLobby::Create(HINSTANCE hInst)
{
	bmp_create = (HBITMAP)LoadBitmap(hInst, MAKEINTRESOURCE(IDB_CREATE));
	bmp_quickjoin = (HBITMAP)LoadBitmap(hInst, MAKEINTRESOURCE(IDB_QUICKJOIN));
	bmp_whojoin = (HBITMAP)LoadBitmap(hInst, MAKEINTRESOURCE(IDB_WHOJOIN));
	bmp_roombackground = (HBITMAP)LoadBitmap(hInst, MAKEINTRESOURCE(IDB_ROOMBG));
	bmp_room = (HBITMAP)LoadBitmap(hInst, MAKEINTRESOURCE(IDB_ROOM));
	bmp_createwindow = (HBITMAP)LoadBitmap(hInst, MAKEINTRESOURCE(IDB_CREATEWINDOW));
	bmp_chatwindow = (HBITMAP)LoadBitmap(hInst, MAKEINTRESOURCE(IDB_CHATWINDOW));
}

void CLobby::Draw(HDC memdc, HDC memdc2)
{
	DrawBitmap(memdc, memdc2, bmp_create, 0, 0, 400, 140); // 방만들기
	DrawBitmap(memdc, memdc2, bmp_quickjoin, 400, 0, 400, 140); // 빠른 참여
	DrawBitmap(memdc, memdc2, bmp_whojoin, 800, 0, 400, 768); // 접속자

	DrawBitmap(memdc, memdc2, bmp_roombackground, 0, 140, 800, 406); // room background
	DrawBitmap(memdc, memdc2, bmp_chatwindow, 0, 518, 800, 250); // chatting

	Rectangle(memdc, 0, 738, 800, 768); // 맨 밑 채팅창

	hFont = CreateFont(24, 0, 0, 0, 0, 0, 0, 0, HANGEUL_CHARSET, 0, 0, 0, VARIABLE_PITCH | FF_ROMAN, TEXT("함초롱바탕"));
	hOldFont = (HFONT)SelectObject(memdc, hFont);
	SetBkMode(memdc, TRANSPARENT);

	for (int i = 0; i < 6; ++i)
	{
		DrawBitmap(memdc, memdc2, bmp_room, room[i].xPos, room[i].yPos, 335, 102); // 방 박스 출력


		TextOut(memdc, room[i].xPos + 5, room[i].yPos + 5, room[i].roomtitle, wcslen(room[i].roomtitle)); // 방 제목 출력

		if (room[i].roomstatus) // 방이 비어있으면 출력하지 않음
		{
			if (room[i].mode == DEATHMATCH)
				TextOut(memdc, room[i].xPos + 5, room[i].yPos + 75, L"데스매치", 4);
			else
				TextOut(memdc, room[i].xPos + 5, room[i].yPos + 75, L"점령전 ", 4);


			if (!room[i]._private)
				TextOut(memdc, room[i].xPos + 260, room[i].yPos + 5, L" 공개", 3);
			else
				TextOut(memdc, room[i].xPos + 260, room[i].yPos + 5, L"비공개", 3);
		}


		switch (room[i].roomstatus)
		{

		case ROOM_EMPTY:
			TextOut(memdc, room[i].xPos + 120, room[i].yPos + 35, L"생성가능", 4);
			break;
		case ROOM_JOINABLE:
			TextOut(memdc, room[i].xPos + 120, room[i].yPos + 35, L"입장가능", 4);
			break;
		case FULL:
			TextOut(memdc, room[i].xPos + 120, room[i].yPos + 35, L"입장불가", 4);
			break;
		case INGAME:
			TextOut(memdc, room[i].xPos + 120, room[i].yPos + 35, L" 게임중", 4);
			break;
		}

		wchar_t s[6];
		wsprintf(s, L"(%d/8)", room[i].playercount);
		TextOut(memdc, room[i].xPos + 270, room[i].yPos + 68, s, 5); // (0/8) 인원수 출력

		if (clickcount == 1)
		{
			Pen = CreatePen(PS_SOLID, 4, RGB(0, 0, 0));
			oldPen = (HPEN)SelectObject(memdc, Pen);
			SelectObject(memdc, GetStockObject(NULL_BRUSH));

			Rectangle(memdc, room[whatclick].xPos, room[whatclick].yPos, room[whatclick].xPos + 335, room[whatclick].yPos + 102); // 방제목 입력

			
			SelectObject(memdc, oldPen);
			DeleteObject(Pen);
			SelectObject(memdc, GetStockObject(WHITE_BRUSH));
		}
		
	}

	if (RoomCreateWindow) // 방만들기 눌렀을 때
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

		if (RoomCreateChat == RNAME)
			Rectangle(memdc, 370, 300, 720, 340); // 선택된 입력박스
		else if (RoomCreateChat == RPASSWORD)
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

		if (IsPassword)
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
			TextOut(memdc, 372 + (i * 15), 343, L"*", 1);
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

	//채팅창 출력
	vector<wstring>::iterator iter = vOutPut.begin() + iFrontRange;
	for (int i = 0; iter != vOutPut.end() + iLastRange; ++i, ++iter)
	{
		TextOut(memdc, 0, 518 + (i * 20), iter->c_str(), wcslen(iter->c_str()));
	}

	GetTextExtentPoint(memdc, input, wcslen(input), &size);
	TextOut(memdc, 0, 740, input, wcslen(input));

}

void CLobby::DrawBitmap(HDC memdc, HDC memdc2, HBITMAP bitmap, int x, int y, int sizeX, int sizeY)
{
	SelectObject(memdc2, bitmap);
	BitBlt(memdc, x, y, sizeX, sizeY, memdc2, 0, 0, SRCCOPY);
}

void CLobby::Password_Input(WPARAM wParam)
{
	if (IsPassword && RoomCreateChat == RPASSWORD) // 방만들기 비밀번호 입력
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

		else if (len > MAX_PASSWORD_LEN - 2)
		{
			return;
		}

		else if (wParam == VK_RETURN)
			return;
		else if (wParam == VK_TAB)
		{
			if (IsPassword)
				RoomCreateChat = RNAME;
			else
				return;
		}
		else
		{
			RoomPassword[len++] = wParam;
			RoomPassword[len] = '\0';
		}

	}
	else if (RoomCreateChat == RNAME)
	{
		if (wParam == VK_TAB)
			RoomCreateChat = RPASSWORD;
	}

}

void CLobby::L_ButtonDown(HWND hWnd, HWND hChat, int mx, int my)
{
	if (!RoomCreateWindow) // 방만들기
	{
		if (MouseInbox(0, 0, 400, 140, mx, my))
		{
			RoomCreateWindow = true;
			RoomCreateChat = RNOPE;

			SetFocus(hWnd);
			memset(input, '\0', sizeof(input));
			SetWindowTextW(hChat, '\0');

		}

		else if (MouseInbox(400, 0, 800, 140, mx, my)) // 빠른참여
		{

			cs_packet_quickjoin *my_packet = reinterpret_cast<cs_packet_quickjoin *>(send_buffer);
			my_packet->size = sizeof(cs_packet_quickjoin);
			send_wsabuf.len = sizeof(cs_packet_quickjoin);
			DWORD iobyte;
			my_packet->type = CS_QUICK_JOIN;

			WSASend(g_mysocket, &send_wsabuf, 1, &iobyte, 0, NULL, NULL);
		}


		for (int i = 0; i < 6; ++i)
		{
			if (MouseInbox(room[i].xPos, room[i].yPos, room[i].xPos + 335, room[i].yPos + 102, mx, my))
			{
				if (clickcount == 0) // 처음누르면 박스를 그림
				{
					clickcount++;
					whatclick = i;
					//cout << "첫번째 누른거" << ends << i;
					//break;
				}
				//else if (gLobby.clickcount == 1 && gLobby.whatclick == i) // 두번누르면 ( 이전에 눌렀던거랑 같으면)
				//{


				cs_packet_joinroom *my_packet = reinterpret_cast<cs_packet_joinroom *>(send_buffer);
				my_packet->size = sizeof(cs_packet_joinroom);
				send_wsabuf.len = sizeof(cs_packet_joinroom);
				DWORD iobyte;

				my_packet->type = CS_JOIN_ROOM;

				my_packet->roomnumber = whatclick;
				WSASend(g_mysocket, &send_wsabuf, 1, &iobyte, 0, NULL, NULL);

				//whatclick = -1;
				clickcount = 0;
				break;
				//cout << "두번째 누른거" << ends << i;
				
				//break;
				//}

			}
			//else
			//{
			//	if (gLobby.clickcount == 1)
			//	{
			//		gLobby.whatclick = -1;
			//		gLobby.clickcount = 0;
			//		cout << "취소" << endl;
			//		break;
			//	}
			//}
		}

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

			SetFocus(g_hWnd);

			cs_packet_makeroom *my_packet = reinterpret_cast<cs_packet_makeroom *>(send_buffer);
			my_packet->size = sizeof(cs_packet_makeroom);
			send_wsabuf.len = sizeof(cs_packet_makeroom);
			DWORD iobyte;

			my_packet->type = CS_MAKE_ROOM;

			wcscpy_s(my_packet->roomtitle, RoomName);
			if (IsPassword)
				strcpy_s(my_packet->password, RoomPassword);
			else
				memset(my_packet->password, '\0', sizeof(my_packet->password));

			my_packet->mode = GameMode;
			

			WSASend(g_mysocket, &send_wsabuf, 1, &iobyte, 0, NULL, NULL);

			if (IsPassword)
				IsPassword = false;

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
				SetFocus(hWnd);
			}

		}

		else if (MouseInbox(730, 343, 770, 383, mx, my)) // 비밀번호 체크박스 활성화
		{
			IsPassword = !IsPassword; // 비밀번호 체크박스 활성화

			if (IsPassword)
			{
				RoomCreateChat = RPASSWORD;
				SetFocus(hWnd);
			}

			else
				RoomCreateChat = RNOPE;


		}
		else //창을 누르면
		{
			RoomCreateChat = RNOPE;
		}
	}
}

void CLobby::MouseWheel(WPARAM wParam)
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
}


