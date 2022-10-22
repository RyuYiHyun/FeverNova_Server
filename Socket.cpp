#include "Socket.h"
#include "LogManager.h"
// 체크 완
#pragma region 생성자&소멸자
Socket::Socket()
{
	ZeroMemory(&m_sendOverlap, sizeof(net::OVERLAPPEDEX));
	ZeroMemory(&m_recvOverlap, sizeof(net::OVERLAPPEDEX));
	ZeroMemory(&m_socket, sizeof(SOCKET));
	ZeroMemory(&m_address, sizeof(SOCKADDR_IN));

	m_sendOverlap.session = this;
	m_sendOverlap.operation = net::E_OPERATION::SEND;

	m_recvOverlap.session = this;
	m_recvOverlap.operation = net::E_OPERATION::RECV;
}

Socket::Socket(const SOCKET& _socket) : Socket()
{
	m_socket = _socket;
	int l_addressLength = sizeof(m_address);
	getpeername(_socket, reinterpret_cast<SOCKADDR*>(&m_address), &l_addressLength);
}

Socket::~Socket()
{
	closesocket(m_socket);
}
#pragma endregion


#pragma region Getter&Setter
net::OVERLAPPEDEX* Socket::GetRecvOverlapped()
{
	return &m_recvOverlap;
}

net::OVERLAPPEDEX* Socket::GetSendOverlapped()
{
	return &m_sendOverlap;
}

SOCKET Socket::GetSocket()
{
	return m_socket;
}

SOCKADDR_IN Socket::GetAddress()
{
	return m_address;
}
#pragma endregion


#pragma region 함수
SOCKET Socket::Accept()
{
	SOCKET l_acceptSock;
	int l_addressLength = sizeof(SOCKADDR_IN);
	l_acceptSock = accept(m_socket, reinterpret_cast<SOCKADDR*> (&m_address), &l_addressLength);
	return l_acceptSock;
}

bool Socket::ListenStart(unsigned long _address, unsigned short _serverPort)
{
	int l_value;
	m_socket = socket(AF_INET, SOCK_STREAM, 0);
	if (m_socket == INVALID_SOCKET)
	{
		LogManager::GetInstance()->LogWrite(9000);
		return false;
	}
	m_address.sin_family = AF_INET;
	m_address.sin_addr.s_addr = htonl(_address);
	m_address.sin_port = htons(_serverPort);
	l_value = bind(m_socket, reinterpret_cast<SOCKADDR*> (&m_address), sizeof(m_address));
	if (l_value == SOCKET_ERROR)
	{
		LogManager::GetInstance()->LogWrite(9001);
		return false;
	}

	l_value = listen(m_socket, SOMAXCONN);

	if (l_value == SOCKET_ERROR)
	{
		LogManager::GetInstance()->LogWrite(9002);
		return false;
	}

	return true;
}


bool Socket::Send(WSABUF _wsaBuf)
{
	ZeroMemory(&m_sendOverlap.overlapped, sizeof(OVERLAPPED));
	DWORD l_transferred;
	DWORD l_flag = 0;

	if (WSASend(m_socket, &_wsaBuf, 1, &l_transferred, l_flag, &m_sendOverlap.overlapped, NULL) == SOCKET_ERROR)
	{
		if (WSAGetLastError() != WSA_IO_PENDING)
		{
			LogManager::GetInstance()->LogWrite(1000);
			return false;
		}
	}
	return true;
}


bool Socket::Recv(WSABUF _wsaBuf)
{
	ZeroMemory(&m_recvOverlap.overlapped, sizeof(OVERLAPPED));
	DWORD l_transferred;
	DWORD l_flag = 0;

	if (WSARecv(m_socket, &_wsaBuf, 1, &l_transferred, &l_flag, &m_recvOverlap.overlapped, NULL) == SOCKET_ERROR)
	{
		if (WSAGetLastError() != WSA_IO_PENDING)
		{
			LogManager::GetInstance()->LogWrite(2000);
			return false;
		}
	}
	return true;
}
#pragma endregion