// stdafx.h : ���� ��������� ���� ��������� �ʴ�
// ǥ�� �ý��� ���� ���� �Ǵ� ������Ʈ ���� ���� ������
// ��� �ִ� ���� �����Դϴ�.
//

#pragma once

#include "targetver.h"

#include <tchar.h>



// TODO: ���α׷��� �ʿ��� �߰� ����� ���⿡�� �����մϴ�.
#pragma warning(disable:4996)
#pragma comment(lib,"ws2_32")
#define WIN32_LEAN_AND_MEAN		// ���� ������ �ʴ� ������ Windows ������� �����մϴ�.

#include <WinSock2.h>
#include <Windows.h>
#include <iostream>
#include <thread>
#include <vector>
#include <mutex>
#include <string.>
#include <chrono>
#include <unordered_set>
#include <chrono>
#include <queue>
#include "MemoryLeak.h"
#include "protocol.h"

// DB
#include <sqlext.h>

using namespace std;
using namespace chrono;
