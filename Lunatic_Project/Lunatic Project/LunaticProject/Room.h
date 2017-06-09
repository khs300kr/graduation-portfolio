#pragma once

#include "LunaticProject.h"

class CRoom
{
public:
	CRoom();
	~CRoom();


	// HBITMAP
	HPEN Pen, oldPen;
	HBITMAP bmp_background;

	
	int HeroSelect = EMPTY;
	

	void Create(HINSTANCE hInst);

	void Draw(HDC memdc, HDC memdc2);
	void DrawBitmap(HDC memdc, HDC memdc2, HBITMAP bitmap, int x, int y, int sizeX, int sizeY);

	void L_ButtonDown(int mx, int my);
};

