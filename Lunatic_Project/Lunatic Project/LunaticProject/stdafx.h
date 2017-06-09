// stdafx.h : ���� ��������� ���� ��������� �ʴ�
// ǥ�� �ý��� ���� ���� �� ������Ʈ ���� ���� ������
// ��� �ִ� ���� �����Դϴ�.
//
#pragma once

#include "targetver.h"

#define WIN32_LEAN_AND_MEAN             // ���� ������ �ʴ� ������ Windows ������� �����մϴ�.
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

// Windows ��� ����:
#include <windows.h>

// C ��Ÿ�� ��� �����Դϴ�.
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>


// TODO: ���α׷��� �ʿ��� �߰� ����� ���⿡�� �����մϴ�.
// DX11 ���� ���
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

// �׽�Ʈ�� ���� �ܼ� ����
#pragma comment(linker, "/entry:wWinMainCRTStartup /subsystem:console")

#define KEY_DOWN(vk_code) ((GetAsyncKeyState(vk_code) & 0x8000) ? 1 : 0)
#define KEY_UP(vk_code)   ((GetAsyncKeyState(vk_code) & 0x8000) ? 0 : 1)

//
// ���̴� ����
//////////////////////////////////////////
#define VS_SLOT_CAMERA			0x00
#define VS_SLOT_WORLD_MATRIX	0x01
#define VS_SLOT_SKINNEDBONE		0x02

// ����� ���� ������ ��� ���� ����
#define PS_SLOT_LIGHT			0x00
#define PS_SLOT_MATERIAL		0x01

// Texture & Sampler State ������ ���� ����
#define PS_SLOT_TEXTURE			0x00
#define PS_SLOT_SAMPLER_STATE	0x00


#define ANIFRAMETIME			0.0333333f
#define GetFBXMesh				GetGameObject(0)->GetMesh(0)


#define MAINMENU 0
#define LOBBY 1
#define ROOM 2
#define GAME 3