#include <iostream>
#include "Que.h"

CQue::CQue()
{
	m_pBuf	   = NULL;
	m_iFront   = 0;
	m_iRear	   = 0;
	m_iRemain  = 0;
	m_iSize	   = 0;
}

CQue::CQue(int _iSize)
{
	m_iFront  = 0;
	m_iRear	  = 0;
	m_iRemain = _iSize;
	m_iSize	  = _iSize;

	m_pBuf = new NetWorkMsg[m_iSize + 1];
	memset(m_pBuf, 0, sizeof(NetWorkMsg)*(m_iSize + 1));
}

CQue::~CQue()
{
	if (m_pBuf)
	{
		delete[] m_pBuf;
		m_pBuf = NULL;
	}
}

int CQue::Enque(NetWorkMsg * pIn)
{

	memcpy(&m_pBuf[m_iRear], pIn, sizeof(NetWorkMsg));

	--m_iRemain;
	++m_iRear;
	m_iRear %= m_iSize;
	return 0;
}

int CQue::Deque(NetWorkMsg * pOut)
{


	memcpy(pOut, &m_pBuf[m_iFront], sizeof(NetWorkMsg));

	++m_iRemain;
	++m_iFront;
	m_iFront %= m_iSize;
	return 0;
}
