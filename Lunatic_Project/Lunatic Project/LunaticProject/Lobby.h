#pragma once

#include "LunaticProject.h"

typedef struct Room
{
	int xPos; // 방 xPos
	int yPos; // 방 yPos
	WCHAR roomtitle[MAX_ROOMTITLE_SIZE];
	int roomstatus; // 방 상태 ( 비었는지 꽉찼는지 게임중인지)
	int mode; // 데스매치? 점령전?
	int playercount; // 유저가 방에 몇명 들어왔는지?
	bool _private; // 암호가 있는지?
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
	// 로비
	bool RoomCreateWindow = false; // 방만들기 윈도우 활성화

	enum { RNOPE, RNAME, RPASSWORD };
	int RoomCreateChat = RNOPE; // 0 아무것도아님 1 Name입력 2 Password입력
	WCHAR RoomName[MAX_ROOMTITLE_SIZE]{}; // 방만들기 방제목
	char RoomPassword[MAX_PASSWORD_LEN]{}; // 방만들기 비밀번호

	bool IsPassword = false;
	int GameMode = DEATHMATCH;
	
	void Draw(HDC memdc, HDC memdc2);
	void DrawBitmap(HDC memdc, HDC memdc2, HBITMAP bitmap, int x, int y, int sizeX, int sizeY);
	void Password_Input(WPARAM wParam);

	void L_ButtonDown(HWND hWnd, HWND hChat, int mx, int my);
	void MouseMove(int mx, int my);
	void MouseWheel(WPARAM wParam);
	
};

