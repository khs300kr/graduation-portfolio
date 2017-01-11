#include "RingBuf.h"
#include <iostream>

CRingBuf::CRingBuf()
{
	m_pBuf		= NULL;
	m_iFront	= 0;
	m_iRear		= 0;
	m_iRemain	= 0;
	m_iSize		= 0;
}

CRingBuf::CRingBuf(int iSize)
{
	m_pBuf		= NULL;
	m_iFront	= 0;
	m_iRear		= 0;
	m_iRemain	= iSize;
	m_iSize		= iSize;

	m_pBuf = new BYTE[m_iSize];
	memset(m_pBuf, 0, sizeof(BYTE)*m_iSize);
}


CRingBuf::~CRingBuf()
{
	if (m_pBuf)
	{
		delete[] m_pBuf;
		m_pBuf = NULL;
	}
}

bool CRingBuf::Enqueue(void * In, int iSize)
{
	if (m_iRemain < iSize || m_iRemain <= 2) // �Է� ������� �۰ų� �����ִ� ������ ������
		return false;

	BYTE*	pIn = (BYTE*)In;
	for (int i = 0; i < iSize; ++i)
	{
		*(m_pBuf + m_iRear) = *(pIn + i);	// ������ ���� , Rear �̵�
		++m_iRear;
		m_iRear %= m_iSize;
	}
	m_iRemain -= iSize;
	
	return true;
}

bool CRingBuf::Dequeue(void * Out, int * pSize)
{
	if (m_iRemain >= (m_iSize - 2)) // Is_Empty?
		return false;

	int iSize = m_pBuf[m_iFront];

	BYTE*	pOut = (BYTE*)Out;
	for (int i = 0; i < iSize; ++i)
	{
		*(pOut + i) = *(m_pBuf + m_iFront);		// ������ ����
		*(m_pBuf + m_iFront) = 0;				// ���� �� 0���� �ʱ�ȭ

		++m_iFront;
		m_iFront %= m_iSize;
	}

	m_iRemain += iSize;
	*pSize = iSize;		//
	return true;
}

void CRingBuf::Clear()
{
	m_iRear = 0;
	memset(m_pBuf, 0, sizeof(BYTE)*(m_iSize + 1));
}

void CRingBuf::Resize(int iSize)
{
	BYTE* pBuf = new BYTE[iSize];
	memset(pBuf, 0, sizeof(BYTE)*iSize);

	if (iSize <= m_iSize)	// ������ ���۰� �������� �������
	{						// ������ ���� ����
		for (int i = 0; i < iSize; ++i)
			*(pBuf++) = *(m_pBuf++);
	}
	else					// ������ ���۰� �������� Ŭ���
	{						// ������ ���� ����
		for (int i = 0; i < m_iSize; ++i)
			*(pBuf++) = *(m_pBuf++);
	}
	if (m_pBuf)
		delete[] m_pBuf;

	m_pBuf = pBuf;			// ���ο� ���� ������ ����
}

int CRingBuf::GetRemain()
{
	return (m_iSize - m_iRear);
}

int CRingBuf::GetSize()
{
	return m_iSize;
}
