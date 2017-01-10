#pragma once

struct NetWorkMsg
{
	unsigned int socket;
	unsigned int nMsg;
};

class CQue
{
private:
	NetWorkMsg*		m_pBuf;
	int				m_iFront;
	int				m_iRear;
	int				m_iRemain;
	int				m_iSize;
public:
	CQue();
	CQue(int _iSize);
	virtual ~CQue();

	int		Enque(NetWorkMsg* pIn);
	int		Deque(NetWorkMsg* pOut);
};

