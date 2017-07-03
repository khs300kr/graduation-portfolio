#pragma once

#include "LunaticProject.h"

class CMainMenu
{
public:
	CMainMenu();
	~CMainMenu();

	void Create(HINSTANCE hInst);

	// HBITMAP

	HBITMAP bmp_mainmenu; // 메인메뉴 배경화면
	HBITMAP bmp_loginwindow; // 로그인 배경화면

	HPEN Pen, oldPen;
	HFONT hFont, hOldFont;



	enum { RNOPE, RNAME, RPASSWORD };

	// 메인메뉴
	//bool onLogin = false; // 로그인 윈도우가 출력중인지?
	WCHAR user_id[MAX_ID_LEN]{}; // 로그인 유저 아이디
	WCHAR user_password[MAX_PASSWORD_LEN]{}; // 로그인 유저 비밀번호
	int LoginChat = RNOPE; // 0 아무것도아님 1 Name입력 2 Password입력
	bool IsLogin = true; // 아이디 비밀번호가 맞았는지 틀렸는지?

	void Draw(HDC memdc, HDC memdc2);
	void DrawBitmap(HDC memdc, HDC memdc2, HBITMAP bitmap, int x, int y, int sizeX, int sizeY);

	void ID_PASSWORD_Input(WPARAM wParam);

	void L_ButtonDown(HWND hChat, int mx, int my);
};

