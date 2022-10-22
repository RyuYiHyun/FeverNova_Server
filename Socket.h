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

	// accept �۾������ϰ�, ����� �� ������ ����� Ŭ���� ������ ���ϵ�
	SOCKET Accept();
	// �� ���Ͽ� �ڽ��� �ּҿ�, ��Ʈ��ȣ�� bind�ϰ�, ������������ �ٲ��ִ� �Լ� (������ ���)
	bool ListenStart(unsigned long _own_address, unsigned short _serverPort);

protected:
	net::OVERLAPPEDEX m_sendOverlap;
	net::OVERLAPPEDEX m_recvOverlap;

	SOCKET m_socket;
	SOCKADDR_IN m_address;

	bool Send(WSABUF);
	bool Recv(WSABUF);
};