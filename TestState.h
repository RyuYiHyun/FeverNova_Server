#pragma once
#include "State.h"
class TestState : public State
{
public:
    TestState(Session* _session);
    virtual ~TestState() {}
    // ���¸� �غ��Ѵ�.
    virtual void OnEnter() override { }
    // ���¿� ���� Recv ���μ���
    virtual void OnRecv() override;
    virtual void OnSend() override;
    // ���¸� �����Ѵ�.
    virtual void OnExit() override { }
protected:
};

