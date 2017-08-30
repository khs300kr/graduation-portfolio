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
	wcscpy_s(packet.DB_id, g_Clients[object].m_ID);

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
	wcscpy_s(packet.DB_id, g_Clients[object].m_ID);

	Send_Packet(client, &packet);
}

// 방
void SendReadyPacket(int client, int object)
{
	sc_packet_ready packet;
	packet.size = sizeof(packet);
	packet.type = SC_READY;
	packet.id = g_Clients[object].m_GameID;
	packet.serverID = object;
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
	wcscpy_s(packet.DB_id, g_Clients[object].m_ID);

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
	packet.hp = g_Clients[object].m_hp;
	packet.att = g_Clients[object].m_att;

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

void SendColl_CharPacket(int client, int object, int direction)
{
	sc_packet_coll_char packet;
	packet.id = g_Clients[object].m_GameID;
	packet.size = sizeof(packet);
	packet.type = SC_CHAR_COLL;
	packet.direction = direction;
	
	Send_Packet(client, &packet);
}

void SendColl_BuildingPacket(int client, int object, int direction)
{
	sc_packet_coll_building packet;
	packet.id = g_Clients[object].m_GameID;
	packet.size = sizeof(packet);
	packet.type = SC_CHAR_COLL;
	packet.direction = direction;
		
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

void SendAttackHitPacket(int client, int object, int hitid, int clientID)
{
	sc_packet_attack_hit packet;
	packet.id = g_Clients[object].m_GameID;
	packet.size = sizeof(packet);
	packet.type = SC_ATTACK_HIT;
	packet.hp = g_Clients[hitid].m_hp;
	packet.clientid = clientID;

	Send_Packet(client, &packet);
}

void SendDiePacket(int client, int object, int clientID, BYTE team)
{
	sc_char_die packet;
	packet.id = g_Clients[object].m_GameID;
	packet.size = sizeof(packet);
	packet.type = SC_CHAR_DIE;
	packet.clientid = clientID;
	packet.team = team;

	Send_Packet(client, &packet);
}

void SendRespawnPacket(int client, int object)
{
	sc_respawn packet;
	packet.id = g_Clients[object].m_GameID;
	packet.size = sizeof(packet);
	packet.type = SC_RESPAWN;
	packet.x = g_Clients[object].m_fX;
	packet.z = g_Clients[object].m_fZ;
	packet.hp = g_Clients[object].m_hp;

	Send_Packet(client, &packet);
}

void SendResultPacket(int client, int object, bool winflag)
{
	sc_result packet;
	packet.id = g_Clients[object].m_GameID;
	packet.size = sizeof(packet);
	packet.type = SC_RESULT;
	packet.IsAWin = winflag;

	Send_Packet(client, &packet);
}

void SendEndingResult(int client, int object)
{
	sc_packet_endingresult packet;
	packet.id = g_Clients[object].m_GameID;
	packet.size = sizeof(packet);
	packet.type = SC_ENDINGRESULT;
	packet.killcount = g_Clients[object].m_killcount;
	packet.deathcount = g_Clients[object].m_deathcount;
	packet.deal = g_Clients[object].m_deal;
	packet.hit = g_Clients[object].m_hit;

	Send_Packet(client, &packet);
}

void Player_Respawn(int id, int room_number)
{
	switch (g_Clients[id].m_HeroPick)
	{
	case BABARIAN: g_Clients[id].m_hp = BABARIAN_HP;	break;
	case KNIGHT:   g_Clients[id].m_hp = KNIGHT_HP;		break;
	case SWORDMAN: g_Clients[id].m_hp = SWORDMAN_HP;	break;
	case MAGICIAN: g_Clients[id].m_hp = MAGICIAN_HP;	break;
	case ARCHER:   g_Clients[id].m_hp = ARCHER_HP;		break;
	case HEALER:   g_Clients[id].m_hp = HEALER_HP;		break;
	case WITCH:	   g_Clients[id].m_hp = WITCH_HP;		break;
	}
	g_Clients[id].m_fX = 0.f;
	g_Clients[id].m_fZ = -500.f;

	for (auto& d : g_Room[room_number].m_GameID_list)
		SendRespawnPacket(d, id);
}

void LobbyReset(int id, int roomnumber)
{
	// 클라이언트 초기화.
	
	g_Clients[id].m_killcount = 0;
	g_Clients[id].m_deathcount = 0;
	g_Clients[id].m_deal = 0;
	g_Clients[id].m_hit = 0;

	g_Clients[id].m_room_number = 0;
	g_Clients[id].m_Direction = 0;
	g_Clients[id].m_bLobby = true;
	g_Clients[id].m_GameID = 0;


	// Room 초기화.
	g_Room[roomnumber].m_GameID_list.erase(id);

	if (g_Room[roomnumber].m_GameID_list.empty())
	{
		memset(g_Room[roomnumber].m_title, 0, MAX_ROOMTITLE_SIZE);
		memset(g_Room[roomnumber].m_password, 0, MAX_ROOMPASSWORD_SIZE);
		g_Room[roomnumber].m_readycount = 0;
		g_Room[roomnumber].m_loadcount = 0;
		g_Room[roomnumber].A_killcount = 0;
		g_Room[roomnumber].B_killcount = 0;
		g_Room[roomnumber].m_RoomStatus = ROOM_EMPTY;
		g_Room[roomnumber].m_GameID_list.clear();				// 보관된 client ID.
		g_Room[roomnumber].m_AcceptLoading_list.clear();
		g_Room[roomnumber].respawnposition = 0.f;
		// 접속 && 로비 - 방정보 send
		for (int i = 0; i < MAX_USER; ++i)
		{
			if ((g_Clients[i].m_bConnect == true) && (g_Clients[i].m_bLobby == true))
			{
				SendRoomInfo(i, id, g_RoomNum);
			}
		}
	}

	for (int i = 0; i < MAX_ROOM; ++i)
	{
		for (int j = 0; j < MAX_USER; ++j) {
			if (g_Clients[j].m_bConnect == true && g_Clients[j].m_bLobby == true)
				SendRoomInfo(j, id, i);
		}
	}
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
		wcscpy_s(g_Clients[id].m_ID, my_packet->id);
		g_Clients[id].vl_lock.lock();
		Client_Login(my_packet->id, my_packet->password, id);
		g_Clients[id].vl_lock.unlock();
		for (int i = 0; i < MAX_ROOM; ++i)
		{
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
		for (int i = 0; i < MAX_ROOM; ++i)
		{
			if (g_Room[i].m_GameID_list.empty())
			{
				g_RoomNum = i;
				break;
			}
		}

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
		switch (my_packet->hero_pick)
		{
		case BABARIAN:	g_Clients[id].m_hp = BABARIAN_HP; g_Clients[id].m_att = 150;	
			g_Clients[id].m_weakatt = 100; g_Clients[id].m_skillatt = 200; break;
		case KNIGHT:	g_Clients[id].m_hp = KNIGHT_HP; g_Clients[id].m_att = 150;		
			g_Clients[id].m_weakatt = 100; g_Clients[id].m_skillatt = 200; break;
		case SWORDMAN:	g_Clients[id].m_hp = SWORDMAN_HP; g_Clients[id].m_att = 200;	
			g_Clients[id].m_weakatt = 130; g_Clients[id].m_skillatt = 200; break;
		case MAGICIAN:	g_Clients[id].m_hp = MAGICIAN_HP; g_Clients[id].m_att = 180;
			g_Clients[id].m_weakatt = 110; g_Clients[id].m_skillatt = 200; break;
		case ARCHER:	g_Clients[id].m_hp = ARCHER_HP; g_Clients[id].m_att = 170;
			g_Clients[id].m_weakatt = 120; g_Clients[id].m_skillatt = 200; break;
		case HEALER:	g_Clients[id].m_hp = HEALER_HP; g_Clients[id].m_att = 200;
			g_Clients[id].m_weakatt = 130; g_Clients[id].m_skillatt = 200; break;
		case WITCH:		g_Clients[id].m_hp = WITCH_HP; g_Clients[id].m_att = 200;
			g_Clients[id].m_weakatt = 130; g_Clients[id].m_skillatt = 200; break;
		}
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

		// 초기 좌표 초기화.
		if (g_Clients[id].m_GameID & 1) // B Team
		{
			g_Clients[id].m_fX = -30.f + g_Room[room_number].respawnposition;
			g_Clients[id].m_fY = 0.f;
			g_Clients[id].m_fZ = -500.f;
		}
		else // A Team
		{
			g_Clients[id].m_fX = -30.f + g_Room[room_number].respawnposition;
			g_Clients[id].m_fY = 0.f;
			g_Clients[id].m_fZ = -480.f;
		}
		if (g_Room[room_number].m_loadcount % 2 == 0)
		{
			g_Room[room_number].respawnposition += 20.f;
		}

		// 모든 플레이가 로딩완료시 초기위치 전송.
		if (g_Room[room_number].m_loadcount == g_Room[room_number].m_GameID_list.size())
		{
			g_Room[room_number].m_loadcount = 0;
			for (auto& id : g_Room[room_number].m_AcceptLoading_list) {
				for (auto& d : g_Room[room_number].m_GameID_list) 
				{
					SendPutPlayerPacket(d,id);
	
				}
			}
		}
		break;
	}

	// 인 게임
	// (Move)
	case CS_KEYDOWN_UP:		 g_Clients[id].m_Direction |= DIR_BACK;		Do_move(id, packet);	break;
	case CS_KEYDOWN_DOWN:	 g_Clients[id].m_Direction |= DIR_FRONT;	Do_move(id, packet);	break;
	case CS_KEYDOWN_LEFT:	 g_Clients[id].m_Direction |= DIR_LEFT;		Do_move(id, packet);	break;
	case CS_KEYDOWN_RIGHT:	 g_Clients[id].m_Direction |= DIR_RIGHT;	Do_move(id, packet);	break;
	case CS_KEYUP_UP:		 if(g_Clients[id].m_Direction != 0) g_Clients[id].m_Direction ^= DIR_BACK;		Do_move(id, packet);	break;
	case CS_KEYUP_DOWN:		 if(g_Clients[id].m_Direction != 0) g_Clients[id].m_Direction ^= DIR_FRONT;	Do_move(id, packet);	break;
	case CS_KEYUP_LEFT:		 if(g_Clients[id].m_Direction != 0) g_Clients[id].m_Direction ^= DIR_LEFT;		Do_move(id, packet);	break;
	case CS_KEYUP_RIGHT:	 if(g_Clients[id].m_Direction != 0) g_Clients[id].m_Direction ^= DIR_RIGHT;	Do_move(id, packet);	break;
	case CS_CHAR_COLL: 
	{
		cs_packet_char_coll *my_packet = reinterpret_cast<cs_packet_char_coll*>(packet);
	
		int room_number = packet[2];	// roomnumber
		for (auto& d : g_Room[room_number].m_GameID_list)
			SendColl_CharPacket(d, id, my_packet->direction);
		break;
	}
	case CS_BUILDING_COLL:
	{
		cs_packet_building_coll *my_packet = reinterpret_cast<cs_packet_building_coll*>(packet);

		int room_number = packet[2];	// roomnumber
		for (auto& d : g_Room[room_number].m_GameID_list)
			SendColl_CharPacket(d, id, my_packet->direction);
		break;
	}

	//case CS_POS_UPDATE: Do_move(id, packet); break;
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
	// (Hit)
	case CS_ATTACK_HIT:
	{
		cs_packet_attack_hit *my_packet = reinterpret_cast<cs_packet_attack_hit*>(packet);
		int room_number = packet[2];	// roomnumber
		g_Clients[my_packet->hitID].m_hp -= g_Clients[id].m_att; // Hp 감소.

		g_Clients[my_packet->hitID].m_hit += g_Clients[id].m_att;	// 피해량 계산
		g_Clients[id].m_deal += g_Clients[id].m_att;				// 딜량 계산

		if (g_Clients[my_packet->hitID].m_Direction != 0)	// 이동 중일때.
		{
			g_Clients[my_packet->hitID].m_Direction = 0;	// 이동을 멈추어라.
			// 위치 갱신 코드 필요.
		}

		 // 캐릭터 사망시.
		if (g_Clients[my_packet->hitID].m_hp <= 0) 
		{ 
			g_Clients[my_packet->hitID].m_hp = 0; // 클라이언트용 서버 ID.
			++g_Clients[id].m_killcount;								// 데스 계산
			++g_Clients[my_packet->hitID].m_deathcount;					// 킬 계산
			g_Clients[my_packet->hitID].m_room_number = room_number;
		
			// KILL수 늘리기. ( 목표 Kill 수 도달 시 승리 및 패배 패킷 전송 ).
			if (my_packet->clientID & 1) // odd = B_TEAM
			{
				++g_Room[room_number].B_killcount;
				for (auto& d : g_Room[room_number].m_GameID_list)
					SendDiePacket(d, id, my_packet->clientID, B_TEAM);
			}
			else // even = A_TEAM
			{
				++g_Room[room_number].A_killcount;
				for (auto& d : g_Room[room_number].m_GameID_list)
					SendDiePacket(d, id, my_packet->clientID, A_TEAM);
			}

			// 승리조건 + 초기화 작업(not yet)
			if (g_Room[room_number].A_killcount == RESULTDEATH)
			{
				for (auto& d : g_Room[room_number].m_GameID_list)
					SendResultPacket(d, id,false);
				for (auto& id : g_Room[room_number].m_AcceptLoading_list) 
					for (auto& d : g_Room[room_number].m_GameID_list)
					{
						SendEndingResult(d, id);
					}
			}
			else if (g_Room[room_number].B_killcount == RESULTDEATH)
			{
				for (auto& d : g_Room[room_number].m_GameID_list)
					SendResultPacket(d, id,true);
				for (auto& id : g_Room[room_number].m_AcceptLoading_list)
					for (auto& d : g_Room[room_number].m_GameID_list)
					{
						SendEndingResult(d, id);
					}
			}

			else
			{
				// Timer Setting(Respawn)
				Timer_Event t = { my_packet->hitID ,high_resolution_clock::now() + 3s, P_RESPAWN, room_number };
				timerqueue_lock.lock();
				timer_queue.push(t);
				timerqueue_lock.unlock();
			}
			
			
		}
		// 캐릭터 사망시.



		for (auto& d : g_Room[room_number].m_GameID_list)
			SendAttackHitPacket(d, id, my_packet->hitID, my_packet->clientID);
		break;
	}
	case CS_WEAKATTACK_HIT:
	{
		cs_packet_attack_hit *my_packet = reinterpret_cast<cs_packet_attack_hit*>(packet);
		int room_number = packet[2];	// roomnumber

		g_Clients[my_packet->hitID].m_hp -= g_Clients[id].m_weakatt; // Hp 감소.

		g_Clients[my_packet->hitID].m_hit += g_Clients[id].m_weakatt;	// 피해량 계산
		g_Clients[id].m_deal += g_Clients[id].m_weakatt;				// 딜량 계산

		if (g_Clients[my_packet->hitID].m_Direction != 0)	// 이동 중일때.
		{
			g_Clients[my_packet->hitID].m_Direction = 0;	// 이동을 멈추어라.
															// 위치 갱신 코드 필요.
		}

		// 캐릭터 사망시.
		if (g_Clients[my_packet->hitID].m_hp <= 0)
		{
			g_Clients[my_packet->hitID].m_hp = 0; // 클라이언트용 서버 ID.
			++g_Clients[id].m_killcount;								// 데스 계산
			++g_Clients[my_packet->hitID].m_deathcount;					// 킬 계산
			g_Clients[my_packet->hitID].m_room_number = room_number;


			// KILL수 늘리기. ( 목표 Kill 수 도달 시 승리 및 패배 패킷 전송 ).
			if (my_packet->clientID & 1) // odd = B_TEAM
			{
				++g_Room[room_number].B_killcount;
				for (auto& d : g_Room[room_number].m_GameID_list)
					SendDiePacket(d, id, my_packet->clientID, B_TEAM);
			}
			else // even = A_TEAM
			{
				++g_Room[room_number].A_killcount;
				for (auto& d : g_Room[room_number].m_GameID_list)
					SendDiePacket(d, id, my_packet->clientID, A_TEAM);
			}

			// 승리조건 + 초기화 작업(not yet)
			if (g_Room[room_number].A_killcount == RESULTDEATH)
			{
				for (auto& d : g_Room[room_number].m_GameID_list)
					SendResultPacket(d, id, false);
				for (auto& id : g_Room[room_number].m_AcceptLoading_list)
					for (auto& d : g_Room[room_number].m_GameID_list)
					{
						SendEndingResult(d, id);
					}
			}
			else if (g_Room[room_number].B_killcount == RESULTDEATH)
			{
				for (auto& d : g_Room[room_number].m_GameID_list)
					SendResultPacket(d, id, true);
				for (auto& id : g_Room[room_number].m_AcceptLoading_list)
					for (auto& d : g_Room[room_number].m_GameID_list)
					{
						SendEndingResult(d, id);
					}
			}

			else
			{
				// Timer Setting(Respawn)
				Timer_Event t = { my_packet->hitID ,high_resolution_clock::now() + 3s, P_RESPAWN, room_number };
				timerqueue_lock.lock();
				timer_queue.push(t);
				timerqueue_lock.unlock();
			}


		}
		// 캐릭터 사망시.



		for (auto& d : g_Room[room_number].m_GameID_list)
			SendAttackHitPacket(d, id, my_packet->hitID, my_packet->clientID);
		break;
	}
	case CS_SKILL_HIT:
	{
		cs_packet_attack_hit *my_packet = reinterpret_cast<cs_packet_attack_hit*>(packet);
		int room_number = packet[2];	// roomnumber

		g_Clients[my_packet->hitID].m_hp -= g_Clients[id].m_skillatt; // Hp 감소.

		g_Clients[my_packet->hitID].m_hit += g_Clients[id].m_skillatt;	// 피해량 계산
		g_Clients[id].m_deal += g_Clients[id].m_skillatt;				// 딜량 계산

		if (g_Clients[my_packet->hitID].m_Direction != 0)	// 이동 중일때.
		{
			g_Clients[my_packet->hitID].m_Direction = 0;	// 이동을 멈추어라.
															// 위치 갱신 코드 필요.
		}

		// 캐릭터 사망시.
		if (g_Clients[my_packet->hitID].m_hp <= 0)
		{
			g_Clients[my_packet->hitID].m_hp = 0; // 클라이언트용 서버 ID.
			++g_Clients[id].m_killcount;								// 데스 계산
			++g_Clients[my_packet->hitID].m_deathcount;					// 킬 계산
			g_Clients[my_packet->hitID].m_room_number = room_number;


			// KILL수 늘리기. ( 목표 Kill 수 도달 시 승리 및 패배 패킷 전송 ).
			if (my_packet->clientID & 1) // odd = B_TEAM
			{
				++g_Room[room_number].B_killcount;
				for (auto& d : g_Room[room_number].m_GameID_list)
					SendDiePacket(d, id, my_packet->clientID, B_TEAM);
			}
			else // even = A_TEAM
			{
				++g_Room[room_number].A_killcount;
				for (auto& d : g_Room[room_number].m_GameID_list)
					SendDiePacket(d, id, my_packet->clientID, A_TEAM);
			}

			// 승리조건 + 초기화 작업(not yet)
			if (g_Room[room_number].A_killcount == RESULTDEATH)
			{
				for (auto& d : g_Room[room_number].m_GameID_list)
					SendResultPacket(d, id, false);
				for (auto& id : g_Room[room_number].m_AcceptLoading_list)
					for (auto& d : g_Room[room_number].m_GameID_list)
					{
						SendEndingResult(d, id);
					}
			}
			else if (g_Room[room_number].B_killcount == RESULTDEATH)
			{
				for (auto& d : g_Room[room_number].m_GameID_list)
					SendResultPacket(d, id, true);
				for (auto& id : g_Room[room_number].m_AcceptLoading_list)
					for (auto& d : g_Room[room_number].m_GameID_list)
					{
						SendEndingResult(d, id);
					}
			}

			else
			{
				// Timer Setting(Respawn)
				Timer_Event t = { my_packet->hitID ,high_resolution_clock::now() + 3s, P_RESPAWN, room_number };
				timerqueue_lock.lock();
				timer_queue.push(t);
				timerqueue_lock.unlock();
			}


		}
		for (auto& d : g_Room[room_number].m_GameID_list)
			SendAttackHitPacket(d, id, my_packet->hitID, my_packet->clientID);
		break;
	}
	case CS_RESULTCONFIRM:
	{
		cs_packet_result_confirm *my_packet = reinterpret_cast<cs_packet_result_confirm*>(packet);
		int room_number = packet[2];	// roomnumber
	
		LobbyReset(id, room_number);
		break;
	}
	default: std::cout << "Unknown Packet Type from Client : " << id << std::endl;
		while (true);
	}

}
