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
#define MAX_CLIENT 1000			// ���� 1000
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


void process_client(CLIENT_TYPE& new_client, vector<CLIENT_TYPE>& vClient)//, thread& thread)
{
	string strMsg{};
	char buf[BUFSIZE]{};

	while (true)
	{
		if (new_client.m_socket != INVALID_SOCKET)
		{
			int retval = recvn(new_client.m_socket, buf, BUFSIZE, 0);

			if (retval == SOCKET_ERROR)
			{
				strMsg = "Client[" + to_string(new_client.m_id) + "]" + " ����";
				cout << strMsg << endl;

				closesocket(new_client.m_socket);
				closesocket(vClient[new_client.m_id].m_socket);
				vClient[new_client.m_id].m_socket = INVALID_SOCKET;

				// Broadcast the disconnection meesage to the other clients
				for (auto&d : vClient)
				{
					if (d.m_socket != INVALID_SOCKET)
						retval = send(d.m_socket, strMsg.c_str(), (int)strMsg.size(), 0);
				}
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
	cout << "Setting up Server ( �ּ� ����, �������� ���� )..." << endl;
	addrinfo hints;
	ZeroMemory(&hints, sizeof(hints));
	hints.ai_flags = AI_PASSIVE;		// The socket address will be used in a call to the bind function.(msdn)
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_family = AF_INET;

	// Create server socket
	addrinfo*	server;
	getaddrinfo(IP_ADDR, SERVER_PORT, &hints, &server);
	SOCKET server_socket = socket(server->ai_family, server->ai_socktype, 0);
	if (server_socket == INVALID_SOCKET) { cout << "server_socket is invalid" << endl; exit(1); }

	// Setup socket options
	bool optval = true;
	setsockopt(server_socket, SOL_SOCKET, SO_REUSEADDR, (char*)&optval, sizeof(optval));	// �����ڵ尡 ó���ϴ� �ɼ� - (�������� ������ ����)
	setsockopt(server_socket, IPPROTO_TCP, TCP_NODELAY, (char*)&optval, sizeof(optval));	// �������ݱ��� �ڵ尡 ó���ϴ� �ɼ� - Reduce response time but increase traffic

	cout << "Binding socket..." << endl;
	bind(server_socket, &server->ai_addr, (int)server->ai_addrlen);

	cout << "Listening..." << endl;
	listen(server_socket, SOMAXCONN);
	
	// ��ź���
	vector<CLIENT_TYPE> client(MAX_CLIENT);		// key-value? trying map?
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

		// ���� Ŭ���̾�Ʈ�� ���� ���� id ����?
		iIndex = NOTYET_CONNECT;
		for (int i = 0; i < MAX_CLIENT; ++i)
		{
			if (client[i].m_socket == INVALID_SOCKET && iIndex == NOTYET_CONNECT)
			{
				client[i].m_socket = sTemp;
				client[i].m_id = i;
				iIndex = i;
			}
			
			if (client[i].m_socket != INVALID_SOCKET)
				++iClient_Number;							// ������?
		}
	


		if (iIndex == NOTYET_CONNECT)
		{
			strMsg = "Server is Full";
			send(sTemp, strMsg.c_str(), (int)strMsg.size(), 0);
			cout << strMsg << endl;
		}
		else
		{
			// id�� Ŭ���̾�Ʈ���� ������?
			cout << "Client[" << client[iIndex].m_id << "]" << "����" << endl;
			strMsg = to_string(client[iIndex].m_id);
			send(client[iIndex].m_socket, strMsg.c_str(), (int)strMsg.size(), 0);

			// �ش� Ŭ���̾�Ʈ�� ���� ������ ���μ����� �����.
			client_thread[iIndex] = thread(
				process_client,
				std::ref(client[iIndex]),		// client_type
				std::ref(client));//,				// vector
				//std::ref(client_thread[iIndex]));	// thread array		
		}
	} // end while

	// ����(listening)���� ����
	closesocket(server_socket);

	// ������ , Ŭ���̾�Ʈ ���� ����
	for (int i = 0; i < iClient_Number; ++i)
	{
		client_thread[i].detach();		// ���μ����� ������ �и�.
		closesocket(client[i].m_socket);
	}

	WSACleanup();
}


