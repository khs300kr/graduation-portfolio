#pragma once



#include "resource.h"
#include "Lobby.h"
#include "MainMenu.h"
#include "Room.h"

// server
void ReadPacket(SOCKET sock);
void ProcessPacket(char *ptr);
