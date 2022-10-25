#pragma once
#include "Global.h"
#include "Socket.h"
#include "Session.h"
#include "SessionManager.h"
#include "PacketData.h"
#include "RoomManager.h"
#include "LogManager.h"
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
		CRYPTOKEY,      // �ʱ� �Ϻ�ȣȭŰ ���� ��ȣ
		IDCREATE,       // ���̵� ����

		PLAYTYPE,       // ��Ƽ or �̱� ����
		WAIT,           // ��Ƽ ���
		SINGLE_START,           // �̱� ����
		MULTI_HOST_START,       // ȣ��Ʈ(��ü) ����
		MULTI_GUEST_START,      // �Խ�Ʈ(����) ����

		LEAVE,
		EXIT,

		PLAYER_SPAWN,
		PLAYER_TRANSFORM,
		PLAYER_JUMP,
		PLAYER_DODGE,
		PLAYER_FIRE,

		NPC_SPAWN,
		// JY : Replace
		// jjch PROTOCOL - REMOTE_MOVE
		NPC_TRANSFORM,
		// JY : Replace
		// jjch PROTOCOL - REMOTE_HIT
		NPC_UPDATEHP,
		// JY : Replace
		// jjch PROTOCOL - REMOTE_DEATH
		NPC_TRIGGER,
		// JY : Replace
		// jjch PROTOCOL - REMOTE_ATTACK
		NPC_SKILL,

		ITEM_SPAWN,
		ITEM_DESPAWN,
	};

	void Function(Session* _session);

	void IdCreateProcess(Session* _session);
	void PlayTypeProcess(Session* _session);

	void PlayerSpawnProcess(Session* _session);
	void PlayerTransformProcess(Session* _session);
	void PlayerJumpProcess(Session* _session);
	void PlayerDodgeProcess(Session* _session);
	void PlayerFireProcess(Session* _session);

	void NpcSpawnProcess(Session* _session);
	void NpcTransformProcess(Session* _session);
	void NpcUpdateHpProcess(Session* _session);
	void NpcTriggerProcess(Session* _session);
	void NpcSkillProcess(Session* _session);

	void ItemSpawnProcess(Session* _session);
	void ItemDeSpawnProcess(Session* _sesison);

	void ExitProcess(Session* _session);
	void ForceExitProcess(Session* _session);

#pragma region Packing&Unpacking
	// packing
	int IdDataMake(BYTE* _data, int _id);
	int PlayerSpawnDataMake(BYTE* _data, Room& _room);
	int PlayerTransformDataMake(BYTE* _data, PlayerTransformData _playerTransformData);
	int PlayerJumpDataMake(BYTE* _data, int _id);
	int PlayerDodgeDataMake(BYTE* _data, int _id);
	int PlayerFireDataMake(BYTE* _data, PlayerFireData _playerFireData);

	int NpcSpawnDataMake(BYTE* _data, NpcSpawnData _npcSpawnData);
	int NpcTransformDataMake(BYTE* _data, NpcTransformData _npcTransformData);
	int NpcUpdateHpDataMake(BYTE* _data, NpcHpData _npcHpData);
	int NpcTriggerDataMake(BYTE* _data, NpcTriggerData _npcTriggerData);
	int NpcSkillDataMake(BYTE* _data, NpcSkillData _npcSkillData);

	int ItemSpawnDataMake(BYTE* _data, ItemSpawnData _itemSpawnData);
	int ItemDeSpawnDataMake(BYTE* _data, ItemDeSpawnData _itemDeSpawnData);

	int ExitDataMake(BYTE* _data, int _id);

	// unpacking
	void PlayerTransformDataSplit(BYTE* _data, PlayerTransformData& _playerTransformData);
	void PlayerFireDataSplit(BYTE* _data, PlayerFireData& _playerFireData);

	void NpcSpawnDataSplit(BYTE* _data, NpcSpawnData& _npcSpawnData);
	void NpcTransformDataSplit(BYTE* _data, NpcTransformData& _npcTransformData);
	void NpcUpdateHpDataSplit(BYTE* _data, NpcHpData& _npcHpData);
	void NpcTriggerDataSplit(BYTE* _data, NpcTriggerData& _npcTriggerData);
	void NpcSkillDataSplit(BYTE* _data, NpcSkillData& _npcSkillData);

	void ItemSpawnDataSplit(BYTE* _data, ItemSpawnData& _itemSpawnData);
	void ItemDeSpawnDataSplit(BYTE* _data, ItemDeSpawnData& _itemDeSpawnData);
#pragma endregion
private:
	CriticalKey m_criticalKey;
	int m_giveIdCounter;
	list<Session*> m_playerList;
};
namespace game
{
	class util
	{
	public:
		static bool SEND(Session* _session, GameManager::E_PROTOCOL _protocol, int& _dataSize, BYTE* _data)
		{
			if (!_session->SendPacket(static_cast<int>(_protocol), _dataSize, _data))
			{
				LogManager::GetInstance()->LogWrite(1006);
				return false;
			}
			return true;
		}
	};
};