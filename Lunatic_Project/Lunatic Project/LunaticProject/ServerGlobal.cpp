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
bool activate{};
bool HeroDataReceive = false;
int g_ping_send{};
int g_ping_recv{};
bool g_bDoing_Ani = false;
int g_A_Teamcount = 0;
int g_B_Teamcount = 0;