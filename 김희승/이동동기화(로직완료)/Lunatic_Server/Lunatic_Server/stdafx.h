// stdafx.h : ���� ��������� ���� ��������� �ʴ�
// ǥ�� �ý��� ���� ���� �Ǵ� ������Ʈ ���� ���� ������
// ��� �ִ� ���� �����Դϴ�.
//

#pragma once

#include "targetver.h"

#include <stdio.h>
#include <tchar.h>



// TODO: ���α׷��� �ʿ��� �߰� ����� ���⿡�� �����մϴ�.
#pragma warning(disable:4996)
#pragma comment(lib,"ws2_32")
#include <WinSock2.h>
#include <winsock.h>
#include <Windows.h>
#include <iostream>
#include <thread>
#include <vector>
#include <mutex>
#include "protocol.h"

enum OPTYPE { OP_SEND, OP_RECV };


// Client Define
// �Է� ���� Define
#define DIR_BACK		0x01
#define DIR_FRONT		0x02
#define DIR_LEFT		0x04
#define DIR_RIGHT		0x08
#define DIR_LEFT_BACK	0x05
#define DIR_RIGHT_BACK	0x09
#define DIR_LEFT_FRONT	0x06
#define DIR_RIGHT_FRONT	0x10