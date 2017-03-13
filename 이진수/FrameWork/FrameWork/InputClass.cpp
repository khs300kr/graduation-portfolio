#include "InputClass.h"



CInputClass::CInputClass()
{
}

CInputClass::CInputClass(const CInputClass& other)
{
}


CInputClass::~CInputClass()
{
}

void CInputClass::Initialize()
{
	// 모든 키들을 불리지 않은 상태로 초기화 한다.
	for (int i = 0; i < 256; ++i)
	{
		m_keys[i] = false;
	}

	return;
}

void CInputClass::KeyDown(unsigned int input)
{
	// 키가 눌렸다면 그 상태를 배열에 저장한다.
	m_keys[input] = true;
	return;
}

void CInputClass::KeyUp(unsigned int input)
{
	// 키가 떼어졌다면 그 상태를 배열에 저장한다.
	m_keys[input] = false;
	return;
}

bool CInputClass::IsKeyDown(unsigned int key)
{
	// 현재 키가 눌림/떔 상태인지 반환한다.
	return m_keys[key];
}