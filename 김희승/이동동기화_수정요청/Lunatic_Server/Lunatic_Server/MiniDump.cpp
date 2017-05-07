#include "stdafx.h"
#include "MiniDump.h"


typedef BOOL(WINAPI *MINIDUMPWRITEDUMP)( // Callback 함수의 원형
	HANDLE hProcess,
	DWORD dwPid,
	HANDLE hFile,
	MINIDUMP_TYPE DumpType,
	CONST PMINIDUMP_EXCEPTION_INFORMATION ExceptionParam,
	CONST PMINIDUMP_USER_STREAM_INFORMATION UserStreamParam,
	CONST PMINIDUMP_CALLBACK_INFORMATION CallbackParam);

LPTOP_LEVEL_EXCEPTION_FILTER PreviousExceptionFilter = NULL;



LONG WINAPI UnHandledExceptionFilter(struct _EXCEPTION_POINTERS *exceptionInfo)
{
	HMODULE	DllHandle = NULL;

	// Windows 2000 이전에는따로DBGHELP를배포해서설정해주어야한다.
	DllHandle = LoadLibrary(_T("DBGHELP.DLL"));

	if (DllHandle)
	{
		// 프로세스의 주소를 가져온다.
		MINIDUMPWRITEDUMP Dump = (MINIDUMPWRITEDUMP)GetProcAddress(DllHandle, "MiniDumpWriteDump");

		if (Dump)
		{
			TCHAR		DumpPath[MAX_PATH] = { 0, };
			SYSTEMTIME	SystemTime;

			GetLocalTime(&SystemTime);

			// 날짜와 시간을 기록한다.
			_sntprintf_s(DumpPath, MAX_PATH, _T("%d-%d-%d %d_%d_%d.dmp"),
				SystemTime.wYear,
				SystemTime.wMonth,
				SystemTime.wDay,
				SystemTime.wHour,
				SystemTime.wMinute,
				SystemTime.wSecond);
			// 파일을 하나 생성한다.
			HANDLE FileHandle = CreateFile(
				DumpPath,
				GENERIC_WRITE,
				FILE_SHARE_WRITE,
				NULL, CREATE_ALWAYS,
				FILE_ATTRIBUTE_NORMAL,
				NULL);

			if (FileHandle != INVALID_HANDLE_VALUE)
			{
				_MINIDUMP_EXCEPTION_INFORMATION MiniDumpExceptionInfo;

				MiniDumpExceptionInfo.ThreadId = GetCurrentThreadId();
				MiniDumpExceptionInfo.ExceptionPointers = exceptionInfo;
				MiniDumpExceptionInfo.ClientPointers = NULL;

				// window 함수 (dump)
				BOOL Success = Dump(
					GetCurrentProcess(),
					GetCurrentProcessId(),
					FileHandle,	// 방금 만든 파일에다가 프로세스 내용을 덤프한다. 
					MiniDumpNormal,
					&MiniDumpExceptionInfo,
					NULL,
					NULL);
				if (Success)
				{
					CloseHandle(FileHandle);

					return EXCEPTION_EXECUTE_HANDLER;
				}
			}

			CloseHandle(FileHandle);
		}
	}

	return EXCEPTION_CONTINUE_SEARCH;
}

BOOL CMiniDump::Begin(VOID)
{
	// 에러가 났을때 어떻게 할것인가?
	SetErrorMode(SEM_FAILCRITICALERRORS);
	// SetUnhandledExceptionFilter - window 함수
	// Exception이 났을때, 이 함수를 호출 한 후 죽는것이다.
	// 무슨함수냐? 위에있다.
	PreviousExceptionFilter = SetUnhandledExceptionFilter(UnHandledExceptionFilter);

	return true;
}

BOOL CMiniDump::End(VOID)
{
	// callback 함수.
	SetUnhandledExceptionFilter(PreviousExceptionFilter);

	return true;
}
