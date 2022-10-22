#include "Util.h"

//BYTE* net::util::WriteToByteStream(BYTE* _buffer, int& _size, const TCHAR* _source)
//{
//	BYTE* l_focusPointer = _buffer;
//	int l_strLength = 2 * _tcslen(_source);
//
//	memcpy(l_focusPointer, &l_strLength, sizeof(int));
//	_size += sizeof(int);
//	l_focusPointer += sizeof(int);
//
//	memcpy(l_focusPointer, _source, l_strLength);
//	_size += l_strLength;
//	l_focusPointer += l_strLength;
//
//	return l_focusPointer;
//}