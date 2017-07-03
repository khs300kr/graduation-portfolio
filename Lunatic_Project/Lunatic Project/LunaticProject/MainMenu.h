#pragma once

#include "LunaticProject.h"

class CMainMenu
{
public:
	CMainMenu();
	~CMainMenu();

	void Create(HINSTANCE hInst);

	// HBITMAP

	HBITMAP bmp_mainmenu; // ���θ޴� ���ȭ��
	HBITMAP bmp_loginwindow; // �α��� ���ȭ��

	HPEN Pen, oldPen;
	HFONT hFont, hOldFont;



	enum { RNOPE, RNAME, RPASSWORD };

	// ���θ޴�
	//bool onLogin = false; // �α��� �����찡 ���������?
	WCHAR user_id[MAX_ID_LEN]{}; // �α��� ���� ���̵�
	WCHAR user_password[MAX_PASSWORD_LEN]{}; // �α��� ���� ��й�ȣ
	int LoginChat = RNOPE; // 0 �ƹ��͵��ƴ� 1 Name�Է� 2 Password�Է�
	bool IsLogin = true; // ���̵� ��й�ȣ�� �¾Ҵ��� Ʋ�ȴ���?

	void Draw(HDC memdc, HDC memdc2);
	void DrawBitmap(HDC memdc, HDC memdc2, HBITMAP bitmap, int x, int y, int sizeX, int sizeY);

	void ID_PASSWORD_Input(WPARAM wParam);

	void L_ButtonDown(HWND hChat, int mx, int my);
};

