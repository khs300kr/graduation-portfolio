#include "stdafx.h"
#include "Room.h"



CRoom::CRoom()
{
	for (int i = 0; i < MAX_ROOM; ++i)
	{
		RoomUI[i].HeroSelect = EMPTY;
		RoomUI[i].IsReady = false;
		RoomUI[i].Team = 0;
	}

	RoomUI[0].xPos = 81;
	RoomUI[0].yPos = 48;

	RoomUI[1].xPos = 843;
	RoomUI[1].yPos = 48;

	RoomUI[2].xPos = 81;
	RoomUI[2].yPos = 204;

	RoomUI[3].xPos = 843;
	RoomUI[3].yPos = 204;

	RoomUI[4].xPos = 81;
	RoomUI[4].yPos = 362;

	RoomUI[5].xPos = 843;
	RoomUI[5].yPos = 362;

	RoomUI[6].xPos = 81;
	RoomUI[6].yPos = 522;

	RoomUI[7].xPos = 843;
	RoomUI[7].yPos = 522;
	
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

	bmp_Ready = (HBITMAP)LoadBitmap(hInst, MAKEINTRESOURCE(IDB_READY));

}

void CRoom::Draw(HDC memdc, HDC memdc2)
{
	DrawBitmap(memdc, memdc2, bmp_background, 0, 0, 1024, 768);

	Pen = CreatePen(PS_SOLID, 3, RGB(255, 0, 0));
	oldPen = (HPEN)SelectObject(memdc, Pen);

	SelectObject(memdc, GetStockObject(NULL_BRUSH));

	
	for (int i = 0; i < MAX_ROOM; ++i)
	{
		if (RoomUI[i].IsReady)
		{
			DrawBitmap(memdc, memdc2, bmp_Ready, RoomUI[i].xPos - 52, RoomUI[i].yPos - 17, 198, 128);
			
			/*if (RoomUI[i].Team == RoomUI[GetMyGame_id()].Team)
			{
				if (RoomUI[i].HeroSelect == BABARIAN)
					DrawBitmap(memdc, memdc2, bmp_Babarian_Select, 406, 48, 198, 128);
				else if (RoomUI[i].HeroSelect == KNIGHT)
					DrawBitmap(memdc, memdc2, bmp_Knight_Select, 526, 48, 198, 128);
				else if (RoomUI[i].HeroSelect == SWORDMAN)
					DrawBitmap(memdc, memdc2, bmp_Swordman_Select, 355, 173, 198, 128);
				else if (RoomUI[i].HeroSelect == MAGICIAN)
					DrawBitmap(memdc, memdc2, bmp_Magician_Select, 469, 173, 198, 128);
				else if (RoomUI[i].HeroSelect == ARCHER)
					DrawBitmap(memdc, memdc2, bmp_Archer_Select, 578, 173, 198, 128);
				else if (RoomUI[i].HeroSelect == HEALER)
					DrawBitmap(memdc, memdc2, bmp_Healer_Select, 407, 300, 198, 128);
				else if (RoomUI[i].HeroSelect == WITCH)
					DrawBitmap(memdc, memdc2, bmp_Witch_Select, 526, 300, 198, 128);
			}*/

		}
			

		if (RoomUI[i].HeroSelect == BABARIAN)
			DrawBitmap(memdc, memdc2, bmp_Babarian_Image, RoomUI[i].xPos, RoomUI[i].yPos, 94, 94);
		else if (RoomUI[i].HeroSelect == KNIGHT)
			DrawBitmap(memdc, memdc2, bmp_Knight_Image, RoomUI[i].xPos, RoomUI[i].yPos, 94, 94);
		else if (RoomUI[i].HeroSelect == SWORDMAN)
			DrawBitmap(memdc, memdc2, bmp_Swordman_Image, RoomUI[i].xPos, RoomUI[i].yPos, 94, 94);
		else if (RoomUI[i].HeroSelect == MAGICIAN)
			DrawBitmap(memdc, memdc2, bmp_Magician_Image, RoomUI[i].xPos, RoomUI[i].yPos, 94, 94);
		else if (RoomUI[i].HeroSelect == ARCHER)
			DrawBitmap(memdc, memdc2, bmp_Archer_Image, RoomUI[i].xPos, RoomUI[i].yPos, 94, 94);
		else if (RoomUI[i].HeroSelect == HEALER)
			DrawBitmap(memdc, memdc2, bmp_Healer_Image, RoomUI[i].xPos, RoomUI[i].yPos, 94, 94);
		else if (RoomUI[i].HeroSelect == WITCH)
			DrawBitmap(memdc, memdc2, bmp_Witch_Image, RoomUI[i].xPos, RoomUI[i].yPos, 94, 94);
		
	}
	
	

	switch (RoomUI[GetMyGame_id()].HeroSelect) // 내꺼 클릭한 Hero의 빨간박스를 그린다.
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

	Pen = CreatePen(PS_SOLID, 3, RGB(0, 0, 255));
	oldPen = (HPEN)SelectObject(memdc, Pen);

	Rectangle(memdc, RoomUI[GetMyGame_id()].xPos - 52, RoomUI[GetMyGame_id()].yPos - 17, RoomUI[GetMyGame_id()].xPos + 198 - 52, RoomUI[GetMyGame_id()].yPos + 128 - 17);


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
	if (!RoomUI[GetMyGame_id()].IsReady)
	{
		if (MouseInbox(406, 48, 499, 143, mx, my)) // Babarian
		{
			RoomUI[GetMyGame_id()].HeroSelect = BABARIAN;
		}
		else if (MouseInbox(526, 48, 622, 143, mx, my)) // Knight
		{
			RoomUI[GetMyGame_id()].HeroSelect = KNIGHT;
		}
		else if (MouseInbox(355, 173, 449, 265, mx, my)) // SwordMan
		{
			RoomUI[GetMyGame_id()].HeroSelect = SWORDMAN;
		}
		else if (MouseInbox(469, 173, 561, 265, mx, my)) // Magician
		{
			RoomUI[GetMyGame_id()].HeroSelect = MAGICIAN;
		}
		else if (MouseInbox(578, 173, 672, 265, mx, my)) // Archer
		{
			RoomUI[GetMyGame_id()].HeroSelect = ARCHER;
		}
		else if (MouseInbox(407, 300, 499, 393, mx, my)) // Healer
		{
			RoomUI[GetMyGame_id()].HeroSelect = HEALER;
		}
		else if (MouseInbox(526, 300, 622, 393, mx, my)) // Witch
		{
			RoomUI[GetMyGame_id()].HeroSelect = WITCH;
		}

		else if (MouseInbox(398, 443, 624, 527, mx, my)) // Ready 버튼
		{
			SendReadyButton();
			RoomUI[GetMyGame_id()].IsReady = true; // 내꺼
			InvalidateRect(g_hWnd, NULL, false);
		}
	}

	cout << mx << ends << my << endl;
}
