// stdafx.h : ���� ��������� ���� ��������� �ʴ�
// ǥ�� �ý��� ���� ���� �� ������Ʈ ���� ���� ������
// ��� �ִ� ���� �����Դϴ�.
//

#pragma once

#include "targetver.h"

#define WIN32_LEAN_AND_MEAN             // ���� ������ �ʴ� ������ Windows ������� �����մϴ�.
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

// �Է� ���� Define
#define DIR_BACK		0x01
#define DIR_FRONT	0x02
#define DIR_LEFT		0x04
#define DIR_RIGHT		0x08
#define DIR_UP			0x10 //Y������ �̵�
#define DIR_DOWN		0x20
#define DIR_LEFT_BACK	0x05
#define DIR_RIGHT_BACK	0x09
#define DIR_LEFT_FRONT	0x06
#define DIR_RIGHT_FRONT	0x10


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