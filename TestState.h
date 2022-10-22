#pragma once
#include "State.h"
class TestState : public State
{
public:
    TestState(Session* _session);
    virtual ~TestState() {}
    // 상태를 준비한다.
    virtual void OnEnter() override { }
    // 상태에 대한 Recv 프로세스
    virtual void OnRecv() override;
    virtual void OnSend() override;
    // 상태를 해제한다.
    virtual void OnExit() override { }
protected:
};

