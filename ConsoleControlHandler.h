#pragma once
#include "Global.h"
class ConsoleControlHandler
{
#pragma region Singleton
public:
	static bool CreateInstance(); // 생성
	static void DestroyInstance(); // 삭제
	bool Initialize(); // 초기화
	void Release(); // 후처리
	static ConsoleControlHandler* GetInstance();
private:
	static ConsoleControlHandler* m_instance;		// 인스턴스 변수

	ConsoleControlHandler();
	virtual ~ConsoleControlHandler();
#pragma endregion

public:
	static BOOL ControlHandler(DWORD _controlEvent);
private:
};