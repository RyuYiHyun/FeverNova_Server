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

RoomManager* RoomManager::m_instance = nullptr;	// Singleton ��ü
#pragma endregion


Room* RoomManager::CreateRoom(Room::Type _type = Room::Type::Single)
{
	Room* newRoom = new Room(_type);	// �� ����
	roomlist.push_back(newRoom); // �� ����Ʈ�� ���� �߰�
	return newRoom;
}

Room* RoomManager::FindEmptyRoom()
{
	for (auto room : roomlist)
	{
		if (!room->isfull && room->type == Room::Type::Multi)
		{
			return room;
		}
	}
	return nullptr;
}

void RoomManager::OutCheck(Session* player)
{
	Room* room = reinterpret_cast<Room*>(player->GetRoom());
	if (room != nullptr) // Ŭ���̾�Ʈ�� �����Ҷ� , �������� ������ ������
	{
		if (room->state == Room::State::WAITSTART)// �濡 ȥ�� ������̸� ���� ����
		{
			room->exitPlayer(player);
			RemoveRoom(room);
		}
		if (room->state == Room::State::SINGLEPLAY) // �������϶�
		{
			room->exitPlayer(player);
			RemoveRoom(room);
		}
		if (room->state == Room::State::MULTIPLAY) // �������϶�
		{
			room->exitPlayer(player);
			RemoveRoom(room);
		}
		if (room->state == Room::State::ENDGAME) // ������ ����
		{
			room->exitPlayer(player);
			RemoveRoom(room);
		}
	}
}

void RoomManager::RemoveRoom(Room* target)
{
	if (target == nullptr) // ������ ���� ������
	{
		return;
	}
	if (target->players.empty())// ����̸� ����
	{
		for (list<Room*>::iterator iter = roomlist.begin(); iter != roomlist.end(); )
		{
			if (target == (*iter))
			{
				delete (*iter);
				iter = roomlist.erase(iter);
			}
			else
			{
				iter++;
			}
		}
	}
}