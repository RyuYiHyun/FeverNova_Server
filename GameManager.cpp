#include "GameManager.h"
#include "SessionManager.h"

//
#pragma region Singleton
bool GameManager::CreateInstance()
{
	if (m_instance == nullptr)
	{
		m_instance = new GameManager();
		return true;
	}
	else
	{
		return false;
	}
}
void GameManager::DestroyInstance()
{
	if (m_instance != nullptr)
	{
		delete m_instance;
	}
}
bool GameManager::Initialize() // 초기화
{
	m_giveIdCounter = 1;
	return true;
}
void GameManager::Release() // 후처리
{
}
GameManager* GameManager::GetInstance()
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
GameManager::GameManager()// 생성자
{
}
GameManager::~GameManager()// 소멸자
{
}
GameManager* GameManager::m_instance = nullptr;	// Singleton 객체
#pragma endregion


void GameManager::Function(Session* _session)
{
	E_PROTOCOL l_protocol = static_cast<E_PROTOCOL>(_session->GetProtocol());
	switch (l_protocol)
	{
	case GameManager::E_PROTOCOL::IDCREATE:
		IdCreateProcess(_session);
		break;
	case GameManager::E_PROTOCOL::PLAYTYPE:
		PlayTypeProcess(_session);
		break;
	case GameManager::E_PROTOCOL::EXIT:
		ExitProcess(_session);
		break;
	case GameManager::E_PROTOCOL::PLAYER_SPAWN:
		PlayerSpawnProcess(_session);
		break;
	case GameManager::E_PROTOCOL::PLAYER_TRANSFORM:
		PlayerTransformProcess(_session);
		break;
	case GameManager::E_PROTOCOL::PLAYER_JUMP:
		PlayerJumpProcess(_session);
		break;
	case GameManager::E_PROTOCOL::PLAYER_DODGE:
		PlayerDodgeProcess(_session);
		break;
	case GameManager::E_PROTOCOL::PLAYER_FIRE:
		PlayerFireProcess(_session);
		break;
	case GameManager::E_PROTOCOL::NPC_SPAWN:
		NpcSpawnProcess(_session);
		break;
	case GameManager::E_PROTOCOL::NPC_TRANSFORM:
		NpcTransformProcess(_session);
		break;
	case GameManager::E_PROTOCOL::NPC_UPDATEHP:
		NpcUpdateHpProcess(_session);
		break;
	case GameManager::E_PROTOCOL::NPC_TRIGGER:
		NpcTriggerProcess(_session);
		break;
	case GameManager::E_PROTOCOL::NPC_SKILL:
		NpcSkillProcess(_session);
		break;
	case GameManager::E_PROTOCOL::ITEM_SPAWN:
		ItemSpawnProcess(_session);
		break;
	case GameManager::E_PROTOCOL::ITEM_DESPAWN:
		ItemDeSpawnProcess(_session);
		break;
	case GameManager::E_PROTOCOL::PC_HIT:
		PcHitProcess(_session);
		break;
	case GameManager::E_PROTOCOL::Door_Use:
		DoorUseProcess(_session);
		break;
	case GameManager::E_PROTOCOL::REQESTION_SHOW:
		ReqestionShowProcess(_session);
		break;
	case GameManager::E_PROTOCOL::REQESTION_YES:
		ReqestionYesProcess(_session);
		break;
	case GameManager::E_PROTOCOL::REQESTION_NO:
		ReqestionNoProcess(_session);
		break;
	// JJCH -------------------------------------------
	case GameManager::E_PROTOCOL::GOMAIN:
		GoMainProcess(_session);
		break;
	case GameManager::E_PROTOCOL::LOAD_COMPLETE:
		LoadCompleteProcess(_session);
		break;
	// --------------------------------------------------
	default:
		LogManager::GetInstance()->LogWrite(7777);
		break;
	}

}
void GameManager::IdCreateProcess(Session* _session)
{
#pragma region ProcessSetting
	LockGuard l_lockGuard(&m_criticalKey); // LOCK
	BYTE l_data[BUFSIZE];
	ZeroMemory(l_data, BUFSIZE);
	int l_dataSize = -1;
	MyStream l_stream;
#pragma endregion

	_session->SetIdNumber(m_giveIdCounter);
	l_dataSize = l_stream->DataPacketMake(l_data, m_giveIdCounter);
	m_playerList.push_back(_session);
	m_giveIdCounter++;

	game::util::SEND(_session, E_PROTOCOL::IDCREATE, l_dataSize, l_data);
}

void GameManager::ExitProcess(Session* _session)
{
#pragma region ProcessSetting
	LockGuard l_lockGuard(&m_criticalKey); // LOCK
	BYTE l_data[BUFSIZE];
	ZeroMemory(l_data, BUFSIZE);
	int l_dataSize = -1;
	MyStream l_stream;
#pragma endregion

	int id = _session->GetIdNumber();
	l_dataSize = l_stream->DataPacketMake(l_data, id);

	Room* room = reinterpret_cast<Room*>(_session->GetRoom());
	if (room != nullptr)
	{
		for (auto player : room->players)
		{
			if (player == _session)
			{
				RoomManager::GetInstance()->OutCheck(_session);
				game::util::SEND(player, E_PROTOCOL::EXIT, l_dataSize, l_data);
				continue;
			}
			game::util::SEND(player, E_PROTOCOL::LEAVE, l_dataSize, l_data);
		}
	}
	else
	{
		game::util::SEND(_session, E_PROTOCOL::EXIT, l_dataSize, l_data);
	}

	for (list<Session*>::iterator iter = m_playerList.begin(); iter != m_playerList.end(); )
	{
		if ((*iter) == _session)
		{
			m_playerList.erase(iter);
			break;
		}
		else
		{
			iter++;
		}
	}
	return;
}

void GameManager::ForceExitProcess(Session* _session)
{
#pragma region ProcessSetting
	LockGuard l_lockGuard(&m_criticalKey); // LOCK
	BYTE l_data[BUFSIZE];
	ZeroMemory(l_data, BUFSIZE);
	int l_dataSize = -1;
	MyStream l_stream;
#pragma endregion

	int id = _session->GetIdNumber();
	l_dataSize = l_stream->DataPacketMake(l_data, id);

	Room* room = reinterpret_cast<Room*>(_session->GetRoom());
	if (room != nullptr)
	{
		for (auto player : room->players)
		{
			if (player == _session)
			{
				RoomManager::GetInstance()->OutCheck(_session);
				game::util::SEND(player, E_PROTOCOL::EXIT, l_dataSize, l_data);
				continue;
			}
			game::util::SEND(player, E_PROTOCOL::LEAVE, l_dataSize, l_data);
		}
	}
	else
	{
		game::util::SEND(_session, E_PROTOCOL::EXIT, l_dataSize, l_data);
	}

	for (list<Session*>::iterator iter = m_playerList.begin(); iter != m_playerList.end(); )
	{
		if ((*iter) == _session)
		{
			m_playerList.erase(iter);
			break;
		}
		else
		{
			iter++;
		}
	}
	return;
}

void GameManager::PlayTypeProcess(Session* _session)
{
#pragma region ProcessSetting
	LockGuard l_lockGuard(&m_criticalKey); // LOCK
	BYTE l_data[BUFSIZE];
	ZeroMemory(l_data, BUFSIZE);
	int l_dataSize = -1;
	MyStream l_stream;
#pragma endregion

	int select;
	l_stream->DataPacketSplit(_session->GetDataField(), select);
	l_dataSize = 0;

	if (select == 1)   // 1P
	{
		Room* room = RoomManager::GetInstance()->CreateRoom(Room::Type::Single);
		room->enterPlayer(_session);
		_session->SetRoom(room);
		// 게임 시작 메세지
		game::util::SEND(room->players[0], E_PROTOCOL::SINGLE_START, l_dataSize, l_data);
	}
	else if (select == 2) // 2P
	{
		Room* room;
		room = RoomManager::GetInstance()->FindEmptyRoom();
		if (room == nullptr)
		{
			room = RoomManager::GetInstance()->CreateRoom(Room::Type::Multi);
			room->enterPlayer(_session);
			_session->SetRoom(room);
			// 게임 대기..................
			game::util::SEND(room->players[0], E_PROTOCOL::WAIT, l_dataSize, l_data);
		}
		else
		{
			room->enterPlayer(_session);
			_session->SetRoom(room);
			// 게임 시작..................
			game::util::SEND(room->players[0], E_PROTOCOL::MULTI_HOST_START, l_dataSize, l_data);
			game::util::SEND(room->players[1], E_PROTOCOL::MULTI_GUEST_START, l_dataSize, l_data);
		}
	}
	return;
}

void GameManager::PlayerSpawnProcess(Session* _session)
{
#pragma region ProcessSetting
	LockGuard l_lockGuard(&m_criticalKey); // LOCK
	BYTE l_data[BUFSIZE];
	ZeroMemory(l_data, BUFSIZE);
	int l_dataSize = -1;
#pragma endregion

	Room* room = reinterpret_cast<Room*>(_session->GetRoom());
	l_dataSize = PlayerSpawnDataMake(l_data, *room);

	if (room == nullptr) { return; }// 예외

	for (auto player : room->players)
	{
		game::util::SEND(player, E_PROTOCOL::PLAYER_SPAWN, l_dataSize, l_data);
	}
	return;
}

void GameManager::PlayerTransformProcess(Session* _session)
{
#pragma region ProcessSetting
	LockGuard l_lockGuard(&m_criticalKey); // LOCK
	BYTE l_data[BUFSIZE];
	ZeroMemory(l_data, BUFSIZE);
	int l_dataSize = -1;
	MyStream l_stream;
#pragma endregion

	PlayerTransformData moveData;
	l_stream->DataPacketSplit(_session->GetDataField(), moveData);

	Room* room = reinterpret_cast<Room*>(_session->GetRoom());
	if (room == nullptr) { return; }// 예외

	l_dataSize = l_stream->DataPacketMake(l_data, moveData);

	for (auto player : room->players)
	{
		game::util::SEND(player, E_PROTOCOL::PLAYER_TRANSFORM, l_dataSize, l_data);
	}
	return;
}

void GameManager::PlayerJumpProcess(Session* _session)
{
#pragma region ProcessSetting
	LockGuard l_lockGuard(&m_criticalKey); // LOCK
	BYTE l_data[BUFSIZE];
	ZeroMemory(l_data, BUFSIZE);
	int l_dataSize = -1;
	MyStream l_stream;
#pragma endregion

	int idData;
	l_stream->DataPacketSplit(_session->GetDataField(), idData);

	l_dataSize = l_stream->DataPacketMake(l_data, idData);


	Room* room = reinterpret_cast<Room*>(_session->GetRoom());
	if (room == nullptr) { return; }// 예외

	for (auto player : room->players)
	{
		game::util::SEND(player, E_PROTOCOL::PLAYER_JUMP, l_dataSize, l_data);
	}
	return;
}

void GameManager::PlayerDodgeProcess(Session* _session)
{
#pragma region ProcessSetting
	LockGuard l_lockGuard(&m_criticalKey); // LOCK
	BYTE l_data[BUFSIZE];
	ZeroMemory(l_data, BUFSIZE);
	int l_dataSize = -1;
	MyStream l_stream;
#pragma endregion

	int idData;
	l_stream->DataPacketSplit(_session->GetDataField(), idData);

	l_dataSize = l_stream->DataPacketMake(l_data, idData);


	Room* room = reinterpret_cast<Room*>(_session->GetRoom());
	if (room == nullptr) { return; }// 예외

	for (auto player : room->players)
	{
		game::util::SEND(player, E_PROTOCOL::PLAYER_DODGE, l_dataSize, l_data);
	}
	return;
}

void GameManager::PlayerFireProcess(Session* _session)
{
#pragma region ProcessSetting
	LockGuard l_lockGuard(&m_criticalKey); // LOCK
	BYTE l_data[BUFSIZE];
	ZeroMemory(l_data, BUFSIZE);
	int l_dataSize = -1;
	MyStream l_stream;
#pragma endregion

	PlayerFireData l_packet;
	l_stream->DataPacketSplit(_session->GetDataField(), l_packet);

	l_dataSize = l_stream->DataPacketMake(l_data, l_packet);

	Room* room = reinterpret_cast<Room*>(_session->GetRoom());
	if (room == nullptr) { return; }// 예외

	for (auto player : room->players)
	{
		game::util::SEND(player, E_PROTOCOL::PLAYER_FIRE, l_dataSize, l_data);
	}
	return;
}

void GameManager::NpcSpawnProcess(Session* _session)
{
#pragma region ProcessSetting
	LockGuard l_lockGuard(&m_criticalKey); // LOCK
	BYTE l_data[BUFSIZE];
	ZeroMemory(l_data, BUFSIZE);
	int l_dataSize = -1;
	MyStream l_stream;
#pragma endregion

	NpcSpawnData l_packet;
	l_stream->DataPacketSplit(_session->GetDataField(), l_packet);

	l_dataSize = l_stream->DataPacketMake(l_data, l_packet);

	Room* room = reinterpret_cast<Room*>(_session->GetRoom());
	if (room == nullptr) { return; }// 예외

	for (auto player : room->players)
	{
		if (_session != player)
		{
			game::util::SEND(player, E_PROTOCOL::NPC_SPAWN, l_dataSize, l_data);
		}
	}
}

void GameManager::NpcTransformProcess(Session* _session)
{
#pragma region ProcessSetting
	LockGuard l_lockGuard(&m_criticalKey); // LOCK
	BYTE l_data[BUFSIZE];
	ZeroMemory(l_data, BUFSIZE);
	int l_dataSize = -1;
	MyStream l_stream;
#pragma endregion

	NpcTransformListData l_packet;
	l_stream->DataPacketSplit(_session->GetDataField(), l_packet);

	l_dataSize = l_stream->DataPacketMake(l_data, l_packet);

	Room* room = reinterpret_cast<Room*>(_session->GetRoom());
	if (room == nullptr) { return; }// 예외

	for (auto player : room->players)
	{
		if (_session != player)
		{
			game::util::SEND(player, E_PROTOCOL::NPC_TRANSFORM, l_dataSize, l_data);
		}
	}
}

void GameManager::NpcUpdateHpProcess(Session* _session)
{
#pragma region ProcessSetting
	LockGuard l_lockGuard(&m_criticalKey); // LOCK
	BYTE l_data[BUFSIZE];
	ZeroMemory(l_data, BUFSIZE);
	int l_dataSize = -1;
	MyStream l_stream;
#pragma endregion

	NpcHpData l_packet;
	l_stream->DataPacketSplit(_session->GetDataField(), l_packet);

	l_dataSize = l_stream->DataPacketMake(l_data, l_packet);

	Room* room = reinterpret_cast<Room*>(_session->GetRoom());
	if (room == nullptr) { return; }// 예외

	for (auto player : room->players)
	{
		if (_session != player)
		{
			game::util::SEND(player, E_PROTOCOL::NPC_UPDATEHP, l_dataSize, l_data);
		}
	}
}

void GameManager::NpcTriggerProcess(Session* _session)
{
#pragma region ProcessSetting
	LockGuard l_lockGuard(&m_criticalKey); // LOCK
	BYTE l_data[BUFSIZE];
	ZeroMemory(l_data, BUFSIZE);
	int l_dataSize = -1;
	MyStream l_stream;
#pragma endregion

	NpcTriggerData l_packet;
	l_stream->DataPacketSplit(_session->GetDataField(), l_packet);

	l_dataSize = l_stream->DataPacketMake(l_data, l_packet);

	Room* room = reinterpret_cast<Room*>(_session->GetRoom());
	if (room == nullptr) { return; }// 예외

	for (auto player : room->players)
	{
		if (_session != player)
		{
			game::util::SEND(player, E_PROTOCOL::NPC_TRIGGER, l_dataSize, l_data);
		}
	}
	return;
}

void GameManager::NpcSkillProcess(Session* _session)
{
#pragma region ProcessSetting
	LockGuard l_lockGuard(&m_criticalKey); // LOCK
	BYTE l_data[BUFSIZE];
	ZeroMemory(l_data, BUFSIZE);
	int l_dataSize = -1;
	MyStream l_stream;
#pragma endregion

	NpcSkillData l_packet;
	l_stream->DataPacketSplit(_session->GetDataField(), l_packet);

	l_dataSize = l_stream->DataPacketMake(l_data, l_packet);

	Room* room = reinterpret_cast<Room*>(_session->GetRoom());
	if (room == nullptr) { return; }// 예외

	for (auto player : room->players)
	{
		if (_session != player)
		{
			game::util::SEND(player, E_PROTOCOL::NPC_SKILL, l_dataSize, l_data);
		}
	}
	return;
}

void GameManager::ItemSpawnProcess(Session* _session)
{
#pragma region ProcessSetting
	LockGuard l_lockGuard(&m_criticalKey); // LOCK
	BYTE l_data[BUFSIZE];
	ZeroMemory(l_data, BUFSIZE);
	int l_dataSize = -1;
	MyStream l_stream;
#pragma endregion

	ItemSpawnData l_packet;
	l_stream->DataPacketSplit(_session->GetDataField(), l_packet);

	l_dataSize = l_stream->DataPacketMake(l_data, l_packet);

	Room* room = reinterpret_cast<Room*>(_session->GetRoom());
	if (room == nullptr) { return; }// 예외

	for (auto player : room->players)
	{
		if (_session != player)
		{
			game::util::SEND(player, E_PROTOCOL::ITEM_SPAWN, l_dataSize, l_data);
		}
	}
}

void GameManager::ItemDeSpawnProcess(Session* _session)
{
#pragma region ProcessSetting
	LockGuard l_lockGuard(&m_criticalKey); // LOCK
	BYTE l_data[BUFSIZE];
	ZeroMemory(l_data, BUFSIZE);
	int l_dataSize = -1;
	MyStream l_stream;
#pragma endregion

	ItemDeSpawnData l_packet;
	l_stream->DataPacketSplit(_session->GetDataField(), l_packet);

	l_dataSize = l_stream->DataPacketMake(l_data, l_packet);

	Room* room = reinterpret_cast<Room*>(_session->GetRoom());
	if (room == nullptr) { return; }// 예외

	for (auto player : room->players)
	{
		if (_session != player)
		{
			game::util::SEND(player, E_PROTOCOL::ITEM_DESPAWN, l_dataSize, l_data);
		}
	}
}

void GameManager::ReqestionShowProcess(Session* _session)
{
#pragma region ProcessSetting
	LockGuard l_lockGuard(&m_criticalKey); // LOCK
	BYTE l_data[BUFSIZE];
	ZeroMemory(l_data, BUFSIZE);
	int l_dataSize = -1;
	MyStream l_stream;
#pragma endregion

	// JJCH -------------------------------------------------------
	ReqestionUIData l_packet;
	l_stream->DataPacketSplit(_session->GetDataField(), l_packet);

	l_dataSize = l_stream->DataPacketMake(l_data, l_packet);
	// ------------------------------------------------------------

	Room* room = reinterpret_cast<Room*>(_session->GetRoom());
	if (room == nullptr) { return; }// 예외

	for (auto player : room->players)
	{
		if (_session != player)
		{
			game::util::SEND(player, E_PROTOCOL::REQESTION_SHOW, l_dataSize, l_data);
		}
	}
}
void GameManager::ReqestionYesProcess(Session* _session)
{
#pragma region ProcessSetting
	LockGuard l_lockGuard(&m_criticalKey); // LOCK
	BYTE l_data[BUFSIZE];
	ZeroMemory(l_data, BUFSIZE);
	int l_dataSize = -1;
	MyStream l_stream;
#pragma endregion

	// JJCH -------------------------------------------------------
	int l_packet;
	l_stream->DataPacketSplit(_session->GetDataField(), l_packet);

	l_dataSize = l_stream->DataPacketMake(l_data, l_packet);
	// ------------------------------------------------------------

	Room* room = reinterpret_cast<Room*>(_session->GetRoom());
	if (room == nullptr) { return; }// 예외

	for (auto player : room->players)
	{
		game::util::SEND(player, E_PROTOCOL::REQESTION_YES, l_dataSize, l_data);
	}
}
void GameManager::ReqestionNoProcess(Session* _session)
{
#pragma region ProcessSetting
	LockGuard l_lockGuard(&m_criticalKey); // LOCK
	BYTE l_data[BUFSIZE];
	ZeroMemory(l_data, BUFSIZE);
	int l_dataSize = -1;
	MyStream l_stream;
#pragma endregion

	// JJCH -------------------------------------------------------
	int l_packet;
	l_stream->DataPacketSplit(_session->GetDataField(), l_packet);

	l_dataSize = l_stream->DataPacketMake(l_data, l_packet);
	// ------------------------------------------------------------

	Room* room = reinterpret_cast<Room*>(_session->GetRoom());
	if (room == nullptr) { return; }// 예외

	for (auto player : room->players)
	{
		if (_session != player)
		{
			game::util::SEND(player, E_PROTOCOL::REQESTION_NO, l_dataSize, l_data);
		}
	}
}

void GameManager::GoMainProcess(Session* _session)
{
#pragma region ProcessSetting
	LockGuard l_lockGuard(&m_criticalKey); // LOCK
	BYTE l_data[BUFSIZE];
	ZeroMemory(l_data, BUFSIZE);
	int l_dataSize = -1;
	MyStream l_stream;
#pragma endregion

	Room* room = reinterpret_cast<Room*>(_session->GetRoom());
	if (room == nullptr) { return; }// 예외

	room->exitPlayer(_session);
	RoomManager::GetInstance()->RemoveRoom(room);
}

void GameManager::LoadCompleteProcess(Session* _session)
{
#pragma region ProcessSetting
	LockGuard l_lockGuard(&m_criticalKey); // LOCK
	BYTE l_data[BUFSIZE];
	ZeroMemory(l_data, BUFSIZE);
	int l_dataSize = -1;
	MyStream l_stream;
#pragma endregion

	l_dataSize = 0;

	Room* room = reinterpret_cast<Room*>(_session->GetRoom());
	if (room == nullptr) { return; }// 예외

	room->loadCompleteCount++;

	if (room->loadCompleteCount == room->players.size())
	{
		room->loadCompleteCount = 0;

		for (auto player : room->players)
		{
			game::util::SEND(player, E_PROTOCOL::LOAD_COMPLETE, l_dataSize, l_data);
		}
	}
}

void GameManager::PcHitProcess(Session* _session)
{
#pragma region ProcessSetting
	LockGuard l_lockGuard(&m_criticalKey); // LOCK
	BYTE l_data[BUFSIZE];
	ZeroMemory(l_data, BUFSIZE);
	int l_dataSize = -1;
	MyStream l_stream;
#pragma endregion

	PcHitData l_packet;
	l_stream->DataPacketSplit(_session->GetDataField(), l_packet);

	l_dataSize = l_stream->DataPacketMake(l_data, l_packet);

	Room* room = reinterpret_cast<Room*>(_session->GetRoom());
	if (room == nullptr) { return; }// 예외

	for (auto player : room->players)
	{
		if (_session != player)
		{
			game::util::SEND(player, E_PROTOCOL::PC_HIT, l_dataSize, l_data);
		}
	}
}

void GameManager::DoorUseProcess(Session* _session)
{
#pragma region ProcessSetting
	LockGuard l_lockGuard(&m_criticalKey); // LOCK
	BYTE l_data[BUFSIZE];
	ZeroMemory(l_data, BUFSIZE);
	int l_dataSize = -1;
	MyStream l_stream;
#pragma endregion

	DoorUseData l_packet;
	l_stream->DataPacketSplit(_session->GetDataField(), l_packet);

	l_dataSize = l_stream->DataPacketMake(l_data, l_packet);

	Room* room = reinterpret_cast<Room*>(_session->GetRoom());
	if (room == nullptr) { return; }// 예외

	for (auto player : room->players)
	{
		if (_session != player)
		{
			game::util::SEND(player, E_PROTOCOL::Door_Use, l_dataSize, l_data);
		}
	}
}

// JJCH Ryu씨가 만든 프로세스 함수 현재 사용 안하므로 삭제

int GameManager::PlayerSpawnDataMake(BYTE* _data, Room& _room)
{
	MyStream l_stream;
	l_stream->SetStream(_data);

	l_stream->WriteInt(static_cast<int>(_room.players.size()));
	for (auto player : _room.players)
	{
		l_stream->WriteInt(player->GetIdNumber());
	}

	return l_stream->GetLength();
}