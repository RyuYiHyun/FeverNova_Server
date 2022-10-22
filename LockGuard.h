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
	CRITICAL_SECTION m_key; // �Ӱ� ����
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


