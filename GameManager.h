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
		NPC_TRANSFORM,
		NPC_UPDATEHP,
		NPC_TRIGGER,
		NPC_SKILL,

		ITEM_SPAWN,
		ITEM_DESPAWN,

		//kjb
		PC_HIT,
		Door_Use,
		//RYU
		REPLAY_REQUEST,
		REPLAY_REPLY,
		ROADCOMPLETE,
		REPLAY_START,
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

	void PcHitProcess(Session* _session);
	void DoorUseProcess(Session* _sesison);

	void ExitProcess(Session* _session);
	void ForceExitProcess(Session* _session);

	void ReplayRequestProcess(Session* _session);
	void ReplyProcess(Session* _session);
	void RoadCompleteProcess(Session* _session);

#pragma region Packing&Unpacking
	// packing
	int PlayerSpawnDataMake(BYTE* _data, Room& _room);
	// unpacking
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