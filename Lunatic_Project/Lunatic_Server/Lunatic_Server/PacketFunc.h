#pragma once

void Send_Packet(int client, void* packet);
// 접속
void SendIDPlayer(int client, int object);

// 방
void SendReadyPacket(int client, int object);
void SendAllReadyPacket(int client, int object);

// 인게임
void ProcessPacket(int id, unsigned char packet[]);
// (Move)
void SendPutPlayerPacket(int client, int object);
void SendPositionPacket(int client, int object);
void Do_move(int id, unsigned char packet[]);
// (Att)
void SendAttackPacket(int client, int object);
// (Skill)
void SendSkillQPacket(int client, int object);
void SendSkillWPacket(int client, int object);
void SendSkillEPacket(int client, int object);
void SendSkillRPacket(int client, int object);



void SendRemovePlayerPacket(int client, int object);
