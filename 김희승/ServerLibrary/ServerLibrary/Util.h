#pragma once
#include <algorithm>

// string �ʱ�ȭ
#define UNDEFINE_NAME		L"Undefine_Name"

// ���� ���� �� üũ
#define fixInRange(minimum,x,maximum)	min(maximum,max(x,minimum))
#define isInRange(minimum,x,maximum)	(x = fixInRange(minimum,x,maximum))?true:false

// overflow üũ
inline bool isOverFlower_uint(unsigned int original, unsigned int add)
{
	unsigned int before = original;
	unsigned int after = original + add;
	if ((original & 0x80000000) != (after & 0x80000000)) {
		return false;
	}
	return true;
}
