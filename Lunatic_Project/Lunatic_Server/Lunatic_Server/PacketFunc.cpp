#include "stdafx.h"
#include "Global.h"
#include "PacketFunc.h"

void Send_Packet(int client, void* packet)
{
	int packet_size = reinterpret_cast<unsigned char *>(packet)[0];
	int packet_type = reinterpret_cast<unsigned char *>(packet)[1];
	OverlappedEx *over = new OverlappedEx;
	over->m_Event_type = OP_SEND;
	memcpy(over->m_IOCP_buf, packet, packet_size);

	ZeroMemory(&over->m_Over, sizeof(over->m_Over));
	over->m_Wsabuf.buf = reinterpret_cast<CHAR *>(over->m_IOCP_buf);
	over->m_Wsabuf.len = packet_size;

	int ret = WSASend(g_Clients[client].m_client_socket, &over->m_Wsabuf, 1, NULL, 0,
		&over->m_Over, NULL);
	if (ret != 0)
	{
		int err_no = WSAGetLastError();
		if (WSA_IO_PENDING != err_no)
			error_display("Error in SendPacket:", err_no);
	}
	std::cout << "Send Packet [" << packet_type << "] To Client : " << client << std::endl;
}

// 접속
void SendIDPlayer(int client, int object)
{
	sc_packet_id packet;
	packet.size = sizeof(packet);
	packet.type = SC_ID;
	packet.id = object;

	Send_Packet(client, &packet);
}

// 방
void SendReadyPacket(int client, int object)
{
	sc_packet_ready packet;
	packet.size = sizeof(packet);
	packet.type = SC_READY;
	packet.id = object;
	packet.hero_pick = g_Clients[object].m_HeroPick;

	Send_Packet(client, &packet);
}
void SendAllReadyPacket(int client, int object)
{
	sc_packet_allready packet;
	packet.size = sizeof(packet);
	packet.type = SC_ALLREADY;
	packet.id = object;

	Send_Packet(client, &packet);
}

// 인게임
void SendPutPlayerPacket(int client, int object)
{
	sc_packet_put_player packet;
	packet.id = object;
	packet.size = sizeof(packet);
	packet.type = SC_PUT_PLAYER;
	packet.x = g_Clients[object].m_fX;
	packet.y = g_Clients[object].m_fY;
	packet.z = g_Clients[object].m_fZ;

	Send_Packet(client, &packet);
}

void SendPositionPacket(int client, int object)
{
	sc_packet_pos packet;
	packet.id = object;
	packet.size = sizeof(packet);
	packet.type = SC_POS;
	packet.direction = g_Clients[object].m_Direction;
	packet.x = g_Clients[object].m_fX;
	packet.y = g_Clients[object].m_fY;
	packet.z = g_Clients[object].m_fZ;

	Send_Packet(client, &packet);
}

void Do_move(int id, unsigned char packet[])
{
	cs_packet_pos *my_packet = reinterpret_cast<cs_packet_pos*>(packet);
	g_Clients[id].m_fX = my_packet->x;
	g_Clients[id].m_fY = my_packet->y;
	g_Clients[id].m_fZ = my_packet->z;
	for (int i = 0; i < MAX_USER; ++i)
	{
		if (g_Clients[i].m_bConnect == true)
			SendPositionPacket(i, id);
	}
}

void SendRemovePlayerPacket(int client, int object)
{
	sc_packet_remove_player packet;
	packet.id = object;
	packet.size = sizeof(packet);
	packet.type = SC_REMOVE_PLAYER;

	Send_Packet(client, &packet);
}




void ProcessPacket(int id, unsigned char packet[])
{
	switch (packet[1])
	{
	case CS_KEYDOWN_UP:
		g_Clients[id].m_Direction |= DIR_BACK;
		Do_move(id, packet);
		break;
	case CS_KEYDOWN_DOWN:
		g_Clients[id].m_Direction |= DIR_FRONT;
		Do_move(id, packet);
		break;
	case CS_KEYDOWN_LEFT:
		g_Clients[id].m_Direction |= DIR_LEFT;
		Do_move(id, packet);
		break;
	case CS_KEYDOWN_RIGHT:
		g_Clients[id].m_Direction |= DIR_RIGHT;
		Do_move(id, packet);
		break;
	case CS_KEYUP_UP:
		g_Clients[id].m_Direction ^= DIR_BACK;
		Do_move(id, packet);
		break;
	case CS_KEYUP_DOWN:
		g_Clients[id].m_Direction ^= DIR_FRONT;
		Do_move(id, packet);
		break;
	case CS_KEYUP_LEFT:
		g_Clients[id].m_Direction ^= DIR_LEFT;
		Do_move(id, packet);
		break;
	case CS_KEYUP_RIGHT:
		g_Clients[id].m_Direction ^= DIR_RIGHT;
		Do_move(id, packet);
		break;

	case CS_READY:
	{
		++g_ReadyNum;
		cs_packet_ready *my_packet = reinterpret_cast<cs_packet_ready*>(packet);
		g_Clients[id].m_HeroPick = my_packet->hero_pick;

		for (int i = 0; i < MAX_USER; ++i)
		{
			if (g_Clients[i].m_bConnect == true)
				SendReadyPacket(i, id);
		}

		if (g_ReadyNum == g_CCU)
		{
			g_ReadyNum = 0;
			for (int i = 0; i < MAX_USER; ++i)
				if (g_Clients[i].m_bConnect == true) SendAllReadyPacket(i, id);	
		}
		break;
	}
	case CS_LOADCOMPLETE:
	++g_ReadyNum;
	std::cout << "Loading Num : " << g_ReadyNum << std::endl;
	SendPutPlayerPacket(id, id);
	if (g_ReadyNum == g_CCU)	// 나중에 방 인원 수 만큼.
	{
		g_ReadyNum = 0;
		for (int i = 0; i < MAX_USER; ++i)
		{
			if (g_Clients[i].m_bConnect == true)
			{
				if (id != i)
				{
					SendPutPlayerPacket(id, i);
					SendPutPlayerPacket(i, id);
				}
			}
		}// for loop
	}
	break;
	
	default: std::cout << "Unknown Packet Type from Client : " << id << std::endl;
		while (true);
	}

}
