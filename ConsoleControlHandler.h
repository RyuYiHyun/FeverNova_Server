#pragma once
#include "Global.h"
class ConsoleControlHandler
{
#pragma region Singleton
public:
	static bool CreateInstance(); // ����
	static void DestroyInstance(); // ����
	bool Initialize(); // �ʱ�ȭ
	void Release(); // ��ó��
	static ConsoleControlHandler* GetInstance();
private:
	static ConsoleControlHandler* m_instance;		// �ν��Ͻ� ����

	ConsoleControlHandler();
	virtual ~ConsoleControlHandler();
#pragma endregion

public:
	static BOOL ControlHandler(DWORD _controlEvent);
private:
};