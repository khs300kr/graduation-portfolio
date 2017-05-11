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
