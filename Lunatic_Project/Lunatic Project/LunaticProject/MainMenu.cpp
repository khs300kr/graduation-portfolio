#include "stdafx.h"
#include "MainMenu.h"
#include "LunaticProject.h"

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
	else // 로그인 윈도우가 활성화 되었을 때
	{
		DrawBitmap(memdc, memdc2, bmp_loginwindow, 240, 170, 544, 408);
		Rectangle(memdc, 370, 300, 720, 340); // 방제목 입력
		Rectangle(memdc, 370, 343, 720, 383); // 비밀번호 입력

		Pen = CreatePen(PS_SOLID, 4, RGB(0, 0, 0));
		oldPen = (HPEN)SelectObject(memdc, Pen);

		SelectObject(memdc, GetStockObject(NULL_BRUSH));

		if (LoginChat == RNAME)
			Rectangle(memdc, 370, 300, 720, 340); // 선택된 입력박스
		else if (LoginChat == RPASSWORD)
			Rectangle(memdc, 370, 343, 720, 383); // 선택된 입력박스

		SelectObject(memdc, oldPen);
		DeleteObject(Pen);
		SelectObject(memdc, GetStockObject(WHITE_BRUSH));

		hFont = CreateFont(30, 0, 0, 0, 0, 0, 0, 0, HANGEUL_CHARSET, 0, 0, 0, VARIABLE_PITCH | FF_ROMAN, TEXT("함초롱바탕"));
		hOldFont = (HFONT)SelectObject(memdc, hFont);
		SetBkMode(memdc, TRANSPARENT);

		TextOutA(memdc, 370, 305, user_id, strlen(user_id));

		for (int i = 0; i < strlen(user_password); ++i)
		{
			TextOut(memdc, 372 + (i * 15), 343, L"*", 1);
		}

		if (!IsLogin)
		{
			TextOut(memdc, 300, 397, L"아이디와 비밀번호를 확인해주세요.", 18);
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