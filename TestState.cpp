#include "TestState.h"
#include "LogManager.h"
#include "GameManager.h"
void TestState::OnRecv()
{
	// Test ���¿����� Recv ���μ���
	GameManager::GetInstance()->Function(m_session);
}

void TestState::OnSend()
{
	// Test���¿��� ���� ���·� ����ɶ� ���ǰ� �۾��� �ֱ�...
	
}

TestState::TestState(Session* _session) : State(_session)
{
}