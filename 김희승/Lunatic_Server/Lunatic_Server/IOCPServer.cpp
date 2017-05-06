#include "IOCPServer.h"

CIOCP_Server::CIOCP_Server()
{
	this->Init_Server();
	// 작업자 스레드 생성.

	//for (int i = 0; i < 6; ++i)			// 코어4 * 1.5 = 6
	//	m_vWorker_threads.push_back(new std::thread{ this->Worker_Thread });
	//std::thread accept_thread = MAKE_THREAD(CIOCP_Server, Worker_Thread);
	//accept_thread.join();
	//for (auto d : m_vWorker_threads)
	//{
	//	d->join();
	//	delete d;
	//}
}

CIOCP_Server::~CIOCP_Server()
{
}

void CIOCP_Server::Init_Server()
{
	// 윈속초기화
	std::wcout.imbue(std::locale("korean"));
	WSADATA wsadata;
	WSAStartup(MAKEWORD(2, 2), &wsadata);

	// IOCP 생성
	m_Handle = CreateIoCompletionPort(INVALID_HANDLE_VALUE, 0, NULL, 0);

	// socket()
	m_listenSocket = WSASocket(AF_INET, SOCK_STREAM, IPPROTO_TCP, NULL, 0, WSA_FLAG_OVERLAPPED);

	// bind()
	SOCKADDR_IN ServerAddr;
	ZeroMemory(&ServerAddr, sizeof(SOCKADDR_IN));
	ServerAddr.sin_family = AF_INET;
	ServerAddr.sin_port = htons(MY_SERVER_PORT);
	ServerAddr.sin_addr.s_addr = INADDR_ANY;
	::bind(m_listenSocket, reinterpret_cast<sockaddr *>(&ServerAddr), sizeof(ServerAddr));

	listen(m_listenSocket, 5);
	for (int i = 0; i < MAX_USER; ++i)
	{
		m_Clients[i].m_bConnect = false;
	}
}

DWORD WINAPI CIOCP_Server::Accept_Thread(LPVOID ServerPtr)
	/*
	<Accept Thread>
	- 새로 접속해 오는 클라이언트를 IOCP로 넘기는 역할
	- 무한 루프를 돌면서,
	ㅇ Accept() 호출
	ㅇ 새 클라이언트 접속 -> 클라이언트 정보 구조체 생성
	ㅇ IOCP에 소켓 등록 (send/recv가 IOCP를 통해 수행)
	ㅇ WSARecv() 호출 (Overlapped I/O recv 상태를 항상 유지)
	*/
{
	while (true)
	{
		SOCKADDR_IN ClientAddr;
		ZeroMemory(&ClientAddr, sizeof(SOCKADDR_IN));
		ClientAddr.sin_family = AF_INET;
		ClientAddr.sin_port = htons(MY_SERVER_PORT);
		ClientAddr.sin_addr.s_addr = INADDR_ANY;
		int addr_size = sizeof(ClientAddr);
		SOCKET client_sock = WSAAccept(m_listenSocket,
			reinterpret_cast<sockaddr *>(&ClientAddr), &addr_size, NULL, NULL);
#ifdef _DEBUG
		std::cout << "[TCP서버] 클라이언트 접속 : IP =" << inet_ntoa(ClientAddr.sin_addr) << ", 포트 번호 = " <<
			ntohs(ClientAddr.sin_port) << "\n";
#endif
		// Accet loop 처리.
		int new_id{ -1 };
		for (int i = 0; i < MAX_USER; ++i)
			if (m_Clients[i].m_bConnect == false) { new_id = i; break; }
		if (new_id == -1) { std::cout << "MAX USER : " << MAX_USER << "명 동접 OVERFLOW\n"; closesocket(client_sock); continue; }

		m_Clients[new_id].m_bConnect = true;
		m_Clients[new_id].m_client_socket = client_sock;
		m_Clients[new_id].curr_packet_size = 0;
		m_Clients[new_id].prev_packet_data = 0;
		ZeroMemory(&m_Clients[new_id].m_recv_over, sizeof(m_Clients[new_id].m_recv_over));
		m_Clients[new_id].m_recv_over.m_Event_type = OP_RECV;
		// WSABUF -> IOCP 버퍼
		m_Clients[new_id].m_recv_over.m_Wsabuf.buf =
			reinterpret_cast<CHAR *>(m_Clients[new_id].m_recv_over.m_IOCP_buf);
		m_Clients[new_id].m_recv_over.m_Wsabuf.len = sizeof(m_Clients[new_id].m_recv_over.m_IOCP_buf);
		// 초기위치
		m_Clients[new_id].m_iX = 4;
		m_Clients[new_id].m_iZ = 4;

		// 비동기 입출력 시작
		DWORD recv_flag = 0;
		CreateIoCompletionPort(reinterpret_cast<HANDLE>(client_sock), m_Handle, new_id, 0);
		WSARecv(client_sock, &m_Clients[new_id].m_recv_over.m_Wsabuf, 1,
			NULL, &recv_flag, &m_Clients[new_id].m_recv_over.m_Over, NULL);

		// 위치 하기.
		SendPutPlayerPacket(new_id, new_id);

		for (int i = 0; i < MAX_USER; ++i)
		{
			if (m_Clients[i].m_bConnect == true)
			{
				if (new_id != i)
				{
					SendPutPlayerPacket(new_id, i);
					SendPutPlayerPacket(i, new_id);
				}
			}
		}// for loop
	}
}

DWORD WINAPI CIOCP_Server::Worker_Thread(LPVOID ServerPtr)
{
	while (true)
	{
		// 비동기 입출력 완료 기다리기. 
		DWORD io_size;					// 비동기 입출력 작업의 바이트
		unsigned long long id;			// 클라이언트 id
		OverlappedEx *over;				// 오버랩 구조체 주소
										// IOCP 에 입출력완료 패킷이 들어올 때 까지 대기한다. 입출력 완료 패킷이 IOCP에 들어오면
										// 운영체제는 실행 중인 작업자 스레드 개수를 체크한다. 이 개수가 코어 개수보다 작으면 대기 중인 스레드를
										// 꺠워서 입출력 완료 패킷을 처리한다.
										// (<1> IOCP핸들 <2> 비동기 입출력 작업의 바이트 저장 <3> 클라이언트_id <4> OVERLAPPED 구조체의 주소 저장)
		BOOL ret = GetQueuedCompletionStatus(m_Handle, &io_size, &id,
			reinterpret_cast<LPWSAOVERLAPPED *>(&over), INFINITE);

		// Error 처리
		std::cout << "GQCS :";
		if (ret == FALSE)
		{
			int err_no = WSAGetLastError();
			if (err_no == 64)
				DisconnectClient(id);
			else error_display("GQCS : ", WSAGetLastError());
		}
		if (io_size == 0)
		{
			DisconnectClient(id);
			continue;
		}

		// Send, Recv 처리
		if (over->m_Event_type == OP_RECV)
		{
			std::cout << "RECV from Client :" << id;
			std::cout << "  IO_SIZE : " << io_size << std::endl;
			unsigned char *buf = m_Clients[id].m_recv_over.m_IOCP_buf;
			unsigned cu_size = m_Clients[id].curr_packet_size;
			unsigned pr_size = m_Clients[id].prev_packet_data;

			while (io_size > 0)
			{
				if (cu_size == 0) cu_size = buf[0];		// 패킷 사이즈가 0 일때, 
														// 1. 바로 전에 처리하던 패킷이 처리가 끝났다.
														// 2. Accept 하고 처음 데이터를 받는다.
														// io_size는 적어도 1이니까, 클라이언트에서 날라온 의미있는 데이터이다.

				if (io_size + pr_size >= cu_size)		// 패킷을 완성 할 수 있다.
				{
					unsigned char packet[MAX_PACKET_SIZE];
					memcpy(packet, m_Clients[id].packet_buf, pr_size);
					memcpy(packet + pr_size, buf, cu_size - pr_size);
					ProcessPacket(static_cast<int>(id), packet);
					io_size -= cu_size - pr_size;
					buf += cu_size - pr_size;
					cu_size = 0; pr_size = 0;
				}
				else									// 패킷을 완성 시킬 수 없다.
				{
					memcpy(m_Clients[id].packet_buf + pr_size, buf, io_size);
					pr_size += io_size;
					io_size = 0;
				}
			}
			m_Clients[id].curr_packet_size = cu_size;
			m_Clients[id].prev_packet_data = pr_size;

			DWORD recv_flag = 0;
			WSARecv(m_Clients[id].m_client_socket,
				&m_Clients[id].m_recv_over.m_Wsabuf, 1,
				NULL, &recv_flag, &m_Clients[id].m_recv_over.m_Over, NULL);
		}//OP_RECV

		else if (over->m_Event_type == OP_SEND)
		{
			if (over->m_Wsabuf.len != io_size)
			{
				std::cout << "Send Incomplete Error!\n";
				closesocket(m_Clients[id].m_client_socket);
				m_Clients[id].m_bConnect = false;
			}
			delete over;
		}//OP_SEND

		else
		{
			std::cout << "Unknown GQCS event!\n";
			while (true);
		}//UNKNOWN_GQCS

	}//Worker_Loop
}


void CIOCP_Server::Close_Server()
{
	closesocket(m_listenSocket);
	CloseHandle(m_Handle);
	WSACleanup();
}

void CIOCP_Server::ProcessPacket(int id, unsigned char packet[])
{
	switch (packet[1])
	{
	case CS_UP: if (m_Clients[id].m_iZ > 0) m_Clients[id].m_iZ--; break;
	case CS_DOWN: if (m_Clients[id].m_iZ < BOARD_HEIGHT - 1) m_Clients[id].m_iZ++; break;
	case CS_LEFT: if (m_Clients[id].m_iX > 0) m_Clients[id].m_iX--; break;
	case CS_RIGHT: if (m_Clients[id].m_iX < BOARD_WIDTH - 1) m_Clients[id].m_iX++; break;
	default: std::cout << "Unknown Packet Type from Client : " << id << std::endl;
		while (true);
	}
	for (int i = 0; i < MAX_USER; ++i)
	{
		if (m_Clients[i].m_bConnect == true)
			SendPositionPacket(i, id);
	}
}

void CIOCP_Server::Send_Packet(int client, void * packet)
{
	int packet_size = reinterpret_cast<unsigned char *>(packet)[0];
	int packet_type = reinterpret_cast<unsigned char *>(packet)[1];
	OverlappedEx *over = new OverlappedEx;
	over->m_Event_type = OP_SEND;
	memcpy(over->m_IOCP_buf, packet, packet_size);

	ZeroMemory(&over->m_Over, sizeof(over->m_Over));
	over->m_Wsabuf.buf = reinterpret_cast<CHAR *>(over->m_IOCP_buf);
	over->m_Wsabuf.len = packet_size;

	int ret = WSASend(m_Clients[client].m_client_socket, &over->m_Wsabuf, 1, NULL, 0,
		&over->m_Over, NULL);
	if (ret != 0)
	{
		int err_no = WSAGetLastError();
		if (WSA_IO_PENDING != err_no)
			error_display("Error in SendPacket:", err_no);
	}
	std::cout << "Send Packet [" << packet_type << "] To Client : " << client << std::endl;
}

void CIOCP_Server::SendPutPlayerPacket(int client, int object)
{
	sc_packet_put_player packet;
	packet.id = object;
	packet.size = sizeof(packet);
	packet.type = SC_PUT_PLAYER;
	packet.x = m_Clients[object].m_iX;
	packet.y = m_Clients[object].m_iZ;

	Send_Packet(client, &packet);
}

void CIOCP_Server::SendPositionPacket(int client, int object)
{
	sc_packet_pos packet;
	packet.id = object;
	packet.size = sizeof(packet);
	packet.type = SC_POS;
	packet.x = m_Clients[object].m_iX;
	packet.y = m_Clients[object].m_iZ;

	Send_Packet(client, &packet);
}

void CIOCP_Server::SendRemovePlayerPacket(int client, int object)
{
	sc_packet_pos packet;
	packet.id = object;
	packet.size = sizeof(packet);
	packet.type = SC_REMOVE_PLAYER;

	Send_Packet(client, &packet);
}

void CIOCP_Server::DisconnectClient(int id)
{
	closesocket(m_Clients[id].m_client_socket);
	m_Clients[id].m_bConnect = false;

	for (int i = 0; i < MAX_USER; ++i)
	{
		if (m_Clients[i].m_bConnect == true)
			SendRemovePlayerPacket(i, id);
	}
}

