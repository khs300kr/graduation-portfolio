#pragma once

void error_display(char *msg, int err_no);
enum OPTYPE { OP_SEND, OP_RECV };

struct OverlappedEx
{
	// WSAOVERLAPPED 구조체 역할 2가지.
	// 1. 비동기 입출력을 위한 정보를 운영체제에 전달한다.
	// 2. 운영체제가 비동기 입출력 결과를 응용프로그램에 알려줄 떄 사용한다.
	WSAOVERLAPPED	m_Over;
	// WSABUF (1. 길이 2. 버퍼 시작주소) => Scatter/Gather
	WSABUF			m_Wsabuf;
	// IOCP send/recv 버퍼
	unsigned char	m_IOCP_buf[MAX_BUFF_SIZE];
	// Send(?) Recv(?)
	OPTYPE			m_Event_type;

};

struct CLIENT
{
	// DB_ID
	char			m_ID[MAX_ID_LEN];
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

	//
	bool			m_bConnect;
	SOCKET			m_client_socket;
	OverlappedEx	m_recv_over;
	unsigned char	packet_buf[MAX_PACKET_SIZE];
	int prev_packet_data;	// 조립 중 데이터
	int curr_packet_size;	// 받은 양
	mutex vl_lock;
};

struct ROOM
{
	WCHAR m_title[MAX_ROOMTITLE_SIZE];// 방제(문자열)
	char m_password[MAX_ROOMPASSWORD_SIZE];// 비밀번호(문자열)
	bool m_private;						   // 비공개(bool)
	BYTE  m_mode;// 게임모드(BYTE)
	BYTE  m_RoomStatus;
	unordered_set<int> m_GameID_list;// 인원(BYTE)
	unordered_set<int> m_AcceptLoading_list; 
	BYTE m_readycount;
	BYTE m_loadcount;
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
enum Event_Type { E_MOVE, P_HEAL, P_HIT };

struct Timer_Event {
	int object_id;											// 어떤 아이디냐?
	high_resolution_clock::time_point exec_time;			// 언제 실행할꺼냐?
	Event_Type event;										// 어떤 이벤트인가??
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