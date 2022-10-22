#include "Packet.h"
#include "LogManager.h"

#pragma region PacketData
Packet::PacketData::PacketData()
{
	m_data = new BYTE[BUFSIZE];
	ZeroMemory(m_data, BUFSIZE);
	m_completeSize = 0;
	m_dataSize = sizeof(int);
}
Packet::PacketData::PacketData(const BYTE* _data, int _dataSize)
{
	m_data = new BYTE[_dataSize];
	memcpy(m_data, _data, _dataSize);
	m_completeSize = 0;
	m_dataSize = _dataSize;
}
Packet::PacketData::~PacketData()
{
	delete[] m_data;
}
#pragma endregion


#pragma region ������&�Ҹ���
Packet::Packet() : Socket()
{
	m_sizeFlag = false;
	m_sendCount = 0;
	m_recvCount = 0;
	m_cryptionMainKey = rand();
	m_cryptionSubKey1 = rand();
	m_cryptionSubKey2 = rand();
}
Packet::Packet(const SOCKET& _sock) : Socket(_sock)
{
	m_sizeFlag = false;
	m_sendCount = 0;
	m_recvCount = 0;
	m_cryptionMainKey = rand();
	m_cryptionSubKey1 = rand();
	m_cryptionSubKey2 = rand();
}
//
Packet::~Packet()
{
	while (m_sendBuf.size() > 1)
	{	// �ܿ� sendť ������ ����
		PacketData* l_deletePacket = m_sendBuf.front();
		m_sendBuf.pop();
		delete l_deletePacket;
	}
}
#pragma endregion


#pragma region Getter&Setter
BYTE* Packet::GetDataField()
{
	return m_recvBuf.m_data + sizeof(int) + sizeof(int);
}
int Packet::GetProtocol()
{
	int l_protocol;
	memcpy(&l_protocol, m_recvBuf.m_data, sizeof(int));
	return l_protocol;
}
#pragma endregion


#pragma region Send����
bool Packet::Send(unsigned int _protocol, int _size, BYTE* _data, bool _flag)
{
	LockGuard l_lockGuard(&m_criticalKey);

	PacketData* l_pakcetData;
	WSABUF l_wsaBuf;

	PacketDataAdd(_protocol, _size, _data, _flag);

	if (m_sendBuf.size() == 1)
	{	// Sendť ũ�Ⱑ 1�̸�, Send ����
		l_pakcetData = m_sendBuf.front();
		l_wsaBuf.buf = reinterpret_cast<CHAR*>(l_pakcetData->m_data + l_pakcetData->m_completeSize);
		l_wsaBuf.len = l_pakcetData->m_dataSize - l_pakcetData->m_completeSize;
		if (!Socket::Send(l_wsaBuf))
		{
			return false;
		}
	}
	// Sendť ũ�Ⱑ 2�̻��̸�, �ٸ� Send �����ϰ� ����. Send�� �����ʴ´�.
	return true;
}

void Packet::Encryption(int _dataSize, BYTE* _data)
{
	unsigned int l_tempKey = m_cryptionMainKey;
	BYTE l_tempData[BUFSIZE];
	ZeroMemory(l_tempData, BUFSIZE);

	for (int i = 0; i < _dataSize; i++)
	{
		l_tempData[i] = _data[i] ^ l_tempKey >> 8;
		l_tempKey = (l_tempData[i] + l_tempKey) * m_cryptionSubKey1 + m_cryptionSubKey2;
	}

	ZeroMemory(_data, _dataSize);
	memcpy(_data, l_tempData, _dataSize);
}

void Packet::Decryption(int _dataSize, BYTE* _data)
{
	unsigned int l_tempKey = m_cryptionMainKey;
	BYTE l_previousBlock;
	BYTE l_tempData[BUFSIZE];
	ZeroMemory(l_tempData, BUFSIZE);

	for (int i = 0; i < _dataSize; i++)
	{
		l_previousBlock = _data[i];
		l_tempData[i] = _data[i] ^ l_tempKey >> 8;
		l_tempKey = (l_previousBlock + l_tempKey) * m_cryptionSubKey1 + m_cryptionSubKey2;
	}

	ZeroMemory(_data, _dataSize);
	memcpy(_data, l_tempData, _dataSize);
}

void Packet::InitCryptoKeySend(int _sendProtocol)
{
	BYTE l_data[BUFSIZE];
	ZeroMemory(l_data, BUFSIZE);

	MyStream l_stream;
	l_stream->SetStream(l_data);
	l_stream->WriteUnsignedInt(m_cryptionMainKey);
	l_stream->WriteUnsignedInt(m_cryptionSubKey1);
	l_stream->WriteUnsignedInt(m_cryptionSubKey2);

	Send(_sendProtocol, l_stream->GetLength(), l_data, true);


	/*int l_dataSize = 0;
	BYTE* l_focusPointer = l_data;
	l_focusPointer = net::util::WriteToByteStream(l_focusPointer, l_dataSize, m_cryptionMainKey);
	l_focusPointer = net::util::WriteToByteStream(l_focusPointer, l_dataSize, m_cryptionSubKey1);
	l_focusPointer = net::util::WriteToByteStream(l_focusPointer, l_dataSize, m_cryptionSubKey2);*/

	//Send(_sendProtocol, l_dataSize, l_data, true);

}

Packet::E_RESULT Packet::SendCompleteCheck(int _transferred)
{
	LockGuard l_lockGuard(&m_criticalKey);

	if (m_sendBuf.size() <= 0)
	{
		return E_RESULT::SUCCESS;
	}
	PacketData* l_packetData = m_sendBuf.front();

	WSABUF l_wsaBuf;

	// �Ϸ� �뷮 ���
	l_packetData->m_completeSize += _transferred;


	if (l_packetData->m_dataSize == l_packetData->m_completeSize)
	{	// ��Ŷ�� ������ �Ϸ��Ѱ��
		// ť�� �Ǿ��� �����͸� pop�ϰ� delete���ش�.
		// ť�� pop�� ���شٰ� �ؼ� �����Ҵ�� �����Ͱ� delete�� �Ǵ°��� �ƴϱ⋚���� delete data�� �Ʒ�ó�� ���� ���־����.
		m_sendBuf.pop();
		delete l_packetData;

		if (m_sendBuf.empty())
		{
			return E_RESULT::SUCCESS;
		}
		else
		{
			l_packetData = m_sendBuf.front();
		}
	}
	l_wsaBuf.buf = reinterpret_cast<CHAR*>(l_packetData->m_data + l_packetData->m_completeSize);
	l_wsaBuf.len = l_packetData->m_dataSize - l_packetData->m_completeSize;
	if (!Socket::Send(l_wsaBuf))
	{
		return E_RESULT::DISCONNECTED;
	}
	return E_RESULT::FAIL;
}

void Packet::PacketDataAdd(unsigned int _protocol, int _dataSize, BYTE* _data, bool _flag)
{
	BYTE l_buf[BUFSIZE];
	ZeroMemory(l_buf, BUFSIZE);

	int l_dataSize = 0;
	BYTE* l_focusPointer = l_buf + sizeof(int);

	l_focusPointer = net::util::WriteToByteStream(l_focusPointer, l_dataSize, _protocol);
	l_focusPointer = net::util::WriteToByteStream(l_focusPointer, l_dataSize, m_sendCount);
	m_sendCount++;
	if (m_sendCount > MAXPACKETNUM) // ��Ŷ �ѹ� ������
	{
		m_sendCount = 0;
	}

	memcpy(l_focusPointer, _data, _dataSize);
	l_dataSize += _dataSize;

	if (!_flag)
	{
		Encryption(l_dataSize, reinterpret_cast<BYTE*>(l_focusPointer) - sizeof(unsigned int) - sizeof(int));
	}

	l_focusPointer = l_buf;

	l_focusPointer = net::util::WriteToByteStream(l_focusPointer, l_dataSize, l_dataSize);

	m_sendBuf.push(new PacketData(l_buf, l_dataSize));
}
#pragma endregion


#pragma region Recv����
bool Packet::Recv()
{
	LockGuard l_lockGuard(&m_criticalKey);
	WSABUF m_wsaBuf;
	m_wsaBuf.buf = reinterpret_cast<CHAR*>(m_recvBuf.m_data);
	m_wsaBuf.len = m_recvBuf.m_dataSize - m_recvBuf.m_completeSize;
	return Socket::Recv(m_wsaBuf);
}
Packet::E_RESULT Packet::RecvCompleteCheck(int _transferred)
{
	LockGuard l_lockGuard(&m_criticalKey);

	m_recvBuf.m_completeSize += _transferred;
	if (m_recvBuf.m_dataSize == m_recvBuf.m_completeSize)
	{
		if (m_sizeFlag)
		{
			Decryption(m_recvBuf.m_dataSize, reinterpret_cast<BYTE*>(m_recvBuf.m_data));

			int l_packetNumber = 0;
			BYTE* l_focusPointer = nullptr;

			m_sizeFlag = false;
			m_recvBuf.m_dataSize = sizeof(int); // ������ �ޱ�
			m_recvBuf.m_completeSize = 0;

			l_focusPointer = m_recvBuf.m_data + sizeof(unsigned int);
			memcpy(&l_packetNumber, l_focusPointer, sizeof(int));
			if (l_packetNumber != m_recvCount)
			{
				LogManager::GetInstance()->LogWrite(5001);
				return E_RESULT::PACKETNUM_MISMATCH;
			}
			m_recvCount++;
			if (m_recvCount > MAXPACKETNUM)
			{
				m_recvCount = 0;
			}

			return E_RESULT::SUCCESS;
		}
		else
		{
			m_sizeFlag = true;
			m_recvBuf.m_completeSize = 0;

			memcpy(&m_recvBuf.m_dataSize, m_recvBuf.m_data, sizeof(int));
		}
	}
	if (!Recv())
	{
		return E_RESULT::DISCONNECTED;
	}
	return E_RESULT::FAIL;
}
#pragma endregion