#pragma once
#include "GameRoom.h"
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
	// ���� �� ���� ����Ʈ
	list<Room*> roomlist;
public:
	// �� ����
	Room* CreateRoom()
	{
		Room* newRoom = new Room();	// �� ����
		roomlist.push_back(newRoom); // �� ����Ʈ�� ���� �߰�
		return newRoom;
	}
	// �� ���� �˻�
	Room* FindEmptyRoom()
	{
		for (list<Room*>::iterator iter = roomlist.begin(); iter != roomlist.end(); iter++) // ��ü ��ȸ
		{
			if (!(*iter)->isfull) // �������� ���� Ȯ��
			{
				return (*iter);
			}
		}
	}
	// ���� Ŭ���̾�Ʈ�� ���� üũ
	void OutCheck(ClientInfo* ptr)
	{
		if (ptr->gameroom != nullptr) // Ŭ���̾�Ʈ�� �����Ҷ� , �������� ������ ������
		{
			if (ptr->gameroom->state == RoomState::WAITSTART)// �濡 ȥ�� ������̸� ���� ����
			{
				RemoveRoom(ptr->gameroom);
			}
			if (ptr->gameroom->state == RoomState::PLAYING) // �������϶�
			{
				ptr->gameroom->Surrender(ptr);// �׺�
				ptr->gameroom->exitPlayer(ptr);// �� ������ 
				SafeRemoveRoom(ptr->gameroom);// ���� �� ����
			}
			if (ptr->gameroom->state == RoomState::ENDGAME) // ������ ����
			{
				ptr->gameroom->exitPlayer(ptr); // �� ������ 
				SafeRemoveRoom(ptr->gameroom);// ���� �� ����
			}
		}
	}
	// ���� �� ����
	void SafeRemoveRoom(Room* target)
	{
		if (target == nullptr) // ������ ���� ������
		{
			return;
		}
		// �÷��̾ ��� ���� ���̸� �� ����
		if (target->player1 == nullptr && target->player2 == nullptr)
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
};

