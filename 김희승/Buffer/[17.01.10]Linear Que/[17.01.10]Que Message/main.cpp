#include <iostream>
#include "Que.h"

using namespace std;

void main()
{
	cout << "Simple Circular Que Program" << endl;
	
	CQue* pQue = new CQue(20);

	for (int i = 0; i < 10; ++i)
	{
		for (int j = 0; j < 30; ++j)
		{
			NetWorkMsg nwMsg;			// Socket & nMsg
			nwMsg.socket = i;
			nwMsg.nMsg = i * 30 + j;
			pQue->Enque(&nwMsg);
		}

		for (int j = 0; j < 30; ++j)
		{
			NetWorkMsg nwMsg;
			pQue->Deque(&nwMsg);

			cout << nwMsg.socket << "," << nwMsg.nMsg << endl;
		}
	}

	delete pQue;
}