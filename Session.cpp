#include "Session.h"
#include "LogManager.h"

#pragma region 생성자&소멸자
Session::Session() :Packet()
{
	m_idNumber = -1;
	//m_loginInfo = nullptr;
}
Session::Session(const SOCKET& _sock) : Packet(_sock)
{
	m_idNumber = -1;
	//m_loginInfo = nullptr;
}
//
Session::~Session()
{
	/*if (m_loginState != nullptr)
	{
		delete m_loginState;
	}*/
	if (m_testState != nullptr)
	{
		delete m_testState;
	}
}
#pragma endregion


#pragma region Getter&Setter
//void* Session::GetLoginInfo()
//{
//	return m_loginInfo;
//}
State* Session::GetState()
{
	return m_state;
}
//
//void Session::SetLoginInfo(void* _loginInfo)
//{
//	m_loginInfo = _loginInfo;
//}
#pragma endregion


#pragma region 함수
bool Session::SendPacket(unsigned int _protocol, unsigned int _datasize, BYTE* _data)
{
	if (!Send(_protocol, _datasize, _data))
	{
		return false;
	}
	return true;
}
//
bool Session::RecvPacket()
{
	if (!Recv())
	{
		return false;
	}
	return true;
}
bool Session::Initialize()
{
	//m_loginState = new LoginState(this);


	m_testState = new TestState(this);
	//m_state = m_loginState;
	m_state = m_testState;
	return true;
}
void Session::Release()
{
	/*if (m_loginState != nullptr)
	{
		delete m_loginState;
	}*/
	if (m_testState != nullptr)
	{
		delete m_testState;
	}
}
#pragma endregion


#pragma region GetSet
void Session::SetIdNumber(int _idNumber)
{
	m_idNumber = _idNumber;
}
int Session::GetIdNumber()
{
	return m_idNumber;
}
#pragma endregion
