#pragma once

#include "resource.h"

// server
SOCKET g_mysocket;
WSABUF	send_wsabuf;
char 	send_buffer[BUF_SIZE];
WSABUF	recv_wsabuf;
char	recv_buffer[BUF_SIZE];
char	packet_buffer[BUF_SIZE];
DWORD	in_packet_size = 0;
int		saved_packet_size = 0;
int		g_myid;
HWND	g_hWnd;

// server
void ReadPacket(SOCKET sock);
void ProcessPacket(char *ptr);