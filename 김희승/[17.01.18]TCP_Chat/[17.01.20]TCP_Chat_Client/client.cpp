#pragma comment(lib,"Ws2_32.lib")

#include<iostream>
#include<winsock2.h>
#include<WS2tcpip.h>
#include<thread>
#include<string>

using namespace std;

#define IP_ADDR "127.0.0.1"		// loop back
#define SERVER_PORT "8000"
#define BUFSIZE 512

typedef struct tagClientType
{
	int		m_id;
	SOCKET	m_socket;
	char	m_recv_msg[BUFSIZE];
	tagClientType()
		: m_id(-1), m_socket(INVALID_SOCKET) {
		memset(m_recv_msg, 0, BUFSIZE);
	}
	~tagClientType() {}

}CLIENT_TYPE;

void process_client(CLIENT_TYPE& new_client)
{
	while (true)
	{
		memset(new_client.m_recv_msg, 0, BUFSIZE);
		if (new_client.m_socket != INVALID_SOCKET)
		{
			int retval = recv(new_client.m_socket, new_client.m_recv_msg, BUFSIZE, 0);

			if (retval == SOCKET_ERROR)	{ cout << "recv() failed" << endl; exit(1);	}
			else
			{
				cout << new_client.m_recv_msg << endl;
			}
		}
	}

	if (WSAGetLastError() == WSAECONNRESET) //An existing connection was forcibly closed by the remote host.
		cout << "서버와 연결이 끊겼습니다" << endl;
}


void main()
{
	cout << "Starting Client..." << endl;
	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0) { cout << "< Winsock Intializing failed >" << endl; exit(1); }

	// Setup hints
	addrinfo  hints;
	addrinfo* result = NULL;
	ZeroMemory(&hints, sizeof(hints));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;
	if (getaddrinfo(IP_ADDR, SERVER_PORT, &hints, &result) != 0) { cout << "< getaddrinfo failed >" << endl; exit(1); }


	cout << "Connecting..." << endl;
	addrinfo* ptr = NULL;
	CLIENT_TYPE client;

	
	// 성공할때까지 시도한다.
	int retval;
	for (ptr = result; ptr != NULL; ptr = ptr->ai_next)
	{
		client.m_socket = socket(ptr->ai_family, ptr->ai_socktype, ptr->ai_protocol);
		if (client.m_socket == INVALID_SOCKET) { cout << "socket()" << endl; exit(1); }
		
		//connect
		retval = connect(client.m_socket, ptr->ai_addr, (int)ptr->ai_addr);
		if (retval == SOCKET_ERROR) 
		{
			closesocket(client.m_socket);
			client.m_socket = INVALID_SOCKET;
			continue;
		}
		break;
	}

	freeaddrinfo(result);

	if (client.m_socket == INVALID_SOCKET) {
		cout << "서버에 접속할 수 없습니다" << endl;
		exit(1);
	}
	else
		cout << "서버 접속 성공" << endl;

	// 서버로부터 ID 획득
	recv(client.m_socket, client.m_recv_msg, BUFSIZE, 0);
	string strMsg = client.m_recv_msg;


	string str_send_msg{};
	if (strMsg != "Server is Full")
	{
		client.m_id = atoi(client.m_recv_msg);	// ascii - to - int

		thread client_thread(process_client, std::ref(client));
		while (true)
		{
			getline(cin, str_send_msg);
			retval = send(client.m_socket, str_send_msg.c_str(), (int)str_send_msg.size(), 0);
			if (retval == SOCKET_ERROR) { cout << "send() failed" << endl; exit(1); }

		}
		// 보낼 데이터가 없으면 shutdown
		client_thread.detach();
	}
	else
		cout << client.m_recv_msg << endl; // server is full

	cout << "소켓 닫는중..." << endl;
	retval = shutdown(client.m_socket, SD_SEND);
	if (retval == SOCKET_ERROR) { cout << "shutdown() error" << endl; exit(1); }

	closesocket(client.m_socket);
	WSACleanup();


}