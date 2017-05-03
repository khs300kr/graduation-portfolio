#pragma once
#include "stdafx.h"

template<class T, int ALLOC_BLOCK_SIZE = 50>
class CMemoryPool
{
private:
	static UCHAR *m_FreePointer;
	static void allocBlock()
	{
		m_FreePointer = new UCHAR[sizeof(T) * ALLOC_BLOCK_SIZE];
		// m_freePointer�� ����� ũ���� �޸𸮸� �Ҵ��մϴ�.

		UCHAR** Current = reinterpret_cast<UCHAR**>(m_FreePointer);
		// �Ҵ��� �޸��� ù�Ҹ��� �����͸� Current�� �ֽ��ϴ�.
		UCHAR* Next = m_FreePointer;		// �Ҵ�� �޸� ù ����Դϴ�.

		for (INT i = 0; i < ALLOC_BLOCK_SIZE - 1; ++i)
		{
			Next += sizeof(T);		// ���� ��� ���
			*Current = Next;		// �Ҵ� �޸� �� 4����Ʈ�� ���� ����� �ּ� �ִ´�.
			Current = reinterpret_cast<UCHAR**>(Next);	
		}
		*Current = 0; // �������� ��� �տ� 4����Ʈ�� NULL
	}
public:
	static VOID* operator new(std::stize_t allocLength)
	// new ���� �� ȣ��.
	{
		assert(sizeof(T) == allocLength);
		assert(sizeof(t) >= sizeof(UCHAR*));

		// ���� �� �̻� �Ҵ��� �� �ִ� ������ ���� ��� ���� �Ҵ�.
		if (!m_FreePointer)
			allocBlock();
		UCHAR* ReturnPointer = m_FreePointer;
		m_FreePointer = *reinterpret_cast<UCHAR**>(ReturnPointer);
		// m_FreePointer ���� �����ϴ� ��� �տ� 4����Ʈ�� �ִ� �ּҰ� ����.

		return ReturnPointer;
	}
	static VOID operator delete(VOID* deletePotiner)	// delete�������
	{
		*reinterpret_cast<UCHAR**>(deletePointer) = m_FreePointer;
		// delete �� ����� next�� ���� m_FreePointer�� �ּҸ� �־��ݴϴ�.
		m_FreePointer = static_cast<UCHAR*>(deletePointer);
		// m_FreePointer ������Ʈ
	}
protected:
	~CMemoryPool(){}
};

template <class T,int ALLOC_BLOCKSIZE>
UCHAR* CMemoryPool<T, ALLOC_BLOCK_SIZE>::m_FreePointer;

