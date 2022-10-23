#pragma pack (push, 1)
struct Vector2
{
	Vector2() { X = 0; Y = 0; }
	Vector2(float _X, float _Y) { X = _X; Y = _Y; }
	float X; float Y;
};

struct Vector3
{
	Vector3(){ X = 0; Y = 0; Z = 0; }
	Vector3(float _X, float _Y, float _Z) { X = _X; Y = _Y; Z = _Z; }
	float X; float Y; float Z;
};
struct Quaternion
{
	Quaternion(){ X = 0; Y = 0; Z = 0; W = 0; }
	Quaternion(float _X, float _Y, float _Z, float _W) { X = _X; Y = _Y; Z = _Z; W = _W; }
	float X; float Y; float Z; float W;
};
struct MoveData
{
	MoveData() { m_id = -1; m_state = 0; }
	MoveData(int _id) { m_id = _id; m_state = 0; }
	void CopyData(MoveData _Src)
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

struct FireData
{
	FireData() { m_id = -1; }
	FireData(int _id) { m_id = _id; }
	void CopyData(FireData _Src)
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

struct NpcTriggerData
{
	NpcTriggerData(){}
	NpcTriggerData(int _objectID) { m_objectID = _objectID, m_flag = -1; }
	void CopyData(NpcTriggerData _Src)
	{
		m_objectID = _Src.m_objectID;
		m_flag = _Src.m_flag;
	}
	int m_objectID;
	int m_flag;
};

struct NpcAttackData
{
	NpcAttackData() {}
	void CopyData(NpcAttackData _Src)
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

#pragma pack(pop)