#include "ConsoleControlHandler.h"
#include "MainManager.h"
#pragma region Singleton
bool ConsoleControlHandler::CreateInstance()
{
	if (m_instance == nullptr)
	{
		m_instance = new ConsoleControlHandler();
	}
	else
	{
		return false;
	}
	return true; // 성공적 생성
}

void ConsoleControlHandler::DestroyInstance()
{
	if (m_instance != nullptr)
	{
		delete m_instance;
	}
}

bool ConsoleControlHandler::Initialize()
{
	SetConsoleCtrlHandler((PHANDLER_ROUTINE)ControlHandler, TRUE);
	return true;
}

void ConsoleControlHandler::Release()
{

}

ConsoleControlHandler* ConsoleControlHandler::GetInstance()
{
	if (m_instance != nullptr)
	{
		return m_instance;
	}
	else
	{
		return nullptr;
	}
}

ConsoleControlHandler::ConsoleControlHandler()
{
}

ConsoleControlHandler::~ConsoleControlHandler()
{
}

ConsoleControlHandler* ConsoleControlHandler::m_instance = nullptr; // Singleton 객체
#pragma endregion



BOOL ConsoleControlHandler::ControlHandler(DWORD _controlEvent)
{
	// 모든 이벤트는 콘솔창을 종료하는 이벤트 -> 모두 Release와 Destroy 작업 수행
	switch (_controlEvent)
	{
	case CTRL_C_EVENT:// control + c 종료
	case CTRL_CLOSE_EVENT:// X 눌러서 콘솔종료
	case CTRL_BREAK_EVENT:// control + break 종료   
	case CTRL_LOGOFF_EVENT:
	case CTRL_SHUTDOWN_EVENT:
	default:
		MainManager::GetInstance()->Release();
		MainManager::DestroyInstance();
		return FALSE;
	}
}