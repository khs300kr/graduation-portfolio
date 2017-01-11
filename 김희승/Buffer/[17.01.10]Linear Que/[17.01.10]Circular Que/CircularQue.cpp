#include "CircularQue.h"

#include <iostream>

CircularQue::CircularQue()
{
	m_pBuf = NULL;
	m_iFront = 0;
	m_iRear = 0;
	m_iSpace = 0;
	m_iSize = 0;
}

CircularQue::CircularQue(int _iSize)
{
	m_iFront = 0;
	m_iRear = 0;
	m_iSpace = _iSize;
	m_iSize = _iSize;

	m_pBuf = new int[_iSize + 1];				
	memset(m_pBuf, 0, sizeof(int)*(m_iSize + 1));
}

CircularQue::~CircularQue()
{
	if (m_pBuf)
	{
		delete[] m_pBuf;
		m_pBuf = NULL;
	}
}

int CircularQue::Enque(int _iIn)
{
	if (m_iSpace == 0)		// IsFull?
		return -1;
	m_pBuf[m_iRear] = _iIn;

	--m_iSpace;

	++m_iRear;
	m_iRear %= m_iSize;
	return 0;
}

int CircularQue::Deque(int * _iOut)
{
	if (m_iSpace == m_iSize)	// ISEmpty?
		return -1;
	*_iOut = m_pBuf[m_iFront];

	++m_iSpace;

	++m_iFront;
	m_iFront %= m_iSize;

	return 0;
}
