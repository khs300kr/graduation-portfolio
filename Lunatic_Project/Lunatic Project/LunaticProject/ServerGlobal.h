#pragma once

// server
extern SOCKET  g_mysocket;
extern WSABUF 	recv_wsabuf;
extern char	recv_buffer[BUF_SIZE];
extern char	packet_buffer[BUF_SIZE];
extern DWORD	in_packet_size;
extern int		saved_packet_size;
extern int		g_myid;
extern HWND		g_hWnd;
extern WSABUF	send_wsabuf;
extern char 	send_buffer[BUF_SIZE];
extern bool activate;
extern bool HeroDataReceive;
extern int g_ping_send;
extern int g_ping_recv;
extern bool g_bDoing_Ani;
extern int g_A_Teamcount;
extern int g_B_Teamcount;