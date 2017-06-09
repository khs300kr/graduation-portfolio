// stdafx.h : 자주 사용하지만 자주 변경되지는 않는
// 표준 시스템 포함 파일 및 프로젝트 관련 포함 파일이
// 들어 있는 포함 파일입니다.
//
#pragma once

#include "targetver.h"

#define WIN32_LEAN_AND_MEAN             // 거의 사용되지 않는 내용은 Windows 헤더에서 제외합니다.
// server
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#define INITGUID
#include <WinSock2.h>
#include "../../Lunatic_Server/Lunatic_Server/protocol.h"
#pragma comment (lib, "ws2_32.lib")

#define	BUF_SIZE				1024
#define	WM_SOCKET				WM_USER + 1

#define MAX_LOADSTRING 100
#define CHAT_LENGTH 50
#define ID_EDIT 100
#define MAX_CHAT_LINE 30

#include "ServerGlobal.h"

// Windows 헤더 파일:
#include <windows.h>

// C 런타임 헤더 파일입니다.
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>


// TODO: 프로그램에 필요한 추가 헤더는 여기에서 참조합니다.
// DX11 전용 헤더
#include <D3D11.h>
#include <D3DX11.h>
#include <mmsystem.h>
//#include <xnamath.h>

#include <math.h>
#include <d3dcompiler.h>
#include <D3DX10math.h>
#include <stdio.h>

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>

#include <DirectXMath.h>
#include <DirectXPackedVector.h>
#include <DirectXColors.h>
#include <DirectXCollision.h>

using namespace std;
using namespace DirectX;
using namespace DirectX::PackedVector;


#define FRAME_BUFFER_WIDTH	1024
#define FRAME_BUFFER_HEIGHT	768

#define RANDOM_COLOR	D3DXCOLOR((rand() * 0xFFFFFF / RAND_MAX))
#define ASPECT_RATIO	(float(FRAME_BUFFER_WIDTH) / float(FRAME_BUFFER_HEIGHT))

// 테스트를 위한 콘솔 띄우기
#pragma comment(linker, "/entry:wWinMainCRTStartup /subsystem:console")

#define KEY_DOWN(vk_code) ((GetAsyncKeyState(vk_code) & 0x8000) ? 1 : 0)
#define KEY_UP(vk_code)   ((GetAsyncKeyState(vk_code) & 0x8000) ? 0 : 1)

//
// 쉐이더 슬롯
//////////////////////////////////////////
#define VS_SLOT_CAMERA			0x00
#define VS_SLOT_WORLD_MATRIX	0x01
#define VS_SLOT_SKINNEDBONE		0x02

// 조명과 재질 설정용 상수 버퍼 슬롯
#define PS_SLOT_LIGHT			0x00
#define PS_SLOT_MATERIAL		0x01

// Texture & Sampler State 설정을 위한 슬롯
#define PS_SLOT_TEXTURE			0x00
#define PS_SLOT_SAMPLER_STATE	0x00


#define ANIFRAMETIME			0.0333333f
#define GetFBXMesh				GetGameObject(0)->GetMesh(0)


#define MAINMENU 0
#define LOBBY 1
#define ROOM 2
#define GAME 3