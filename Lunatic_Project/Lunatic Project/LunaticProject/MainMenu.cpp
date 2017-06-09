#include "stdafx.h"
#include "MainMenu.h"


CMainMenu::CMainMenu()
{
}


CMainMenu::~CMainMenu()
{
}

void CMainMenu::Create(HINSTANCE hInst)
{
	bmp_mainmenu = (HBITMAP)LoadBitmap(hInst, MAKEINTRESOURCE(IDB_MAINMENU));
	bmp_loginwindow = (HBITMAP)LoadBitmap(hInst, MAKEINTRESOURCE(IDB_LOGINWINDOW));
}

void CMainMenu::Draw(HDC memdc, HDC memdc2)
{
	DrawBitmap(memdc, memdc2, bmp_mainmenu, 0, 0, 1024, 768);
	
	if (!onLogin)
	{
		Rectangle(memdc, 400, 600, 600, 680);
	}
	else // �α��� �����찡 Ȱ��ȭ �Ǿ��� ��
	{
		DrawBitmap(memdc, memdc2, bmp_loginwindow, 240, 170, 544, 408);
		Rectangle(memdc, 370, 300, 720, 340); // ������ �Է�
		Rectangle(memdc, 370, 343, 720, 383); // ��й�ȣ �Է�

		Pen = CreatePen(PS_SOLID, 4, RGB(0, 0, 0));
		oldPen = (HPEN)SelectObject(memdc, Pen);

		SelectObject(memdc, GetStockObject(NULL_BRUSH));

		if (LoginChat == RNAME)
			Rectangle(memdc, 370, 300, 720, 340); // ���õ� �Է¹ڽ�
		else if (LoginChat == RPASSWORD)
			Rectangle(memdc, 370, 343, 720, 383); // ���õ� �Է¹ڽ�

		SelectObject(memdc, oldPen);
		DeleteObject(Pen);
		SelectObject(memdc, GetStockObject(WHITE_BRUSH));

		hFont = CreateFont(30, 0, 0, 0, 0, 0, 0, 0, HANGEUL_CHARSET, 0, 0, 0, VARIABLE_PITCH | FF_ROMAN, TEXT("���ʷչ���"));
		hOldFont = (HFONT)SelectObject(memdc, hFont);
		SetBkMode(memdc, TRANSPARENT);

		TextOutA(memdc, 370, 305, user_id, strlen(user_id));

		for (int i = 0; i < strlen(user_password); ++i)
		{
			TextOut(memdc, 372 + (i * 15), 343, L"*", 1);
		}

		if (!IsLogin)
		{
			TextOut(memdc, 300, 397, L"���̵�� ��й�ȣ�� Ȯ�����ּ���.", 18);
		}



		SelectObject(memdc2, hFont);
		DeleteObject(hFont);

	}
	
}

void CMainMenu::DrawBitmap(HDC memdc, HDC memdc2, HBITMAP bitmap, int x, int y, int sizeX, int sizeY)
{
	SelectObject(memdc2, bitmap);
	BitBlt(memdc, x, y, sizeX, sizeY, memdc2, 0, 0, SRCCOPY);
}

void CMainMenu::ID_PASSWORD_Input(WPARAM wParam)
{
	if (LoginChat == RNAME) // ���̵� �Է�â Ȱ��ȭ
	{
		int len = strlen(user_id);


		if (wParam == VK_BACK)
		{
			if (len > 0)
			{
				len--;
				user_id[len] = '\0';
			}

		}

		else if (len > MAX_ID_LEN - 2)
			return;

		else if (wParam == VK_RETURN)
			return;

		else if (wParam == VK_TAB)
		{
			LoginChat = RPASSWORD;
		}

		else
		{
			user_id[len++] = wParam;
			user_id[len] = '\0';
		}
	}
	else if (LoginChat == RPASSWORD)
	{
		int len = strlen(user_password);


		if (wParam == VK_BACK)
		{
			if (len > 0)
			{
				len--;
				user_password[len] = '\0';
			}

		}

		else if (len > MAX_PASSWORD_LEN - 2)
			return;

		else if (wParam == VK_RETURN)
			return;

		else if (wParam == VK_TAB)
		{
			LoginChat = RNAME;
		}
		else
		{
			user_password[len++] = wParam;
			user_password[len] = '\0';
		}
	}
}

void CMainMenu::L_ButtonDown(HWND hChat, int mx, int my)
{
	if (!onLogin)
	{
		if (MouseInbox(400, 600, 597, 676, mx, my)) // ���θ޴� �α��� ��ư
		{
			onLogin = true; // �α��� �����츦 Ȱ��ȭ �Ѵ�.
		}

	}
	else
	{
		if (MouseInbox(325, 472, 485, 542, mx, my)) // �α��� Ȯ�� ��ư
		{
			if (strlen(user_id) != 0 && strlen(user_password) != 0)
			{
				// server send (Ready)
				cs_packet_login *my_packet = reinterpret_cast<cs_packet_login *>(send_buffer);
				my_packet->size = sizeof(cs_packet_login);
				send_wsabuf.len = sizeof(cs_packet_login);
				DWORD iobyte;
				my_packet->type = CS_LOGIN;
				strcpy_s(my_packet->id, user_id);
				strcpy_s(my_packet->password, user_password);
				WSASend(g_mysocket, &send_wsabuf, 1, &iobyte, 0, NULL, NULL);
				//
			}

		}
		else if (MouseInbox(536, 472, 697, 542, mx, my)) // �α��� ��Ҹ� �����ų� �游��������� ���� ������ �游��������츦 �����Ѵ�.
		{
			onLogin = false;
			LoginChat = RNOPE;

			memset(user_id, '\0', sizeof(user_id));
			SetWindowTextW(hChat, '\0');
			memset(user_password, '\0', sizeof(user_password));
			SetWindowTextW(hChat, '\0');
		}
		else if (MouseInbox(370, 300, 720, 340, mx, my)) // ���̵� �Է�â Ȱ��ȭ
		{
			LoginChat = RNAME;
		}
		else if (MouseInbox(370, 343, 720, 383, mx, my)) // ��й�ȣ �Է�â Ȱ��ȭ
		{
			LoginChat = RPASSWORD;
		}

	}
}
