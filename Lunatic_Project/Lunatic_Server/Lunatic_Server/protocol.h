// Server Define
#define MAX_BUFF_SIZE   4000
#define MAX_PACKET_SIZE  255
#define MY_SERVER_PORT  4000
#define MAX_USER 40
#define MAX_ROOM 6

#define MAX_STR_SIZE			50
#define MAX_ROOMTITLE_SIZE		20
#define MAX_ROOMPASSWORD_SIZE	10
#define MAX_ID_LEN				15
#define MAX_PASSWORD_LEN		15

#define CS_KEYDOWN_UP				1
#define CS_KEYDOWN_DOWN				2
#define CS_KEYDOWN_LEFT				3
#define CS_KEYDOWN_RIGHT			4
#define CS_KEYUP_UP					5
#define CS_KEYUP_DOWN				6
#define CS_KEYUP_LEFT				7
#define CS_KEYUP_RIGHT				8
#define CS_LOBBY_CHAT				9
#define CS_ROOM_CHAT				10
#define CS_GAME_CHAT				11
#define CS_READY					12
#define CS_LOADCOMPLETE				13
#define CS_ATTACK					14
#define CS_SKILL_Q					15
#define CS_SKILL_W					16
#define CS_SKILL_E					17
#define CS_SKILL_R					18
#define CS_REGISTER					19
#define CS_LOGIN					20
#define CS_MAKE_ROOM				21

#define SC_POS           1
#define SC_PUT_PLAYER    2
#define SC_REMOVE_PLAYER 3
#define SC_LOBBY_CHAT	 4
#define SC_ROOM_CHAT	 5
#define SC_GAME_CHAT	 6
#define SC_ID			 7 
#define SC_READY		 8
#define SC_ALLREADY		 9
#define SC_ATTACK		 10
#define SC_SKILL_Q		 11
#define SC_SKILL_W		 12
#define SC_SKILL_E		 13
#define SC_SKILL_R		 14
#define SC_LOGIN_FAILED  15
#define SC_SHOW_ROOM	 16

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
#define Empty		0
#define Babarian	1
#define Healer		2
#define SordMan		3

// 방 상태
#define EMPTY		0		// 비어있음
#define JOINABLE	1		// 입장가능
#define FULL		2		// 입장불가
#define PLAYING		3		// 게임중

#define DEATHMATCH		1
#define	TERRITORY		2

#pragma pack (push, 1)
// Client -> Server
// 메인 메뉴
struct cs_packet_login {
	BYTE size;
	BYTE type;
	char id[MAX_ID_LEN];
	char password[MAX_PASSWORD_LEN];
};

// 로비
struct cs_packet_makeroom {
	BYTE size;
	BYTE type;
	WCHAR roomtitle[MAX_ROOMTITLE_SIZE];
	char password[MAX_ROOMPASSWORD_SIZE];
	BYTE mode;
};

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

struct cs_packet_attack {
	BYTE size;
	BYTE type;
};

struct cs_packet_skillQ {
	BYTE size;
	BYTE type;
};

struct cs_packet_skillW {
	BYTE size;
	BYTE type;
};

struct cs_packet_skillE {
	BYTE size;
	BYTE type;
};

struct cs_packet_skillR {
	BYTE size;
	BYTE type;
};


// Server -> Client
// 접속
struct sc_packet_id {
	BYTE size;
	BYTE type;
	WORD id;
};

struct sc_packet_loginfailed {
	BYTE size;
	BYTE type;
	WORD id;
};
// 로비
struct sc_packet_showroom {
	BYTE size;
	BYTE type;
	WORD id;
	BYTE room_id;
	WCHAR roomtitle[MAX_ROOMTITLE_SIZE];
	char password[MAX_ROOMPASSWORD_SIZE];
	BYTE mode;
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

struct sc_packet_attack {
	BYTE size;
	BYTE type;
	WORD id;
};

struct sc_packet_skillQ {
	BYTE size;
	BYTE type;
	WORD id;
};

struct sc_packet_skillW {
	BYTE size;
	BYTE type;
	WORD id;
};

struct sc_packet_skillE {
	BYTE size;
	BYTE type;
	WORD id;
};

struct sc_packet_skillR {
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