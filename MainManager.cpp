#include "MainManager.h"
#include "ConsoleControlHandler.h"
#include "Session.h"
#include "LoginManager.h"
#include "SessionManager.h"
#include "DataBaseManager.h"
#include "LogManager.h"
#include "GameManager.h"
#include "RoomManager.h"
#pragma region Singleton
bool MainManager::CreateInstance()
{
	if (m_instance == nullptr)
	{
		m_instance = new MainManager();
	}
	else
	{
		return false;
	}
	// ���� �Ŵ����� �����ϴ� ���Դϴ�...
	ConsoleControlHandler::CreateInstance();
	LogManager::CreateInstance();
	//DataBaseManager::CreateInstance();
	//LoginManager::CreateInstance();
	SessionManager::CreateInstance();
	RoomManager::CreateInstance();
	GameManager::CreateInstance();
	return true; // ������ ����
}
//
void MainManager::DestroyInstance()
{
	// ���� �Ŵ��� ����
	GameManager::DestroyInstance();
	RoomManager::DestroyInstance();
	SessionManager::DestroyInstance();
	//LoginManager::DestroyInstance();
	//DataBaseManager::DestroyInstance();
	LogManager::DestroyInstance();
	ConsoleControlHandler::DestroyInstance();
	// �ν��Ͻ��� null�� �ƴҋ�
	if (m_instance != nullptr)
	{
		delete m_instance;
	}
}
//
bool MainManager::Initialize()// �ʱ�ȭ
{
	// ���� �ʱ�ȭ
	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
	{
		return false;
	}
	
	m_listenSock.ListenStart(INADDR_ANY, SERVERPORT);// Listen and Bind �۾�
	if (!IOCP::Initialize()) // IOCP ���� �ʱ��۾�
	{
		cout << "IOCP �ʱ��۾� ����" << endl;
		return false;
	}
	// ���� �Ŵ��� �ʱ��۾�...
	ConsoleControlHandler::GetInstance()->Initialize();
	LogManager::GetInstance()->Initialize();
	//DataBaseManager::GetInstance()->Initialize();
	//LoginManager::GetInstance()->Initialize();
	SessionManager::GetInstance()->Initialize();
	GameManager::GetInstance()->Initialize();
	return true;
}
//
void MainManager::Release() // ��ó��
{
	// ���� �Ŵ��� ������... (������ ����)
	GameManager::GetInstance()->Release();

	//BaseStream::ReleaseAllAllocedMemory();

	SessionManager::GetInstance()->Release();
	//LoginManager::GetInstance()->Release();
	//DataBaseManager::GetInstance()->Release();
	LogManager::GetInstance()->Release();
	ConsoleControlHandler::GetInstance()->Release();

	IOCP::Release(); // IOCP ��ó��
	// ���� ����
	WSACleanup();
}
//
MainManager* MainManager::GetInstance()
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
//
MainManager::MainManager() // ������
{
}
//
MainManager::~MainManager() // �Ҹ���
{
}
//
MainManager* MainManager::m_instance = nullptr; // Singleton ��ü
#pragma endregion

void MainManager::Run() // ���� ����
{
	while (1) {
		SOCKET l_clientSock;				// Ŭ���̾�Ʈ�� ����� ����
		SOCKADDR_IN l_clientAddress;		// Ŭ���̾�Ʈ�� �ּ�
		net::OVERLAPPEDEX* l_acceptOverlappedEX;	// accpet Ȯ�� ����ü (accpet ó�� ���� �ӽ�)
		l_clientSock = m_listenSock.Accept(); // Accpet ��� (����)
		if (l_clientSock == INVALID_SOCKET)
		{
			continue;// ������ ������ �ٽ� accpet ���
		}
		l_acceptOverlappedEX = new net::OVERLAPPEDEX(); // ����ü ����
		l_acceptOverlappedEX->operation = net::E_OPERATION::ACCEPT;
		// accept �Ϸ��ߴٴ� �ǹ��� �Ϸ������� �Ϸ���Ʈ�� �߰�
		PostQueuedCompletionStatus(m_hcp, -1, l_clientSock, &l_acceptOverlappedEX->overlapped);
	}
}


bool MainManager::RecvProcess(void* _session, int _completeByte) // Recv�Ϸ�� �۾�
{
	Session* session = reinterpret_cast<Session*>(_session);
	Session::E_RESULT retval = session->RecvCompleteCheck(_completeByte);

	switch (retval)
	{
		// recvcount�� ��Ŷ��ȣ�� ��ġ���� ���� ���
	case Session::E_RESULT::PACKETNUM_MISMATCH:
		// �ٽ� Recv�Ҽ��ְ� �ϱ����� RecvPacketȣ��
		session->RecvPacket();
		return false;
		// Ŭ����� ������ ������ ���
	case Session::E_RESULT::DISCONNECTED:
		// ��ǥ �뷮��ŭ ������ ���� �ʾ��� ���
	case Session::E_RESULT::FAIL:
		return false;
		// ��ǥ �뷮��ŭ ������ �Ϸ��� ���
	case Session::E_RESULT::SUCCESS:
		break;
	}

	// session�� ���¿� ���� Recv Process ����
	session->GetState()->OnRecv();



	// ���� Recv ���
	if (!session->RecvPacket())
	{
		return false;
	}
}


bool MainManager::SendProcess(void* _session, int _completeByte)// Send �Ϸ�� ������ �۾�
{
	Session* session = reinterpret_cast<Session*>(_session);

	Session::E_RESULT retval = session->SendCompleteCheck(_completeByte);

	switch (retval)
	{
		// Ŭ����� ���� ����Ǿ�����
	case Session::E_RESULT::DISCONNECTED:
		// ��ǥ �뷮��ŭ �۽��� ���� �ʾ��� ���
	case Session::E_RESULT::FAIL:
		return false;
		// ��ǥ �뷮��ŭ �۽��� �Ϸ�� ���
	case Session::E_RESULT::SUCCESS:
		break;
	}
	return true;
}

bool MainManager::AcceptProcess(SOCKET _clientSock)
{
	Session* session = SessionManager::GetInstance()->AddSession(_clientSock);
	// Session ��ü�� ����� �����Ҵ���� ���� ���
	if (session == nullptr)
	{
		return false;
	}

	IOCP::IocpPortConnect(reinterpret_cast<HANDLE>(_clientSock), m_hcp);

	if (!session->RecvPacket())
	{
		return false;
	}

	// TestManager :: E_PROTOCOL :: CRYPTOKEY => 0
	session->InitCryptoKeySend((int)GameManager::E_PROTOCOL::CRYPTOKEY);
}

// Ŭ�� ���� ����� ������ �۾�
void MainManager::DisconnectedProcess(void* _session)
{
	Session* session = reinterpret_cast<Session*>(_session);
	// �α��� �޴������� �α��Ή����� �α��� ���� ����
	GameManager::GetInstance()->ForceExitProcess(session);
	SessionManager::GetInstance()->DeleteSession(session);
}