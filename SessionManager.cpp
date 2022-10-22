#include "SessionManager.h"
#include "LogManager.h"


#pragma region Singleton
bool SessionManager::CreateInstance()
{
	if (m_instance == nullptr)
	{
		m_instance = new SessionManager();
		return true;
	}
	else
	{
		return false;
	}
}

void SessionManager::DestroyInstance()
{
	if (m_instance != nullptr)
	{
		delete m_instance;
	}
}

bool SessionManager::Initialize()
{
	return true;
}

void SessionManager::Release()
{
	for (list<Session*>::iterator iter = m_sessionList.begin(); iter != m_sessionList.end();)
	{
		delete (*iter);
		iter = m_sessionList.erase(iter);
	}
	m_sessionList.clear();
}

SessionManager* SessionManager::GetInstance()
{
	if (m_instance != nullptr)
	{
		return m_instance;
	}
	else
	{
		return nullptr;
	}
}

SessionManager::SessionManager()
{
}

SessionManager::~SessionManager()
{
}

SessionManager* SessionManager::m_instance = nullptr;	// Singleton ��ü
#pragma endregion

Session* SessionManager::AddSession(SOCKET _socket)
{
	LockGuard l_lockGuard(&m_criticalKey);

	Session* session = new Session(_socket);
	if (session == nullptr)
	{
		LogManager::GetInstance()->LogWrite(3000);
		return nullptr;
	}
	if (!session->Initialize())
	{
		LogManager::GetInstance()->LogWrite(3001);
		return nullptr;
	}

	cout << "\n[TCP ����] Ŭ���̾�Ʈ ����: IP �ּ�=" << inet_ntoa(session->GetAddress().sin_addr)
		<< ", ��Ʈ ��ȣ=" << ntohs(session->GetAddress().sin_port) << endl;

	m_sessionList.push_back(session);

	return session;
}

void SessionManager::DeleteSession(Session* _session)
{
	LockGuard l_lockGuard(&m_criticalKey);

	for (list<Session*>::iterator iter = m_sessionList.begin(); iter != m_sessionList.end();)
	{
		if (_session == (*iter))
		{
			SOCKADDR_IN l_clientaddr;
			int l_addrlen = sizeof(l_clientaddr);
			getpeername(_session->GetSocket(), reinterpret_cast<SOCKADDR*>(&l_clientaddr), &l_addrlen);

			cout << "[TCP ����] Ŭ���̾�Ʈ ����: IP �ּ�=" << inet_ntoa(l_clientaddr.sin_addr)
				<< ", ��Ʈ ��ȣ=" << ntohs(l_clientaddr.sin_port) << endl;

			closesocket(_session->GetSocket());

			delete (*iter);
			iter = m_sessionList.erase(iter);
		}
		else
		{
			iter++;
		}
	}
}