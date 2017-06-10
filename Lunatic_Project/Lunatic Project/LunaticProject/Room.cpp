#include "stdafx.h"
#include "Room.h"


CRoom::CRoom()
{
}


CRoom::~CRoom()
{
}

void CRoom::Create(HINSTANCE hInst)
{
	bmp_background = (HBITMAP)LoadBitmap(hInst, MAKEINTRESOURCE(IDB_RBACKGROUND));

	bmp_Babarian_Image = (HBITMAP)LoadBitmap(hInst, MAKEINTRESOURCE(IDB_BABARIAN_IMAGE));
	bmp_Babarian_Select = (HBITMAP)LoadBitmap(hInst, MAKEINTRESOURCE(IDB_BABARIAN_SELECT));

	bmp_Knight_Image = (HBITMAP)LoadBitmap(hInst, MAKEINTRESOURCE(IDB_KNIGHT_IMAGE));
	bmp_Knight_Select = (HBITMAP)LoadBitmap(hInst, MAKEINTRESOURCE(IDB_KNIGHT_SELECT));

	bmp_Swordman_Image = (HBITMAP)LoadBitmap(hInst, MAKEINTRESOURCE(IDB_SWORDMAN_IMAGE));
	bmp_Swordman_Select = (HBITMAP)LoadBitmap(hInst, MAKEINTRESOURCE(IDB_SWORDMAN_SELECT));

	bmp_Magician_Image = (HBITMAP)LoadBitmap(hInst, MAKEINTRESOURCE(IDB_MAGICIAN_IMAGE));
	bmp_Magician_Select = (HBITMAP)LoadBitmap(hInst, MAKEINTRESOURCE(IDB_MAGICIAN_SELECT));

	bmp_Archer_Image = (HBITMAP)LoadBitmap(hInst, MAKEINTRESOURCE(IDB_ARCHER_IMAGE));
	bmp_Archer_Select = (HBITMAP)LoadBitmap(hInst, MAKEINTRESOURCE(IDB_ARCHER_SELECT));

	bmp_Healer_Image = (HBITMAP)LoadBitmap(hInst, MAKEINTRESOURCE(IDB_HEALER_IMAGE));
	bmp_Healer_Select = (HBITMAP)LoadBitmap(hInst, MAKEINTRESOURCE(IDB_HEALER_SELECT));

	bmp_Witch_Image = (HBITMAP)LoadBitmap(hInst, MAKEINTRESOURCE(IDB_WITCH_IMAGE));
	bmp_Witch_Select = (HBITMAP)LoadBitmap(hInst, MAKEINTRESOURCE(IDB_WITCH_SELECT));

}

void CRoom::Draw(HDC memdc, HDC memdc2)
{
	DrawBitmap(memdc, memdc2, bmp_background, 0, 0, 1024, 768);

	Pen = CreatePen(PS_SOLID, 3, RGB(255, 0, 0));
	oldPen = (HPEN)SelectObject(memdc, Pen);

	SelectObject(memdc, GetStockObject(NULL_BRUSH));

	// ¿ÞÂÊ
	DrawBitmap(memdc, memdc2, bmp_Babarian_Image, 81, 48, 94, 94);
	DrawBitmap(memdc, memdc2, bmp_Knight_Image, 81, 48 + 156, 94, 94);
	DrawBitmap(memdc, memdc2, bmp_Archer_Image, 81, 48 + 156 + 158, 94, 94);
	DrawBitmap(memdc, memdc2, bmp_Healer_Image, 81, 48 + 156 + 158 + 160, 94, 94);

	// ¿À¸¥ÂÊ
	DrawBitmap(memdc, memdc2, bmp_Babarian_Image, 843, 48, 94, 94);
	DrawBitmap(memdc, memdc2, bmp_Babarian_Image, 843, 48 + 156, 94, 94);
	DrawBitmap(memdc, memdc2, bmp_Babarian_Image, 843, 48 + 156 + 158, 94, 94);
	DrawBitmap(memdc, memdc2, bmp_Babarian_Image, 843, 48 + 156 + 158 + 160, 94, 94);

	switch (HeroSelect)
	{
	case BABARIAN:
		Rectangle(memdc, 406, 48, 499, 143);
		break;
	case KNIGHT:
		Rectangle(memdc, 526, 48, 622, 143);
		break;
	case SWORDMAN:
		Rectangle(memdc, 355, 173, 449, 265);
		break;
	case MAGICIAN:
		Rectangle(memdc, 469, 173, 561, 265);
		break;
	case ARCHER:
		Rectangle(memdc, 578, 173, 672, 265);
		break;
	case HEALER:
		Rectangle(memdc, 407, 300, 499, 393);
		break;
	case WITCH:
		Rectangle(memdc, 526, 300, 622, 393);
		break;
	}



	SelectObject(memdc, oldPen);
	DeleteObject(Pen);
}

void CRoom::DrawBitmap(HDC memdc, HDC memdc2, HBITMAP bitmap, int x, int y, int sizeX, int sizeY)
{
	SelectObject(memdc2, bitmap);
	BitBlt(memdc, x, y, sizeX, sizeY, memdc2, 0, 0, SRCCOPY);
}

void CRoom::L_ButtonDown(int mx, int my)
{
	if (MouseInbox(406, 48, 499, 143, mx, my)) // Babarian
	{
		HeroSelect = BABARIAN;
	}
	else if (MouseInbox(526, 48, 622, 143, mx, my)) // Knight
	{
		HeroSelect = KNIGHT;
	}
	else if (MouseInbox(355, 173, 449, 265, mx, my)) // SwordMan
	{
		HeroSelect = SWORDMAN;
	}
	else if (MouseInbox(469, 173, 561, 265, mx, my)) // Magician
	{
		HeroSelect = MAGICIAN;
	}
	else if (MouseInbox(578, 173, 672, 265, mx, my)) // Archer
	{
		HeroSelect = ARCHER;
	}
	else if (MouseInbox(407, 300, 499, 393, mx, my)) // Healer
	{
		HeroSelect = HEALER;
	}
	else if (MouseInbox(526, 300, 622, 393, mx, my)) // Witch
	{
		HeroSelect = WITCH;
	}

	cout << mx << ends << my << endl;
}
