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
	// 하위 매니저들 생성하는 곳입니다...
	ConsoleControlHandler::CreateInstance();
	LogManager::CreateInstance();
	//DataBaseManager::CreateInstance();
	//LoginManager::CreateInstance();
	SessionManager::CreateInstance();
	RoomManager::CreateInstance();
	GameManager::CreateInstance();
	return true; // 성공적 생성
}
//
void MainManager::DestroyInstance()
{
	// 하위 매니저 삭제
	GameManager::DestroyInstance();
	RoomManager::DestroyInstance();
	SessionManager::DestroyInstance();
	//LoginManager::DestroyInstance();
	//DataBaseManager::DestroyInstance();
	LogManager::DestroyInstance();
	ConsoleControlHandler::DestroyInstance();
	// 인스턴스가 null이 아닐떄
	if (m_instance != nullptr)
	{
		delete m_instance;
	}
}
//
bool MainManager::Initialize()// 초기화
{
	// 윈속 초기화
	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
	{
		return false;
	}
	
	m_listenSock.ListenStart(INADDR_ANY, SERVERPORT);// Listen and Bind 작업
	if (!IOCP::Initialize()) // IOCP 관련 초기작업
	{
		cout << "IOCP 초기작업 오류" << endl;
		return false;
	}
	// 하위 매니저 초기작업...
	ConsoleControlHandler::GetInstance()->Initialize();
	LogManager::GetInstance()->Initialize();
	//DataBaseManager::GetInstance()->Initialize();
	//LoginManager::GetInstance()->Initialize();
	SessionManager::GetInstance()->Initialize();
	GameManager::GetInstance()->Initialize();
	return true;
}
//
void MainManager::Release() // 뒤처리
{
	// 하위 매니저 뒷정리... (생성과 역순)
	GameManager::GetInstance()->Release();

	//BaseStream::ReleaseAllAllocedMemory();

	SessionManager::GetInstance()->Release();
	//LoginManager::GetInstance()->Release();
	//DataBaseManager::GetInstance()->Release();
	LogManager::GetInstance()->Release();
	ConsoleControlHandler::GetInstance()->Release();

	IOCP::Release(); // IOCP 후처리
	// 윈속 종료
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
MainManager::MainManager() // 생성자
{
}
//
MainManager::~MainManager() // 소멸자
{
}
//
MainManager* MainManager::m_instance = nullptr; // Singleton 객체
#pragma endregion

void MainManager::Run() // 서버 실행
{
	while (1) {
		SOCKET l_clientSock;				// 클라이언트와 연결된 소켓
		SOCKADDR_IN l_clientAddress;		// 클라이언트의 주소
		net::OVERLAPPEDEX* l_acceptOverlappedEX;	// accpet 확장 구조체 (accpet 처리 동안 임시)
		l_clientSock = m_listenSock.Accept(); // Accpet 대기 (동기)
		if (l_clientSock == INVALID_SOCKET)
		{
			continue;// 소켓이 없으면 다시 accpet 대기
		}
		l_acceptOverlappedEX = new net::OVERLAPPEDEX(); // 구조체 생성
		l_acceptOverlappedEX->operation = net::E_OPERATION::ACCEPT;
		// accept 완료했다는 의미의 완료통지를 완료포트에 추가
		PostQueuedCompletionStatus(m_hcp, -1, l_clientSock, &l_acceptOverlappedEX->overlapped);
	}
}


bool MainManager::RecvProcess(void* _session, int _completeByte) // Recv완료시 작업
{
	Session* session = reinterpret_cast<Session*>(_session);
	Session::E_RESULT retval = session->RecvCompleteCheck(_completeByte);

	switch (retval)
	{
		// recvcount와 패킷번호가 일치하지 않은 경우
	case Session::E_RESULT::PACKETNUM_MISMATCH:
		// 다시 Recv할수있게 하기위해 RecvPacket호출
		session->RecvPacket();
		return false;
		// 클라와의 연결이 끊어진 경우
	case Session::E_RESULT::DISCONNECTED:
		// 목표 용량만큼 수신이 되지 않았을 경우
	case Session::E_RESULT::FAIL:
		return false;
		// 목표 용량만큼 수신을 완료한 경우
	case Session::E_RESULT::SUCCESS:
		break;
	}

	// session에 상태에 따른 Recv Process 변경
	session->GetState()->OnRecv();



	// 다음 Recv 대기
	if (!session->RecvPacket())
	{
		return false;
	}
}


bool MainManager::SendProcess(void* _session, int _completeByte)// Send 완료시 수행할 작업
{
	Session* session = reinterpret_cast<Session*>(_session);

	Session::E_RESULT retval = session->SendCompleteCheck(_completeByte);

	switch (retval)
	{
		// 클라와의 연결 종료되었을시
	case Session::E_RESULT::DISCONNECTED:
		// 목표 용량만큼 송신이 되지 않았을 경우
	case Session::E_RESULT::FAIL:
		return false;
		// 목표 용량만큼 송신이 완료된 경우
	case Session::E_RESULT::SUCCESS:
		break;
	}
	return true;
}

bool MainManager::AcceptProcess(SOCKET _clientSock)
{
	Session* session = SessionManager::GetInstance()->AddSession(_clientSock);
	// Session 객체가 제대로 동적할당되지 않은 경우
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

// 클라 연결 종료시 수행할 작업
void MainManager::DisconnectedProcess(void* _session)
{
	Session* session = reinterpret_cast<Session*>(_session);
	// 로그인 메니저에서 로그인됬으면 로그인 정보 삭제
	GameManager::GetInstance()->ForceExitProcess(session);
	SessionManager::GetInstance()->DeleteSession(session);
}