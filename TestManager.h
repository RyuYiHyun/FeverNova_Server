#pragma once
#include "Global.h"
#include "Socket.h"
#include "Session.h"
#include "SessionManager.h"
#include "PacketData.h"
#include <map>
class TestManager
{
#pragma region Singleton
public:
	static bool CreateInstance();
	static void DestroyInstance();
	bool Initialize();
	void Release();
	static TestManager* GetInstance();
private:
	static TestManager* m_instance;

	TestManager();
	virtual ~TestManager();
#pragma endregion

public:
	enum class E_PROTOCOL
	{
		// �ʱ� �ʿ� ��
		CRYPTOKEY,		// �ʱ� �Ϻ�ȣȭŰ ���� ��ȣ
		IDCREATE,		// ���̵� ����

		PLAYTYPE,		// ��Ƽ or �̱� ����
		WAIT,			// ��Ƽ ���
		START,			// �̱� ���� or ��Ƽ 2�� ��Ī ����

		SPAWN,
		MOVE,
		JUMP,
		DODGE,
		FIRE,
		LEAVE,
		EXIT,
	};

	void Function(Session* _session);

	void IdCreateProcess(Session* _session);
	void PlayTypeProcess(Session* _session);

	void SpawnProcess(Session* _session);
	void MoveProcess(Session* _session);
	void JumpProcess(Session* _session);
	void DodgeProcess(Session* _session);
	void FireProcess(Session* _session);

	void ExitProcess(Session* _session);
	void ForceExitProcess(Session* _session);

#pragma region Packing&Unpacking
	// packing
	int IdDataMake(BYTE* _data, int _id);
	int SpawnDataMake(BYTE* _data);
	int MoveDataMake(BYTE* _data, MoveData _moveData);
	int JumpDataMake(BYTE* _data, int _id);
	int DodgeDataMake(BYTE* _data, int _id);
	int FireDataMake(BYTE* _data, FireData _fireData);


	int ExitDataMake(BYTE* _data, int _id);
	// unpacking
	void MoveDataSplit(BYTE* _data, MoveData& _moveData);
	void FireDataSplit(BYTE* _data, FireData& _fireData);
#pragma endregion
private:
	CriticalKey m_criticalKey;
	int m_giveIdCounter;
	list<Session*> m_playerList;
	map<Session*, MoveData> m_MoveDataList;
	// �÷��̾� ���� - ����
	// ���� ���� - ����
	// ���� ���� - ����
};

