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
	return true; // ������ ����
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

ConsoleControlHandler* ConsoleControlHandler::m_instance = nullptr; // Singleton ��ü
#pragma endregion



BOOL ConsoleControlHandler::ControlHandler(DWORD _controlEvent)
{
	// ��� �̺�Ʈ�� �ܼ�â�� �����ϴ� �̺�Ʈ -> ��� Release�� Destroy �۾� ����
	switch (_controlEvent)
	{
	case CTRL_C_EVENT:// control + c ����
	case CTRL_CLOSE_EVENT:// X ������ �ܼ�����
	case CTRL_BREAK_EVENT:// control + break ����   
	case CTRL_LOGOFF_EVENT:
	case CTRL_SHUTDOWN_EVENT:
	default:
		MainManager::GetInstance()->Release();
		MainManager::DestroyInstance();
		return FALSE;
	}
}