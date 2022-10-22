#pragma once
#include "Global.h"
class Session;
class  State
{
public:
    State(Session* _session) { m_session = _session; }
    virtual ~State() {}
    // ���¸� �غ��Ѵ�.
    virtual void OnEnter() { }
    // ���¿� ���� Recv ���μ���
    virtual void OnRecv() {}
    virtual void OnSend() {}
    // ���¸� �����Ѵ�.
    virtual void OnExit() { }
protected:
    Session* m_session;
};