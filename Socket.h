#pragma once
#include "Global.h"
class Socket
{
public:
	Socket();
	Socket(const SOCKET& _socket);
	virtual ~Socket();

	// getter
	net::OVERLAPPEDEX* GetRecvOverlapped();
	net::OVERLAPPEDEX* GetSendOverlapped();
	SOCKET GetSocket();
	SOCKADDR_IN GetAddress();

	// accept 작업수행하고, 결과로 이 서버와 연결된 클라의 소켓이 리턴됨
	SOCKET Accept();
	// 이 소켓에 자신의 주소와, 포트번호를 bind하고, 리슨소켓으로 바꿔주는 함수 (서버의 경우)
	bool ListenStart(unsigned long _own_address, unsigned short _serverPort);

protected:
	net::OVERLAPPEDEX m_sendOverlap;
	net::OVERLAPPEDEX m_recvOverlap;

	SOCKET m_socket;
	SOCKADDR_IN m_address;

	bool Send(WSABUF);
	bool Recv(WSABUF);
};