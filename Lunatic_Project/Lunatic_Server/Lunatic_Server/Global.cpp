#include "stdafx.h"
#include "Global.h"


void error_display(char *msg, int err_no)
{
	WCHAR *lpMsgBuf;
	FormatMessage(
		FORMAT_MESSAGE_ALLOCATE_BUFFER |
		FORMAT_MESSAGE_FROM_SYSTEM,
		NULL, err_no,
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(LPTSTR)&lpMsgBuf, 0, NULL);
	std::cout << msg;
	std::wcout << L"¿¡·¯" << lpMsgBuf << std::endl;
	LocalFree(lpMsgBuf);
	while (true);
}

// Server
HANDLE g_Hiocp{};
SOCKET g_ServerSocket{};
CLIENT g_Clients[MAX_USER]{};
WORD   g_CCU{};
ROOM   g_Room[MAX_ROOM]{};
BYTE   g_RoomNum{};

// DB
SQLHENV henv{};
SQLHDBC hdbc{};
SQLHSTMT hstmt{};
SQLRETURN retcode{};;

// Timer
priority_queue<Timer_Event, vector<Timer_Event>, comparison> timer_queue{};
mutex timerqueue_lock{};