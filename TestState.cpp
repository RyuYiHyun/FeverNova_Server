#include "TestState.h"
#include "LogManager.h"
#include "TestManager.h"
void TestState::OnRecv()
{
	// Test ���¿����� Recv ���μ���
	TestManager::GetInstance()->Function(m_session);
}

void TestState::OnSend()
{
	// Test���¿��� ���� ���·� ����ɶ� ���ǰ� �۾��� �ֱ�...
	
}

TestState::TestState(Session* _session) : State(_session)
{
}