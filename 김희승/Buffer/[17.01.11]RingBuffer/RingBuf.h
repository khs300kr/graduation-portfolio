#pragma once

typedef unsigned char	BYTE;

class CRingBuf
{
private:
	BYTE*		m_pBuf;
	int			m_iFront;
	int			m_iRear;
	int			m_iRemain;
	int			m_iSize;

public:
	CRingBuf();
	CRingBuf(int iSize);
	virtual ~CRingBuf();
	
public:
	bool	Enqueue(void* In, int iSize);	 // ������ �߰�(ret = �߰� ���� ������ ������)
	bool	Dequeue(void* Out, int* iSize);	 // ������ ��������(ret = ������ ������)

	void	Clear();						 
	void	Resize(int iSize);				 
	int		GetRemain();
	int		GetSize();
};

