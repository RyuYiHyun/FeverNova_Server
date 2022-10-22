#pragma once
#include <windows.h>
class CriticalKey
{
public:
	CriticalKey()
	{
		InitializeCriticalSection(&m_key);
	}
	virtual ~CriticalKey()
	{
		DeleteCriticalSection(&m_key);
	}
	void Locking()
	{
		EnterCriticalSection(&m_key);
	}
	void UnLocking()
	{
		LeaveCriticalSection(&m_key);
	}
public:
	CRITICAL_SECTION m_key; // 임계 영역
};




class LockGuard
{
public:
	LockGuard(CriticalKey* _key)
	{
		m_key = _key;
		m_key->Locking();
	};
	virtual ~LockGuard()
	{
		m_key->UnLocking();
	};
private:
	CriticalKey* m_key;
};


