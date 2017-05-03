#pragma warning(disable:4996)
#include <WinSock2.h>
#include <winsock.h>
#include <Windows.h>
#include <iostream>
#include <thread>
#include <vector>
#include "protocol.h"

//using namespace std;

// IOCP HANDLE (GLOBAL)
HANDLE g_hiocp;
SOCKET g_ssocket;

enum OPTYPE { OP_SEND, OP_RECV };

struct OverlappedEx
{
	WSAOVERLAPPED over;
	WSABUF wsabuf;
	unsigned char IOCP_buf[MAX_BUFF_SIZE];
	OPTYPE event_type;
};

struct CLIENT
{
	int		m_iX;
	int		m_iY;
	bool	m_bConnect;		// 소켓이 사용중인지 아닌지

	SOCKET client_socket;
	OverlappedEx recv_over;
	unsigned char packet_buf[MAX_PACKET_SIZE];
	int prev_packet_data;	// 패킷을 미처 다 완성 못하고 읽어온 크기
	int curr_packet_size;	// 현재 처리 하고 있는 패키 사이즈
};

CLIENT g_clients[MAX_USER];
void error_display(char *msg, int err_no)
{
	WCHAR *lpMsgBuf;
	FormatMessage(
		FORMAT_MESSAGE_ALLOCATE_BUFFER |
		FORMAT_MESSAGE_FROM_SYSTEM,
		NULL, err_no,
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(LPTSTR)&lpMsgBuf, 0, NULL);
	std::cout << ("%s", msg);
	std::cout << L"에러%s\n" << lpMsgBuf << std::endl;
	LocalFree(lpMsgBuf);
	while (true);
}
void Initialize_Server()
{
	_wsetlocale(LC_ALL, L"korean");
	WSADATA	wsadata;
	WSAStartup(MAKEWORD(2, 2), &wsadata);
	
	g_hiocp = CreateIoCompletionPort(INVALID_HANDLE_VALUE, 0, NULL, 0);

	// 비동기 소켓을 만들기 위해 dwFlags 파라미터에 WSA_FLAG_OVERLAPPED 를 넣는다.
	g_ssocket = WSASocket(AF_INET, SOCK_STREAM, IPPROTO_TCP, NULL, 0, WSA_FLAG_OVERLAPPED);
	
	SOCKADDR_IN ServerAddr;
	ZeroMemory(&ServerAddr, sizeof(SOCKADDR_IN));
	ServerAddr.sin_family = AF_INET;
	ServerAddr.sin_port = htons(MY_SERVER_PORT);
	ServerAddr.sin_addr.s_addr = INADDR_ANY;

	bind(g_ssocket, reinterpret_cast<sockaddr*>(&ServerAddr), sizeof(ServerAddr));
	listen(g_ssocket, 5);			// backlog 동접이 10이므로 5.

	// 모든 커넥트는 false로 초기화.
	for (int i = 0; i < MAX_USER; ++i)	g_clients[i].m_bConnect = false;
}
void SendPacket(int ci, void* packet)
{
	int packet_size = reinterpret_cast<unsigned char*>(packet)[0];
	int ptype = reinterpret_cast<unsigned char*>(packet)[1];
	OverlappedEx *over = new OverlappedEx;
	over->event_type = OP_SEND;
	memcpy(over->IOCP_buf,packet,packet_size);
	ZeroMemory(&over->over, sizeof(over->over));
	over->wsabuf.buf = reinterpret_cast<CHAR *>(over->IOCP_buf);
	over->wsabuf.len = packet_size;
	int ret = WSASend(g_clients[ci].client_socket, &over->wsabuf, 1, NULL, 0,
		&over->over, NULL);
	if (ret != 0)
	{
		int err_no = WSAGetLastError();
		if (WSA_IO_PENDING != err_no)
			error_display("Error in SendPacket :", err_no);
	}
	std::cout << "Send Packet [" <<ptype << "] To Client : " << ci << std::endl;
}
void SendPutPlayerPacket(int client, int object)
{
	sc_packet_put_player packet;
	packet.id = object;
	packet.size = sizeof(packet);
	packet.type = SC_PUT_PLAYER;
	packet.x = g_clients[object].m_iX;
	packet.y = g_clients[object].m_iY;

	SendPacket(client, &packet);
}
void SendPositionPacket(int client, int object)
{
	sc_packet_pos packet;
	packet.id = object;
	packet.size = sizeof(packet);
	packet.type = SC_POS;
	packet.x = g_clients[object].m_iX;
	packet.y = g_clients[object].m_iY;

	SendPacket(client, &packet);
}
void ProcessPacket(int ci, unsigned char packet[])
{
	switch (packet[1])
	{
	case CS_UP: if (g_clients[ci].m_iY > 0) { g_clients[ci].m_iY--; break; }
	case CS_DOWN: if (g_clients[ci].m_iY < BOARD_HEIGHT - 1) { g_clients[ci].m_iY++; break; }
	case CS_LEFT: if (g_clients[ci].m_iX > 0) { g_clients[ci].m_iX--; break; }
	case CS_RIGHT: if (g_clients[ci].m_iX > BOARD_WIDTH - 1) { g_clients[ci].m_iX++; break; }
	default: std::cout << "unknown Packet Type from Client : " << ci << std::endl;
		while (true);
	}
	SendPositionPacket(ci, ci);
}
void Worker_Thread()
{
	while (true)
	{
		DWORD io_size{};
		unsigned long long ci{};
		OverlappedEx *over;
		BOOL ret = GetQueuedCompletionStatus(g_hiocp, &io_size, &ci, reinterpret_cast<LPWSAOVERLAPPED*>(&over), INFINITE);
		if (ret == FALSE)
		{
			std::cout << "Error in GQCS\n";	while (true);
		}
		if (io_size == 0)
		{
			closesocket(g_clients[ci].client_socket);
			g_clients[ci].m_bConnect = false;
			continue;
		}
		std::cout << "GQCS : ";
		if (over->event_type == OP_RECV)
		{
			std::cout << "RECV from Client " << ci;
			std::cout << "    IO_SIZE : " << io_size;
			unsigned char* buf = g_clients[ci].recv_over.IOCP_buf;
			unsigned psize = g_clients[ci].curr_packet_size;
			unsigned prsize = g_clients[ci].prev_packet_data;

			// 데이터 처리
			while (io_size > 0)
			{
				if (psize == 0) psize = buf[0];		// 패킷 사이즈가 0 이면, 바로 전에 처리하던 패킷이 처리가 끝났고 또는 accept 하고 처음 데이터를 받는다.
													// io_size는 적어도 1이니까, 클라이언트에서 날라온 의미있는 데이터이다.
				if (io_size + prsize >= psize)
				{
					// 지금 패킷 완성 가능
					unsigned char packet[MAX_PACKET_SIZE];
					memcpy(packet, g_clients[ci].packet_buf, sizeof(prsize));
					memcpy(packet + prsize, buf, psize - prsize);
					ProcessPacket(ci, packet);
					io_size -= psize - prsize;
					buf += psize - prsize;
					psize = 0; prsize = 0;
				}
				else
				{
					memcpy(g_clients[ci].packet_buf + prsize, buf, io_size);
					prsize += io_size;
					io_size = 0;
				}

			} 
			
			g_clients[ci].curr_packet_size = psize;
			g_clients[ci].prev_packet_data = prsize;
			
			DWORD recv_flag{};
			WSARecv(g_clients[ci].client_socket, &g_clients[ci].recv_over.wsabuf, 1,
				NULL, &recv_flag, &g_clients[ci].recv_over.over, NULL);
		}
		else if (over->event_type == OP_SEND)
		{
			if (io_size != over->wsabuf.len)
			{
				std::cout << "Send Incomplete Error\n";
				closesocket(g_clients[ci].client_socket);
				g_clients[ci].m_bConnect = false;
			}
			delete over;

		}
		else
		{
			std::cout << "Unknown GQCS event!\n";
			while (true);
		}
	}
}
void Accept_Thread()
{
	while (true)
	{
		SOCKADDR_IN ClientAddr;
		ZeroMemory(&ClientAddr, sizeof(SOCKADDR_IN));
		ClientAddr.sin_family = AF_INET;
		ClientAddr.sin_port = htons(MY_SERVER_PORT);
		ClientAddr.sin_addr.s_addr = INADDR_ANY;
		int add_size = sizeof(ClientAddr);

		SOCKET new_client = WSAAccept(g_ssocket, reinterpret_cast<sockaddr*>(&ClientAddr), &add_size, NULL, NULL);

		int new_id = -1;
		for (int i = 0; i < MAX_USER; ++i)
			if (g_clients[i].m_bConnect == false) new_id = i; break;
		
		if (new_id == -1) { std::cout << "MAX USER OVERFLOW!\n" << std::endl; closesocket(new_client); continue; }

		g_clients[new_id].m_bConnect = true;
		g_clients[new_id].client_socket = new_client;
		g_clients[new_id].curr_packet_size = 0;
		g_clients[new_id].prev_packet_data = 0;
		ZeroMemory(&g_clients[new_id].recv_over, sizeof(g_clients[new_id].recv_over));
		g_clients[new_id].recv_over.event_type = OP_RECV;
		g_clients[new_id].recv_over.wsabuf.buf = reinterpret_cast<CHAR*>(g_clients[new_id].recv_over.IOCP_buf);
		g_clients[new_id].recv_over.wsabuf.len = sizeof(g_clients[new_id].recv_over.IOCP_buf);
		g_clients[new_id].m_iX = 4;							// 처음 로그인 위치
		g_clients[new_id].m_iY = 4;							// 처음 로그인 위치(가운데 실행)

		DWORD recv_flag{};
		CreateIoCompletionPort(reinterpret_cast<HANDLE>(new_client), g_hiocp, new_id, 0);
		WSARecv(new_client, &g_clients[new_id].recv_over.wsabuf, 1,
			NULL, &recv_flag, &g_clients[new_id].recv_over.over, NULL);
		SendPutPlayerPacket(new_id, new_id);
	}
}
void Shutdown_Server()
{
	closesocket(g_ssocket);
	CloseHandle(g_hiocp);
	WSACleanup();
}

//void main()
//{
//	vector<std::thread*> worker_threads;
//
//	Initialize_Server();
//	for (int i = 0; i < 6; ++i)			// 쿼드코어 pc = 6개
//		worker_threads.push_back(new thread{ Worker_Thread });
//	std::thread accept_thread{ Accept_Thread };
//	accept_thread.join();
//	for (auto pth : worker_threads)
//	{
//		pth->join();
//		delete pth;
//	}
//	Shutdown_Server();
//}

int main()
{
	Initialize_Server();

	std::vector<std::thread *> worker_threads;

	for (int i = 0; i < 6; ++i) {
		worker_threads.push_back(new std::thread{ Worker_Thread });
	}
	std::thread accept_thread{ Accept_Thread };

	accept_thread.join();

	for (auto pth : worker_threads) {
		pth->join();
		delete pth;
	}

	Shutdown_Server(); // 접속이 끊겼을때
}