#pragma once
#include "Socket.h"
#include <queue>
class Packet :public Socket
{
public:
	enum class E_RESULT
	{
		PACKETNUM_MISMATCH = -2,
		DISCONNECTED = -1,
		FAIL = 0,
		SUCCESS = 1,
	};
	struct PacketData
	{
		BYTE* m_data;			// 패킷 데이터의 시작 주소 or 패킷 데이터
		int m_completeSize;		// 송수신 완료 용량
		int m_dataSize;			// 목표 용량 or 패킷 용량
		PacketData();
		PacketData(const BYTE* _data, int _dataSize);
		~PacketData();
	};

	//
	Packet();
	Packet(const SOCKET& _sock);
	virtual ~Packet();

	// Send Funcs
	bool Send(int _protocol, int _size, BYTE* _data, bool _flag = false);
	E_RESULT SendCompleteCheck(int _cbTrans);

	// Recv Func
	bool Recv();
	E_RESULT RecvCompleteCheck(int _cbTrans);

	// 암호화
	void Encryption(int _dataSize, BYTE* Data);
	void Decryption(int _dataSize, BYTE* Data);

	void InitCryptoKeySend(int _sendProtocol);

	BYTE* GetDataField();
	int GetProtocol();
private:
	PacketData m_recvBuf;
	queue<PacketData*> m_sendBuf;
	int m_sendCount;
	int m_recvCount;
	bool m_sizeFlag;

	unsigned int m_cryptionMainKey;
	unsigned int m_cryptionSubKey1;
	unsigned int m_cryptionSubKey2;

	CriticalKey m_criticalKey;

	void PacketDataAdd(int _protocol, int _dataSize, BYTE* _data, bool _flag);
};