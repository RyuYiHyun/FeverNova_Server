#pragma once
#include "Session.h"
#include <vector>

class Room
{
public:
	enum State		// 방의 상태 정보
	{
		WAITSTART,		// 대기자 대기 상태
		SINGLEPLAY,		// 1명 인원 게임 시작
		MULTIPLAY,		// 2명 인원 게임 시작
		WAIT_REORNEXT,			// 다시하기 대기 및 다음 스테이지 대기
		ENDGAME,		// 게임의 단판의 결과가 나와 게임을 종료하는 상태
	};
	enum Type
	{
		Single,
		Multi
	};
	Room(Type _type = Type::Single)
	{
		isfull = false;					// 방이 차있는지 여부
		state = Room::State::WAITSTART;		// 대기 상태
		type = _type;
	}
	~Room()
	{
		players.clear();
	}

	bool enterPlayer(Session* player)
	{
		if (!isfull) // 방이 가득 찬지 여부 확인
		{
			players.push_back(player);
			if (type == Type::Single)
			{
				if (players.size() >= 1)
				{
					state = State::SINGLEPLAY;
					isfull = true;
				}
			}
			if (type == Type::Multi)
			{
				if (players.size() >= 2)
				{
					state = State::MULTIPLAY;
					isfull = true;
				}
			}
			return true;
		}
		return false;
	}

	bool exitPlayer(Session* player)
	{
		for (int i = 0; i < players.size(); i++)
		{
			if (players[i] == player)
			{
				state = State::ENDGAME;
				players.erase(players.begin() + i);
				player->SetRoom(nullptr);
				return true;
			}
		}
		return false;
	}

public:
	bool isfull;			// 게임 방 꽉찬지 여부 확인
	Room::State state;		// 방 상태 정보
	Type type;
	vector<Session*> players;

	// JJCH 이름 바꾸기 ----------------------------------------
	int loadCompleteCount = 0;
	// --------------------------------------------------------
};