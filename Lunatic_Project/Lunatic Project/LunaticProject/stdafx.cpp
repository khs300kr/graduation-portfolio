// stdafx.cpp : 표준 포함 파일만 들어 있는 소스 파일입니다.
// LunaticProject.pch는 미리 컴파일된 헤더가 됩니다.
// stdafx.obj에는 미리 컴파일된 형식 정보가 포함됩니다.

#include "stdafx.h"

// TODO: 필요한 추가 헤더는
// 이 파일이 아닌 STDAFX.H에서 참조합니다.
void currentDateTime() {
	SYSTEMTIME st;
	char currentTime[84] = "";
	GetLocalTime(&st);
	sprintf_s(currentTime, "%d/%d/%d %d:%d:%d %d", st.wDay, st.wMonth, st.wYear, st.wHour, st.wMinute, st.wSecond, st.wMilliseconds);
	cout << "Current Time : " << currentTime << endl;
}
