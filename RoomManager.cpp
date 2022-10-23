#include "RoomManager.h"
#pragma region Singleton
bool RoomManager::CreateInstance()
{
	if (m_instance == nullptr)
	{
		m_instance = new RoomManager();
		return true;
	}
	else
	{
		return false;
	}
}

void RoomManager::DestroyInstance()
{
	if (m_instance != nullptr)
	{
		delete m_instance;
	}
}

bool RoomManager::Initialize()
{
	return true;
}

void RoomManager::Release()
{
	for (list<Room*>::iterator iter = roomlist.begin(); iter != roomlist.end();)
	{
		delete (*iter);
		iter = roomlist.erase(iter);
	}
	roomlist.clear();
}

RoomManager* RoomManager::GetInstance()
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

RoomManager::RoomManager()
{
}

RoomManager::~RoomManager()
{
}

RoomManager* RoomManager::m_instance = nullptr;	// Singleton °´Ã¼
#pragma endregion