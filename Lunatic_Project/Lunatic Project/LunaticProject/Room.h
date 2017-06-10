#pragma once

#include "LunaticProject.h"

typedef struct RoomInfo
{
	WCHAR roomtitle[MAX_ROOMTITLE_SIZE];
	int room_number;
	int mode; // 데스매치? 점령전?
	int playercount; // 유저가 방에 몇명 들어왔는지?
	bool _private; // 암호가 있는지?
}RoomInfo;

typedef struct RoomUI
{
	int HeroSelect; // 캐릭터를 고른 것.
	bool IsReady; // 레디했는지?
	int xPos;
	int yPos;
	int Team;
}RoomUI;

class CRoom
{
public:
	CRoom();
	~CRoom();

	RoomInfo RoomInfo{};

	WCHAR output[MAX_CHAT_LINE][CHAT_LENGTH];
	WCHAR input[MAX_STR_SIZE];
	wstring chat_id;
	vector<wstring> vOutPut;
	SIZE size{};

	int iLine = 0;
	int iFrontRange = 0;
	int iLastRange = 0;
	bool bScrool = false;

	//HFONT
	HFONT hFont, hOldFont;
	// HBITMAP
	HPEN Pen, oldPen;
	HBITMAP bmp_background, bmp_Ready;

	HBITMAP bmp_Babarian_Image, bmp_Babarian_Select;
	HBITMAP bmp_Knight_Image, bmp_Knight_Select;
	HBITMAP bmp_Swordman_Image, bmp_Swordman_Select;
	HBITMAP bmp_Magician_Image, bmp_Magician_Select;
	HBITMAP bmp_Archer_Image, bmp_Archer_Select;
	HBITMAP bmp_Healer_Image, bmp_Healer_Select;
	HBITMAP bmp_Witch_Image, bmp_Witch_Select;
	
	RoomUI RoomUI[MAX_GAMER];
	

	void Create(HINSTANCE hInst);

	void Draw(HDC memdc, HDC memdc2);
	void DrawBitmap(HDC memdc, HDC memdc2, HBITMAP bitmap, int x, int y, int sizeX, int sizeY);

	void L_ButtonDown(int mx, int my);
};

