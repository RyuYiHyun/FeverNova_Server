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
		ENDGAME,		// ������ ������ ����� ���� ������ �����ϴ� ����
	};
	Room()							
	{
		isfull = false;					// ���� ���ִ��� ����
		state = State::WAITSTART;		// ��� ����
	}
	~Room()
	{
		players.clear();
	}
	// �÷��̾ �濡 �����Ѵ�.
	bool enterPlayer(Session* player)
	{
		if (!isfull) // ���� ���� ���� ���� Ȯ��
		{
			
			return true;
		}
		// ������ ��
		return false;
	}
	// �÷��̾ �濡�� ���´�.
	bool exitPlayer(Session* player)
	{
		players.erase(remove(players.begin(), players.end(), player), players.end());
		return false;
	}

public:
	bool isfull;			// ���� �� ������ ���� Ȯ��
	State state;		// �� ���� ����
	vector<Session*> players;
};



