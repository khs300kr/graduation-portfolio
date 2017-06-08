#include "stdafx.h"
#include "Lobby.h"
#include "LunaticProject.h"


CLobby::CLobby()
{
	// 로비에서 보이는 룸

	for (int i = 0; i < 3; ++i)
	{
		for (int j = 0; j < 2; ++j)
		{
			room[i][j].xPos = 30 + (j * 400); // 방의 X
			room[i][j].yPos = 160 + (i * 120); // 방의 Y
			room[i][j].roomInfo = Empty; // 방 정보
			room[i][j].people = 0; // 유저가 몇명 방에 들어왔는지?
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

	hFont = CreateFont(30, 0, 0, 0, 0, 0, 0, 0, HANGEUL_CHARSET, 0, 0, 0, VARIABLE_PITCH | FF_ROMAN, TEXT("함초롱바탕"));
	hOldFont = (HFONT)SelectObject(memdc, hFont);
	SetBkMode(memdc, TRANSPARENT);

	for (int i = 0; i < 3; ++i)
	{
		for (int j = 0; j < 2; ++j)
		{
			DrawBitmap(memdc, memdc2, bmp_room, room[i][j].xPos, room[i][j].yPos, 335, 102); // 방 박스 출력

			switch (room[i][j].roomInfo)
			{
			case ROOM_EMPTY:
				TextOut(memdc, room[i][j].xPos + 30, room[i][j].yPos + 30, L"생성가능", 4);
				break;
			case ROOM_JOINABLE:
				TextOut(memdc, room[i][j].xPos + 30, room[i][j].yPos + 30, L"입장가능", 4);
				break;
			case FULL:
				TextOut(memdc, room[i][j].xPos + 30, room[i][j].yPos + 30, L"입장불가", 4);
				break;
			case INGAME:
				TextOut(memdc, room[i][j].xPos + 30, room[i][j].yPos + 30, L"게임중", 3);
				break;
			}

			wchar_t s[6];
			wsprintf(s, L"(%d/8)", room[i][j].people);
			TextOut(memdc, room[i][j].xPos + 260, room[i][j].yPos + 68, s, 5); // (0/8) 인원수 출력
		}
	}

	//if (RoomCreateWindow) //방만들기 눌렀을 때
	//{
	//	hFont = CreateFont(30, 0, 0, 0, 0, 0, 0, 0, HANGEUL_CHARSET, 0, 0, 0, VARIABLE_PITCH | FF_ROMAN, TEXT("함초롱바탕"));
	//	hOldFont = (HFONT)SelectObject(memdc, hFont);
	//	SetBkMode(memdc, TRANSPARENT);

	//	DrawBitmap(memdc, memdc2, bmp_createwindow, 240, 170, 544, 408);

	//	Rectangle(memdc, 370, 300, 720, 340); // 방제목 입력
	//	Rectangle(memdc, 370, 343, 720, 383); // 비밀번호 입력

	//	Pen = CreatePen(PS_SOLID, 4, RGB(0, 0, 0));
	//	oldPen = (HPEN)SelectObject(memdc, Pen);

	//	SelectObject(memdc, GetStockObject(NULL_BRUSH));

	//	if (RoomCreateChat == RNAME)
	//		Rectangle(memdc, 370, 300, 720, 340); // 선택된 입력박스
	//	else if (RoomCreateChat == RPASSWORD)
	//		Rectangle(memdc, 370, 343, 720, 383); // 선택된 입력박스


	//	
	//}




	hFont = CreateFont(15, 0, 0, 0, 0, 0, 0, 0, HANGEUL_CHARSET, 0, 0, 0, VARIABLE_PITCH | FF_ROMAN, TEXT("함초롱바탕"));
	hOldFont = (HFONT)SelectObject(memdc, hFont);

}

void CLobby::DrawBitmap(HDC memdc, HDC memdc2, HBITMAP bitmap, int x, int y, int sizeX, int sizeY)
{
	SelectObject(memdc2, bitmap);
	BitBlt(memdc, x, y, sizeX, sizeY, memdc2, 0, 0, SRCCOPY);
}