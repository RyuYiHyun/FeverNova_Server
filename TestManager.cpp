#include "TestManager.h"
#include "LogManager.h"
#include "SessionManager.h"
//
#pragma region Singleton
bool TestManager::CreateInstance()
{
	if (m_instance == nullptr)
	{
		m_instance = new TestManager();
		return true;
	}
	else
	{
		return false;
	}
}
void TestManager::DestroyInstance()
{
	if (m_instance != nullptr)
	{
		delete m_instance;
	}
}
bool TestManager::Initialize() // 초기화
{
	m_giveIdCounter = 1;
	return true;
}
void TestManager::Release() // 후처리
{
}
TestManager* TestManager::GetInstance()
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
TestManager::TestManager()// 생성자
{
}
TestManager::~TestManager()// 소멸자
{
}
TestManager* TestManager::m_instance = nullptr;	// Singleton 객체
#pragma endregion


void TestManager::Function(Session* _session)
{
	E_PROTOCOL l_protocol = static_cast<E_PROTOCOL>(_session->GetProtocol());
	switch (l_protocol)
	{
	case TestManager::E_PROTOCOL::IDCREATE:
		IdCreateProcess(_session);
		break;

	case TestManager::E_PROTOCOL::SPAWN:
		SpawnProcess(_session);
		break;
	case TestManager::E_PROTOCOL::MOVE:
		MoveProcess(_session);
		break;
	case TestManager::E_PROTOCOL::JUMP:
		JumpProcess(_session);
		break;
	case TestManager::E_PROTOCOL::DODGE:
		DodgeProcess(_session);
		break;
	case TestManager::E_PROTOCOL::FIRE:
		FireProcess(_session);
		break;
	case TestManager::E_PROTOCOL::EXIT:
		ExitProcess(_session);
		break;
	default:
		LogManager::GetInstance()->LogWrite(7777);
		break;
	}
}
void TestManager::IdCreateProcess(Session* _session)
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

void TestManager::PlayTypeProcess(Session* _session)
{
	LockGuard l_lockGuard(&m_criticalKey); // 잠금
	BYTE l_data[BUFSIZE];
	ZeroMemory(l_data, BUFSIZE);
	int l_dataSize = -1;

	MyStream l_stream;
	l_stream->SetStream(_session->GetDataField());

	int select;
	l_stream->ReadInt(&select);
	if (select == 1)
	{// 싱글 선택
		// 새로운 방 만들어서 입장
		int Num = m_room.size();
		m_room[Num].isMulti = false;
		m_room[Num].count = 1;
		m_room
		// 게임 시작 패킷 전송...
	}
	else // select == 2
	{//멀티 선택
		// 빈방 찾아보기
		bool flag = false;
		for (auto room : m_room)
		{
			if (room.second.isMulti)
			{
				if (room.second.count == 1)// 1명 이하 일때
				{
					room.second.count = 2;
					// 게임 시작 패킷 전송... (상대에게도)
					flag = true;
				}
			}
		}
		if (!flag)
		{// 빈방이 없으면 방만들기
			int Num = m_room.size();
			m_room[Num].isMulti = true;
			m_room[Num].count = 1;
			// 게임 대기 패킷 전송...
		}
	}
}

void TestManager::SpawnProcess(Session* _session)
{
	LockGuard l_lockGuard(&m_criticalKey); // 잠금
	BYTE l_data[BUFSIZE];
	ZeroMemory(l_data, BUFSIZE);
	int l_dataSize = -1;


	m_MoveDataList.insert(make_pair(_session, MoveData(_session->GetIdNumber())));
	l_dataSize = SpawnDataMake(l_data);

	for (list<Session*>::iterator iter = m_playerList.begin(); iter != m_playerList.end(); iter++)
	{
		if (!(*iter)->SendPacket(static_cast<int>(E_PROTOCOL::SPAWN), l_dataSize, l_data))
		{
			LogManager::GetInstance()->LogWrite(1005);
		}
	}
	return;
}

void TestManager::MoveProcess(Session* _session)
{
	LockGuard l_lockGuard(&m_criticalKey); // 잠금
	BYTE l_data[BUFSIZE];
	ZeroMemory(l_data, BUFSIZE);
	int l_dataSize = -1;

	MoveData moveData;
	MoveDataSplit(_session->GetDataField(), moveData);
	//
	// 이동 가능한 움직임인지 체크 하는 부분??? 
	//
	m_MoveDataList.find(_session)->second.CopyData(moveData);
	
	l_dataSize = MoveDataMake(l_data, moveData);

	for (list<Session*>::iterator iter = m_playerList.begin(); iter != m_playerList.end(); iter++)
	{
		if (!(*iter)->SendPacket(static_cast<int>(E_PROTOCOL::MOVE), l_dataSize, l_data))
		{
			LogManager::GetInstance()->LogWrite(1006);
		}
	}
	return;
}

void TestManager::JumpProcess(Session* _session)
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

	for (list<Session*>::iterator iter = m_playerList.begin(); iter != m_playerList.end(); iter++)
	{
		if (!(*iter)->SendPacket(static_cast<int>(E_PROTOCOL::JUMP), l_dataSize, l_data))
		{
			LogManager::GetInstance()->LogWrite(1006);
		}
	}
	return;
}

void TestManager::DodgeProcess(Session* _session)
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

	for (list<Session*>::iterator iter = m_playerList.begin(); iter != m_playerList.end(); iter++)
	{
		if (!(*iter)->SendPacket(static_cast<int>(E_PROTOCOL::DODGE), l_dataSize, l_data))
		{
			LogManager::GetInstance()->LogWrite(1006);
		}
	}
	return;
}

void TestManager::FireProcess(Session* _session)
{
	LockGuard l_lockGuard(&m_criticalKey); // 잠금
	BYTE l_data[BUFSIZE];
	ZeroMemory(l_data, BUFSIZE);
	int l_dataSize = -1;

	FireData fireData;
	FireDataSplit(_session->GetDataField(), fireData);

	l_dataSize = FireDataMake(l_data, fireData);

	for (list<Session*>::iterator iter = m_playerList.begin(); iter != m_playerList.end(); iter++)
	{
		if (!(*iter)->SendPacket(static_cast<int>(E_PROTOCOL::FIRE), l_dataSize, l_data))
		{
			LogManager::GetInstance()->LogWrite(1006);
		}
	}
	return;
}

void TestManager::ExitProcess(Session* _session)
{
	BYTE l_data[BUFSIZE];
	ZeroMemory(l_data, BUFSIZE);
	int l_dataSize = -1;

	LockGuard l_lockGuard(&m_criticalKey); // 잠금

	l_dataSize = ExitDataMake(l_data, _session->GetIdNumber());

	for (list<Session*>::iterator iter = m_playerList.begin(); iter != m_playerList.end(); iter++)
	{
		if ((*iter) == _session)
		{
			if (!(*iter)->SendPacket(static_cast<int>(E_PROTOCOL::EXIT), l_dataSize, l_data))
			{
				LogManager::GetInstance()->LogWrite(1006);
			}
		}
		if (!(*iter)->SendPacket(static_cast<int>(E_PROTOCOL::LEAVE), l_dataSize, l_data))
		{
			LogManager::GetInstance()->LogWrite(1006);
		}
	}

	m_MoveDataList.erase(_session);

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

void TestManager::ForceExitProcess(Session* _session)
{
	BYTE l_data[BUFSIZE];
	ZeroMemory(l_data, BUFSIZE);
	int l_dataSize = -1;

	LockGuard l_lockGuard(&m_criticalKey); // 잠금

	l_dataSize = ExitDataMake(l_data, _session->GetIdNumber());

	for (list<Session*>::iterator iter = m_playerList.begin(); iter != m_playerList.end(); iter++)
	{
		if ((*iter) == _session)
		{
			if (!(*iter)->SendPacket(static_cast<int>(E_PROTOCOL::EXIT), l_dataSize, l_data))
			{
				LogManager::GetInstance()->LogWrite(1006);
			}
		}
		if (!(*iter)->SendPacket(static_cast<int>(E_PROTOCOL::LEAVE), l_dataSize, l_data))
		{
			LogManager::GetInstance()->LogWrite(1006);
		}
	}

	m_MoveDataList.erase(_session);

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

int TestManager::IdDataMake(BYTE* _data, int _id)
{
	MyStream l_stream;
	l_stream->SetStream(_data);
	l_stream->WriteInt(_id);

	return l_stream->GetLength();
}

int TestManager::SpawnDataMake(BYTE* _data)
{
	MyStream l_stream;
	l_stream->SetStream(_data);

	l_stream->WriteInt(static_cast<int>(m_playerList.size()));

	for (list<Session*>::iterator iter = m_playerList.begin(); iter != m_playerList.end(); iter++)
	{
		l_stream->WriteInt((*iter)->GetIdNumber());
	}

	return l_stream->GetLength();
}

int TestManager::MoveDataMake(BYTE* _data, MoveData _moveData)
{
	MyStream l_stream;
	l_stream->SetStream(_data);

	// 2후보
	l_stream->WriteBytes(reinterpret_cast<BYTE*>(&_moveData), sizeof(MoveData));

	return l_stream->GetLength();
}

int TestManager::JumpDataMake(BYTE* _data, int _id)
{
	MyStream l_stream;
	l_stream->SetStream(_data);
	l_stream->WriteInt(_id);

	return l_stream->GetLength();
}

int TestManager::DodgeDataMake(BYTE* _data, int _id)
{
	MyStream l_stream;
	l_stream->SetStream(_data);
	l_stream->WriteInt(_id);

	return l_stream->GetLength();
}

int TestManager::FireDataMake(BYTE* _data, FireData _fireData)
{
	MyStream l_stream;
	l_stream->SetStream(_data);

	l_stream->WriteBytes(reinterpret_cast<BYTE*>(&_fireData), sizeof(FireData));

	return l_stream->GetLength();
}

int TestManager::ExitDataMake(BYTE* _data, int _id)
{
	MyStream l_stream;
	l_stream->SetStream(_data);
	l_stream->WriteInt(_id);

	return l_stream->GetLength();
}


void TestManager::MoveDataSplit(BYTE* _data, MoveData& _moveData)
{
	MyStream l_stream;
	l_stream->SetStream(_data);
	l_stream->ReadBytes(reinterpret_cast<BYTE*>(& _moveData), sizeof(MoveData));
}

void TestManager::FireDataSplit(BYTE* _data, FireData& _fireData)
{
	MyStream l_stream;
	l_stream->SetStream(_data);
	l_stream->ReadBytes(reinterpret_cast<BYTE*>(&_fireData), sizeof(FireData));
	/*
	BYTE* l_focusPointer = _data;

	memcpy(&_fireData, l_focusPointer, sizeof(FireData));
	l_focusPointer = l_focusPointer + sizeof(FireData);*/
}