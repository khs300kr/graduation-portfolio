#pragma once
class CLinearQue
{
private:
	int*		m_pBuf;				// ť�� ����
	int			m_iRear;			// �Է� ���ڿ� �� ��ġ
	int			m_iWidth;			// ť�� ���� ũ��

public:
	CLinearQue();
	CLinearQue(int _iWidth);
	virtual ~CLinearQue();

public:
	int			Enque(int _iIn);
	int			Deque(int* _iOut);

};

