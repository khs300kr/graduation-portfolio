#pragma once

void error_display(char *msg, int err_no);
void currentDateTime();
enum OPTYPE { OP_SEND, OP_RECV, OP_RESPAWN };

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
	int				m_room_number;
};

struct CLIENT
{
	// DB_ID
	WCHAR			m_ID[MAX_ID_LEN];
	// Room
	bool			m_bLobby;
	BYTE			m_GameID;
	// Pos
	BYTE			m_Direction;
	float			m_fX;
	float			m_fY;
	float			m_fZ;
	// Hero Type
	BYTE			m_HeroPick;
	// Hero Stat
	short			m_att;
	short			m_hp;
	int				m_room_number;
	
	// ���â Stat
	WORD			m_killcount;
	WORD			m_deathcount;
	int				m_deal;
	int				m_hit;

	//
	bool			m_bConnect;
	SOCKET			m_client_socket;
	OverlappedEx	m_recv_over;
	unsigned char	packet_buf[MAX_PACKET_SIZE];
	int prev_packet_data;	// ���� �� ������
	int curr_packet_size;	// ���� ��
	mutex vl_lock;
};

struct ROOM
{
	WCHAR m_title[MAX_ROOMTITLE_SIZE];// ����(���ڿ�)
	char m_password[MAX_ROOMPASSWORD_SIZE];// ��й�ȣ(���ڿ�)
	bool m_private;						   // �����(bool)
	BYTE  m_mode;// ���Ӹ��(BYTE)
	BYTE  m_RoomStatus;
	unordered_set<int> m_GameID_list;// �ο�(BYTE)
	unordered_set<int> m_AcceptLoading_list; 
	BYTE m_readycount;
	BYTE m_loadcount;
	WORD A_killcount;
	WORD B_killcount;
};
// Server
extern HANDLE g_Hiocp;
extern SOCKET g_ServerSocket;
extern CLIENT g_Clients[MAX_USER];
extern ROOM	  g_Room[MAX_ROOM];
extern WORD   g_CCU; // CCU(Concurrent Users)
extern BYTE	  g_RoomNum;

// DB
extern SQLHENV henv;
extern SQLHDBC hdbc;
extern SQLHSTMT hstmt;
extern SQLRETURN retcode;

// Timer
enum Event_Type { P_RESPAWN };

struct Timer_Event {
	int object_id;											// � ���̵��?
	high_resolution_clock::time_point exec_time;			// ���� �����Ҳ���?
	Event_Type event;										// � �̺�Ʈ�ΰ�??
	int room_number;										// � ���̳�?
};

class comparison {
	bool reverse;
public:
	comparison() {}
	bool operator()(const Timer_Event first, const Timer_Event second) const {
		return first.exec_time > second.exec_time;
	}
};

extern priority_queue<Timer_Event, vector<Timer_Event>, comparison> timer_queue;
extern mutex timerqueue_lock;