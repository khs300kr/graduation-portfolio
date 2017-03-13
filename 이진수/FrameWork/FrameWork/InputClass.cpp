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
	// ��� Ű���� �Ҹ��� ���� ���·� �ʱ�ȭ �Ѵ�.
	for (int i = 0; i < 256; ++i)
	{
		m_keys[i] = false;
	}

	return;
}

void CInputClass::KeyDown(unsigned int input)
{
	// Ű�� ���ȴٸ� �� ���¸� �迭�� �����Ѵ�.
	m_keys[input] = true;
	return;
}

void CInputClass::KeyUp(unsigned int input)
{
	// Ű�� �������ٸ� �� ���¸� �迭�� �����Ѵ�.
	m_keys[input] = false;
	return;
}

bool CInputClass::IsKeyDown(unsigned int key)
{
	// ���� Ű�� ����/�� �������� ��ȯ�Ѵ�.
	return m_keys[key];
}