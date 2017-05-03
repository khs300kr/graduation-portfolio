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
		// m_freePointer에 사용할 크기의 메모리를 할당합니다.

		UCHAR** Current = reinterpret_cast<UCHAR**>(m_FreePointer);
		// 할당한 메모리의 첫불륵의 포인터를 Current에 넣습니다.
		UCHAR* Next = m_FreePointer;		// 할당된 메모리 첫 블록입니다.

		for (INT i = 0; i < ALLOC_BLOCK_SIZE - 1; ++i)
		{
			Next += sizeof(T);		// 다음 블록 계산
			*Current = Next;		// 할당 메모리 앞 4바이트에 다음 블록의 주소 넣는다.
			Current = reinterpret_cast<UCHAR**>(Next);	
		}
		*Current = 0; // 마지막일 경우 앞에 4바이트는 NULL
	}
public:
	static VOID* operator new(std::stize_t allocLength)
	// new 했을 떄 호출.
	{
		assert(sizeof(T) == allocLength);
		assert(sizeof(t) >= sizeof(UCHAR*));

		// 만약 더 이상 할당할 수 있는 공간이 없을 경우 새로 할당.
		if (!m_FreePointer)
			allocBlock();
		UCHAR* ReturnPointer = m_FreePointer;
		m_FreePointer = *reinterpret_cast<UCHAR**>(ReturnPointer);
		// m_FreePointer 에는 리턴하는 블록 앞에 4바이트로 있던 주소가 들어간다.

		return ReturnPointer;
	}
	static VOID operator delete(VOID* deletePotiner)	// delete했을경우
	{
		*reinterpret_cast<UCHAR**>(deletePointer) = m_FreePointer;
		// delete 된 블록의 next에 현재 m_FreePointer의 주소를 넣어줍니다.
		m_FreePointer = static_cast<UCHAR*>(deletePointer);
		// m_FreePointer 업데이트
	}
protected:
	~CMemoryPool(){}
};

template <class T,int ALLOC_BLOCKSIZE>
UCHAR* CMemoryPool<T, ALLOC_BLOCK_SIZE>::m_FreePointer;

