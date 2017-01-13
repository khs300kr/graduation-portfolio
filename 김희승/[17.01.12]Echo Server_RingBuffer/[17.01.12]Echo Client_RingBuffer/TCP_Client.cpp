#pragma warning(disable:4996)
#pragma comment(lib,"ws2_32")
#include <WinSock2.h>
#include <iostream>

using namespace std;

#define SERVERIP "127.0.0.1"
#define SERVERPORT 9000
#define BUFSIZE 1024

void main()
{
	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
		exit(1);

	// socket
	SOCKET sock = socket(AF_INET, SOCK_STREAM, 0);
	if (sock == INVALID_SOCKET) { cout << "socket is invalid" << endl; }

	// connect
	SOCKADDR_IN server_addr;
	ZeroMemory(&server_addr, sizeof(server_addr));
	server_addr.sin_family = AF_INET;
	server_addr.sin_addr.s_addr = inet_addr(SERVERIP);
	server_addr.sin_port = htons(SERVERPORT);
	if (connect(sock, (SOCKADDR*)&server_addr, sizeof(server_addr)) == SOCKET_ERROR) { cout << "connect() Error" << endl; exit(1); }

	// 통신 변수
	char buf[BUFSIZE];
	char *tesdata[] = {
		"HI",
		"Hello",
		"World",
		"Wonderful day"
	};
	char* tempdata[]{
	"abcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyz"
	};

	memcpy(buf, tempdata[0], (int)strlen(tempdata[0]));

	int iLen{};
	int retval{};

	//for (int i = 0; i < 4; ++i)
	//{
	//	iLen = (int)strlen(tesdata[i]);
	//	strncpy(buf, tesdata[i], iLen);
	//	retval = send(sock, (char*)&iLen, sizeof(int), 0);
	//	if (retval == SOCKET_ERROR) { cout << "send()" << endl; break; }
	//
	//	retval = send(sock, buf, iLen, 0);
	//	if (retval == SOCKET_ERROR) { cout << "send()" << endl; break; }
	//	cout << "[ Send ] " << retval + 4 << " byte" << endl;
	//}
	for (int i = 0; i < 10; ++i)
	{
		retval = send(sock, buf, (int)strlen(tempdata[0]), 0);
		if (retval == SOCKET_ERROR) { cout << "send()" << endl; }
		cout << "[ Send ] " << retval << " byte" << endl;
	}
	closesocket(sock);

	WSACleanup();
}