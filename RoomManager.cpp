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

RoomManager* RoomManager::m_instance = nullptr;	// Singleton 객체
#pragma endregion


Room* RoomManager::CreateRoom(Room::Type _type = Room::Type::Single)
{
	Room* newRoom = new Room(_type);	// 방 생성
	roomlist.push_back(newRoom); // 방 리스트에 정보 추가
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
	if (room != nullptr) // 클라이언트가 종료할때 , 방정보를 가지고 있으면
	{
		if (room->state == Room::State::WAITSTART)// 방에 혼자 대기중이면 방을 삭제
		{
			room->exitPlayer(player);
			RemoveRoom(room);
		}
		if (room->state == Room::State::SINGLEPLAY) // 게임중일때
		{
			room->exitPlayer(player);
			RemoveRoom(room);
		}
		if (room->state == Room::State::MULTIPLAY) // 게임중일때
		{
			room->exitPlayer(player);
			RemoveRoom(room);
		}
		if (room->state == Room::State::ENDGAME) // 게임이 종료
		{
			room->exitPlayer(player);
			RemoveRoom(room);
		}
	}
}

void RoomManager::RemoveRoom(Room* target)
{
	if (target == nullptr) // 삭제할 방이 없으면
	{
		return;
	}
	if (target->players.empty())// 빈방이면 삭제
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