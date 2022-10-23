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
		ENDGAME,		// 게임의 단판의 결과가 나와 게임을 종료하는 상태
	};
	Room()							
	{
		isfull = false;					// 방이 차있는지 여부
		state = State::WAITSTART;		// 대기 상태
	}
	~Room()
	{
		players.clear();
	}
	// 플레이어가 방에 참가한다.
	bool enterPlayer(Session* player)
	{
		if (!isfull) // 방이 가득 찬지 여부 확인
		{
			
			return true;
		}
		// 가득찬 방
		return false;
	}
	// 플레이어가 방에서 나온다.
	bool exitPlayer(Session* player)
	{
		players.erase(remove(players.begin(), players.end(), player), players.end());
		return false;
	}

public:
	bool isfull;			// 게임 방 꽉찬지 여부 확인
	State state;		// 방 상태 정보
	vector<Session*> players;
};



