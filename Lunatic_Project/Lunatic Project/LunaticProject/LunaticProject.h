#pragma once




#include "resource.h"
#include "Lobby.h"
#include "MainMenu.h"
#include "Room.h"
#include "GameFramework.h"



void SendReadyButton();
int GetMyGame_id();
void EnterRoom();
bool MouseInbox(int left, int top, int right, int bottom, int x, int y);





// server
void ReadPacket(SOCKET sock);
void ProcessPacket(char *ptr);



// Sound

void SoundInit();