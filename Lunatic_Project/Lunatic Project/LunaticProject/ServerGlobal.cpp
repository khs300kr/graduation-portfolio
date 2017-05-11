#include "stdafx.h"
#include "ServerGlobal.h"

SOCKET  g_mysocket{};
WSABUF 	recv_wsabuf{};
char	recv_buffer[BUF_SIZE]{};
char	packet_buffer[BUF_SIZE]{};
DWORD	in_packet_size{};
int		saved_packet_size{};
int		g_myid{};
HWND	g_hWnd{};
WSABUF	send_wsabuf{};
char 	send_buffer[BUF_SIZE]{};