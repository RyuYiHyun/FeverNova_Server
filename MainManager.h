#pragma once
#include "IOCP.h"
#include "Socket.h"
// 완료
class MainManager : public IOCP
{
#pragma region Singleton
public:
	static bool CreateInstance(); // 생성
	static void DestroyInstance(); // 삭제
	bool Initialize(); // 초기화
	void Release(); // 후처리
	static MainManager* GetInstance();
private:
	static MainManager* m_instance;		// 인스턴스 변수

	MainManager();
	virtual ~MainManager();
#pragma endregion
public:
	void Run(); // IOCP 서버 실행 (override from IOCP class)

	bool AcceptProcess(SOCKET _client_sock); // Session Accept 작업 (override from IOCP class)
	bool RecvProcess(void* _session, int _completeByte); // Session message Recv 작업 (override from IOCP class)
	bool SendProcess(void* _session, int _completeByte); // Session message Send 작업 (override from IOCP class)
	void DisconnectedProcess(void* _session); // Session Disconnected 작업 (override from IOCP class)
private:
	Socket m_listenSock; // listen 소켓
};

