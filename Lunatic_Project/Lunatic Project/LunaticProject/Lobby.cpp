#include "stdafx.h"
#include "Lobby.h"



CLobby::CLobby()
{
	// �κ񿡼� ���̴� ��

	int k = 0;

	for (int i = 0; i < 3; ++i)
	{
		for (int j = 0; j < 2; ++j)
		{
			room[k].xPos = 30 + (j * 400); // ���� X
			room[k].yPos = 160 + (i * 120); // ���� Y
			room[k].roomstatus = EMPTY; // �� ����
			room[k++].playercount = 0; // ������ ��� �濡 ���Դ���?
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
	DrawBitmap(memdc, memdc2, bmp_create, 0, 0, 400, 140); // �游���
	DrawBitmap(memdc, memdc2, bmp_quickjoin, 400, 0, 400, 140); // ���� ����
	DrawBitmap(memdc, memdc2, bmp_whojoin, 800, 0, 400, 768); // ������

	DrawBitmap(memdc, memdc2, bmp_roombackground, 0, 140, 800, 406); // room background
	DrawBitmap(memdc, memdc2, bmp_chatwindow, 0, 518, 800, 250); // chatting

	Rectangle(memdc, 0, 738, 800, 768); // �� �� ä��â

	hFont = CreateFont(24, 0, 0, 0, 0, 0, 0, 0, HANGEUL_CHARSET, 0, 0, 0, VARIABLE_PITCH | FF_ROMAN, TEXT("���ʷչ���"));
	hOldFont = (HFONT)SelectObject(memdc, hFont);
	SetBkMode(memdc, TRANSPARENT);

	for (int i = 0; i < 6; ++i)
	{
		DrawBitmap(memdc, memdc2, bmp_room, room[i].xPos, room[i].yPos, 335, 102); // �� �ڽ� ���


		TextOut(memdc, room[i].xPos + 5, room[i].yPos + 5, room[i].roomtitle, wcslen(room[i].roomtitle)); // �� ���� ���

		if (room[i].roomstatus) // ���� ��������� ������� ����
		{
			if (room[i].mode == DEATHMATCH)
				TextOut(memdc, room[i].xPos + 5, room[i].yPos + 75, L"������ġ", 4);
			else
				TextOut(memdc, room[i].xPos + 5, room[i].yPos + 75, L"������ ", 4);


			if (!room[i]._private)
				TextOut(memdc, room[i].xPos + 260, room[i].yPos + 5, L" ����", 3);
			else
				TextOut(memdc, room[i].xPos + 260, room[i].yPos + 5, L"�����", 3);
		}


		switch (room[i].roomstatus)
		{

		case ROOM_EMPTY:
			TextOut(memdc, room[i].xPos + 120, room[i].yPos + 35, L"��������", 4);
			break;
		case ROOM_JOINABLE:
			TextOut(memdc, room[i].xPos + 120, room[i].yPos + 35, L"���尡��", 4);
			break;
		case FULL:
			TextOut(memdc, room[i].xPos + 120, room[i].yPos + 35, L"����Ұ�", 4);
			break;
		case INGAME:
			TextOut(memdc, room[i].xPos + 120, room[i].yPos + 35, L" ������", 4);
			break;
		}

		wchar_t s[6];
		wsprintf(s, L"(%d/8)", room[i].playercount);
		TextOut(memdc, room[i].xPos + 270, room[i].yPos + 68, s, 5); // (0/8) �ο��� ���

		if (clickcount == 1)
		{
			Pen = CreatePen(PS_SOLID, 4, RGB(0, 0, 0));
			oldPen = (HPEN)SelectObject(memdc, Pen);
			SelectObject(memdc, GetStockObject(NULL_BRUSH));

			Rectangle(memdc, room[whatclick].xPos, room[whatclick].yPos, room[whatclick].xPos + 335, room[whatclick].yPos + 102); // ������ �Է�

			
			SelectObject(memdc, oldPen);
			DeleteObject(Pen);
			SelectObject(memdc, GetStockObject(WHITE_BRUSH));
		}
		
	}

	if (RoomCreateWindow) // �游��� ������ ��
	{
		hFont = CreateFont(30, 0, 0, 0, 0, 0, 0, 0, HANGEUL_CHARSET, 0, 0, 0, VARIABLE_PITCH | FF_ROMAN, TEXT("���ʷչ���"));
		hOldFont = (HFONT)SelectObject(memdc, hFont);
		SetBkMode(memdc, TRANSPARENT);

		DrawBitmap(memdc, memdc2, bmp_createwindow, 240, 170, 544, 408);

		Rectangle(memdc, 370, 300, 720, 340); // ������ �Է�
		Rectangle(memdc, 370, 343, 720, 383); // ��й�ȣ �Է�

		Pen = CreatePen(PS_SOLID, 4, RGB(0, 0, 0));
		oldPen = (HPEN)SelectObject(memdc, Pen);

		SelectObject(memdc, GetStockObject(NULL_BRUSH));

		if (RoomCreateChat == RNAME)
			Rectangle(memdc, 370, 300, 720, 340); // ���õ� �Է¹ڽ�
		else if (RoomCreateChat == RPASSWORD)
			Rectangle(memdc, 370, 343, 720, 383); // ���õ� �Է¹ڽ�


		SelectObject(memdc, oldPen);
		DeleteObject(Pen);
		SelectObject(memdc, GetStockObject(WHITE_BRUSH));


		Rectangle(memdc, 730, 343, 770, 383); // ��й�ȣ üũ�ڽ�
		Rectangle(memdc, 520, 397, 550, 427); //������ġ üũ�ڽ�
		Rectangle(memdc, 670, 397, 700, 427); //������ üũ�ڽ�

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

			Rectangle(memdc, 372, 343, 718, 383); // ��й�ȣ üũ�ڽ�

			SelectObject(memdc, oldBrush);
			DeleteObject(Brush);
		}

		SelectObject(memdc2, hFont);
		DeleteObject(hFont);
	}




	hFont = CreateFont(15, 0, 0, 0, 0, 0, 0, 0, HANGEUL_CHARSET, 0, 0, 0, VARIABLE_PITCH | FF_ROMAN, TEXT("���ʷչ���"));
	hOldFont = (HFONT)SelectObject(memdc, hFont);

	//ä��â ���
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
	if (IsPassword && RoomCreateChat == RPASSWORD) // �游��� ��й�ȣ �Է�
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
	if (!RoomCreateWindow) // �游���
	{
		if (MouseInbox(0, 0, 400, 140, mx, my))
		{
			RoomCreateWindow = true;
			RoomCreateChat = RNOPE;

			SetFocus(hWnd);
			memset(input, '\0', sizeof(input));
			SetWindowTextW(hChat, '\0');

		}

		else if (MouseInbox(400, 0, 800, 140, mx, my)) // ��������
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
				if (clickcount == 0) // ó�������� �ڽ��� �׸�
				{
					clickcount++;
					whatclick = i;
					//cout << "ù��° ������" << ends << i;
					//break;
				}
				//else if (gLobby.clickcount == 1 && gLobby.whatclick == i) // �ι������� ( ������ �������Ŷ� ������)
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
				//cout << "�ι�° ������" << ends << i;
				
				//break;
				//}

			}
			//else
			//{
			//	if (gLobby.clickcount == 1)
			//	{
			//		gLobby.whatclick = -1;
			//		gLobby.clickcount = 0;
			//		cout << "���" << endl;
			//		break;
			//	}
			//}
		}

	}

	else if (RoomCreateWindow) // �游��� �����찡 Ȱ��ȭ �Ǿ����� ��
	{
		if (!MouseInbox(240, 170, 784, 578, mx, my) || MouseInbox(536, 473, 696, 541, mx, my)) //��Ҹ� �����ų� �游��������� ���� ������ �游��������츦 �����Ѵ�.
		{
			if (IsPassword)
				IsPassword = false;

			RoomCreateWindow = false; // �游��� ������ ����

			SetFocus(hWnd);
			memset(RoomName, '\0', sizeof(RoomName));
			SetWindowTextW(hChat, '\0');
		}


		else if (MouseInbox(325, 473, 484, 541, mx, my)) // Ȯ���� ������ �濡 ����
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

		else if (GameMode != DEATHMATCH && MouseInbox(378, 397, 548, 425, mx, my)) // ������ġ Ŭ��
		{
			GameMode = DEATHMATCH;
		}

		else if (GameMode != TERRITORY && MouseInbox(567, 397, 698, 425, mx, my)) // ������ Ŭ��
		{
			GameMode = TERRITORY;
		}

		else if (MouseInbox(370, 300, 720, 340, mx, my)) // ������ Ȱ��ȭ
		{
			RoomCreateChat = RNAME;
			SetFocus(hChat); // ������ Ȱ��ȭ
		}

		else if (MouseInbox(370, 343, 720, 383, mx, my)) // ��й�ȣ Ȱ��ȭ
		{
			if (IsPassword)
			{
				RoomCreateChat = RPASSWORD;
				SetFocus(hWnd);
			}

		}

		else if (MouseInbox(730, 343, 770, 383, mx, my)) // ��й�ȣ üũ�ڽ� Ȱ��ȭ
		{
			IsPassword = !IsPassword; // ��й�ȣ üũ�ڽ� Ȱ��ȭ

			if (IsPassword)
			{
				RoomCreateChat = RPASSWORD;
				SetFocus(hWnd);
			}

			else
				RoomCreateChat = RNOPE;


		}
		else //â�� ������
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


