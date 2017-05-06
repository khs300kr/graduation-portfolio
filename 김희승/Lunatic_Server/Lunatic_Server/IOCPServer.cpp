#include "IOCPServer.h"

CIOCP_Server::CIOCP_Server()
{
	this->Init_Server();
	// �۾��� ������ ����.

	//for (int i = 0; i < 6; ++i)			// �ھ�4 * 1.5 = 6
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
	// �����ʱ�ȭ
	std::wcout.imbue(std::locale("korean"));
	WSADATA wsadata;
	WSAStartup(MAKEWORD(2, 2), &wsadata);

	// IOCP ����
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
	- ���� ������ ���� Ŭ���̾�Ʈ�� IOCP�� �ѱ�� ����
	- ���� ������ ���鼭,
	�� Accept() ȣ��
	�� �� Ŭ���̾�Ʈ ���� -> Ŭ���̾�Ʈ ���� ����ü ����
	�� IOCP�� ���� ��� (send/recv�� IOCP�� ���� ����)
	�� WSARecv() ȣ�� (Overlapped I/O recv ���¸� �׻� ����)
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
		std::cout << "[TCP����] Ŭ���̾�Ʈ ���� : IP =" << inet_ntoa(ClientAddr.sin_addr) << ", ��Ʈ ��ȣ = " <<
			ntohs(ClientAddr.sin_port) << "\n";
#endif
		// Accet loop ó��.
		int new_id{ -1 };
		for (int i = 0; i < MAX_USER; ++i)
			if (m_Clients[i].m_bConnect == false) { new_id = i; break; }
		if (new_id == -1) { std::cout << "MAX USER : " << MAX_USER << "�� ���� OVERFLOW\n"; closesocket(client_sock); continue; }

		m_Clients[new_id].m_bConnect = true;
		m_Clients[new_id].m_client_socket = client_sock;
		m_Clients[new_id].curr_packet_size = 0;
		m_Clients[new_id].prev_packet_data = 0;
		ZeroMemory(&m_Clients[new_id].m_recv_over, sizeof(m_Clients[new_id].m_recv_over));
		m_Clients[new_id].m_recv_over.m_Event_type = OP_RECV;
		// WSABUF -> IOCP ����
		m_Clients[new_id].m_recv_over.m_Wsabuf.buf =
			reinterpret_cast<CHAR *>(m_Clients[new_id].m_recv_over.m_IOCP_buf);
		m_Clients[new_id].m_recv_over.m_Wsabuf.len = sizeof(m_Clients[new_id].m_recv_over.m_IOCP_buf);
		// �ʱ���ġ
		m_Clients[new_id].m_iX = 4;
		m_Clients[new_id].m_iZ = 4;

		// �񵿱� ����� ����
		DWORD recv_flag = 0;
		CreateIoCompletionPort(reinterpret_cast<HANDLE>(client_sock), m_Handle, new_id, 0);
		WSARecv(client_sock, &m_Clients[new_id].m_recv_over.m_Wsabuf, 1,
			NULL, &recv_flag, &m_Clients[new_id].m_recv_over.m_Over, NULL);

		// ��ġ �ϱ�.
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
		// �񵿱� ����� �Ϸ� ��ٸ���. 
		DWORD io_size;					// �񵿱� ����� �۾��� ����Ʈ
		unsigned long long id;			// Ŭ���̾�Ʈ id
		OverlappedEx *over;				// ������ ����ü �ּ�
										// IOCP �� ����¿Ϸ� ��Ŷ�� ���� �� ���� ����Ѵ�. ����� �Ϸ� ��Ŷ�� IOCP�� ������
										// �ü���� ���� ���� �۾��� ������ ������ üũ�Ѵ�. �� ������ �ھ� �������� ������ ��� ���� �����带
										// �ƿ��� ����� �Ϸ� ��Ŷ�� ó���Ѵ�.
										// (<1> IOCP�ڵ� <2> �񵿱� ����� �۾��� ����Ʈ ���� <3> Ŭ���̾�Ʈ_id <4> OVERLAPPED ����ü�� �ּ� ����)
		BOOL ret = GetQueuedCompletionStatus(m_Handle, &io_size, &id,
			reinterpret_cast<LPWSAOVERLAPPED *>(&over), INFINITE);

		// Error ó��
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

		// Send, Recv ó��
		if (over->m_Event_type == OP_RECV)
		{
			std::cout << "RECV from Client :" << id;
			std::cout << "  IO_SIZE : " << io_size << std::endl;
			unsigned char *buf = m_Clients[id].m_recv_over.m_IOCP_buf;
			unsigned cu_size = m_Clients[id].curr_packet_size;
			unsigned pr_size = m_Clients[id].prev_packet_data;

			while (io_size > 0)
			{
				if (cu_size == 0) cu_size = buf[0];		// ��Ŷ ����� 0 �϶�, 
														// 1. �ٷ� ���� ó���ϴ� ��Ŷ�� ó���� ������.
														// 2. Accept �ϰ� ó�� �����͸� �޴´�.
														// io_size�� ��� 1�̴ϱ�, Ŭ���̾�Ʈ���� ����� �ǹ��ִ� �������̴�.

				if (io_size + pr_size >= cu_size)		// ��Ŷ�� �ϼ� �� �� �ִ�.
				{
					unsigned char packet[MAX_PACKET_SIZE];
					memcpy(packet, m_Clients[id].packet_buf, pr_size);
					memcpy(packet + pr_size, buf, cu_size - pr_size);
					ProcessPacket(static_cast<int>(id), packet);
					io_size -= cu_size - pr_size;
					buf += cu_size - pr_size;
					cu_size = 0; pr_size = 0;
				}
				else									// ��Ŷ�� �ϼ� ��ų �� ����.
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

