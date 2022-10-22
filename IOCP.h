#pragma once
#include "Global.h"

class IOCP
{
protected:
    virtual bool AcceptProcess(SOCKET _client) = 0;// 새 클라 접속했을떄 수행할 작업
    virtual bool RecvProcess(void* _session, int _completebyte) = 0;// Recv 완료후 수행할 작업
    virtual bool SendProcess(void* _session, int _completebyte) = 0;// Send 완료후 수행할 작업
    virtual void DisconnectedProcess(void* session) = 0;// 클라한테 종료 신호 받았을시 수행할 작업

    void IocpPortConnect(HANDLE _connectSokcet, HANDLE _hcp);

    bool Initialize();  // IOCP 초기화
    void Release();     // IOCP 후처리
protected:
    static DWORD WINAPI WorkerThread(LPVOID arg); // Worker Thread
    HANDLE m_hcp; // 입출력 완료 포트
};
