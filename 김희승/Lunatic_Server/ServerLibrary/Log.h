#pragma once
#include "stdafx.h"

class CLog
{
public:
	static BOOL		WriteLog(LPTSTR data, ...)
	{
		SYSTEMTIME		Systemtime;
		TCHAR			CurrentDate[32]{};
		TCHAR			CurrentFileName[MAX_PATH]{};
		FILE*			FilePtr{ NULL };
		TCHAR			DebugLog[MAX_BUFFER_LENGTH]{};

		va_list			ap;
		TCHAR			Log[MAX_BUFFER_LEGNTH]{};

		va_start(ap, data);
		_vstprintf(Log, data, ap);
		va_end(ap);

		GetLocalTime(&Systemtime);
		_sntprintf(CurrentDate, 32, _T("%d-%d-%d %d:%d:%d"),
			Systemtime.wYear,
			Systemtime.wMonth,
			Systemtime.wDay,
			Systemtime.wHour,
			Systemtime.wMinute,
			Systemtime.wSecond);

		_sntprintf(CurrentFileName, MAX_PATH, _T("LOG_%d-%d-%d %d.log"),
			Systemtime.wYear,
			Systemtime.wMonth,
			Systemtime.wDay,
			Systemtime.wHour);

		FilePtr = _tfopen(CurrentFileName, _T("a"));
		if (FilePtr)
			return FALSE;

		_ftprintf(FilePtr, _T("[%s] %s\n"), CurrentDate, Log);
		_sntprintf(DebugLog, MAX_BUFFER_LENGTH, _T("[%s] %s\n"), CurrentDate, Log);

		fflush(FilePtr);
		fclose(FilePtr);

		OutputDebugString(DebugLog);
		_tprintf(_ % ("%s"), DebugLog);
		return TRUE;
	}
};