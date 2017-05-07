#include "stdafx.h"
#include "MiniDump.h"


typedef BOOL(WINAPI *MINIDUMPWRITEDUMP)( // Callback �Լ��� ����
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

	// Windows 2000 �������µ���DBGHELP�������ؼ��������־���Ѵ�.
	DllHandle = LoadLibrary(_T("DBGHELP.DLL"));

	if (DllHandle)
	{
		// ���μ����� �ּҸ� �����´�.
		MINIDUMPWRITEDUMP Dump = (MINIDUMPWRITEDUMP)GetProcAddress(DllHandle, "MiniDumpWriteDump");

		if (Dump)
		{
			TCHAR		DumpPath[MAX_PATH] = { 0, };
			SYSTEMTIME	SystemTime;

			GetLocalTime(&SystemTime);

			// ��¥�� �ð��� ����Ѵ�.
			_sntprintf_s(DumpPath, MAX_PATH, _T("%d-%d-%d %d_%d_%d.dmp"),
				SystemTime.wYear,
				SystemTime.wMonth,
				SystemTime.wDay,
				SystemTime.wHour,
				SystemTime.wMinute,
				SystemTime.wSecond);
			// ������ �ϳ� �����Ѵ�.
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

				// window �Լ� (dump)
				BOOL Success = Dump(
					GetCurrentProcess(),
					GetCurrentProcessId(),
					FileHandle,	// ��� ���� ���Ͽ��ٰ� ���μ��� ������ �����Ѵ�. 
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
	// ������ ������ ��� �Ұ��ΰ�?
	SetErrorMode(SEM_FAILCRITICALERRORS);
	// SetUnhandledExceptionFilter - window �Լ�
	// Exception�� ������, �� �Լ��� ȣ�� �� �� �״°��̴�.
	// �����Լ���? �����ִ�.
	PreviousExceptionFilter = SetUnhandledExceptionFilter(UnHandledExceptionFilter);

	return true;
}

BOOL CMiniDump::End(VOID)
{
	// callback �Լ�.
	SetUnhandledExceptionFilter(PreviousExceptionFilter);

	return true;
}
