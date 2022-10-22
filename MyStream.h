#pragma once
#include "MemoryPool.h"

class BaseStream : public MemoryPool<BaseStream>
{
public:
	BaseStream();
	virtual ~BaseStream();
public:
	bool SetStream(BYTE* stream);
	bool ZeroStream();
	int GetLength();

	bool WriteBool(bool);
	bool WriteShort(short);
	bool WriteInt(int);
	bool WriteUnsignedInt(unsigned int);
	bool WriteFloat(float);
	bool WriteByte(BYTE);
	bool WriteBytes(BYTE*, int);
	bool WriteTchar(TCHAR);
	bool WriteTchars(PTCHAR, int);
	bool WriteStruct(void*, int);

	bool ReadBool(bool*);
	bool ReadShort(short*);
	bool ReadInt(int*);
	bool ReadUnsignedInt(unsigned int*);
	bool ReadFloat(float*);
	bool ReadByte(BYTE*);
	bool ReadBytes(BYTE*, int);
	bool ReadTchar(TCHAR*);
	bool ReadTchars(PTCHAR, int);
private:
	BYTE* m_streamPointer;
	int	m_length;
};

class MyStream
{
public:
	MyStream() { Stream = new BaseStream(); }
	~MyStream() { delete Stream; }

	BaseStream* operator->() { return Stream; }
	operator BaseStream* () { return Stream; }

private:
	BaseStream* Stream;
};