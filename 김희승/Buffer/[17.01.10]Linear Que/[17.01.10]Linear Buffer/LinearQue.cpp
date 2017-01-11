#include <iostream>
#include "LinearQue.h"

using namespace std;

CLinearQue::CLinearQue()
{
	m_pBuf = NULL;
	m_iRear = 0;
	m_iWidth = 0;
}

CLinearQue::CLinearQue(int _iWidth)
{
	m_iRear = 0;
	m_iWidth = _iWidth;

	m_pBuf = new int[m_iWidth + 1];
	memset(m_pBuf, 0, sizeof(int)*(m_iWidth + 1));	
	// memeset(p,value,size)
	// p를 size만큼 value로 초기화.
}


CLinearQue::~CLinearQue()
{
	if (m_pBuf)
	{
		delete[] m_pBuf;
		m_pBuf = NULL;
	}
}

int CLinearQue::Enque(int _iIn)
{
	if (m_iRear == m_iWidth)		// IsFull?
		return -1;
	m_pBuf[m_iRear++] = _iIn;
	return 0;
}

int CLinearQue::Deque(int * _iOut)
{
	if (m_iRear == 0)		// IsEmpty?
		return -1;

	*_iOut = m_pBuf[0];

	for (int i = 0; i < m_iRear; ++i)
		*(m_pBuf + i) = *(m_pBuf + i + 1);		// Shift
	*(m_pBuf + m_iRear) = 0;					// 마지막자리 0 Init
	--m_iRear;									// Rear 위치 이동

	return 0;
}

