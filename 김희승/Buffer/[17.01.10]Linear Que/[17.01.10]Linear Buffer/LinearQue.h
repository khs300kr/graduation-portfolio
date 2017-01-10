#pragma once
class CLinearQue
{
private:
	int*		m_pBuf;				// 큐의 버퍼
	int			m_iRear;			// 입력 문자열 끝 위치
	int			m_iWidth;			// 큐의 버퍼 크기

public:
	CLinearQue();
	CLinearQue(int _iWidth);
	virtual ~CLinearQue();

public:
	int			Enque(int _iIn);
	int			Deque(int* _iOut);

};

