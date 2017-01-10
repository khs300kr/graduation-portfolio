#pragma once
class CircularQue
{
private:
	int*		m_pBuf;	
	int			m_iFront;
	int			m_iRear;
	int			m_iSpace;		// 남아있는 버퍼
	int			m_iSize;
public:
	CircularQue();
	CircularQue(int	_iSize);
	virtual ~CircularQue();

	int Enque(int _iIn);
	int	Deque(int* _iOut);
};

