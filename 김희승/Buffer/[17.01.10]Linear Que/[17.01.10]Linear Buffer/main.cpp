// 1번 선형큐

#include <iostream>
#include <windows.h>
#include "QueApp.h"

using namespace std;

void main()
{
	CQueApp App;

	cout << "Simple Linear Que" << endl;

	// 큐의 최대 크기 (40바이트)-20문자
	App.Init();

	App.Progress();
}