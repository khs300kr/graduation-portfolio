#pragma once
#include "stdafx.h"

#pragma comment(lib,"ws2_32.lib")
#pragma comment(lib,"mswsock.lib")
#pragma comment(lib,"Winmm.lib")

#include <WS2tcpip.h>
#include <WinSock2.h>
#include <MSWSock.h>
#include <mmsystem.h>
#include <WS2spi.h>
#include <mstcpip.h>

#include <Windows.h>
#include <iostream>
#include <io.h>
#include <cstdlib>
#include <stdio.h>
#include <cstdint>

#include <assert.h>
#include <fcntl.h>
#include <algorithm>
#include <functional>

#include <thread>
#include <mutex>
#include <memory>

#include <string>
#include <tchar.h>

#include <ctime>
#include <random>
#include <typeinfo>

// TODO:공용매크로
#if _DEBUG
#define CONTEXT_SWITCH Sleep(1)
#else
#define CONTEXT_SWITCH ::SwitchToThread()
#endif

typedef void(*Function)(void*);

// 기타 유틸

// 패킷들

// 콘텐츠 관련

// 서버

// 전역 변수

