#include "stdafx.h"
#include "Lobby.h"
#include "LunaticProject.h"


CLobby::CLobby()
{
	// 로비에서 보이는 룸

	int k = 0;

	for (int i = 0; i < 3; ++i)
	{
		for (int j = 0; j < 2; ++j)
		{
			room[k].xPos = 30 + (j * 400); // 방의 X
			room[k].yPos = 160 + (i * 120); // 방의 Y
			room[k].roomstatus = Empty; // 방 정보
			room[k++].playercount = 0; // 유저가 몇명 방에 들어왔는지?
		}
	}

}


CLobby::~CLobby()
{
}

void CLobby::Create(HINSTANCE hInst)
{
	bmp_create = (HBITMAP)LoadBitmap(hInst, MAKEINTRESOURCE(IDB_CREATE));
	bmp_quickjoin = (HBITMAP)LoadBitmap(hInst, MAKEINTRESOURCE(IDB_QUICKJOIN));
	bmp_whojoin = (HBITMAP)LoadBitmap(hInst, MAKEINTRESOURCE(IDB_WHOJOIN));
	bmp_roombackground = (HBITMAP)LoadBitmap(hInst, MAKEINTRESOURCE(IDB_ROOMBG));
	bmp_room = (HBITMAP)LoadBitmap(hInst, MAKEINTRESOURCE(IDB_ROOM));
	bmp_createwindow = (HBITMAP)LoadBitmap(hInst, MAKEINTRESOURCE(IDB_CREATEWINDOW));
	bmp_chatwindow = (HBITMAP)LoadBitmap(hInst, MAKEINTRESOURCE(IDB_CHATWINDOW));
}

void CLobby::Draw(HDC memdc, HDC memdc2)
{
	DrawBitmap(memdc, memdc2, bmp_create, 0, 0, 400, 140); // 방만들기
	DrawBitmap(memdc, memdc2, bmp_quickjoin, 400, 0, 400, 140); // 빠른 참여
	DrawBitmap(memdc, memdc2, bmp_whojoin, 800, 0, 400, 768); // 접속자

	DrawBitmap(memdc, memdc2, bmp_roombackground, 0, 140, 800, 406); // room background
	DrawBitmap(memdc, memdc2, bmp_chatwindow, 0, 518, 800, 250); // chatting

	Rectangle(memdc, 0, 738, 800, 768); // 맨 밑 채팅창

	hFont = CreateFont(18, 0, 0, 0, 0, 0, 0, 0, HANGEUL_CHARSET, 0, 0, 0, VARIABLE_PITCH | FF_ROMAN, TEXT("함초롱바탕"));
	hOldFont = (HFONT)SelectObject(memdc, hFont);
	SetBkMode(memdc, TRANSPARENT);

	for (int i = 0; i < 6; ++i)
	{
		DrawBitmap(memdc, memdc2, bmp_room, room[i].xPos, room[i].yPos, 335, 102); // 방 박스 출력


		TextOut(memdc, room[i].xPos + 5, room[i].yPos + 5, room[i].roomtitle, wcslen(room[i].roomtitle));

		if (room[i].roomstatus) // 방이 비어있으면 출력하지 않음
		{
			if (room[i].mode == DEATHMATCH)
				TextOut(memdc, room[i].xPos + 25, room[i].yPos + 25, L"데스매치", 5);
			else
				TextOut(memdc, room[i].xPos + 25, room[i].yPos + 25, L"점령전", 3);


			if (!room[i]._private)
				TextOut(memdc, room[i].xPos + 40, room[i].yPos + 40, L"공개", 2);
			else
				TextOut(memdc, room[i].xPos + 40, room[i].yPos + 40, L"비공개", 3);
		}


		switch (room[i].roomstatus)
		{

		case ROOM_EMPTY:
			TextOut(memdc, room[i].xPos + 50, room[i].yPos + 30, L"생성가능", 4);
			break;
		case ROOM_JOINABLE:
			TextOut(memdc, room[i].xPos + 50, room[i].yPos + 30, L"입장가능", 4);
			break;
		case FULL:
			TextOut(memdc, room[i].xPos + 50, room[i].yPos + 30, L"입장불가", 4);
			break;
		case INGAME:
			TextOut(memdc, room[i].xPos + 50, room[i].yPos + 30, L"게임중", 3);
			break;
		}

		wchar_t s[6];
		wsprintf(s, L"(%d/8)", room[i].playercount);
		TextOut(memdc, room[i].xPos + 260, room[i].yPos + 68, s, 5); // (0/8) 인원수 출력

		if (clickcount == 1)
		{
			Pen = CreatePen(PS_SOLID, 4, RGB(0, 0, 0));
			oldPen = (HPEN)SelectObject(memdc, Pen);
			SelectObject(memdc, GetStockObject(NULL_BRUSH));

			Rectangle(memdc, room[whatclick].xPos, room[whatclick].yPos, room[whatclick].xPos + 335, room[whatclick].yPos + 102); // 방제목 입력

			
			SelectObject(memdc, oldPen);
			DeleteObject(Pen);
			SelectObject(memdc, GetStockObject(WHITE_BRUSH));
		}
		
	}

	if (RoomCreateWindow) //방만들기 눌렀을 때
	{
		hFont = CreateFont(30, 0, 0, 0, 0, 0, 0, 0, HANGEUL_CHARSET, 0, 0, 0, VARIABLE_PITCH | FF_ROMAN, TEXT("함초롱바탕"));
		hOldFont = (HFONT)SelectObject(memdc, hFont);
		SetBkMode(memdc, TRANSPARENT);

		DrawBitmap(memdc, memdc2, bmp_createwindow, 240, 170, 544, 408);

		Rectangle(memdc, 370, 300, 720, 340); // 방제목 입력
		Rectangle(memdc, 370, 343, 720, 383); // 비밀번호 입력

		Pen = CreatePen(PS_SOLID, 4, RGB(0, 0, 0));
		oldPen = (HPEN)SelectObject(memdc, Pen);

		SelectObject(memdc, GetStockObject(NULL_BRUSH));

		if (RoomCreateChat == RNAME)
			Rectangle(memdc, 370, 300, 720, 340); // 선택된 입력박스
		else if (RoomCreateChat == RPASSWORD)
			Rectangle(memdc, 370, 343, 720, 383); // 선택된 입력박스


		SelectObject(memdc, oldPen);
		DeleteObject(Pen);
		SelectObject(memdc, GetStockObject(WHITE_BRUSH));


		Rectangle(memdc, 730, 343, 770, 383); // 비밀번호 체크박스
		Rectangle(memdc, 520, 397, 550, 427); //데스매치 체크박스
		Rectangle(memdc, 670, 397, 700, 427); //점령전 체크박스

		Pen = CreatePen(PS_SOLID, 3, RGB(255, 0, 0));
		oldPen = (HPEN)SelectObject(memdc, Pen);

		SelectObject(memdc, GetStockObject(NULL_BRUSH));

		if (IsPassword)
			Ellipse(memdc, 735, 348, 765, 378);

		if (GameMode == DEATHMATCH)
			Ellipse(memdc, 522, 399, 548, 425);
		else
			Ellipse(memdc, 672, 399, 698, 425);

		SelectObject(memdc, oldPen);
		DeleteObject(Pen);

		TextOut(memdc, 370, 305, RoomName, wcslen(RoomName));

		for (int i = 0; i < strlen(RoomPassword); ++i)
		{
			TextOut(memdc, 372 + (i * 15), 343, L"*", 1);
		}


		if (!IsPassword)
		{
			Brush = CreateSolidBrush(RGB(128, 128, 128));
			oldBrush = (HBRUSH)SelectObject(memdc, Brush);

			Rectangle(memdc, 372, 343, 718, 383); // 비밀번호 체크박스

			SelectObject(memdc, oldBrush);
			DeleteObject(Brush);
		}

		SelectObject(memdc2, hFont);
		DeleteObject(hFont);
	}




	hFont = CreateFont(15, 0, 0, 0, 0, 0, 0, 0, HANGEUL_CHARSET, 0, 0, 0, VARIABLE_PITCH | FF_ROMAN, TEXT("함초롱바탕"));
	hOldFont = (HFONT)SelectObject(memdc, hFont);

}

void CLobby::DrawBitmap(HDC memdc, HDC memdc2, HBITMAP bitmap, int x, int y, int sizeX, int sizeY)
{
	SelectObject(memdc2, bitmap);
	BitBlt(memdc, x, y, sizeX, sizeY, memdc2, 0, 0, SRCCOPY);
}

