#pragma once
#include "Global.h"
#include "Socket.h"
#include "Session.h"
#include "SessionManager.h"
#include "PacketData.h"
#include <map>
class GameManager
{
#pragma region Singleton
public:
	static bool CreateInstance();
	static void DestroyInstance();
	bool Initialize();
	void Release();
	static GameManager* GetInstance();
private:
	static GameManager* m_instance;

	GameManager();
	virtual ~GameManager();
#pragma endregion

public:
	enum class E_PROTOCOL
	{
		CRYPTOKEY,		// 서버 -> 클라				:	초기 암복호화키 전송 신호

		STC_IDCREATE,
		CTS_IDCREATE,
		
		STC_SPAWN,
		CTS_SPAWN,

		STC_MOVE,
		CTS_MOVE,

		STC_JUMP,
		CTS_JUMP,

		STC_DODGE,
		CTS_DODGE,

		STC_FIRE,
		CTS_FIRE,

		STC_OUT,
		CTS_OUT,

		STC_EXIT,
		CTS_EXIT,
		
		STC_NPC_TRIGGER,
		CTS_NPC_TRIGGER,

		STC_NPC_ATTACK,
		CTS_NPC_ATTACK,

		Test,
	};

	void Function(Session* _session);

	void IdCreateProcess(Session* _session);
	void SpawnProcess(Session* _session);
	void MoveProcess(Session* _session);
	void JumpProcess(Session* _session);
	void DodgeProcess(Session* _session);
	void FireProcess(Session* _session);
	void NpcTriggerProcess(Session* _session);
	void NpcAttackProcess(Session* _session);

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
	int NpcTriggerDataMake(BYTE* _data, NpcTriggerData _npcTriggerData);
	int NpcAttackDataMake(BYTE* _data, NpcAttackData _npcTriggerData);

	int ExitDataMake(BYTE* _data, int _id);

	// unpacking
	void MoveDataSplit(BYTE* _data, MoveData& _moveData);
	void FireDataSplit(BYTE* _data, FireData& _fireData);
	

#pragma endregion

#pragma region  Test
	void TestProcess(Session* _session);
	int TestDataMake(BYTE* _data);
#pragma endregion

private:
	CriticalKey m_criticalKey;
	int m_giveIdCounter;
	list<Session*> m_playerList;
	map<Session*, MoveData> m_MoveDataList;
	// 플레이어 정보 - 세션
	// 몬스터 정보 - 무관
	// 지형 정보 - 무관
};

