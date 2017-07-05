// stdafx.h : 자주 사용하지만 자주 변경되지는 않는
// 표준 시스템 포함 파일 또는 프로젝트 관련 포함 파일이
// 들어 있는 포함 파일입니다.
//

#pragma once

#include "targetver.h"

#include <tchar.h>



// TODO: 프로그램에 필요한 추가 헤더는 여기에서 참조합니다.
#pragma warning(disable:4996)
#pragma comment(lib,"ws2_32")
#define WIN32_LEAN_AND_MEAN		// 거의 사용되지 않는 내용은 Windows 헤더에서 제외합니다.

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
