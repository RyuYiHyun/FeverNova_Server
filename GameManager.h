#pragma once
#include "Global.h"
#include "Socket.h"
#include "Session.h"
#include "SessionManager.h"
#include "PacketData.h"
#include "RoomManager.h"
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
		CRYPTOKEY,
		IDCREATE,

		PLAYTYPE,
		WAIT,
		SINGLE_START,
		MULTI_HOST_START,
		MULTI_GUEST_START,

		SPAWN,
		MOVE,
		JUMP,
		DODGE,
		FIRE,
		LEAVE,
		EXIT,

		//JUNYOUNG_PROTOCOL
		NPC_SPAWN,
		NPC_TRANSFORM,
		ITEM_SPAWN,

		//KJB_PROTOCOL
		NPC_TRIGGER,
		NPC_ATTACK,
	};

	void Function(Session* _session);

	void IdCreateProcess(Session* _session);
	void PlayTypeProcess(Session* _session);

	void SpawnProcess(Session* _session);
	void MoveProcess(Session* _session);
	void JumpProcess(Session* _session);
	void DodgeProcess(Session* _session);
	void FireProcess(Session* _session);
	void NpcSpawnProcess(Session* _session);
	void NpcTransformProcess(Session* _session);
	void ItemSpawnProcess(Session* _session);
	void NpcTriggerProcess(Session* _session);
	void NpcAttackProcess(Session* _session);

	void ExitProcess(Session* _session);
	void ForceExitProcess(Session* _session);

#pragma region Packing&Unpacking
	// packing
	int IdDataMake(BYTE* _data, int _id);
	int SpawnDataMake(BYTE* _data, Room& _room);
	int MoveDataMake(BYTE* _data, MoveData _moveData);
	int JumpDataMake(BYTE* _data, int _id);
	int DodgeDataMake(BYTE* _data, int _id);
	int FireDataMake(BYTE* _data, FireData _fireData);
	int NpcSpawnDataMake(BYTE* _data, SpawnData_NPC _SpawnData_npc);
	int NpcTransformDataMake(BYTE* _data, TransformData_NPC _TransformData_npc);
	int ItemSpawnDataMake(BYTE* _data, SpawnData_Item _SpawnData_Item);
	int NpcTriggerDataMake(BYTE* _data, NpcTriggerData _npcTriggerData);
	int NpcAttackDataMake(BYTE* _data, NpcAttackData _npcTriggerData);

	int ExitDataMake(BYTE* _data, int _id);
	// unpacking
	void MoveDataSplit(BYTE* _data, MoveData& _moveData);
	void FireDataSplit(BYTE* _data, FireData& _fireData);
	void NpcSpawnDataSplit(BYTE* _data, SpawnData_NPC& _SpawnData_npc);
	void NpcTransformDataSplit(BYTE* _data, TransformData_NPC& _TransformData_npc);
	void ItemSpawnDataSplit(BYTE* _data, SpawnData_Item& _SpawnData_Item);
#pragma endregion
private:
	CriticalKey m_criticalKey;
	int m_giveIdCounter;
	list<Session*> m_playerList;
};

