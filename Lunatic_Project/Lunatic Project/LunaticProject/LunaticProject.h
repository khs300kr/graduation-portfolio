#pragma once

#include "resource.h"

//client
void DrawBitmap(HDC memdc, HDC memdc2, HBITMAP bitmap, int x, int y, int sizeX, int sizeY);
void EnterRoom();

// server
void ReadPacket(SOCKET sock);
void ProcessPacket(char *ptr);

