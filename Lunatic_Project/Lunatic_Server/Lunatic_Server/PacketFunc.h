#pragma once

void Send_Packet(int client, void* packet);
// 접속
void SendIDPlayer(int client, int object);
void SendLoginFailed(int client, int object);
void SendRemovePlayerPacket(int client, int object);
// 로비
void SendChatLobby(int client, int object, WCHAR str[MAX_STR_SIZE]);
void SendRoomInfo(int client, int object, int room_number);
void SendJoinFail(int client, int object, int roomstatus);
void SendJoinRoom(int client, int object, int game_id, int roomnumber);
void SendQuickJoin(int client, int object, int game_id, int roomnumber);
void SendQuickJoinFail(int client, int object);

// 방
void SendChatRoom(int client, int object, WCHAR str[MAX_STR_SIZE]);
void SendReadyPacket(int client, int object);
void SendAllReadyPacket(int client, int object);
void SendEnterNewPlayer(int client, int object);

// 인게임
void SendChatGame(int client, int object, WCHAR str[MAX_STR_SIZE]);
void ProcessPacket(int id, unsigned char packet[]);
// (Move)
void SendPutPlayerPacket(int client, int object);
void SendPositionPacket(int client, int object);
void SendColl_CharPacket(int client, int object,int direction);
void SendColl_BuildingPacket(int client, int object, int direction);
void Do_move(int id, unsigned char packet[]);
// (Att)
void SendAttackPacket(int client, int object);
// (Skill)
void SendSkillQPacket(int client, int object);
void SendSkillWPacket(int client, int object);
void SendSkillEPacket(int client, int object);
void SendSkillRPacket(int client, int object);
void SendSkillDone(int client, int object);

// (Hit)
void SendAttackHitPacket(int client, int object, int hitid, int clientID);

// (Die & Respawn)
void SendDiePacket(int client, int object, int clientID, BYTE team);
void SendRespawnPacket(int client, int object);

// (Result)
void SendResultPacket(int client, int object,bool winflag);
void SendEndingResult(int client, int object);

// (Timer_Thread_Func)
void Player_Respawn(int id, int room_number);

// (Lobby Reset)
void LobbyReset(int id, int roomnumber);