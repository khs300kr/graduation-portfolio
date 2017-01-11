#include <iostream>
#include "CircularQue.h"

using namespace std;

void  main()
{
	cout << "Simple Circular Que Program" << endl;
	
	CircularQue*	pQue = new CircularQue(20);

	for (int i = 0; i < 10; ++i)
	{
		for (int j = 0; j < 30; ++j)
		{
			pQue->Enque(i * 30 + j);
		}

		
		for (int j = 0; j < 30; ++j)
		{
			int iVal = 0;
			pQue->Deque(&iVal);
			cout << iVal << endl;
		}
	}
	delete pQue;
}

