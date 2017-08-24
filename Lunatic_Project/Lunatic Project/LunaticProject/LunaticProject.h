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



enum
{
	LOBBY_SOUND = 0,
	CLICK_SOUND,
	SWING_SOUND,
	HIT_SOUND,



	SOUND_END
};

// Sound
static FMOD_SYSTEM *g_System;      // �ý��� ������ ����
static FMOD_SOUND *g_Sound[SOUND_END];     // ���� ������ ����
static FMOD_CHANNEL *g_Channel[SOUND_END]; // ä��   ������ ����

void SoundInit();