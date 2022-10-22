#pragma once
#include "Global.h"
class Session;
class  State
{
public:
    State(Session* _session) { m_session = _session; }
    virtual ~State() {}
    // 상태를 준비한다.
    virtual void OnEnter() { }
    // 상태에 대한 Recv 프로세스
    virtual void OnRecv() {}
    virtual void OnSend() {}
    // 상태를 해제한다.
    virtual void OnExit() { }
protected:
    Session* m_session;
};