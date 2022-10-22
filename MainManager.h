#pragma once
#include "IOCP.h"
#include "Socket.h"
// �Ϸ�
class MainManager : public IOCP
{
#pragma region Singleton
public:
	static bool CreateInstance(); // ����
	static void DestroyInstance(); // ����
	bool Initialize(); // �ʱ�ȭ
	void Release(); // ��ó��
	static MainManager* GetInstance();
private:
	static MainManager* m_instance;		// �ν��Ͻ� ����

	MainManager();
	virtual ~MainManager();
#pragma endregion
public:
	void Run(); // IOCP ���� ���� (override from IOCP class)

	bool AcceptProcess(SOCKET _client_sock); // Session Accept �۾� (override from IOCP class)
	bool RecvProcess(void* _session, int _completeByte); // Session message Recv �۾� (override from IOCP class)
	bool SendProcess(void* _session, int _completeByte); // Session message Send �۾� (override from IOCP class)
	void DisconnectedProcess(void* _session); // Session Disconnected �۾� (override from IOCP class)
private:
	Socket m_listenSock; // listen ����
};

