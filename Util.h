#pragma once
#pragma comment(lib, "ws2_32")
#include <winsock2.h>

namespace net
{
#pragma region 확장 오버랩 구조체
	enum class E_OPERATION	// OverlappedEX
	{
		ACCEPT,
		SEND,
		RECV,
	};

	struct OVERLAPPEDEX
	{
		OVERLAPPED	overlapped;	// Overlap
		E_OPERATION operation;	// OverlapEx Type
		void* session;			// session pointer
	};
#pragma endregion


	namespace util
	{

		template<typename T>
		BYTE* WriteToByteStream(BYTE* _buffer, int& _size, T _source)
		{
			BYTE* l_focusPointer = _buffer;
			memcpy(l_focusPointer, &_source, sizeof(T));
			_size += sizeof(T);
			l_focusPointer += sizeof(T);

			return l_focusPointer;
		}
	}
}


