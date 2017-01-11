#pragma once
#include "LinearQue.h"

class CQueApp
{
private:
	enum { MAX_QUE_BUF = 40, };		// 큐의 디폴트 크기
	CLinearQue*	m_pQue;				//CQueLinear 인스턴스
public:
	CQueApp();
	virtual ~CQueApp();
	int		Init(int iSize = MAX_QUE_BUF);		// 큐초기화
	int		Progress();
	void	Destroy();

private:
	void	EnqueingMode();
	void	DequeingMode();
	void	InputString(char* sMsg, int iSize,FILE* fp=stdin);
};

