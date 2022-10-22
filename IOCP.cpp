#include "IOCP.h"
#include "LogManager.h"
// 완료
DWORD CALLBACK IOCP::WorkerThread(LPVOID arg)
{
	IOCP* l_iocp = reinterpret_cast<IOCP*>(arg);
	while (1)
	{
		DWORD l_cbTransferred; // Send, Recv 완료 용량
		net::OVERLAPPEDEX* l_overlappedEx;// 확장 overlap 구조체 포인터
		LONG l_competionKey;// 완료통지 객체에 같이 실려오는 부가정보 (competionkey)

		int l_value = GetQueuedCompletionStatus(l_iocp->m_hcp, &l_cbTransferred,
			reinterpret_cast<PULONG_PTR>(&l_competionKey), reinterpret_cast<LPOVERLAPPED*>(&l_overlappedEx), INFINITE);

		// GetQueuedCompletionStatus 실패시 l_value 0 리턴 -> 클라이언트 종료
		// cbTransferred == 0  클라이언트 정상 종료
		if (l_value == 0 || l_cbTransferred == 0)
		{
			if (l_value == 0)
			{
				DWORD l_transfer, l_flag;
				WSAGetOverlappedResult(static_cast<SOCKET>(l_competionKey), &l_overlappedEx->overlapped, &l_transfer, FALSE, &l_flag);
			}
			if (l_overlappedEx != nullptr)
			{
				l_iocp->DisconnectedProcess(l_overlappedEx->session);
			}
			continue;
		}
		switch (l_overlappedEx->operation)
		{
		case net::E_OPERATION::ACCEPT:
			l_iocp->AcceptProcess(static_cast<SOCKET>(l_competionKey));// accept 됨
			delete l_overlappedEx;
			break;
		case net::E_OPERATION::SEND:
			l_iocp->SendProcess(l_overlappedEx->session, l_cbTransferred); // send 완료됨
			break;
		case net::E_OPERATION::RECV:
			l_iocp->RecvProcess(l_overlappedEx->session, l_cbTransferred); // recv 완료됨
			break;
		}
	}
	return 0;
}

void IOCP::IocpPortConnect(HANDLE _connectSokcet, HANDLE _hcp)
{
	// 완료포트 소켓과 연결
	CreateIoCompletionPort(reinterpret_cast<HANDLE>(_connectSokcet), _hcp, NULL, 0);
}

bool IOCP::Initialize()
{
	// 입출력 완료 포트 생성
	m_hcp = CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, 0, 0);
	if (m_hcp == NULL)
	{
		cout << "포트 생성 실패" << endl;
		return false;// 포트 생성 실패
	}
	// CPU 개수 확인
	SYSTEM_INFO l_si;
	GetSystemInfo(&l_si);
	// (CPU 개수 * 2)개의 작업자 스레드 생성
	HANDLE l_hThread;
	for (int i = 0; i < static_cast<int>(l_si.dwNumberOfProcessors) * 2; i++)
	{
		l_hThread = CreateThread(NULL, 0, IOCP::WorkerThread, this, 0, NULL);
		if (l_hThread == NULL)
		{
			cout << "workerthread 생성 실패" << endl;
			return false; // 스레드 생성 실패
		}
		CloseHandle(l_hThread);
	}
	return true;
}

void IOCP::Release()
{
	// 후처리 작업을 넣으세요...
	CloseHandle(m_hcp); // 포트 닫기
}
