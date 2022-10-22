#include "MyStream.h"

#pragma region 생성자&소멸자
BaseStream::BaseStream()
{
	m_streamPointer = nullptr;
	m_length = 0;
}
BaseStream::~BaseStream()
{
}
#pragma endregion

#pragma region Getter&Setter
bool BaseStream::SetStream(BYTE* stream)
{
	if (stream == nullptr)
	{
		return false;
	}
	m_streamPointer = stream;
	m_length = 0;
	return true;
}
bool BaseStream::ZeroStream()
{
	if (m_streamPointer == nullptr)
	{
		return false;
	}
	m_length = 0;
	return true;
}
int BaseStream::GetLength()
{
	return m_length;
}
#pragma endregion


#pragma region 함수
bool BaseStream::WriteBool(bool _data)
{
	memcpy(m_streamPointer + m_length, &_data, sizeof(bool));
	m_length += sizeof(bool);
	return true;
}
bool BaseStream::WriteShort(short _data)
{
	memcpy(m_streamPointer + m_length, &_data, sizeof(short));
	m_length += sizeof(short);
	return true;
}
bool BaseStream::WriteInt(int _data)
{
	memcpy(m_streamPointer + m_length, &_data, sizeof(int));
	m_length += sizeof(int);
	return true;
}
bool BaseStream::WriteUnsignedInt(unsigned int _data)
{
	memcpy(m_streamPointer + m_length, &_data, sizeof(unsigned int));
	m_length += sizeof(unsigned int);
	return true;
}
bool BaseStream::WriteFloat(float _data)
{
	memcpy(m_streamPointer + m_length, &_data, sizeof(float));
	m_length += sizeof(float);
	return true;
}
bool BaseStream::WriteByte(BYTE _data)
{
	memcpy(m_streamPointer + m_length, &_data, sizeof(BYTE));
	m_length += sizeof(BYTE);
	return true;
}
bool BaseStream::WriteBytes(BYTE* _data, int _dataSize)
{
	memcpy(m_streamPointer + m_length, _data, _dataSize * sizeof(BYTE));
	m_length += _dataSize * sizeof(BYTE);
	return true;
}
bool BaseStream::WriteTchar(TCHAR _data)
{
	memcpy(m_streamPointer + m_length, &_data, sizeof(TCHAR));
	m_length += sizeof(TCHAR);
	return true;
}
bool BaseStream::WriteTchars(PTCHAR _data, int _dataSize)
{
	memcpy(m_streamPointer + m_length, _data, _dataSize * sizeof(TCHAR));
	m_length += _dataSize * sizeof(TCHAR);
	return true;
}

bool BaseStream::WriteStruct(void* _data, int _dataSize)
{
	memcpy(m_streamPointer + m_length, _data, _dataSize * sizeof(BYTE));
	m_length += _dataSize * sizeof(BYTE);
	return true;
}


bool BaseStream::ReadBool(bool* _data)
{
	memcpy(_data, m_streamPointer + m_length, sizeof(bool));
	m_length += sizeof(bool);
	return true;
}
bool BaseStream::ReadShort(short* _data)
{
	memcpy(_data, m_streamPointer + m_length, sizeof(short));
	m_length += sizeof(short);
	return true;
}
bool BaseStream::ReadInt(int* _data)
{
	memcpy(_data, m_streamPointer + m_length, sizeof(int));
	m_length += sizeof(int);
	return true;
}
bool BaseStream::ReadUnsignedInt(unsigned int* _data)
{
	memcpy(_data, m_streamPointer + m_length, sizeof(unsigned int));
	m_length += sizeof(unsigned int);
	return true;
}
bool BaseStream::ReadFloat(float* _data)
{
	memcpy(_data, m_streamPointer + m_length, sizeof(float));
	m_length += sizeof(float);
	return true;
}
bool BaseStream::ReadByte(BYTE* _data)
{
	memcpy(_data, m_streamPointer + m_length, sizeof(BYTE));
	m_length += sizeof(BYTE);
	return true;
}
bool BaseStream::ReadBytes(BYTE* _data, int _dataSize)
{
	memcpy(_data, m_streamPointer + m_length, _dataSize * sizeof(BYTE));
	m_length += _dataSize * sizeof(BYTE);
	return true;
}
bool BaseStream::ReadTchar(TCHAR* _data)
{
	memcpy(_data, m_streamPointer + m_length, sizeof(TCHAR));
	m_length += sizeof(TCHAR);
	return true;
}
bool BaseStream::ReadTchars(PTCHAR _data, int _dataSize)
{
	memcpy(_data, m_streamPointer + m_length, _dataSize * sizeof(TCHAR));
	m_length += _dataSize * sizeof(TCHAR);
	return true;
}
#pragma endregion