#pragma warning(disable:4996)
#pragma comment(lib,"ws2_32")
#include <WinSock2.h>
#include <iostream>

using namespace std;

#define SERVERPORT 9000
#define BUFSIZE 1024

void main()
{
	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
		exit(1);

	// socket
	SOCKET listen_sock = socket(AF_INET, SOCK_STREAM, 0);			// Ipv4 + TCP
	if (listen_sock == INVALID_SOCKET) { cout << "listen_sock is invalid" << endl; }

	// bind (소켓 + 주소구조체)
	SOCKADDR_IN server_addr;
	ZeroMemory(&server_addr, sizeof(server_addr));		// same as memset(&serveraddr,0,sizeof(serveraddr))
	server_addr.sin_family = AF_INET;
	server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	server_addr.sin_port = htons(SERVERPORT);
	if (bind(listen_sock, (SOCKADDR*)&server_addr, sizeof(server_addr)) == SOCKET_ERROR) { cout << "bind() Error" << endl; }

	// listen
	if (listen(listen_sock, SOMAXCONN) == SOCKET_ERROR) { cout << "listen() Error" << endl; }

	// 통신 변수
	SOCKET client_sock;
	SOCKADDR_IN client_addr;
	char buf[BUFSIZE + 1];
	int addrlen{};
	
	// 크기 받기 데이터
	int iLen{};

	int retval{};
	while (true)	// accept loop
	{
		addrlen = sizeof(client_addr);
		client_sock = accept(listen_sock, (SOCKADDR*)&client_addr, &addrlen);
		if (client_sock == INVALID_SOCKET) { cout << "accept() Error" << endl; break; }
		//cout << addrlen << endl; system("cls");

		cout << " [ Accept - IP : " << inet_ntoa(client_addr.sin_addr) << ", Port : " << ntohs(client_addr.sin_port) << " ] " << endl;

		while (true)	// communication loop
		{
			// recv
			retval = recv(client_sock, buf, BUFSIZE, 0);
			cout << "[len] 받은 데이터 : " << retval << endl;
			if (retval == SOCKET_ERROR) { cout << "recv()" << endl; break; }

			//retval = recv(client_sock, buf, iLen , 0);
			//cout << "[buf] 받은 데이터 : " << retval << endl;
			//if (retval == SOCKET_ERROR) { cout << "recv()" << endl; break; }
			else if (retval == 0) break; // client Quit

			// Data Process Logic
			buf[retval] = '\0';
			cout << buf << endl;
		}

		closesocket(client_sock);
		cout << " [ Quit - IP : " << inet_ntoa(client_addr.sin_addr) << ", Port : " << ntohs(client_addr.sin_port) << " ] " << endl;
	}
	closesocket(listen_sock);

	WSACleanup();
}