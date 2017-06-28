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
#if _DEBUG
	std::cout << "Send Packet [" << packet_type << "] To Client : " << client << std::endl;
#endif
}

// 접속
void SendIDPlayer(int client, int object)
{
	sc_packet_id packet;
	packet.size = sizeof(packet);
	packet.type = SC_ID;

	Send_Packet(client, &packet);
}

void SendLoginFailed(int client, int object)
{
	sc_packet_id packet;
	packet.size = sizeof(packet);
	packet.type = SC_LOGIN_FAILED;

	Send_Packet(client, &packet);
}

// 로비
void SendChatLobby(int client, int object, WCHAR str[MAX_STR_SIZE])
{
	sc_packet_chat packet;
	packet.size = sizeof(packet);
	packet.type = SC_LOBBY_CHAT;
	wcscpy_s(packet.message, str);
	strcpy(packet.DB_id, g_Clients[object].m_ID);

	Send_Packet(client, &packet);
}

void SendRoomInfo(int client, int object, int room_number)
{
	sc_packet_roominfo packet;
	packet.size = sizeof(packet);
	packet.type = SC_ROOM_INFO;
	wcscpy_s(packet.roomtitle, g_Room[room_number].m_title);
	packet.mode = g_Room[room_number].m_mode;
	packet.roomstatus = g_Room[room_number].m_RoomStatus;
	packet.room_number = room_number;
	packet.playercount = g_Room[room_number].m_GameID_list.size();
	packet.m_private = g_Room[room_number].m_private;
	
	Send_Packet(client, &packet);
}

void SendJoinRoom(int client, int object, int game_id, int roomnumber)
{
	sc_packet_join_room packet;
	packet.size = sizeof(packet);
	packet.type = SC_JOIN_ROOM;
	packet.roomnumber = roomnumber;
	packet.game_id = game_id;
	wcscpy_s(packet.roomtitle, g_Room[roomnumber].m_title);
	packet.mode = g_Room[roomnumber].m_mode;
	packet.roomstatus = g_Room[roomnumber].m_RoomStatus;
	packet.playercount = g_Room[roomnumber].m_GameID_list.size();
	packet.m_private = g_Room[roomnumber].m_private;

	Send_Packet(client, &packet);
}

void SendQuickJoin(int client, int object, int game_id, int roomnumber)
{
	sc_packet_quick_join packet;
	packet.size = sizeof(packet);
	packet.type = SC_QUICK_JOIN;
	packet.roomnumber = roomnumber;
	packet.game_id = game_id;
	wcscpy_s(packet.roomtitle, g_Room[roomnumber].m_title);
	packet.mode = g_Room[roomnumber].m_mode;
	packet.roomstatus = g_Room[roomnumber].m_RoomStatus;
	packet.playercount = g_Room[roomnumber].m_GameID_list.size();
	packet.m_private = g_Room[roomnumber].m_private;
	Send_Packet(client, &packet);
}

void SendQuickJoinFail(int client, int object)
{
	sc_packet_join_fail packet;
	packet.size = sizeof(packet);
	packet.type = SC_QUICK_JOIN_FAIL;

	Send_Packet(client, &packet);
}

void SendJoinFail(int client, int object,int roomstatus)
{
	sc_packet_join_fail packet;
	packet.size = sizeof(packet);
	roomstatus == FULL ? (packet.type == SC_JOIN_FAIL_FULL) : (roomstatus == INGAME ? packet.type = SC_JOIN_FAIL_INGAME : packet.type = SC_JOIN_FAIL_EMPTY);

	Send_Packet(client, &packet);
}

void SendChatRoom(int client, int object, WCHAR str[MAX_STR_SIZE])
{
	sc_packet_chat packet;
	packet.size = sizeof(packet);
	packet.type = SC_ROOM_CHAT;
	wcscpy_s(packet.message, str);
	strcpy(packet.DB_id, g_Clients[object].m_ID);

	Send_Packet(client, &packet);
}

// 방
void SendReadyPacket(int client, int object)
{
	sc_packet_ready packet;
	packet.size = sizeof(packet);
	packet.type = SC_READY;
	packet.id = g_Clients[object].m_GameID;
	packet.hero_pick = g_Clients[object].m_HeroPick;

	Send_Packet(client, &packet);
}
void SendAllReadyPacket(int client, int object)
{
	sc_packet_allready packet;
	packet.size = sizeof(packet);
	packet.type = SC_ALLREADY;
	packet.id = g_Clients[object].m_GameID;

	Send_Packet(client, &packet);
}

void SendEnterNewPlayer(int client, int object)
{
	sc_packet_enter_newplayer packet;
	packet.id = g_Clients[object].m_GameID;
	packet.size = sizeof(packet);
	packet.type = SC_ENTER_NEWPLAYER;
	strcpy_s(packet.DB_id, g_Clients[object].m_ID);

	Send_Packet(client, &packet);
}

// 인게임
void SendPutPlayerPacket(int client, int object)
{
	sc_packet_put_player packet;
	packet.id = g_Clients[object].m_GameID;
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
	packet.id = g_Clients[object].m_GameID;
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
	int room_number = my_packet->roomnumber;

	g_Clients[id].m_fX = my_packet->x;
	g_Clients[id].m_fY = my_packet->y;
	g_Clients[id].m_fZ = my_packet->z;

	for (auto& d : g_Room[room_number].m_GameID_list)
	{
		SendPositionPacket(d, id);
	}

}

void SendAttackPacket(int client, int object)
{
	sc_packet_attack packet;
	packet.id = g_Clients[object].m_GameID;
	packet.size = sizeof(packet);
	packet.type = SC_ATTACK;

	Send_Packet(client, &packet);
}

void SendSkillQPacket(int client, int object)
{
	sc_packet_skillQ packet;
	packet.id = g_Clients[object].m_GameID;
	packet.size = sizeof(packet);
	packet.type = SC_SKILL_Q;

	Send_Packet(client, &packet);
}
void SendSkillWPacket(int client, int object)
{
	sc_packet_skillW packet;
	packet.id = g_Clients[object].m_GameID;
	packet.size = sizeof(packet);
	packet.type = SC_SKILL_W;

	Send_Packet(client, &packet);
}
void SendSkillEPacket(int client, int object)
{
	sc_packet_skillE packet;
	packet.id = g_Clients[object].m_GameID;
	packet.size = sizeof(packet);
	packet.type = SC_SKILL_E;

	Send_Packet(client, &packet);
}
void SendSkillRPacket(int client, int object)
{
	sc_packet_skillR packet;
	packet.id = g_Clients[object].m_GameID;
	packet.size = sizeof(packet);
	packet.type = SC_SKILL_R;

	Send_Packet(client, &packet);
}

void SendSkillDone(int client, int object)
{
	sc_packet_remove_player packet;
	packet.id = g_Clients[object].m_GameID;
	packet.size = sizeof(packet);
	packet.type = SC_SKILL_DONE;

	Send_Packet(client, &packet);
}

bool Circile_Coll(int from, int to)
{
	// Sphere Coll
	float Temp_radius = 30.f;
	float deltaX = g_Clients[from].m_fX - g_Clients[to].m_fX;
	float deltaY = g_Clients[from].m_fY - g_Clients[to].m_fY;
	float len = sqrtf((deltaX * deltaX) + (deltaY * deltaY));
	return (len < (Temp_radius * 2.f));

	//return ((abs(g_Clients[from].m_iX - g_Clients[to].m_iX) < 9) &&
	//	(abs(g_Clients[from].m_iY - g_Clients[to].m_iY) < 9));
}

void SendRemovePlayerPacket(int client, int object)
{
	sc_packet_remove_player packet;
	packet.id = g_Clients[object].m_GameID;
	packet.size = sizeof(packet);
	packet.type = SC_REMOVE_PLAYER;

	Send_Packet(client, &packet);
}

void SendChatGame(int client, int object, WCHAR str[MAX_STR_SIZE])
{
}

void ProcessPacket(int id, unsigned char packet[])
{
	switch (packet[1])
	{
	// 메인 메뉴
	case CS_LOGIN:
	{
		cs_packet_login* my_packet = reinterpret_cast<cs_packet_login*>(packet);
		strcpy_s(g_Clients[id].m_ID, my_packet->id);
		g_Clients[id].vl_lock.lock();
		Client_Login(my_packet->id, my_packet->password, id);
		g_Clients[id].vl_lock.unlock();
		for (int i = 0; i < MAX_ROOM; ++i)
		{
			if (g_Room[i].m_GameID_list.size() == 0) break;	// 방에 아무도 없으면 탈출
			for (int j = 0; j < MAX_USER; ++j) {
				if(g_Clients[j].m_bConnect == true && g_Clients[j].m_bLobby == true)
					SendRoomInfo(j, id, i);
			}
		}
		break;
	}
	// 로비
	case CS_LOBBY_CHAT:
	{
		cs_packet_lobbychat *my_packet = reinterpret_cast<cs_packet_lobbychat*>(packet);
			for (int i = 0; i < MAX_USER; ++i) {
			if (g_Clients[i].m_bConnect == true && g_Clients[i].m_bLobby == true)
				SendChatLobby(i, id, my_packet->message);
		}break;
	}
	case CS_MAKE_ROOM:
	{
		if (g_RoomNum > MAX_ROOM) { cout << "방 갯수 초과\n"; break; }
		
		// 룸 정보 서버 저장.
		cs_packet_makeroom* my_packet = reinterpret_cast<cs_packet_makeroom*>(packet);
		g_Clients[id].m_GameID = 0;			// 최초 방장 아이디는 0
		g_Clients[id].m_bLobby = false;		// 로비 탈출.

		g_Clients[id].vl_lock.lock();		//////////////////// LOCK
		wcscpy(g_Room[g_RoomNum].m_title, my_packet->roomtitle);
		strcpy_s(g_Room[g_RoomNum].m_password, my_packet->password);

		if (g_Room[g_RoomNum].m_password[0] == '\0')
		{
			g_Room[g_RoomNum].m_private = false;
		}
		else
		{
			g_Room[g_RoomNum].m_private = true;
		}

		g_Room[g_RoomNum].m_mode = my_packet->mode;
		g_Room[g_RoomNum].m_RoomStatus = ROOM_JOINABLE;
		g_Room[g_RoomNum].m_GameID_list.insert(id);
		g_Clients[id].vl_lock.unlock();		//////////////////// UNLOCK

		SendJoinRoom(id, id, g_Clients[id].m_GameID, g_RoomNum);


		// 접속 && 로비 - 방정보 send
		for (int i = 0; i < MAX_USER; ++i) 
		{
			if ((g_Clients[i].m_bConnect == true) && (g_Clients[i].m_bLobby == true))
			{
				SendRoomInfo(i, id, g_RoomNum);
			}
		}

		++g_RoomNum; // 우선 순서대로 룸 생성.
		break;
	}
	case CS_JOIN_ROOM:
	{
		cs_packet_joinroom* my_packet = reinterpret_cast<cs_packet_joinroom*>(packet);
		int roomnumber = my_packet->roomnumber;
		// 입장 fail 처리.
		if (g_Room[roomnumber].m_RoomStatus == FULL) { SendJoinFail(id, id, FULL); break; }
		if (g_Room[roomnumber].m_RoomStatus == INGAME){ SendJoinFail(id, id, INGAME); break; }
		if (g_Room[roomnumber].m_RoomStatus == ROOM_EMPTY) { SendJoinFail(id, id, ROOM_EMPTY); break; }
		// 해당 방 레디 카운트 초기화.
		g_Room[roomnumber].m_readycount = 0;

		g_Clients[id].m_bLobby = false;
		
		g_Clients[id].vl_lock.lock();	////////////////////LOCK
		g_Room[roomnumber].m_GameID_list.insert(id);
		g_Clients[id].vl_lock.unlock();	////////////////////UNLOCK
		g_Clients[id].m_GameID = g_Room[roomnumber].m_GameID_list.size() - 1;

		if (g_Room[roomnumber].m_GameID_list.size() == 8)
			g_Room[roomnumber].m_RoomStatus = FULL;

		SendJoinRoom(id, id, g_Clients[id].m_GameID, roomnumber );
		for (auto&d : g_Room[roomnumber].m_GameID_list)
		{
			if (d != id) {
				SendEnterNewPlayer(d, id); 
				SendEnterNewPlayer(id, d); 
			}
		}



		// 접속 && 로비 - 방정보 send
		for (int i = 0; i < MAX_USER; ++i)
		{
			if ((g_Clients[i].m_bConnect == true) && (g_Clients[i].m_bLobby == true))
			{
				SendRoomInfo(i, id, roomnumber);
			}
		}
		break;
	}

	case CS_QUICK_JOIN:
	{
		int new_room{ -1 };
		for (int i = 0; i < MAX_ROOM; ++i)
		{
			if (g_Room[i].m_RoomStatus == ROOM_JOINABLE) {
				new_room = i; break;}
		}
		if (new_room == -1) { SendQuickJoinFail(id, id); break; }

		// 해당 방 레디 카운트 초기화.
		g_Room[new_room].m_readycount = 0;

		g_Clients[id].m_bLobby = false;
		g_Clients[id].vl_lock.lock();	////////////////////LOCK
		g_Room[new_room].m_GameID_list.insert(id);
		g_Clients[id].vl_lock.unlock();	////////////////////UNLOCK
		g_Clients[id].m_GameID = g_Room[new_room].m_GameID_list.size() - 1;

		if (g_Room[new_room].m_GameID_list.size() == 8)
			g_Room[new_room].m_RoomStatus = FULL;

		SendQuickJoin(id, id, g_Clients[id].m_GameID, new_room);
		for (auto&d : g_Room[new_room].m_GameID_list)
		{
			if (d != id) {
				SendEnterNewPlayer(d, id);
				SendEnterNewPlayer(id, d);
			}
		}

		// 접속 && 로비 - 방정보 send
		for (int i = 0; i < MAX_USER; ++i)
		{
			if ((g_Clients[i].m_bConnect == true) && (g_Clients[i].m_bLobby == true))
			{
				SendRoomInfo(i, id, new_room);
			}
		}
		break;
	}
	// 방
	case CS_ROOM_CHAT:
	{
		cs_packet_roomchat *my_packet = reinterpret_cast<cs_packet_roomchat*>(packet);
		int room_number = my_packet->roomnumber;
		for (auto& d : g_Room[room_number].m_GameID_list)
			SendChatRoom(d, id, my_packet->message);
		break;
	}
	case CS_READY:
	{
		cs_packet_ready *my_packet = reinterpret_cast<cs_packet_ready*>(packet);
		int room_number = my_packet->roomnumber;
		g_Clients[id].vl_lock.lock();	////////////////////// LOCK
		g_Clients[id].m_HeroPick = my_packet->hero_pick;
		++g_Room[room_number].m_readycount;
		g_Clients[id].vl_lock.unlock();	////////////////////// UNLOCK
		// Ready 여부 알리기.
		for (auto& d : g_Room[room_number].m_GameID_list)
		{
			SendReadyPacket(d, id);
		}


		// 모든 플레이가 Ready 일시 게임시작 알림.
		if (g_Room[room_number].m_readycount == g_Room[room_number].m_GameID_list.size())
		{
			g_Room[room_number].m_readycount = 0;
			g_Room[room_number].m_RoomStatus = INGAME;
			Sleep(1000);
			for (auto& d : g_Room[room_number].m_GameID_list) {
				SendAllReadyPacket(d, id);
			}
		}
		break;
	}
	case CS_LOADCOMPLETE:
	{
		cs_packet_LoadingComplete *my_packet = reinterpret_cast<cs_packet_LoadingComplete*>(packet);
		int room_number = my_packet->roomnumber;
		++g_Room[room_number].m_loadcount;
		
		g_Clients[id].vl_lock.lock();	/////////////////////////////// LOCK
		g_Room[room_number].m_AcceptLoading_list.insert(id);
		g_Clients[id].vl_lock.unlock();	/////////////////////////////// UNLOCK
		// 모든 플레이가 로딩완료시 초기위치 전송.
		if (g_Room[room_number].m_loadcount == g_Room[room_number].m_GameID_list.size())
		{
			g_Room[room_number].m_loadcount = 0;
			for (auto& id : g_Room[room_number].m_AcceptLoading_list) {
				for (auto& d : g_Room[room_number].m_GameID_list) 
				{
					//cout << "d : " << d << " id : " << id << endl;
					SendPutPlayerPacket(d,id);
	
				}
			}
		}
		break;
	}

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
	{
		int room_number = packet[2];	// roomnumber
		for (auto& d : g_Room[room_number].m_GameID_list)
			SendAttackPacket(d, id);

		break;
	}
	case CS_SKILL_Q:
	{
		int room_number = packet[2];	// roomnumber
		for (auto& d : g_Room[room_number].m_GameID_list)
			SendSkillQPacket(d, id);

		break;
	}
	case CS_SKILL_W:
	{
		int room_number = packet[2];	// roomnumber
		for (auto& d : g_Room[room_number].m_GameID_list)
			SendSkillWPacket(d, id);

		break;
	}
	case CS_SKILL_E:
	{
		int room_number = packet[2];	// roomnumber
		for (auto& d : g_Room[room_number].m_GameID_list)
			SendSkillEPacket(d, id);

		break;
	}
	case CS_SKILL_R:
	{
		int room_number = packet[2];	// roomnumber
		for (auto& d : g_Room[room_number].m_GameID_list)
			SendSkillRPacket(d, id);

		break;
	}
	case CS_SKILL_DONE:
	{
		int room_number = packet[2];	// roomnumber
		for (auto& d : g_Room[room_number].m_GameID_list)
			SendSkillDone(d, id);

		break;
	}
	default: std::cout << "Unknown Packet Type from Client : " << id << std::endl;
		while (true);
	}

}
