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
	// 관리 방 정보 리스트
	list<Room*> roomlist;
public:
	// 방 생성
	Room* CreateRoom()
	{
		Room* newRoom = new Room();	// 방 생성
		roomlist.push_back(newRoom); // 방 리스트에 정보 추가
		return newRoom;
	}
	// 빈 방을 검색
	Room* FindEmptyRoom()
	{
		for (list<Room*>::iterator iter = roomlist.begin(); iter != roomlist.end(); iter++) // 전체 순회
		{
			if (!(*iter)->isfull) // 빈장인지 여부 확인
			{
				return (*iter);
			}
		}
	}
	// 나간 클라이언트의 방을 체크
	void OutCheck(ClientInfo* ptr)
	{
		if (ptr->gameroom != nullptr) // 클라이언트가 종료할때 , 방정보를 가지고 있으면
		{
			if (ptr->gameroom->state == RoomState::WAITSTART)// 방에 혼자 대기중이면 방을 삭제
			{
				RemoveRoom(ptr->gameroom);
			}
			if (ptr->gameroom->state == RoomState::PLAYING) // 게임중일때
			{
				ptr->gameroom->Surrender(ptr);// 항복
				ptr->gameroom->exitPlayer(ptr);// 방 나가기 
				SafeRemoveRoom(ptr->gameroom);// 안전 방 삭제
			}
			if (ptr->gameroom->state == RoomState::ENDGAME) // 게임이 종료
			{
				ptr->gameroom->exitPlayer(ptr); // 방 나가기 
				SafeRemoveRoom(ptr->gameroom);// 안전 방 삭제
			}
		}
	}
	// 안전 방 삭제
	void SafeRemoveRoom(Room* target)
	{
		if (target == nullptr) // 삭제할 방이 없으면
		{
			return;
		}
		// 플레이어가 모두 나간 방이면 방 삭제
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

