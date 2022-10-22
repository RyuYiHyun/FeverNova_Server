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
	m_giveIdCounter = 10;

#pragma region 태스트용
	//TestList.push_back(TestListData(101, 85, 15.125f));
	//TestList.push_back(TestListData(111, 185, 915.125f));
	//TestList.push_back(TestListData(121, 285, 815.125f));
	//TestList.push_back(TestListData(131, 385, 115.125f));
#pragma endregion

	
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
	case TestManager::E_PROTOCOL::CTS_IDCREATE:
		IdCreateProcess(_session);
		break;
	case TestManager::E_PROTOCOL::CTS_SPAWN:
		SpawnProcess(_session);
		break;
	case TestManager::E_PROTOCOL::CTS_MOVE:
		MoveProcess(_session);
		break;
	case TestManager::E_PROTOCOL::CTS_JUMP:
		JumpProcess(_session);
		break;
	case TestManager::E_PROTOCOL::CTS_DODGE:
		DodgeProcess(_session);
		break;
	case TestManager::E_PROTOCOL::CTS_FIRE:
		FireProcess(_session);
		break;
	case TestManager::E_PROTOCOL::CTS_EXIT:
		ExitProcess(_session);
		break;
	case TestManager::E_PROTOCOL::Test:
		TestProcess(_session);
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
	
	if (!_session->SendPacket(static_cast<int>(E_PROTOCOL::STC_IDCREATE), l_dataSize, l_data))
	{
		LogManager::GetInstance()->LogWrite(1005);
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
		if (!(*iter)->SendPacket(static_cast<int>(E_PROTOCOL::STC_SPAWN), l_dataSize, l_data))
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
		if (!(*iter)->SendPacket(static_cast<int>(E_PROTOCOL::STC_MOVE), l_dataSize, l_data))
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
		if (!(*iter)->SendPacket(static_cast<int>(E_PROTOCOL::STC_JUMP), l_dataSize, l_data))
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
		if (!(*iter)->SendPacket(static_cast<int>(E_PROTOCOL::STC_DODGE), l_dataSize, l_data))
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
		if (!(*iter)->SendPacket(static_cast<int>(E_PROTOCOL::STC_FIRE), l_dataSize, l_data))
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
			if (!(*iter)->SendPacket(static_cast<int>(E_PROTOCOL::STC_EXIT), l_dataSize, l_data))
			{
				LogManager::GetInstance()->LogWrite(1006);
			}
		}
		if (!(*iter)->SendPacket(static_cast<int>(E_PROTOCOL::STC_OUT), l_dataSize, l_data))
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
			if (!(*iter)->SendPacket(static_cast<int>(E_PROTOCOL::STC_EXIT), l_dataSize, l_data))
			{
				LogManager::GetInstance()->LogWrite(1006);
			}
		}
		if (!(*iter)->SendPacket(static_cast<int>(E_PROTOCOL::STC_OUT), l_dataSize, l_data))
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
	/*int l_packedSize = 0;
	BYTE* l_focusPointer = _data;
	l_focusPointer = net::util::WriteToByteStream(l_focusPointer, l_packedSize, _id);
	return l_packedSize;*/
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
	

	/*int l_packedSize = 0;
	BYTE* l_focusPointer = _data;
	int counter = 5;
	l_focusPointer = net::util::WriteToByteStream(l_focusPointer, l_packedSize, static_cast<int>(m_playerList.size()));
	for (list<Session*>::iterator iter = m_playerList.begin(); iter != m_playerList.end(); iter++)
	{
		l_focusPointer = net::util::WriteToByteStream(l_focusPointer, l_packedSize, (*iter)->GetIdNumber());
		counter--;
	}

	while (counter > 0)
	{
		l_focusPointer = net::util::WriteToByteStream(l_focusPointer, l_packedSize, -1);
		counter--;
		if (counter <= 0)
		{
			break;
		}
	}

	return l_packedSize;*/
}

int TestManager::MoveDataMake(BYTE* _data, MoveData _moveData)
{
	MyStream l_stream;
	l_stream->SetStream(_data);

	// 2후보
	l_stream->WriteBytes(reinterpret_cast<BYTE*>(&_moveData), sizeof(MoveData));

	return l_stream->GetLength();

	/*int l_packedSize = 0;
	BYTE* l_focusPointer = _data;

	l_focusPointer = net::util::WriteToByteStream(l_focusPointer, l_packedSize, _moveData);

	return l_packedSize;*/
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
	/*int l_packedSize = 0;
	BYTE* l_focusPointer = _data;

	l_focusPointer = net::util::WriteToByteStream(l_focusPointer, l_packedSize, _id);

	return l_packedSize;*/
}


void TestManager::MoveDataSplit(BYTE* _data, MoveData& _moveData)
{
	BYTE* l_focusPointer = _data;

	memcpy(&_moveData, l_focusPointer, sizeof(MoveData));
	l_focusPointer = l_focusPointer + sizeof(MoveData);
}

void TestManager::FireDataSplit(BYTE* _data, FireData& _fireData)
{
	BYTE* l_focusPointer = _data;

	memcpy(&_fireData, l_focusPointer, sizeof(FireData));
	l_focusPointer = l_focusPointer + sizeof(FireData);
}



#pragma region 태스트용
void TestManager::TestProcess(Session* _session)
{
	LockGuard l_lockGuard(&m_criticalKey); // 잠금
	BYTE l_data[BUFSIZE];
	ZeroMemory(l_data, BUFSIZE);
	int l_dataSize = -1;

	l_dataSize = TestDataMake(l_data);

	for (list<Session*>::iterator iter = m_playerList.begin(); iter != m_playerList.end(); iter++)
	{
		if (!(*iter)->SendPacket(static_cast<int>(E_PROTOCOL::Test), l_dataSize, l_data))
		{
			LogManager::GetInstance()->LogWrite(1005);
		}
	}

	return;
}

int TestManager::TestDataMake(BYTE* _data)
{
	int l_packedSize = 0;
	BYTE* l_focusPointer = _data;
	l_focusPointer = net::util::WriteToByteStream(l_focusPointer, l_packedSize, static_cast<int>(TestList.size()));
	for (list<TestListData>::iterator iter = TestList.begin(); iter != TestList.end(); iter++)
	{
		l_focusPointer = net::util::WriteToByteStream(l_focusPointer, l_packedSize, (*iter));
	}

	return l_packedSize;
}
#pragma endregion
