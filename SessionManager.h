#pragma once
#include "Session.h"
#include "LockGuard.h"
class SessionManager
{
#pragma region Singleton
public:
	static bool CreateInstance();
	static void DestroyInstance();
	bool Initialize();
	void Release();
	static SessionManager* GetInstance();
private:
	static SessionManager* m_instance;	

	SessionManager();
	virtual ~SessionManager();
#pragma endregion
public:
	Session* AddSession(SOCKET _socket);
	void DeleteSession(Session* _session);
private:	
	CriticalKey m_criticalKey;
	list<Session*> m_sessionList;			
};