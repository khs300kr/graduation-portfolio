#pragma warning(disable:4996)
#pragma comment(lib,"Ws2_32.lib")

#include <iostream>
#include <WinSock2.h>
#include <WS2tcpip.h> // getaddrinfo()
#include <thread>	  // C+11 multithread
#include <string>
#include <vector>

#define IP_ADDR "127.0.0.1"		// loop back
#define SERVER_PORT "8000"
#define BUFSIZE 512
#define MAX_CLIENT 1000			// 동접 1000
#define NOTYET_CONNECT -1

using namespace std;

typedef struct tagClientType
{
	int		m_id;
	SOCKET	m_socket;
	tagClientType()
		: m_id(-1), m_socket(INVALID_SOCKET) {}
	~tagClientType() {}

}CLIENT_TYPE;

typedef struct tagClientAddr
{
	SOCKADDR_IN m_addr;
	int			m_iLen;
	tagClientAddr() 
	{ 	m_iLen = sizeof(m_addr); }
}CLIENT_ADDR;

int recvn(SOCKET S, char *buf, int len, int flags)
{
	int received{};
	char* ptr = buf;
	int left = len;	// BUFSIZE

	while (left > 0)
	{
		received = recv(S, ptr, left, flags);
		if (received == SOCKET_ERROR) return SOCKET_ERROR;
		else if (received == 0) break;
		cout << "[Received] : " << received << endl;
		left -= received;
		ptr += received;
	}
	return (len - left);
}


void process_client(CLIENT_TYPE& new_client, vector<CLIENT_TYPE>& vClient, thread& thread)
{
	string strMsg{};
	char buf[BUFSIZE]{};

	while (true)
	{
		memset(buf, 0, BUFSIZE);
		if (new_client.m_socket != INVALID_SOCKET)
		{
			int retval = recv(new_client.m_socket, buf, BUFSIZE, 0);

			if (retval == SOCKET_ERROR)
			{
				strMsg = "Client[" + to_string(new_client.m_id) + "]" + " 끊김";
				cout << strMsg << endl;

				closesocket(new_client.m_socket);
				vClient[new_client.m_id].m_socket = INVALID_SOCKET;

				// Broadcast the disconnection meesage to the other clients
				for (auto&d : vClient)
				{
					if (d.m_socket != INVALID_SOCKET)
						retval = send(d.m_socket, strMsg.c_str(), (int)strMsg.size(), 0);
				}
				thread.detach();
				break;
			}
			else
			{
				if (buf != NULL)
					strMsg = "Client[" + to_string(new_client.m_id) + "]" + ": " + buf;
				cout << strMsg << endl;
				// Broadcast the disconnection meesage to the other clients
				for (auto&d : vClient)
				{
					if (d.m_socket != INVALID_SOCKET)
						retval = send(d.m_socket, strMsg.c_str(), (int)strMsg.size(), 0);
				}
			}
		} 
	} // end while
}

void main()
{
	// Winsock Initializing
	cout << "Initializing Winsock..." << endl;
	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0) { cout << "< Winsock Intializing failed >" << endl; exit(1);}

	// Setup hints
	cout << "Setting up Server ( 주소 설정, 서버소켓 생성 )..." << endl;
	addrinfo hints;
	ZeroMemory(&hints, sizeof(hints));
	hints.ai_flags = AI_PASSIVE;		// The socket address will be used in a call to the bind function.(msdn)
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;

	// Create server socket
	addrinfo*	server_addrinfo = NULL;
	if (getaddrinfo(IP_ADDR, SERVER_PORT, &hints, &server_addrinfo) != 0) { cout << "< getaddrinfo failed >" << endl; exit(1); }
	SOCKET server_socket = socket(server_addrinfo->ai_family, server_addrinfo->ai_socktype, server_addrinfo->ai_protocol);
	if (server_socket == INVALID_SOCKET) { cout << "server_socket is invalid" << endl; exit(1); }

	// Setup socket options
	bool optval = true;
	setsockopt(server_socket, SOL_SOCKET, SO_REUSEADDR, (char*)&optval, sizeof(optval));	// 소켓코드가 처리하는 옵션 - (프로토콜 독립적 성격) - 재사용
	setsockopt(server_socket, IPPROTO_TCP, TCP_NODELAY, (char*)&optval, sizeof(optval));	// 프로토콜구현 코드가 처리하는 옵션 - Reduce response time but increase traffic

	cout << "Binding socket..." << endl;
	if (::bind(server_socket, server_addrinfo->ai_addr, (int)server_addrinfo->ai_addrlen) == SOCKET_ERROR) { cout << "bind() Error" << endl; }

	cout << "Listening..." << endl;
	if (listen(server_socket, SOMAXCONN) == SOCKET_ERROR) { cout << "listen() Error" << endl; }

	// 통신변수
	vector<CLIENT_TYPE> vClient(MAX_CLIENT);		// key-value? trying map?
	thread client_thread[MAX_CLIENT];
	CLIENT_ADDR client_addr;
	int		iIndex{};
	int		iClient_Number{};
	string	strMsg{};

	while (true)
	{
		SOCKET sTemp = accept(server_socket, (SOCKADDR*)&client_addr.m_addr, &client_addr.m_iLen);
		if (sTemp == INVALID_SOCKET) continue;
		cout << " [ Accept - IP : " << inet_ntoa(client_addr.m_addr.sin_addr) << ", Port : " << ntohs(client_addr.m_addr.sin_port) << " ] " << endl;
		iClient_Number = 0; // 동접수 초기화

		// 다음 클라이언트를 위해 템프 id 생성?
		iIndex = NOTYET_CONNECT;
		for (int i = 0; i < MAX_CLIENT; ++i)
		{
			if (vClient[i].m_socket == INVALID_SOCKET && iIndex == NOTYET_CONNECT)
			{
				vClient[i].m_socket = sTemp;
				vClient[i].m_id = i;
				iIndex = i;
			}
			
			if (vClient[i].m_socket != INVALID_SOCKET)
				++iClient_Number;							
		}
		cout << iClient_Number << endl;
	

		if (iIndex == NOTYET_CONNECT)		// Meaning Server is Full.
		{
			strMsg = "Server is Full";
			send(sTemp, strMsg.c_str(), (int)strMsg.size(), 0);
			cout << strMsg << endl;
		}

		else
		{
			// id를 클라이언트에게 보낸다?
			cout << "Client[" << vClient[iIndex].m_id << "]" << " 접속" << endl;
			strMsg = to_string(vClient[iIndex].m_id);
			// ID 전송.
			send(vClient[iIndex].m_socket, strMsg.c_str(), (int)strMsg.size(), 0);

			// 해당 클라이언트를 위한 스레드 프로세스를 만든다.
			client_thread[iIndex] = thread(
				process_client,
				std::ref(vClient[iIndex]),		// client_type
				std::ref(vClient),				// vector
				std::ref(client_thread[iIndex]));	// thread array		
		}
	} // end while

	// 서버(listening)소켓 종료
	closesocket(server_socket);

	// 스레드 , 클라이언트 소켓 종료
	for (int i = 0; i < MAX_CLIENT; ++i)
	{
		client_thread[i].detach();				// 프로세스와 스레드 분리.
		closesocket(vClient[i].m_socket);
	}

	WSACleanup();
}


