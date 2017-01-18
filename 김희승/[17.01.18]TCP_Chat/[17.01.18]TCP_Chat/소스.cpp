#pragma comment(lib,"Ws2_32.lib")

#include <iostream>
#include <WinSock2.h>
#include <WS2tcpip.h> // getaddrinfo()
#include <thread>	  // C+11 multithread
#include <string>
#include <vector>

#define IP_ADDR "127.0.0.1"		// loop back
#define SERVER_PORT 8000
#define BUF_SIZE 512

using namespace std;

struct tagClientType
{
	int		m_id;
	SOCKET	m_socket;
};

void main()
{
	cout << "Initializing Winsock..." << endl;
	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0) { cout << "< Winsock Intializing failed >" << endl; exit(1);}

	cout << "Setting up Server ( 家南积己 + bind )..." << endl;
	SOCKET listen_sock = INVALID_SOCKET;


}