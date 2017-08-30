#pragma once

#include "LunaticProject.h"

typedef struct Room
{
	int xPos; // �� xPos
	int yPos; // �� yPos
	WCHAR roomtitle[MAX_ROOMTITLE_SIZE];
	int roomstatus; // �� ���� ( ������� ��á���� ����������)
	int mode; // ������ġ? ������?
	int playercount; // ������ �濡 ��� ���Դ���?
	bool _private; // ��ȣ�� �ִ���?
}Room;

typedef struct Ending
{
	int xPos;
	int yPos;
}Ending;

class CLobby
{
public:
	CLobby();
	~CLobby();

	
	WCHAR output[MAX_CHAT_LINE][CHAT_LENGTH];
	WCHAR input[MAX_STR_SIZE];
	wstring chat_id;
	vector<wstring> vOutPut;
	SIZE size{};

	int iLine = 0;
	int iFrontRange = 0;
	int iLastRange = 0;
	bool bScrool = false;


	Room room[6];
	Ending ending[MAX_GAMER];

	void Create(HINSTANCE hInst);

	int whatclick;

	//HBITMAP
	HBITMAP bmp_lobbyhome, bmp_create, bmp_quickjoin, bmp_whojoin, bmp_createwindow, bmp_room;

	bool create_over{}, quickjoin_over{};
	int room_over = -1;

	HFONT hFont, hOldFont;
	HPEN Pen, oldPen;
	HBRUSH Brush, oldBrush;
	// �κ�
	bool RoomCreateWindow = false; // �游��� ������ Ȱ��ȭ

	enum { RNOPE, RNAME, RPASSWORD };
	int RoomCreateChat = RNOPE; // 0 �ƹ��͵��ƴ� 1 Name�Է� 2 Password�Է�
	WCHAR RoomName[MAX_ROOMTITLE_SIZE]{}; // �游��� ������
	char RoomPassword[MAX_PASSWORD_LEN]{}; // �游��� ��й�ȣ

	bool IsPassword = false;
	int GameMode = DEATHMATCH;
	
	void Draw(HDC memdc, HDC memdc2);
	void DrawBitmap(HDC memdc, HDC memdc2, HBITMAP bitmap, int x, int y, int sizeX, int sizeY);
	void Password_Input(WPARAM wParam);

	void L_ButtonDown(HWND hWnd, HWND hChat, int mx, int my);
	void MouseMove(int mx, int my);
	void MouseWheel(WPARAM wParam);
	
};

