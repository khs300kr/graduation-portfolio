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
	bmp_background = (HBITMAP)LoadBitmap(hInst, MAKEINTRESOURCE(IDB_BITMAP1));
}

void CRoom::Draw(HDC memdc, HDC memdc2)
{
	DrawBitmap(memdc, memdc2, bmp_background, 0, 0, 1024, 768);

	Pen = CreatePen(PS_SOLID, 3, RGB(255, 0, 0));
	oldPen = (HPEN)SelectObject(memdc, Pen);

	SelectObject(memdc, GetStockObject(NULL_BRUSH));

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
