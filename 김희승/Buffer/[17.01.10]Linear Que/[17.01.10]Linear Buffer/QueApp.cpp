#include <windows.h>
#include <iostream>
#include "QueApp.h"

#include "QueApp.h"

using namespace std;

CQueApp::CQueApp() : m_pQue(0) {}
CQueApp::~CQueApp(){}

int CQueApp::Init(int iSize)
{
	if (iSize < 0)
		return -1;
	m_pQue = new CLinearQue(iSize);
	return 0;
}

int CQueApp::Progress()
{
	while (true)
	{
		cout << "QUE PRACTICE" << endl;
		cout << "1. Enque" << endl;
		cout << "2. Deque" << endl;
		cout << "3. Resize Que" << endl;
		cout << "Q. Program Quit" << endl;

		char	sMsg[8] = { 0 };
		InputString(sMsg, 8);
		system("cls");

		if ('q' == sMsg[0] || 'Q' == sMsg[0])
		{
			cout << "����" << endl;
			break;
		}
		else if ('1' == sMsg[0])
			EnqueingMode();
		else if ('2' == sMsg[0])
			DequeingMode();
	}
	Destroy();
	return 0;
}

void CQueApp::Destroy()
{
	if (m_pQue)
	{
		delete m_pQue;
		m_pQue = NULL;
	}
}

void CQueApp::EnqueingMode()
{
	cout << "<<Enqueing Mode>>" << endl;
	while (true)
	{
		cout << endl << endl;
		cout << "ť�� �Է��� ���� �Է�" << endl;
		cout << " �����޴��� Q" << endl;;

		char	sMsg[256] = { 0 };
		InputString(sMsg, 256);
		system("cls");
		if ('q' == sMsg[0] || 'Q' == sMsg[0])
			break;

		int iSize = strlen(sMsg);
		int iReturn = m_pQue->Enque(sMsg[0]);

		// IF_FULL
		if (iReturn < 0)
		{
			char*	p = sMsg;
			p += iSize - iReturn;
			cout << "Que is full" << endl;
			cout << "Data Remain " << p << endl;
		}
	}
}

void CQueApp::DequeingMode()
{
	cout << "<<Dequeing Mode>>" << endl;
	while (true)
	{
		cout << "ť���� �����͸� ���������� Enter Ű �Է�" << endl;
		char	sMsg[256] = { 0 };
		InputString(sMsg, 256);
		system("cls");
		if ('q' == sMsg[0] || 'Q' == sMsg[0])
			break;

		int iVal = 0;
		int hr = m_pQue->Deque(&iVal);

		if (hr < 0)
		{
			cout << "Empty!" << endl;
			continue;
		}
		cout << (char)iVal << endl;
	}
}

void CQueApp::InputString(char * sMsg, int iSize, FILE * fp)
{
	char	sTemp[512] = { 0 };
	char*p = sTemp;

	// stream���� ���ڿ��� �޴´�
	// 1. MAXDATA - 1
	// 2. ���๮��
	// 3. eof
	fgets(sTemp, 512, fp);
	// 1���� - �о���� ���ڿ� ����
	// 2���� - ������
	// 3���� - stdin ǥ���Է�

	for (int i = 0; i < 512, *p != 0; ++i, ++p)
	{
		if (*p == '\n' || *p == '\r')
		{
			*p = 0;
			break;
		}
	}
	memcpy(sMsg, sTemp, iSize);
}

