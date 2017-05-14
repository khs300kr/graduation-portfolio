#pragma once

void Send_Packet(int client, void* packet);
// 접속
void SendIDPlayer(int client, int object);

// 방
void SendReadyPacket(int client, int object);
void SendAllReadyPacket(int client, int object);

// 인게임
void SendPutPlayerPacket(int client, int object);
void SendPositionPacket(int client, int object);
void Do_move(int id, unsigned char packet[]);
void SendRemovePlayerPacket(int client, int object);
void ProcessPacket(int id, unsigned char packet[]);
