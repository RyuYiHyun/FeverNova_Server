#pragma once
#include "Room.h"
#include "LockGuard.h"
class RoomManager
{
#pragma region Singleton
public:
	static bool CreateInstance();
	static void DestroyInstance();
	bool Initialize();
	void Release();
	static RoomManager* GetInstance();
private:
	static RoomManager* m_instance;

	RoomManager();
	virtual ~RoomManager();
#pragma endregion

public:
	list<Room*> roomlist;
public:
	Room* CreateRoom(Room::Type _type);
	Room* FindEmptyRoom();
	void OutCheck(Session* player);
	void RemoveRoom(Room* target);
};

