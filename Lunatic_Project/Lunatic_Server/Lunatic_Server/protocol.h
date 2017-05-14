#define MAX_BUFF_SIZE   4000
#define MAX_PACKET_SIZE  255

#define BOARD_WIDTH   8
#define BOARD_HEIGHT  8

#define MAX_USER 8

#define NPC_START  1000
#define NUM_OF_NPC  10000

#define MY_SERVER_PORT  4000

#define MAX_STR_SIZE  100

#define CS_KEYDOWN_UP				1
#define CS_KEYDOWN_DOWN				2
#define CS_KEYDOWN_LEFT				3
#define CS_KEYDOWN_RIGHT			4
#define CS_KEYUP_UP					5
#define CS_KEYUP_DOWN				6
#define CS_KEYUP_LEFT				7
#define CS_KEYUP_RIGHT				8
#define CS_CHAT						9
#define CS_READY					10
#define CS_LOADCOMPLETE				11


#define SC_POS           1
#define SC_PUT_PLAYER    2
#define SC_REMOVE_PLAYER 3
#define SC_CHAT			 4
#define SC_ID			 5 
#define SC_READY		 6
#define SC_ALLREADY		 7

//#define SC_BABARIAN		11
//#define SC_HEALER		12
//#define SC_SWORDMAN		13

// Client Define
// 키보드 입력
#define DIR_BACK		0x01
#define DIR_FRONT		0x02
#define DIR_LEFT		0x04
#define DIR_RIGHT		0x08
#define DIR_LEFT_BACK	0x05
#define DIR_RIGHT_BACK	0x09
#define DIR_LEFT_FRONT	0x06
#define DIR_RIGHT_FRONT	0xA
// 애니메이션
#define ANI_IDLE		0
#define ANI_RUN			1
#define ANI_ATTACK		2
#define ANI_Q			3
#define ANI_W			4
#define ANI_E			5
#define ANI_R			6
#define ANI_HIT			7
#define ANI_STUN		8
#define ANI_DIE			9
// 팀
#define A_TEAM 1
#define B_TEAM 2

// 캐릭터
#define Empty 0
#define Babarian 1
#define SordMan 3
#define Healer 2



#pragma pack (push, 1)

// Client -> Server
// 방
struct cs_packet_ready {
	BYTE size;
	BYTE type;
	BYTE hero_pick;
};

struct cs_packet_LoadingComplete {
	BYTE size;
	BYTE type;
};

// 인게임
struct cs_packet_pos {
	BYTE size;
	BYTE type;
	float x;
	float y;
	float z;
};

struct cs_packet_chat {
	BYTE size;
	BYTE type;
	WCHAR message[MAX_STR_SIZE];
};



// Server -> Client
// 접속
struct sc_packet_id {
	BYTE size;
	BYTE type;
	WORD id;
};

// 방
struct sc_packet_ready {
	BYTE size;
	BYTE type;
	WORD id;
	BYTE hero_pick;		// 캐릭터 정보.
};

struct sc_packet_allready
{
	BYTE size;
	BYTE type;
	WORD id;
};

// 인게임
struct sc_packet_put_player {
	BYTE size;
	BYTE type;
	WORD id;
	float x;
	float y;
	float z;
};

struct sc_packet_pos {
	BYTE size;
	BYTE type;
	WORD id;
	BYTE direction;
	float x;
	float y;
	float z;
};

struct sc_packet_remove_player {
	BYTE size;
	BYTE type;
	WORD id;
};

struct sc_packet_chat {
	BYTE size;
	BYTE type;
	WORD id;
	WCHAR message[MAX_STR_SIZE];
};
#pragma pack (pop)