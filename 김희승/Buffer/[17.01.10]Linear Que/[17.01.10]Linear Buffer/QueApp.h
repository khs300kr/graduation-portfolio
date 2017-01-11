#pragma once
#include "LinearQue.h"

class CQueApp
{
private:
	enum { MAX_QUE_BUF = 40, };		// ť�� ����Ʈ ũ��
	CLinearQue*	m_pQue;				//CQueLinear �ν��Ͻ�
public:
	CQueApp();
	virtual ~CQueApp();
	int		Init(int iSize = MAX_QUE_BUF);		// ť�ʱ�ȭ
	int		Progress();
	void	Destroy();

private:
	void	EnqueingMode();
	void	DequeingMode();
	void	InputString(char* sMsg, int iSize,FILE* fp=stdin);
};

