#include "stdafx.h"
#include "Global.h"
#include "PacketFunc.h"
#include "DataBaseFunc.h"

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
void SendRoomInfo(int client, int object)
{
	sc_packet_showroom packet;
	packet.size = sizeof(packet);
	packet.type = SC_SHOW_ROOM;
	packet.id = object;
	
	Send_Packet(client, &packet);
}
void SendIDPlayer(int client, int object)
{
	sc_packet_id packet;
	packet.size = sizeof(packet);
	packet.type = SC_ID;
	packet.id = object;

	Send_Packet(client, &packet);
}

void SendLoginFailed(int client, int object)
{
	sc_packet_id packet;
	packet.size = sizeof(packet);
	packet.id = object;
	packet.type = SC_LOGIN_FAILED;

	Send_Packet(client, &packet);
}

// 로비
void SendChatPacket(int client, int object, WCHAR str[MAX_STR_SIZE])
{
	sc_packet_chat packet;
	packet.size = sizeof(packet);
	packet.type = SC_LOBBY_CHAT;
	packet.id = object;
	wcscpy_s(packet.message, str);

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

void SendAttackPacket(int client, int object)
{
	sc_packet_attack packet;
	packet.id = object;
	packet.size = sizeof(packet);
	packet.type = SC_ATTACK;

	Send_Packet(client, &packet);
}

void SendSkillQPacket(int client, int object)
{
	sc_packet_skillQ packet;
	packet.id = object;
	packet.size = sizeof(packet);
	packet.type = SC_SKILL_Q;

	Send_Packet(client, &packet);
}
void SendSkillWPacket(int client, int object)
{
	sc_packet_skillW packet;
	packet.id = object;
	packet.size = sizeof(packet);
	packet.type = SC_SKILL_W;

	Send_Packet(client, &packet);
}
void SendSkillEPacket(int client, int object)
{
	sc_packet_skillE packet;
	packet.id = object;
	packet.size = sizeof(packet);
	packet.type = SC_SKILL_E;

	Send_Packet(client, &packet);
}
void SendSkillRPacket(int client, int object)
{
	sc_packet_skillR packet;
	packet.id = object;
	packet.size = sizeof(packet);
	packet.type = SC_SKILL_R;

	Send_Packet(client, &packet);
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
	// 메인 메뉴
	case CS_REGISTER:
	{
		break;
	}
	case CS_LOGIN:
	{
		cs_packet_login* my_packet = reinterpret_cast<cs_packet_login*>(packet);
		cout << my_packet->id << endl;
		cout << my_packet->password << endl;
		g_Clients[id].vl_lock.lock();
		Client_Login(my_packet->id, my_packet->password, id);
		g_Clients[id].vl_lock.unlock();
		break;
	}
	// 로비
	case CS_LOBBY_CHAT:
	{
		cs_packet_chat* my_packet = reinterpret_cast<cs_packet_chat*>(packet);
		for (int i = 0; i < MAX_USER; ++i) {
			if (g_Clients[i].m_bLobby == true)
				SendChatPacket(i, id, my_packet->message);
		}break;
	}
	case CS_MAKE_ROOM:
	{
		g_Clients[id].m_RoomID = 0;		// 최초 방장.
		g_Clients[id].m_bLobby = false;

		cs_packet_makeroom* my_packet = reinterpret_cast<cs_packet_makeroom*>(packet);
		wcscpy(g_Room[g_RoomNum].m_title, my_packet->roomtitle);
		strcpy_s(g_Room[g_RoomNum].m_password, my_packet->password);
		g_Room[g_RoomNum].m_mode = my_packet->mode;

		wcout << my_packet->roomtitle << endl;
		cout << my_packet->password << endl;
		cout << (short)my_packet->mode << endl;

		++g_RoomNum;
		break;
	}

	// 방
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
		//SendPutPlayerPacket(id, id);

			for (int i = 0; i < MAX_USER; ++i)
			{
				if (g_Clients[i].m_bConnect == true)
				{
				/*	if (i != id)
					{*/
						//SendPutPlayerPacket(id, i);
						SendPutPlayerPacket(i, id);
					//}
				}
			}// for loop
		break;
	// 인 게임
	// (Move)
	case CS_KEYDOWN_UP:		g_Clients[id].m_Direction |= DIR_BACK;	Do_move(id, packet);	break;
	case CS_KEYDOWN_DOWN:	g_Clients[id].m_Direction |= DIR_FRONT;	Do_move(id, packet);	break;
	case CS_KEYDOWN_LEFT:	g_Clients[id].m_Direction |= DIR_LEFT;	Do_move(id, packet);	break;
	case CS_KEYDOWN_RIGHT:	g_Clients[id].m_Direction |= DIR_RIGHT;	Do_move(id, packet);	break;
	case CS_KEYUP_UP:		g_Clients[id].m_Direction ^= DIR_BACK;	Do_move(id, packet);	break;
	case CS_KEYUP_DOWN:		g_Clients[id].m_Direction ^= DIR_FRONT;	Do_move(id, packet);	break;
	case CS_KEYUP_LEFT:		g_Clients[id].m_Direction ^= DIR_LEFT;	Do_move(id, packet);	break;
	case CS_KEYUP_RIGHT:	g_Clients[id].m_Direction ^= DIR_RIGHT;	Do_move(id, packet);	break;
	// (Att)
	case CS_ATTACK:			
		std::cout << "Attack\n";
		for (int i = 0; i < MAX_USER; ++i){
			if (g_Clients[i].m_bConnect == true)
				SendAttackPacket(i, id);
		}break;
	case CS_SKILL_Q:
		std::cout << "Q\n";
		for (int i = 0; i < MAX_USER; ++i) {
			if (g_Clients[i].m_bConnect == true)
				SendSkillQPacket(i, id);
		}break;
	case CS_SKILL_W:
		std::cout << "W\n";
		for (int i = 0; i < MAX_USER; ++i) {
			if (g_Clients[i].m_bConnect == true)
				SendSkillWPacket(i, id);
		}break;
	case CS_SKILL_E:
		std::cout << "E\n";
		for (int i = 0; i < MAX_USER; ++i) {
			if (g_Clients[i].m_bConnect == true)
				SendSkillEPacket(i, id);
		}break;
	case CS_SKILL_R:
		std::cout << "R\n";
		for (int i = 0; i < MAX_USER; ++i) {
			if (g_Clients[i].m_bConnect == true)
				SendSkillRPacket(i, id);
		}break;
	default: std::cout << "Unknown Packet Type from Client : " << id << std::endl;
		while (true);
	}

}
