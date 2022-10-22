//#include "LoginManager.h"
//#include "DataBaseManager.h"
//#include "LogManager.h"
//#include "LoginState.h"
//
//#pragma region Singleton
//bool LoginManager::CreateInstance()
//{
//	if (m_instance == nullptr)
//	{
//		m_instance = new LoginManager();
//		return true;
//	}
//	else
//	{
//		return false;
//	}
//}
//void LoginManager::DestroyInstance()
//{
//	if (m_instance != nullptr)
//	{
//		delete m_instance;
//	}
//}
//bool LoginManager::Initialize() // 초기화
//{
//	//DB에서 JOIN리스트에 데이터 불러오기
//	DataBaseManager::GetInstance()->LoginInfoLoad();
//
//	return true;
//}
//void LoginManager::Release() // 후처리
//{
//}
//LoginManager* LoginManager::GetInstance()
//{
//	if (m_instance != nullptr)
//	{
//		return m_instance;
//	}
//	else
//	{
//		return nullptr;
//	}
//}
//LoginManager::LoginManager()// 생성자
//{
//}
//LoginManager::~LoginManager()// 소멸자
//{
//}
//LoginManager* LoginManager::m_instance = nullptr;	// Singleton 객체
//#pragma endregion
//
//
//void LoginManager::Function(Session* _session)
//{
//	E_PROTOCOL l_protocol = static_cast<E_PROTOCOL>(_session->GetProtocol());
//	switch (l_protocol)
//	{
//	case LoginManager::E_PROTOCOL::START:
//		_session->InitCryptoKeySend();
//		MenuSend(_session);
//		break;
//	case LoginManager::E_PROTOCOL::MAINMENU:
//		MainMenuProcess(_session);
//		break;
//	case LoginManager::E_PROTOCOL::LOGINEDMENU:
//		LoginedMenuProcess(_session);
//		break;
//	case LoginManager::E_PROTOCOL::JOIN:
//		JoinProcess(_session);
//		break;
//	case LoginManager::E_PROTOCOL::LOGIN:
//		LoginProcess(_session);
//		break;
//	case LoginManager::E_PROTOCOL::LOGOUT:
//		LogoutProcess(_session);
//		break;
//	case LoginManager::E_PROTOCOL::DROP:
//		DropProcess(_session);
//		break;
//		//case LoginManager::E_PROTOCOL::EXIT:  --> 서버가 클라에게 EXIT 프로토콜 전송하도록 변경
//		//	ExitProcess(_session); ==> MainMenuProcess함수내로 이동
//		//	break;
//	default:
//		LogManager::GetInstance()->LogWrite(7777);
//		break;
//	}
//}
//
//void LoginManager::JoinProcess(Session* _session)
//{
//	BYTE l_data[BUFSIZE];
//	ZeroMemory(l_data, BUFSIZE);
//	int l_dataSize = 0;
//	_LoginInfo* l_logininfo = new _LoginInfo();
//	LoginInfoDataSplit(_session->GetDataField(), l_logininfo->m_id, l_logininfo->m_password);
//	bool l_flag = true;
//
//	LockGuard l_lockGuard(&m_criticalKey); // 잠금
//
//	for (list<_LoginInfo*>::iterator iter = m_joinList.begin(); iter != m_joinList.end(); iter++)
//	{
//		if (!_tcscmp((*iter)->m_id, l_logininfo->m_id))
//		{
//			l_flag = false;
//			break;
//		}
//	}
//
//	// 가입 절차
//	if (l_flag)
//	{
//		m_joinList.push_back(l_logininfo);
//
//		// DB 메니저로 DB에 가입 정보 추가
//		DataBaseManager::GetInstance()->LoginInfoInsert(l_logininfo->m_id, l_logininfo->m_password);
//
//		// 결과 전송 (가입성공)
//		l_dataSize = ResultDataMake(l_data, E_RESULT::SUCCESS, JOIN_SUCESS_MSG);
//
//		if (!_session->SendPacket(static_cast<int>(E_PROTOCOL::RESULT), l_dataSize, l_data))
//		{
//			LogManager::GetInstance()->LogWrite(1001);
//		}
//
//		MenuSend(_session);
//		return;
//	}
//	// 동적 할당 해제
//	delete l_logininfo;
//
//	// 결과 전송(아이디 중복 가입 실패)
//	l_dataSize = ResultDataMake(l_data, E_RESULT::DUPLICATION, ALREADY_EXIST_MSG);
//
//	if (!_session->SendPacket(static_cast<int>(E_PROTOCOL::RESULT), l_dataSize, l_data))
//	{
//		LogManager::GetInstance()->LogWrite(1002);
//	}
//
//	MenuSend(_session);
//}
//
//void LoginManager::LoginProcess(Session* _session)
//{
//	BYTE l_data[BUFSIZE];
//	ZeroMemory(l_data, BUFSIZE);
//	int l_dataSize = 0;
//	_LoginInfo* l_logininfo = new _LoginInfo();
//	LoginInfoDataSplit(_session->GetDataField(), l_logininfo->m_id, l_logininfo->m_password);
//	E_RESULT l_Result = E_RESULT::IDERROR;
//
//	LockGuard l_lockGuard(&m_criticalKey);//잠금
//	for (list<_LoginInfo*>::iterator iter = m_loginList.begin(); iter != m_loginList.end(); iter++)
//	{
//		if (!_tcscmp((*iter)->m_id, l_logininfo->m_id))
//		{// 아이디는 맞음
//			l_Result = E_RESULT::DUPLICATION;
//			break;
//		}
//	}
//	if (l_Result != E_RESULT::DUPLICATION)
//	{
//		for (list<_LoginInfo*>::iterator iter = m_joinList.begin(); iter != m_joinList.end(); iter++)
//		{
//			if (!_tcscmp((*iter)->m_id, l_logininfo->m_id))
//			{// 아이디는 맞음
//				if (!_tcscmp((*iter)->m_password, l_logininfo->m_password))
//				{// 로그인 성공
//					l_Result = E_RESULT::SUCCESS;
//					break;
//				}
//				else
//				{
//					l_Result = E_RESULT::PWERROR;
//					break;
//				}
//			}
//		}
//	}
//
//
//	switch (l_Result)
//	{
//	case LoginManager::E_RESULT::DUPLICATION:
//		l_dataSize = ResultDataMake(l_data, l_Result, ANOTHER_LOGINED_MSG); // 아이디 중복 (이미 로그인된 계정)
//		break;
//	case LoginManager::E_RESULT::IDERROR:
//		l_dataSize = ResultDataMake(l_data, l_Result, ID_ERROR_MSG); //로그인 실패 결과 전송(아이디 없음)
//		break;
//	case LoginManager::E_RESULT::PWERROR:
//		l_dataSize = ResultDataMake(l_data, l_Result, PW_ERROR_MSG);
//		break;
//	case LoginManager::E_RESULT::SUCCESS:
//		// 로그인 성공 결과 전송
//		_session->SetLoginInfo(l_logininfo);
//		m_loginList.push_back(l_logininfo);
//
//		(dynamic_cast<LoginState*>(_session->GetState()))->SetIsLogin(true);
//		//_session->SetIslogin(true);
//
//		l_dataSize = ResultDataMake(l_data, l_Result, LOGIN_SUCESS_MSG);
//		break;
//	}
//	if (!_session->SendPacket(static_cast<int>(E_PROTOCOL::RESULT), l_dataSize, l_data))
//	{
//		LogManager::GetInstance()->LogWrite(1003);
//	}
//
//	MenuSend(_session);
//}
//
//void LoginManager::LogoutProcess(Session* _session)
//{
//	BYTE l_data[BUFSIZE];
//	ZeroMemory(l_data, BUFSIZE);
//	int l_dataSize = 0;
//
//	// 로그인 정보 삭제
//	LockGuard l_lockGuard(&m_criticalKey);
//	for (list<_LoginInfo*>::iterator iter = m_loginList.begin(); iter != m_loginList.end(); )
//	{
//		if ((*iter) == reinterpret_cast<_LoginInfo*>(_session->GetLoginInfo()))
//		{	// 아이디는 맞음
//			delete (*iter);
//			m_loginList.erase(iter);
//			(dynamic_cast<LoginState*>(_session->GetState()))->SetIsLogin(false);
//			//_session->SetIslogin(false);
//			break;
//		}
//		else
//		{
//			iter++;
//		}
//	}
//
//
//	// 결과전송 ->  응 그래 너 로그아웃해 
//	l_dataSize = ResultDataMake(l_data, E_RESULT::SUCCESS, LOGOUT_SUCESS_MSG);
//
//	if (!_session->SendPacket(static_cast<int>(E_PROTOCOL::RESULT), l_dataSize, l_data))
//	{
//		LogManager::GetInstance()->LogWrite(1004);
//	}
//
//	MenuSend(_session);
//}
//
//void LoginManager::DropProcess(Session* _session)
//{
//	//if (_session->GetIslogin())
//	if (dynamic_cast<LoginState*>(_session->GetState())->GetIsLogin())
//	{
//		BYTE l_data[BUFSIZE];
//		ZeroMemory(l_data, BUFSIZE);
//		int l_dataSize = 0;
//		TCHAR l_id[INFOSIZE];
//		ZeroMemory(l_id, INFOSIZE);
//
//		_LoginInfo* l_logininfo = reinterpret_cast<_LoginInfo*>(_session->GetLoginInfo());
//
//		memcpy(l_id, l_logininfo->m_id, 2*_tcslen(l_logininfo->m_id));
//		// 가입정보 삭제
//		LockGuard l_lockGuard(&m_criticalKey);
//		for (list<_LoginInfo*>::iterator iter = m_loginList.begin(); iter != m_loginList.end(); )
//		{
//			if ((*iter) == l_logininfo)
//			{	// 아이디는 맞음
//				delete (*iter);
//				m_loginList.erase(iter);
//				(dynamic_cast<LoginState*>(_session->GetState()))->SetIsLogin(false);
//				//_session->SetIslogin(false);
//				break;
//			}
//			else
//			{
//				iter++;
//			}
//		}
//		for (list<_LoginInfo*>::iterator iter = m_joinList.begin(); iter != m_joinList.end(); )
//		{
//			if (!_tcscmp((*iter)->m_id, l_id))
//			{
//				delete (*iter);
//				m_joinList.erase(iter);
//
//				//DB에서 정보 삭제
//				DataBaseManager::GetInstance()->LoginInfoDelete(l_id);
//				break;
//			}
//			else
//			{
//				iter++;
//			}
//		}
//
//		l_dataSize = ResultDataMake(l_data, E_RESULT::SUCCESS, DROP_SUCESS_MSG);
//		if (!_session->SendPacket(static_cast<int>(E_PROTOCOL::RESULT), l_dataSize, l_data))
//		{
//			LogManager::GetInstance()->LogWrite(1005);
//		}
//	}
//	else
//	{
//		// 로그인 상태가 아닙니다.
//	}
//
//	MenuSend(_session);
//	return;
//}
//
//void LoginManager::ExitProcess(Session* _session)
//{
//	BYTE l_data[BUFSIZE];
//	ZeroMemory(l_data, BUFSIZE);
//	int l_dataSize = 0;
//	//if (_session->GetIslogin())
//	if (dynamic_cast<LoginState*>(_session->GetState())->GetIsLogin())
//	{
//		LockGuard l_lockGuard(&m_criticalKey);
//		for (list<_LoginInfo*>::iterator iter = m_loginList.begin(); iter != m_loginList.end(); )
//		{
//			if ((*iter) == reinterpret_cast<_LoginInfo*>(_session->GetLoginInfo()))
//			{	// 아이디는 맞음
//				delete (*iter);
//				m_loginList.erase(iter);
//				(dynamic_cast<LoginState*>(_session->GetState()))->SetIsLogin(false);
//				//_session->SetIslogin(false);
//				break;
//			}
//			else
//			{
//				iter++;
//			}
//		}
//	}
//	l_dataSize = ResultDataMake(l_data, E_RESULT::SUCCESS, END_MSG);
//	if (!_session->SendPacket(static_cast<int>(E_PROTOCOL::EXIT), l_dataSize, l_data))
//	{
//		LogManager::GetInstance()->LogWrite(1006);
//	}
//}
//
//void LoginManager::MainMenuProcess(Session* _session)
//{
//	BYTE l_data[BUFSIZE];
//	ZeroMemory(l_data, BUFSIZE);
//	int l_dataSize = 0;
//	int l_menuSelect = 0;
//	bool l_sendFlag = false;
//	E_PROTOCOL l_sendProtocol;
//
//	SelectInfoDataSplit(_session->GetDataField(), &l_menuSelect);
//
//	switch (l_menuSelect)
//	{
//		//회원가입
//	case 1:
//		l_dataSize = MsgDataMake(l_data, JOIN_MENU_MSG);
//		l_sendProtocol = E_PROTOCOL::JOIN;
//		l_sendFlag = true;
//		break;
//		//로그인
//	case 2:
//		l_dataSize = MsgDataMake(l_data, LOGIN_MENU_MSG);
//		l_sendProtocol = E_PROTOCOL::LOGIN;
//		l_sendFlag = true;
//		break;
//		//종료
//	case 3:
//		ExitProcess(_session);
//		break;
//		//잘못된값 입력시 다시 메뉴로
//	default:
//		MenuSend(_session);
//		return;
//	}
//
//	if (l_sendFlag)
//	{
//		if (!_session->SendPacket(static_cast<int>(l_sendProtocol), l_dataSize, l_data))
//		{
//			LogManager::GetInstance()->LogWrite(1007);
//		}
//	}
//}
//
//void LoginManager::LoginedMenuProcess(Session* _session)
//{
//	BYTE l_data[BUFSIZE];
//	ZeroMemory(l_data, BUFSIZE);
//	int l_dataSize = 0;
//	int l_menuSelect = 0;
//	bool l_sendFlag = false;
//	E_PROTOCOL l_sendProtocol;
//
//	SelectInfoDataSplit(_session->GetDataField(), &l_menuSelect);
//
//	switch (l_menuSelect)
//	{
//		//로그아웃
//	case 1:
//		l_dataSize = MsgDataMake(l_data, LOGOUT_MENU_MSG);
//		l_sendProtocol = E_PROTOCOL::LOGOUT;
//		l_sendFlag = true;
//		break;
//		//회원탈퇴
//	case 2:
//		l_dataSize = MsgDataMake(l_data, DROP_MENU_MSG);
//		l_sendProtocol = E_PROTOCOL::DROP;
//		l_sendFlag = true;
//		break;
//		//종료
//	case 3:
//		ExitProcess(_session);
//		break;
//		//잘못된값 입력시 다시 메뉴로
//	default:
//		MenuSend(_session);
//		return;
//	}
//
//	if (l_sendFlag)
//	{
//		if (!_session->SendPacket(static_cast<int>(l_sendProtocol), l_dataSize, l_data))
//		{
//			LogManager::GetInstance()->LogWrite(1008);
//		}
//	}
//}
//
//void LoginManager::MenuSend(Session* _session)
//{
//	BYTE l_data[BUFSIZE];
//	ZeroMemory(l_data, BUFSIZE);
//	int l_dataSize = 0;
//	E_PROTOCOL l_sendProtocol;
//
//	if (dynamic_cast<LoginState*>(_session->GetState())->GetIsLogin())
//	{
//		l_dataSize = MsgDataMake(l_data, LOGINED_MENU_MSG);
//		l_sendProtocol = E_PROTOCOL::LOGINEDMENU;
//	}
//	else
//	{
//		l_dataSize = MsgDataMake(l_data, MAIN_MENU_MSG);
//		l_sendProtocol = E_PROTOCOL::MAINMENU;
//	}
//
//	if (!_session->SendPacket(static_cast<int>(l_sendProtocol), l_dataSize, l_data))
//	{
//		LogManager::GetInstance()->LogWrite(1008);
//	}
//}
//
//void LoginManager::LoginInfoPush(TCHAR* _id, TCHAR* _password)
//{
//	_LoginInfo* l_loginInfo = new _LoginInfo(_id, _password);
//	m_joinList.push_back(l_loginInfo);
//}
//
//#pragma region Packing&Unpacking
//int LoginManager::ResultDataMake(BYTE* _data, E_RESULT _result, const TCHAR* _message)
//{
//	int l_packedSize = 0;
//	BYTE* l_focusPointer = _data;
//
//	l_focusPointer = MemoryCopy(l_focusPointer, l_packedSize, static_cast<int>(_result));
//	l_focusPointer = MemoryCopy(l_focusPointer, l_packedSize, _message);
//
//	return l_packedSize;
//}
//
//int LoginManager::MsgDataMake(BYTE* _data, const TCHAR* _message)
//{
//	int l_packedSize = 0;
//	BYTE* l_focusPointer = _data;
//
//	l_focusPointer = MemoryCopy(l_focusPointer, l_packedSize, _message);
//
//	return l_packedSize;
//}
//
//// 문자열 2개 언팩
//void LoginManager::LoginInfoDataSplit(BYTE* _data, TCHAR* _id, TCHAR* _password)
//{
//	BYTE* l_focusPointer = _data;
//	int l_idSize, l_passwordSize;
//
//	memcpy(&l_idSize, l_focusPointer, sizeof(int));
//	l_focusPointer = l_focusPointer + sizeof(int);
//
//	memcpy(_id, l_focusPointer, l_idSize);
//	l_focusPointer = l_focusPointer + l_idSize;
//
//	memcpy(&l_passwordSize, l_focusPointer, sizeof(int));
//	l_focusPointer = l_focusPointer + sizeof(int);
//
//	memcpy(_password, l_focusPointer, l_passwordSize);
//	l_focusPointer = l_focusPointer + l_passwordSize;
//}
//void LoginManager::SelectInfoDataSplit(BYTE* _data, int* _selectInfo)
//{
//	BYTE* l_focusPointer = _data;
//
//	memcpy(_selectInfo, l_focusPointer, sizeof(int));
//	l_focusPointer = l_focusPointer + sizeof(int);
//}
//#pragma endregion