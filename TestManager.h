#pragma once
#include "Global.h"
#include "Socket.h"
#include "Session.h"
#include "SessionManager.h"
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

		Test,
	};
	struct Vector2
	{
		Vector2()
		{
			X = 0; Y = 0;
		}
		Vector2(float _X, float _Y)
		{
			X = _X; Y = _Y;
		}
		float X;
		float Y;
	};
	struct Vector3
	{
		Vector3()
		{
			X = 0; Y = 0; Z = 0;
		}
		Vector3(float _X, float _Y, float _Z)
		{
			X = _X; Y = _Y; Z = _Z;
		}
		float X;
		float Y;
		float Z;
	};
	struct Quaternion
	{
		Quaternion()
		{
			X = 0; Y = 0; Z = 0; W = 0;
		}
		Quaternion(float _X, float _Y, float _Z, float _W)
		{
			X = _X; Y = _Y; Z = _Z; W = _W;
		}
		float X;
		float Y;
		float Z;
		float W;
	};
	

	void Function(Session* _session);

	void IdCreateProcess(Session* _session);
	void SpawnProcess(Session* _session);
	void MoveProcess(Session* _session);
	void JumpProcess(Session* _session);
	void DodgeProcess(Session* _session);
	void FireProcess(Session* _session);

	void ExitProcess(Session* _session);
	void ForceExitProcess(Session* _session);


#pragma region 태스트용 집합

#pragma pack (push, 1)
	struct MoveData
	{
		MoveData()
		{
			m_id = -1;
			m_state = 0;
		}
		MoveData(int _id)
		{
			m_id = _id;
			m_state = 0;
		}
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
		FireData()
		{
			m_id = -1;
		}
		FireData(int _id)
		{
			m_id = _id;
		}
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



	struct TestListData
	{
		TestListData(int i, short s, float f)
		{
			m_int = i;
			m_short = s;
			m_float = f;
		}
		int m_int;
		short m_short;
		float m_float;
	};
	void TestProcess(Session* _session);
	int TestDataMake(BYTE* _data);

	list<TestListData> TestList;
#pragma pack(pop)

#pragma endregion

	
#pragma region Packing&Unpacking
	// packing
	//int SpawnDataMake(BYTE* _data, int _id);
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
	// 플레이어 정보 - 세션
	// 몬스터 정보 - 무관
	// 지형 정보 - 무관
};

