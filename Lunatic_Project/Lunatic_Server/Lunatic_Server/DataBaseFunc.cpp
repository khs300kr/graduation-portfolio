#include "stdafx.h"
#include "Global.h"
#include "DataBaseFunc.h"
#include "PacketFunc.h"

char sql_buf[255];
wchar_t sql_data[255];

SQLLEN cbID{}, cbPasswrod{};
SQLWCHAR szID[ID_LEN]{};
SQLWCHAR szPassword[PASSWORD_LEN]{};

void Init_DB(void)
{
	// Allocate environment handle  
	retcode = SQLAllocHandle(SQL_HANDLE_ENV, SQL_NULL_HANDLE, &henv);

	// Set the ODBC version environment attribute  
	if (retcode == SQL_SUCCESS || retcode == SQL_SUCCESS_WITH_INFO) {
		retcode = SQLSetEnvAttr(henv, SQL_ATTR_ODBC_VERSION, (SQLPOINTER*)SQL_OV_ODBC3, 0);

		// Allocate connection handle  
		if (retcode == SQL_SUCCESS || retcode == SQL_SUCCESS_WITH_INFO) {
			retcode = SQLAllocHandle(SQL_HANDLE_DBC, henv, &hdbc);

			// Set login timeout to 5 seconds  
			if (retcode == SQL_SUCCESS || retcode == SQL_SUCCESS_WITH_INFO) {
				SQLSetConnectAttr(hdbc, SQL_LOGIN_TIMEOUT, (SQLPOINTER)5, 0);
			}
		}
	}
}

void Client_Login(char id[], char password[],int ci)
{
	// Connect to data source  
	retcode = SQLConnect(hdbc, (SQLWCHAR*)L"Lunatic_Project", SQL_NTS, (SQLWCHAR*)NULL, 0, NULL, 0);

	// Allocate statement handle  
	if (retcode == SQL_SUCCESS || retcode == SQL_SUCCESS_WITH_INFO)
	{
		retcode = SQLAllocHandle(SQL_HANDLE_STMT, hdbc, &hstmt);

		sprintf(sql_buf, "EXEC dbo.client_login %s, %s", id, password);
		MultiByteToWideChar(CP_UTF8, 0, sql_buf, strlen(sql_buf), sql_data, sizeof sql_data / sizeof *sql_data);
		sql_data[strlen(sql_buf)] = '\0';


		retcode = SQLExecDirect(hstmt, sql_data, SQL_NTS);

		if (retcode == SQL_SUCCESS || retcode == SQL_SUCCESS_WITH_INFO)
		{
			// Bind columns 1, 2, and 3  
			retcode = SQLBindCol(hstmt, 1, SQL_C_WCHAR, szID, ID_LEN, &cbID);
			retcode = SQLBindCol(hstmt, 2, SQL_C_WCHAR, szPassword, PASSWORD_LEN, &cbPasswrod);
				
			// Fetch and print each row of data. On an error, display a message and exit.  

			retcode = SQLFetch(hstmt);
			if (retcode == SQL_SUCCESS || retcode == SQL_SUCCESS_WITH_INFO)
			{
				wcout << "connect ID : " << szID << endl;
				SendIDPlayer(ci, ci);
			}
			else
			{
				cout << "No ID\n";
				SendLoginFailed(ci, ci);
			}
		}

		// Process data  
		if (retcode == SQL_SUCCESS || retcode == SQL_SUCCESS_WITH_INFO) {
			SQLCancel(hstmt);
			SQLFreeHandle(SQL_HANDLE_STMT, hstmt);
		}
		SQLDisconnect(hdbc);
	}
}

void Close_DB(void)
{
	SQLDisconnect(hdbc);
	SQLFreeHandle(SQL_HANDLE_DBC, hdbc);
	SQLFreeHandle(SQL_HANDLE_ENV, henv);
}