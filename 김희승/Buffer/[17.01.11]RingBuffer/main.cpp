#include <iostream>
#include <ctime>
#include <windows.h>
#include "RingBuf.h"

void main()
{
	srand((unsigned)time(NULL));

	CRingBuf* pRingBuf = new CRingBuf(45);
	char	str[] =
		"12345678901234567890abcdefghijabcdefghij"\
		"12345678901234567890abcdefghijabcdefghij"\
		"12345678901234567890abcdefghijabcdefghij"\
		"12345678901234567890abcdefghijabcdefghij"\
		"12345678901234567890abcdefghijabcdefghij"\

		"12345678901234567890abcdefghijabcdefghij"\
		"12345678901234567890abcdefghijabcdefghij"\
		"12345678901234567890abcdefghijabcdefghij"\
		"12345678901234567890abcdefghijabcdefghij"\
		"12345678901234567890abcdefghijabcdefghij"\

		"12345678901234567890abcdefghijabcdefghij"\
		"12345678901234567890abcdefghijabcdefghij"\
		"12345678901234567890abcdefghijabcdefghij"\
		"12345678901234567890abcdefghijabcdefghij"\
		"12345678901234567890abcdefghijabcdefghij"\

		"12345678901234567890abcdefghijabcdefghij"\
		"12345678901234567890abcdefghijabcdefghij"\
		"12345678901234567890abcdefghijabcdefghij"\
		"12345678901234567890abcdefghijabcdefghij"\
		"12345678901234567890abcdefghijabcdefghij"\

		"12345678901234567890abcdefghijabcdefghij"\
		"12345678901234567890abcdefghijabcdefghij"\
		"12345678901234567890abcdefghijabcdefghij"\
		"12345678901234567890abcdefghijabcdefghij"\
		"12345678901234567890abcdefghijabcdefghij"\

		"12345678901234567890abcdefghijabcdefghij"\
		"12345678901234567890abcdefghijabcdefghij"\
		"12345678901234567890abcdefghijabcdefghij"\
		"12345678901234567890abcdefghijabcdefghij"\
		"12345678901234567890abcdefghijabcdefghij"\
		;

	for (int i = 0; i < 1024; ++i)
	{
		char	sBuf[128] = { 0 };
		int		iSize = 10 + rand() % 20;

		strncpy(sBuf + 1, str + i, iSize);
		iSize = strlen(sBuf + 1) + 1;	// one for [0] Memory, one for Null

		sBuf[0] = iSize;
		pRingBuf->Enqueue(sBuf, sBuf[0]);

		memset(sBuf, 0, sizeof(sBuf));
		pRingBuf->Dequeue(sBuf, &iSize);

		iSize -= 1;

		printf("%3d, (%2d)%s\n", i, iSize, sBuf + 1);
	}
	//	pQueue->Dequeue();

	delete pRingBuf;
}