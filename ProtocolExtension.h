#pragma once
#include "Global.h"
class ProtocolExtension
{
public:
	enum class E_AREA
	{
		MAIN = 8 * 3,
		SUB1 = 8 * 2,
		SUB2 = 8 * 1,
		SUB3 = 8 * 0
	};
	/// <summary>
	/// Protocol ,  _protocolNumber�� 0 ~ 255������ �� �ֱ�
	/// </summary>
	static void SynthesizeProtocol(unsigned int* _protocol, unsigned int _protocolNumber, E_AREA _area)// Synthesize �ռ�
	{
		_protocolNumber = _protocolNumber << static_cast<unsigned int>(_area);
		*_protocol = *_protocol | _protocolNumber;
	}
	static void ExtractionProtocol(unsigned int _protocol, unsigned int* _protocolNumber, E_AREA _area)// Extraction ����
	{
		unsigned int l_extractor = 255;
		l_extractor = l_extractor << static_cast<unsigned int>(_area);
		l_extractor = _protocol & l_extractor;
		l_extractor = l_extractor >> static_cast<unsigned int>(_area);
		*_protocolNumber = l_extractor;
	}
};

