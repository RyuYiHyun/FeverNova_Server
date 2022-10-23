#include "TestState.h"
#include "LogManager.h"
#include "GameManager.h"
void TestState::OnRecv()
{
	// Test 상태에서의 Recv 프로세스
	GameManager::GetInstance()->Function(m_session);
}

void TestState::OnSend()
{
	// Test상태에서 다음 상태로 변경될때 조건과 작업을 넣기...
	
}

TestState::TestState(Session* _session) : State(_session)
{
}