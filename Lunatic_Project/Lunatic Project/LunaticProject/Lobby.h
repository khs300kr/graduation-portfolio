#pragma once

typedef struct Room
{
	int xPos; // 방 xPos
	int yPos; // 방 yPos
	WCHAR roomtitle[MAX_ROOMTITLE_SIZE];
	int roomstatus; // 방 상태 ( 비었는지 꽉찼는지 게임중인지)
	int mode; // 데스매치? 점령전?
	int playercount; // 유저가 방에 몇명 들어왔는지?
	int _private; // 암호가 있는지?
}Room;

class CLobby
{
public:
	CLobby();
	~CLobby();

	Room room[6];

	void Create(HINSTANCE hInst);

	//HBITMAP
	HBITMAP bmp_chatwindow, bmp_create, bmp_quickjoin, bmp_whojoin, bmp_roombackground, bmp_createwindow, bmp_room;

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
	
};

