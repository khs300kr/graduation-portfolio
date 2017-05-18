#pragma once

void error_display(char *msg, int err_no);
enum OPTYPE { OP_SEND, OP_RECV };

struct OverlappedEx
{
	// WSAOVERLAPPED ����ü ���� 2����.
	// 1. �񵿱� ������� ���� ������ �ü���� �����Ѵ�.
	// 2. �ü���� �񵿱� ����� ����� �������α׷��� �˷��� �� ����Ѵ�.
	WSAOVERLAPPED	m_Over;
	// WSABUF (1. ���� 2. ���� �����ּ�) => Scatter/Gather
	WSABUF			m_Wsabuf;
	// IOCP send/recv ����
	unsigned char	m_IOCP_buf[MAX_BUFF_SIZE];
	// Send(?) Recv(?)
	OPTYPE			m_Event_type;

};

struct CLIENT
{
	// Room
	bool			m_bLobby;
	BYTE			m_RoomID;
	// Pos
	BYTE			m_Direction;
	float			m_fX;
	float			m_fY;
	float			m_fZ;
	// Hero Type
	BYTE			m_HeroPick;

	bool			m_bConnect;
	SOCKET			m_client_socket;
	OverlappedEx	m_recv_over;
	// recv�� ���� ����.
	unsigned char	packet_buf[MAX_PACKET_SIZE];
	// ������ ���� ������.
	int prev_packet_data;	// ���� �� ������
	int curr_packet_size;	// ���� ��

	mutex vl_lock;
};

struct Room
{
	WCHAR m_title[MAX_ROOMTITLE_SIZE];// ����(���ڿ�)
	WCHAR m_password[4];// ��й�ȣ(���ڿ�)
	// �����(bool)
	BYTE  m_mode;// ���Ӹ��(BYTE)
	unordered_set<int> m_RoomID_list;// �ο�(BYTE)
};

extern HANDLE g_Hiocp;
extern SOCKET g_ServerSocket;
extern CLIENT g_Clients[MAX_USER];
extern Room	  g_Room[MAX_ROOM];
extern BYTE   g_ReadyNum;
extern WORD   g_CCU; // CCU(Concurrent Users)
