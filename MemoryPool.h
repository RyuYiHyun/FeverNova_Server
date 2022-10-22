#pragma once
#include <iostream>
#include <vector>
#include <assert.h>
#include "LockGuard.h"

template <class T, int PoolSize = 40>
class MemoryPool
{
protected:
	MemoryPool() {}
	virtual ~MemoryPool(){}
public:
	static void* operator new(std::size_t allocLength)
	{
		LockGuard l_lockGuard(&m_criticalKey);

		assert(sizeof(T) == allocLength);// 에러시 오류 확인
		assert(sizeof(T) >= sizeof(BYTE*));
		
		if (m_allocPointer == nullptr)
		{
			AllocBlock();
		}

		BYTE* newPointer = m_allocPointer;
		m_allocPointer = *reinterpret_cast<BYTE**>(newPointer);

		return newPointer;
	}
	static void	operator delete(void* deletePointer)
	{
		LockGuard l_lockGuard(&m_criticalKey);

		*reinterpret_cast<BYTE**>(deletePointer) = m_allocPointer;
		m_allocPointer = static_cast<BYTE*>(deletePointer);
	}

	static void ReleaseAllAllocedMemory()
	{
		LockGuard l_lockGuard(&m_criticalKey);

		for (size_t i = 0; i < m_allocedMemoryList.size(); ++i)
		{
			delete[] m_allocedMemoryList[i];
		}
		m_allocedMemoryList.clear();
	}
private:
	static void	AllocBlock()
	{
		m_allocPointer = new BYTE[sizeof(T) * PoolSize];
		m_allocedMemoryList.push_back(m_allocPointer);

		BYTE** Current = reinterpret_cast<BYTE**>(m_allocPointer);
		BYTE* Next = m_allocPointer;

		for (INT i = 0; i < PoolSize - 1; ++i)
		{
			Next += sizeof(T);
			*Current = Next;
			Current = reinterpret_cast<BYTE**>(Next);
		}

		*Current = nullptr;//0;
	}
protected:
	static CriticalKey m_criticalKey;
private:
	static BYTE* m_allocPointer;
	static std::vector<BYTE*> m_allocedMemoryList;
};

template <class T, int PoolSize>
CriticalKey MemoryPool<T, PoolSize>::m_criticalKey;

template <class T, int PoolSize>
BYTE* MemoryPool<T, PoolSize>::m_allocPointer;

template <class T, int PoolSize>
std::vector<BYTE*> MemoryPool<T, PoolSize>::m_allocedMemoryList;