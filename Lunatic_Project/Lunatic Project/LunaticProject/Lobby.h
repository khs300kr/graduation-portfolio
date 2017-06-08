#pragma once

typedef struct Room
{
	int xPos; // �� xPos
	int yPos; // �� yPos
	WCHAR roomtitle[MAX_ROOMTITLE_SIZE];
	int roomstatus; // �� ���� ( ������� ��á���� ����������)
	int mode; // ������ġ? ������?
	int playercount; // ������ �濡 ��� ���Դ���?
	int _private; // ��ȣ�� �ִ���?
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
	
};

