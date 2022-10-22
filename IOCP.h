#pragma once
#include "Global.h"

class IOCP
{
protected:
    virtual bool AcceptProcess(SOCKET _client) = 0;// �� Ŭ�� ���������� ������ �۾�
    virtual bool RecvProcess(void* _session, int _completebyte) = 0;// Recv �Ϸ��� ������ �۾�
    virtual bool SendProcess(void* _session, int _completebyte) = 0;// Send �Ϸ��� ������ �۾�
    virtual void DisconnectedProcess(void* session) = 0;// Ŭ������ ���� ��ȣ �޾����� ������ �۾�

    void IocpPortConnect(HANDLE _connectSokcet, HANDLE _hcp);

    bool Initialize();  // IOCP �ʱ�ȭ
    void Release();     // IOCP ��ó��
protected:
    static DWORD WINAPI WorkerThread(LPVOID arg); // Worker Thread
    HANDLE m_hcp; // ����� �Ϸ� ��Ʈ
};
