#pragma once
#include "stdafx.h"

class CIOCP_Server
{
private:
	SOCKET						m_listenSocket;
	HANDLE						m_Handle;
	std::thread*				m_Accept_thread;
	std::vector<std::thread *>	m_vWorker_threads;
	CLIENT						m_Clients[MAX_USER];
private:
	void Init_Server();
	static DWORD WINAPI Worker_Thread(LPVOID ServerPtr);
	static DWORD WINAPI Accept_Thread(LPVOID ServerPtr);
	void Close_Server();
private:
	void ProcessPacket(int id, unsigned char packet[]);
	void Send_Packet(int client, void* packet);
	void SendPutPlayerPacket(int client, int object);
	void SendPositionPacket(int client, int object);
	void SendRemovePlayerPacket(int client, int object);
	void DisconnectClient(int id);
public:
	CIOCP_Server();
	~CIOCP_Server();
};

