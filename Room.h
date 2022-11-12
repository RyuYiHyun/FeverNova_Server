#pragma once
#include "Session.h"
#include <vector>

class Room
{
public:
	enum State		// ���� ���� ����
	{
		WAITSTART,		// ����� ��� ����
		SINGLEPLAY,		// 1�� �ο� ���� ����
		MULTIPLAY,		// 2�� �ο� ���� ����
		WAIT_REORNEXT,			// �ٽ��ϱ� ��� �� ���� �������� ���
		ENDGAME,		// ������ ������ ����� ���� ������ �����ϴ� ����
	};
	enum Type
	{
		Single,
		Multi
	};
	Room(Type _type = Type::Single)
	{
		isfull = false;					// ���� ���ִ��� ����
		state = Room::State::WAITSTART;		// ��� ����
		type = _type;
	}
	~Room()
	{
		players.clear();
	}

	bool enterPlayer(Session* player)
	{
		if (!isfull) // ���� ���� ���� ���� Ȯ��
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
	bool isfull;			// ���� �� ������ ���� Ȯ��
	Room::State state;		// �� ���� ����
	Type type;
	vector<Session*> players;

	// JJCH �̸� �ٲٱ� ----------------------------------------
	int loadCompleteCount = 0;
	// --------------------------------------------------------
};