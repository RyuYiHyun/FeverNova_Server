#include "IOCP.h"
#include "LogManager.h"
// �Ϸ�
DWORD CALLBACK IOCP::WorkerThread(LPVOID arg)
{
	IOCP* l_iocp = reinterpret_cast<IOCP*>(arg);
	while (1)
	{
		DWORD l_cbTransferred; // Send, Recv �Ϸ� �뷮
		net::OVERLAPPEDEX* l_overlappedEx;// Ȯ�� overlap ����ü ������
		LONG l_competionKey;// �Ϸ����� ��ü�� ���� �Ƿ����� �ΰ����� (competionkey)

		int l_value = GetQueuedCompletionStatus(l_iocp->m_hcp, &l_cbTransferred,
			reinterpret_cast<PULONG_PTR>(&l_competionKey), reinterpret_cast<LPOVERLAPPED*>(&l_overlappedEx), INFINITE);

		// GetQueuedCompletionStatus ���н� l_value 0 ���� -> Ŭ���̾�Ʈ ����
		// cbTransferred == 0  Ŭ���̾�Ʈ ���� ����
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
			l_iocp->AcceptProcess(static_cast<SOCKET>(l_competionKey));// accept ��
			delete l_overlappedEx;
			break;
		case net::E_OPERATION::SEND:
			l_iocp->SendProcess(l_overlappedEx->session, l_cbTransferred); // send �Ϸ��
			break;
		case net::E_OPERATION::RECV:
			l_iocp->RecvProcess(l_overlappedEx->session, l_cbTransferred); // recv �Ϸ��
			break;
		}
	}
	return 0;
}

void IOCP::IocpPortConnect(HANDLE _connectSokcet, HANDLE _hcp)
{
	// �Ϸ���Ʈ ���ϰ� ����
	CreateIoCompletionPort(reinterpret_cast<HANDLE>(_connectSokcet), _hcp, NULL, 0);
}

bool IOCP::Initialize()
{
	// ����� �Ϸ� ��Ʈ ����
	m_hcp = CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, 0, 0);
	if (m_hcp == NULL)
	{
		cout << "��Ʈ ���� ����" << endl;
		return false;// ��Ʈ ���� ����
	}
	// CPU ���� Ȯ��
	SYSTEM_INFO l_si;
	GetSystemInfo(&l_si);
	// (CPU ���� * 2)���� �۾��� ������ ����
	HANDLE l_hThread;
	for (int i = 0; i < static_cast<int>(l_si.dwNumberOfProcessors) * 2; i++)
	{
		l_hThread = CreateThread(NULL, 0, IOCP::WorkerThread, this, 0, NULL);
		if (l_hThread == NULL)
		{
			cout << "workerthread ���� ����" << endl;
			return false; // ������ ���� ����
		}
		CloseHandle(l_hThread);
	}
	return true;
}

void IOCP::Release()
{
	// ��ó�� �۾��� ��������...
	CloseHandle(m_hcp); // ��Ʈ �ݱ�
}
