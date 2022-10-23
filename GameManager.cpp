#include "GameManager.h"
#include "LogManager.h"
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
	case GameManager::E_PROTOCOL::SPAWN:
		SpawnProcess(_session);
		break;
	case GameManager::E_PROTOCOL::MOVE:
		MoveProcess(_session);
		break;
	case GameManager::E_PROTOCOL::JUMP:
		JumpProcess(_session);
		break;
	case GameManager::E_PROTOCOL::DODGE:
		DodgeProcess(_session);
		break;
	case GameManager::E_PROTOCOL::FIRE:
		FireProcess(_session);
		break;
	case GameManager::E_PROTOCOL::NPC_SPAWN:
		NpcSpawnProcess(_session);
		break;
	case GameManager::E_PROTOCOL::NPC_TRANSFORM:
		NpcTransformProcess(_session);
		break;
	case GameManager::E_PROTOCOL::ITEM_SPAWN:
		ItemSpawnProcess(_session);
		break;
	case GameManager::E_PROTOCOL::NPC_TRIGGER:
		NpcTriggerProcess(_session);
		break;
	case GameManager::E_PROTOCOL::NPC_ATTACK:
		NpcAttackProcess(_session);
		break;
	case GameManager::E_PROTOCOL::NPC_HP:
		NpcHPProcess(_session);
		break;
	case GameManager::E_PROTOCOL::EXIT:
		ExitProcess(_session);
		break;
	default:
		LogManager::GetInstance()->LogWrite(7777);
		break;
	}

}
void GameManager::IdCreateProcess(Session* _session)
{
	LockGuard l_lockGuard(&m_criticalKey); // 잠금
	BYTE l_data[BUFSIZE];
	ZeroMemory(l_data, BUFSIZE);
	int l_dataSize = -1;
	_session->SetIdNumber(m_giveIdCounter);
	l_dataSize = IdDataMake(l_data, m_giveIdCounter);
	m_playerList.push_back(_session);
	m_giveIdCounter++;

	if (!_session->SendPacket(static_cast<int>(E_PROTOCOL::IDCREATE), l_dataSize, l_data))
	{
		LogManager::GetInstance()->LogWrite(1005);
	}
}

void GameManager::PlayTypeProcess(Session* _session)
{
	LockGuard l_lockGuard(&m_criticalKey); // 잠금
	BYTE l_data[BUFSIZE];
	ZeroMemory(l_data, BUFSIZE);
	int l_dataSize = -1;

	MyStream l_stream;
	l_stream->SetStream(_session->GetDataField());

	int select;
	l_stream->ReadInt(&select);


	l_dataSize = 0;

	if (select == 1)   // 1P
	{
		Room* room = RoomManager::GetInstance()->CreateRoom(Room::Type::Single);
		room->enterPlayer(_session);
		_session->SetRoom(room);
		// 게임 시작 메세지
		if (!room->players[0]->SendPacket(static_cast<int>(E_PROTOCOL::SINGLE_START), l_dataSize, l_data))
		{
			LogManager::GetInstance()->LogWrite(1005);
		}
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
			if (!room->players[0]->SendPacket(static_cast<int>(E_PROTOCOL::WAIT), l_dataSize, l_data))
			{
				LogManager::GetInstance()->LogWrite(1005);
			}
		}
		else
		{
			room->enterPlayer(_session);
			_session->SetRoom(room);
			// 게임 시작..................
			if (!room->players[0]->SendPacket(static_cast<int>(E_PROTOCOL::MULTI_HOST_START), l_dataSize, l_data))
			{
				LogManager::GetInstance()->LogWrite(1005);
			}
			if (!room->players[1]->SendPacket(static_cast<int>(E_PROTOCOL::MULTI_GUEST_START), l_dataSize, l_data))
			{
				LogManager::GetInstance()->LogWrite(1005);
			}
		}
	}
	return;
}

void GameManager::SpawnProcess(Session* _session)
{
	LockGuard l_lockGuard(&m_criticalKey); // 잠금
	BYTE l_data[BUFSIZE];
	ZeroMemory(l_data, BUFSIZE);
	int l_dataSize = -1;

	Room* room = reinterpret_cast<Room*>(_session->GetRoom());
	l_dataSize = SpawnDataMake(l_data, *room);

	if (room == nullptr) { return; }// 예외

	for (auto player : room->players)
	{
		if (!player->SendPacket(static_cast<int>(E_PROTOCOL::SPAWN), l_dataSize, l_data))
		{
			LogManager::GetInstance()->LogWrite(1005);
		}
	}
	return;
}

void GameManager::MoveProcess(Session* _session)
{
	LockGuard l_lockGuard(&m_criticalKey); // 잠금
	BYTE l_data[BUFSIZE];
	ZeroMemory(l_data, BUFSIZE);
	int l_dataSize = -1;

	MoveData moveData;
	MoveDataSplit(_session->GetDataField(), moveData);

	Room* room = reinterpret_cast<Room*>(_session->GetRoom());
	if (room == nullptr) { return; }// 예외

	l_dataSize = MoveDataMake(l_data, moveData);

	for (auto player : room->players)
	{
		if (!player->SendPacket(static_cast<int>(E_PROTOCOL::MOVE), l_dataSize, l_data))
		{
			LogManager::GetInstance()->LogWrite(1006);
		}
	}
	return;
}

void GameManager::JumpProcess(Session* _session)
{
	LockGuard l_lockGuard(&m_criticalKey); // 잠금
	BYTE l_data[BUFSIZE];
	ZeroMemory(l_data, BUFSIZE);
	int l_dataSize = -1;


	MyStream l_stream;
	l_stream->SetStream(_session->GetDataField());

	int idData;
	l_stream->ReadInt(&idData);

	l_dataSize = JumpDataMake(l_data, idData);

	Room* room = reinterpret_cast<Room*>(_session->GetRoom());
	if (room == nullptr) { return; }// 예외

	for (auto player : room->players)
	{
		if (!player->SendPacket(static_cast<int>(E_PROTOCOL::JUMP), l_dataSize, l_data))
		{
			LogManager::GetInstance()->LogWrite(1006);
		}
	}
	return;
}

void GameManager::DodgeProcess(Session* _session)
{
	LockGuard l_lockGuard(&m_criticalKey); // 잠금
	BYTE l_data[BUFSIZE];
	ZeroMemory(l_data, BUFSIZE);
	int l_dataSize = -1;


	MyStream l_stream;
	l_stream->SetStream(_session->GetDataField());

	int idData;
	l_stream->ReadInt(&idData);

	l_dataSize = DodgeDataMake(l_data, idData);

	Room* room = reinterpret_cast<Room*>(_session->GetRoom());
	if (room == nullptr) { return; }// 예외

	for (auto player : room->players)
	{
		if (!player->SendPacket(static_cast<int>(E_PROTOCOL::DODGE), l_dataSize, l_data))
		{
			LogManager::GetInstance()->LogWrite(1006);
		}
	}
	return;
}

void GameManager::FireProcess(Session* _session)
{
	LockGuard l_lockGuard(&m_criticalKey); // 잠금
	BYTE l_data[BUFSIZE];
	ZeroMemory(l_data, BUFSIZE);
	int l_dataSize = -1;

	FireData fireData;
	FireDataSplit(_session->GetDataField(), fireData);

	l_dataSize = FireDataMake(l_data, fireData);

	Room* room = reinterpret_cast<Room*>(_session->GetRoom());
	if (room == nullptr) { return; }// 예외

	for (auto player : room->players)
	{
		if (!player->SendPacket(static_cast<int>(E_PROTOCOL::FIRE), l_dataSize, l_data))
		{
			LogManager::GetInstance()->LogWrite(1006);
		}
	}
	return;
}

void GameManager::NpcSpawnProcess(Session* _session)
{
	LockGuard l_lockGuard(&m_criticalKey);
	BYTE l_data[BUFSIZE];
	ZeroMemory(l_data, BUFSIZE);
	int l_dataSize = -1;

	SpawnData_NPC spawnData_NPC;
	NpcSpawnDataSplit(_session->GetDataField(), spawnData_NPC);

	l_dataSize = NpcSpawnDataMake(l_data, spawnData_NPC);

	Room* room = reinterpret_cast<Room*>(_session->GetRoom());
	if (room == nullptr) { return; }// 예외

	for (auto player : room->players)
	{
		if (_session != player)
		{
			if (!player->SendPacket(static_cast<int>(E_PROTOCOL::NPC_SPAWN), l_dataSize, l_data))
			{
				LogManager::GetInstance()->LogWrite(1006);
			}
			break;
		}
	}
}

void GameManager::NpcTransformProcess(Session* _session)
{
	LockGuard l_lockGuard(&m_criticalKey);
	BYTE l_data[BUFSIZE];
	ZeroMemory(l_data, BUFSIZE);
	int l_dataSize = -1;

	TransformData_NPC transformData_NPC;
	NpcTransformDataSplit(_session->GetDataField(), transformData_NPC);

	l_dataSize = NpcTransformDataMake(l_data, transformData_NPC);

	Room* room = reinterpret_cast<Room*>(_session->GetRoom());
	if (room == nullptr) { return; }// 예외

	for (auto player : room->players)
	{
		if (_session != player)
		{
			if (!player->SendPacket(static_cast<int>(E_PROTOCOL::NPC_TRANSFORM), l_dataSize, l_data))
			{
				LogManager::GetInstance()->LogWrite(1006);
			}
			break;
		}
	}
}

void GameManager::ItemSpawnProcess(Session* _session)
{
	LockGuard l_lockGuard(&m_criticalKey);
	BYTE l_data[BUFSIZE];
	ZeroMemory(l_data, BUFSIZE);
	int l_dataSize = -1;

	SpawnData_Item spawnData_Item;
	ItemSpawnDataSplit(_session->GetDataField(), spawnData_Item);

	l_dataSize = ItemSpawnDataMake(l_data, spawnData_Item);

	Room* room = reinterpret_cast<Room*>(_session->GetRoom());
	if (room == nullptr) { return; }// 예외

	for (auto player : room->players)
	{
		if (_session != player)
		{
			if (!player->SendPacket(static_cast<int>(E_PROTOCOL::ITEM_SPAWN), l_dataSize, l_data))
			{
				LogManager::GetInstance()->LogWrite(1006);
			}
			break;
		}
	}
}

void GameManager::NpcTriggerProcess(Session* _session)
{
	LockGuard l_lockGuard(&m_criticalKey); // 잠금
	BYTE l_data[BUFSIZE];
	ZeroMemory(l_data, BUFSIZE);
	int l_dataSize = -1;

	MyStream l_stream;
	l_stream->SetStream(_session->GetDataField());

	NpcTriggerData _npcTriggerData;
	l_stream->ReadBytes(reinterpret_cast<BYTE*>(&_npcTriggerData), sizeof(NpcTriggerData));

	l_dataSize = NpcTriggerDataMake(l_data, _npcTriggerData);

	Room* room = reinterpret_cast<Room*>(_session->GetRoom());
	if (room == nullptr) { return; }// 예외

	for (auto player : room->players)
	{
		if (_session != player)
		{
			if (!player->SendPacket(static_cast<int>(E_PROTOCOL::NPC_TRIGGER), l_dataSize, l_data))
			{
				LogManager::GetInstance()->LogWrite(1006);
			}
			break;
		}
	}
	return;
}

void GameManager::NpcAttackProcess(Session* _session)
{
	LockGuard l_lockGuard(&m_criticalKey); // 잠금
	BYTE l_data[BUFSIZE];
	ZeroMemory(l_data, BUFSIZE);
	int l_dataSize = -1;

	MyStream l_stream;
	l_stream->SetStream(_session->GetDataField());

	NpcAttackData _npcAttackData;
	l_stream->ReadBytes(reinterpret_cast<BYTE*>(&_npcAttackData), sizeof(NpcAttackData));

	l_dataSize = NpcAttackDataMake(l_data, _npcAttackData);

	Room* room = reinterpret_cast<Room*>(_session->GetRoom());
	if (room == nullptr) { return; }// 예외

	for (auto player : room->players)
	{
		if (_session != player)
		{
			if (!player->SendPacket(static_cast<int>(E_PROTOCOL::NPC_ATTACK), l_dataSize, l_data))
			{
				LogManager::GetInstance()->LogWrite(1006);
			}
			break;
		}
	}
	return;
}

void GameManager::NpcHPProcess(Session* _session)
{
	LockGuard l_lockGuard(&m_criticalKey);
	BYTE l_data[BUFSIZE];
	ZeroMemory(l_data, BUFSIZE);
	int l_dataSize = -1;

	NpcHPData HPData_NPC;
	NpcHPDataSplit(_session->GetDataField(), HPData_NPC);

	l_dataSize = NpcHPDataMake(l_data, HPData_NPC);

	Room* room = reinterpret_cast<Room*>(_session->GetRoom());
	if (room == nullptr) { return; }// 예외

	for (auto player : room->players)
	{
		if (_session != player)
		{
			if (!player->SendPacket(static_cast<int>(E_PROTOCOL::NPC_HP), l_dataSize, l_data))
			{
				LogManager::GetInstance()->LogWrite(1006);
			}
			break;
		}
	}
}

void GameManager::ExitProcess(Session* _session)
{
	BYTE l_data[BUFSIZE];
	ZeroMemory(l_data, BUFSIZE);
	int l_dataSize = -1;

	LockGuard l_lockGuard(&m_criticalKey); // 잠금

	l_dataSize = ExitDataMake(l_data, _session->GetIdNumber());

	Room* room = reinterpret_cast<Room*>(_session->GetRoom());
	if (room != nullptr)
	{
		for (auto player : room->players)
		{
			if (player == _session)
			{
				RoomManager::GetInstance()->OutCheck(_session);
				if (!player->SendPacket(static_cast<int>(E_PROTOCOL::EXIT), l_dataSize, l_data))
				{
					LogManager::GetInstance()->LogWrite(1006);
				}
			}
			if (!player->SendPacket(static_cast<int>(E_PROTOCOL::LEAVE), l_dataSize, l_data))
			{
				LogManager::GetInstance()->LogWrite(1006);
			}
		}
	}
	else
	{
		if (!_session->SendPacket(static_cast<int>(E_PROTOCOL::EXIT), l_dataSize, l_data))
		{
			LogManager::GetInstance()->LogWrite(1006);
		}
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
	BYTE l_data[BUFSIZE];
	ZeroMemory(l_data, BUFSIZE);
	int l_dataSize = -1;

	LockGuard l_lockGuard(&m_criticalKey); // 잠금

	l_dataSize = ExitDataMake(l_data, _session->GetIdNumber());

	Room* room = reinterpret_cast<Room*>(_session->GetRoom());

	if (room != nullptr)
	{
		for (auto player : room->players)
		{
			if (player == _session)
			{
				RoomManager::GetInstance()->OutCheck(_session);
				if (!player->SendPacket(static_cast<int>(E_PROTOCOL::EXIT), l_dataSize, l_data))
				{
					LogManager::GetInstance()->LogWrite(1006);
				}
			}
			if (!player->SendPacket(static_cast<int>(E_PROTOCOL::LEAVE), l_dataSize, l_data))
			{
				LogManager::GetInstance()->LogWrite(1006);
			}
		}
	}
	else
	{
		if (!_session->SendPacket(static_cast<int>(E_PROTOCOL::EXIT), l_dataSize, l_data))
		{
			LogManager::GetInstance()->LogWrite(1006);
		}
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

int GameManager::IdDataMake(BYTE* _data, int _id)
{
	MyStream l_stream;
	l_stream->SetStream(_data);
	l_stream->WriteInt(_id);

	return l_stream->GetLength();
}

int GameManager::SpawnDataMake(BYTE* _data, Room& _room)
{
	MyStream l_stream;
	l_stream->SetStream(_data);

	l_stream->WriteInt(static_cast<int>(_room.players.size()));
	for (auto player : _room.players)
	{
		LogManager::GetInstance()->LogWrite(player->GetIdNumber());
		l_stream->WriteInt(player->GetIdNumber());
	}

	return l_stream->GetLength();
}

int GameManager::MoveDataMake(BYTE* _data, MoveData _moveData)
{
	MyStream l_stream;
	l_stream->SetStream(_data);

	// 2후보
	l_stream->WriteBytes(reinterpret_cast<BYTE*>(&_moveData), sizeof(MoveData));

	return l_stream->GetLength();
}

int GameManager::JumpDataMake(BYTE* _data, int _id)
{
	MyStream l_stream;
	l_stream->SetStream(_data);
	l_stream->WriteInt(_id);

	return l_stream->GetLength();
}

int GameManager::DodgeDataMake(BYTE* _data, int _id)
{
	MyStream l_stream;
	l_stream->SetStream(_data);
	l_stream->WriteInt(_id);

	return l_stream->GetLength();
}

int GameManager::FireDataMake(BYTE* _data, FireData _fireData)
{
	MyStream l_stream;
	l_stream->SetStream(_data);

	l_stream->WriteBytes(reinterpret_cast<BYTE*>(&_fireData), sizeof(FireData));

	return l_stream->GetLength();
}

int GameManager::NpcSpawnDataMake(BYTE* _data, SpawnData_NPC _SpawnData_npc)
{
	MyStream l_stream;
	l_stream->SetStream(_data);
	l_stream->WriteBytes(reinterpret_cast<BYTE*>(&_SpawnData_npc), sizeof(SpawnData_NPC));

	return l_stream->GetLength();
}

int GameManager::NpcTransformDataMake(BYTE* _data, TransformData_NPC _TransformData_npc)
{
	MyStream l_stream;
	l_stream->SetStream(_data);
	l_stream->WriteBytes(reinterpret_cast<BYTE*>(&_TransformData_npc), sizeof(TransformData_NPC));

	return l_stream->GetLength();
}

int GameManager::ItemSpawnDataMake(BYTE* _data, SpawnData_Item _SpawnData_Item)
{
	MyStream l_stream;
	l_stream->SetStream(_data);
	l_stream->WriteBytes(reinterpret_cast<BYTE*>(&_SpawnData_Item), sizeof(SpawnData_Item));

	return l_stream->GetLength();
}

int GameManager::NpcTriggerDataMake(BYTE* _data, NpcTriggerData _npcTriggerData)
{
	MyStream l_stream;
	l_stream->SetStream(_data);
	l_stream->WriteBytes(reinterpret_cast<BYTE*>(&_npcTriggerData), sizeof(NpcTriggerData));

	return l_stream->GetLength();
}

int GameManager::NpcAttackDataMake(BYTE* _data, NpcAttackData _npcAttackData)
{
	MyStream l_stream;
	l_stream->SetStream(_data);
	l_stream->WriteBytes(reinterpret_cast<BYTE*>(&_npcAttackData), sizeof(NpcAttackData));

	return l_stream->GetLength();
}

int GameManager::NpcHPDataMake(BYTE* _data, NpcHPData _NpcHPData)
{
	MyStream l_stream;
	l_stream->SetStream(_data);
	l_stream->WriteBytes(reinterpret_cast<BYTE*>(&_NpcHPData), sizeof(NpcHPData));

	return l_stream->GetLength();
}

int GameManager::ExitDataMake(BYTE* _data, int _id)
{
	MyStream l_stream;
	l_stream->SetStream(_data);
	l_stream->WriteInt(_id);

	return l_stream->GetLength();
}

void GameManager::MoveDataSplit(BYTE* _data, MoveData& _moveData)
{
	MyStream l_stream;
	l_stream->SetStream(_data);
	l_stream->ReadBytes(reinterpret_cast<BYTE*>(&_moveData), sizeof(MoveData));
}

void GameManager::FireDataSplit(BYTE* _data, FireData& _fireData)
{
	MyStream l_stream;
	l_stream->SetStream(_data);
	l_stream->ReadBytes(reinterpret_cast<BYTE*>(&_fireData), sizeof(FireData));
	/*
	BYTE* l_focusPointer = _data;

	memcpy(&_fireData, l_focusPointer, sizeof(FireData));
	l_focusPointer = l_focusPointer + sizeof(FireData);*/
}

void GameManager::NpcSpawnDataSplit(BYTE* _data, SpawnData_NPC& _SpawnData_npc)
{
	MyStream l_stream;
	l_stream->SetStream(_data);
	l_stream->ReadBytes(reinterpret_cast<BYTE*>(&_SpawnData_npc), sizeof(SpawnData_NPC));
}

void GameManager::NpcTransformDataSplit(BYTE* _data, TransformData_NPC& _TransformData_npc)
{
	MyStream l_stream;
	l_stream->SetStream(_data);
	l_stream->ReadBytes(reinterpret_cast<BYTE*>(&_TransformData_npc), sizeof(TransformData_NPC));
}

void GameManager::ItemSpawnDataSplit(BYTE* _data, SpawnData_Item& _SpawnData_Item)
{
	MyStream l_stream;
	l_stream->SetStream(_data);
	l_stream->ReadBytes(reinterpret_cast<BYTE*>(&_SpawnData_Item), sizeof(SpawnData_Item));
}

void GameManager::NpcHPDataSplit(BYTE* _data, NpcHPData& _NpcHPData)
{
	MyStream l_stream;
	l_stream->SetStream(_data);
	l_stream->ReadBytes(reinterpret_cast<BYTE*>(&_NpcHPData), sizeof(NpcHPData));
}