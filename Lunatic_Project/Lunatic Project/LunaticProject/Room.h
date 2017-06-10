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

	HBITMAP bmp_Babarian_Image, bmp_Babarian_Select;
	HBITMAP bmp_Knight_Image, bmp_Knight_Select;
	HBITMAP bmp_Swordman_Image, bmp_Swordman_Select;
	HBITMAP bmp_Magician_Image, bmp_Magician_Select;
	HBITMAP bmp_Archer_Image, bmp_Archer_Select;
	HBITMAP bmp_Healer_Image, bmp_Healer_Select;
	HBITMAP bmp_Witch_Image, bmp_Witch_Select;
	
	int HeroSelect = EMPTY;
	

	void Create(HINSTANCE hInst);

	void Draw(HDC memdc, HDC memdc2);
	void DrawBitmap(HDC memdc, HDC memdc2, HBITMAP bitmap, int x, int y, int sizeX, int sizeY);

	void L_ButtonDown(int mx, int my);
};

