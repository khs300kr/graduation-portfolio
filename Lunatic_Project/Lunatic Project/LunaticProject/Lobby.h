#pragma once

typedef struct Room
{
	int xPos; // �� xPos
	int yPos; // �� yPos
	int roomInfo; // �� ����
	int people; // ������ �濡 ��� ���Դ���?
}Room;

class CLobby
{
public:
	CLobby();
	~CLobby();

	Room room[3][2];

	void Create(HINSTANCE hInst);

	//HBITMAP
	HBITMAP bmp_chatwindow, bmp_create, bmp_quickjoin, bmp_whojoin, bmp_roombackground, bmp_createwindow, bmp_room;

	HFONT hFont, hOldFont;
	HPEN Pen, oldPen;
	// �κ�
	bool RoomCreateWindow = false; // �游��� ������ Ȱ��ȭ

	enum { RNOPE, RNAME, RPASSWORD };
	int RoomCreateChat = RNOPE; // 0 �ƹ��͵��ƴ� 1 Name�Է� 2 Password�Է�
	WCHAR RoomName[MAX_ROOMTITLE_SIZE]{}; // �游��� ������
	char RoomPassword[MAX_PASSWORD_LEN]{}; // �游��� ��й�ȣ


	
	void Draw(HDC memdc, HDC memdc2);
	void DrawBitmap(HDC memdc, HDC memdc2, HBITMAP bitmap, int x, int y, int sizeX, int sizeY);
};

