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
	bool	Enqueue(void* In, int iSize);	 // 데이터 추가(ret = 추가 못한 나머지 사이즈)
	bool	Dequeue(void* Out, int* iSize);	 // 데이터 가져오기(ret = 꺼내온 사이즈)

	void	Clear();						 
	void	Resize(int iSize);				 
	int		GetRemain();
	int		GetSize();
};

