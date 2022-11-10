#pragma pack (push, 1)
struct IntData
{
	int m_int;
};

struct Vector2
{
	Vector2() { X = 0; Y = 0; }
	Vector2(float _X, float _Y) { X = _X; Y = _Y; }
	float X; float Y;
};

struct Vector3
{
	Vector3() { X = 0; Y = 0; Z = 0; }
	Vector3(float _X, float _Y, float _Z) { X = _X; Y = _Y; Z = _Z; }
	float X; float Y; float Z;
};
struct Quaternion
{
	Quaternion() { X = 0; Y = 0; Z = 0; W = 0; }
	Quaternion(float _X, float _Y, float _Z, float _W) { X = _X; Y = _Y; Z = _Z; W = _W; }
	float X; float Y; float Z; float W;
};

// JY : Rename
// Before : MoveData
struct PlayerTransformData
{
	PlayerTransformData() { m_id = -1; m_state = 0; }
	PlayerTransformData(int _id) { m_id = _id; m_state = 0; }
	void CopyData(PlayerTransformData _Src)
	{
		m_id = _Src.m_id;
		m_velocity = _Src.m_velocity;
		m_position = _Src.m_position;
		m_rotation = _Src.m_rotation;
		m_move = _Src.m_move;
		m_animing = _Src.m_animing;
		m_state = _Src.m_state;
	}
	int m_id;
	Vector3 m_velocity;
	Vector3 m_position;
	Quaternion m_rotation;
	Vector2 m_move;
	float m_animing;
	int m_state;
};

// JY : Rename
// Before : FireData
struct PlayerFireData
{
	PlayerFireData() { m_id = -1; }
	PlayerFireData(int _id) { m_id = _id; }
	void CopyData(PlayerFireData _Src)
	{
		m_id = _Src.m_id;
		m_position = _Src.m_position;
		m_direction = _Src.m_direction;
		m_currentMosueRadius = _Src.m_currentMosueRadius;
	}
	int m_id;
	Vector3 m_position;
	Vector3 m_direction;
	float m_currentMosueRadius;
};

// JY : Rename
// Before : PacketSpawnData_NPC
struct NpcSpawnData
{
	NpcSpawnData() {}
	NpcSpawnData(int _objectID, int _monsterID, Vector3 _position)
	{
		m_objectID = _objectID;
		m_monsterID = _monsterID;
		m_position = _position;
	}
	int m_objectID;
	int m_monsterID;
	Vector3 m_position;
};

// JY : Rename
// Before : PacketMoveData_NPC
struct NpcTransformData
{
	NpcTransformData() {}
	NpcTransformData(int _objectID, Vector3 _position, Quaternion _rotation)
	{
		m_objectID = _objectID;
		m_position = _position;
		m_rotation = _rotation;
	}
	int m_objectID;
	Vector3 m_position;
	Quaternion m_rotation;
};

struct NpcTransformListData
{
	int m_size;
	NpcTransformData m_npcTransformListData[50];
};

struct NpcHpData
{
	NpcHpData() {}
	NpcHpData(int _objectID, int _currentHP, int _lastAttackedPlayerID)
	{
		m_objectID = _objectID;
		m_currentHP = _currentHP;
		m_lastAttackedPlayerID = _lastAttackedPlayerID;
	}
	int m_objectID;
	int m_currentHP;
	int m_lastAttackedPlayerID;
};

struct NpcTriggerData
{
	NpcTriggerData() {}
	NpcTriggerData(int _objectID) { m_objectID = _objectID, m_flag = -1; }
	void CopyData(NpcTriggerData _Src)
	{
		m_objectID = _Src.m_objectID;
		m_flag = _Src.m_flag;
	}
	int m_objectID;
	int m_flag;
};

// JY : Rename
// Before : NpcAttackData
struct NpcSkillData
{
	NpcSkillData() {}
	void CopyData(NpcSkillData _Src)
	{
		m_objectID = _Src.m_objectID;
		m_position = _Src.m_position;
		m_rotation = _Src.m_rotation;
		m_index = _Src.m_index;
		m_targetID = _Src.m_targetID;
	}

	int m_objectID;
	Vector3 m_position;
	Quaternion m_rotation;

	int m_index;
	int m_targetID;


};

struct ItemSpawnData
{
	ItemSpawnData() {}
	ItemSpawnData(int _itemID, Vector3 _position)
	{
		m_itemID = _itemID;
		m_position = _position;
	}
	int m_itemID;
	Vector3 m_position;
};

struct ItemDeSpawnData
{
	ItemDeSpawnData() {}
	ItemDeSpawnData(int _playerID, int _itemID)
	{
		m_playerID = _playerID;
		m_itemID = _itemID;
	}
	int m_playerID;
	int m_itemID;
};

struct PcHitData//kjb 추가
{
	int m_shooterType;
	int m_playerID;
	int m_playerHP;
};

struct DoorUseData//kjb 추가
{
	int m_doorID;
	float m_position_Y;
};

// JJCH ---------------------
struct ReqestionUIData
{
	int m_reqestionKind;
	int m_beforeSceneKind;
};
// ---------------------------

struct PlayerRunParticleChangeData//kjb 22.11.11
{
	int m_playerID;
	int m_paticleIndex;
};
#pragma pack(pop)