#pragma once
class CRoom
{
public:
	CRoom();
	~CRoom();

	void Draw();
	void DrawBitmap(HDC memdc, HDC memdc2, HBITMAP bitmap, int x, int y, int sizeX, int sizeY);
};

