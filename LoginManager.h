//#pragma once
//#include "Socket.h"
//#include "Session.h"
//#include "SessionManager.h"
//
//class LoginManager
//{
//#pragma region Singleton
//public:
//	static bool CreateInstance();
//	static void DestroyInstance();
//	bool Initialize();
//	void Release();
//	static LoginManager* GetInstance();
//private:
//	static LoginManager* m_instance;
//
//	LoginManager();
//	virtual ~LoginManager();
//#pragma endregion
//public:
//	struct _LoginInfo
//	{
//		TCHAR m_id[INFOSIZE];
//		TCHAR m_password[INFOSIZE];
//		_LoginInfo()
//		{
//			ZeroMemory(m_id, sizeof(m_id));
//			ZeroMemory(m_password, sizeof(m_password));
//		}
//		_LoginInfo(const TCHAR* _id, const TCHAR* _pw)
//		{
//			_tcscpy(m_id, _id);
//			_tcscpy(m_password, _pw);
//		}
//	};
//
//#pragma region Enum
//	enum class E_PROTOCOL
//	{
//		MAINMENU,			// 서버 -> 클라 : 메인메뉴 출력, 클라 -> 서버 : 메인 메뉴 선택값
//		LOGINEDMENU,		// 서버 -> 클라 : 로그인메뉴 출력, 클라 -> 서버 : 로그인 메뉴 선택값
//
//		START,					// 클라 -> 서버 : 초기 메뉴 출력 요청
//		CRYPTOKEY,			// 서버 -> 클라 : 대칭키 전송
//		JOIN,						// 서버 -> 클라 : 회원가입 메뉴 출력, 클라 -> 서버 : 회원가입 요청
//		LOGIN,					// 서버 -> 클라 : 로그인 메뉴 출력, 클라 -> 서버 : 로그인 요청
//		LOGOUT,				//  서버 -> 클라 : 로그아웃 메뉴 출력, 클라 -> 서버 : 로그아웃 요청
//		DROP,					//  서버 -> 클라 : 회원탈퇴 메뉴 출력, 클라 -> 서버  : 탈퇴 요청
//		EXIT,						// 서버 -> 클라 :  종료 신호
//
//		RESULT					//서버 -> 클라
//	};
//	//레벨 -> 싱글톤 , static 기능
//
//	enum class E_SUB1_PROTOCOL
//	{
//		//메뉴요청
//		//메뉴 전송
//		//입력요청
//		//입력
//	};
//
//	enum class E_SUB2_PROTOCOL
//	{
//
//	};
//
//	enum class E_DETAIL_PROTOCOL
//	{
//		//입력값
//	};
//	// PR::MAKEMainPROTOCOL(&int, enum || BYTE*);
//	// PR::MAKEsub1PROTOCOL(&int, enum || BYTE*);
//	// PR::MAKEsub2PROTOCOL(&int, enum || BYTE*);
//	// PR::MAKEsub3PROTOCOL(&int, enum || BYTE*);
//	//&int 
//
//
//
//	enum class E_RESULT
//	{
//		NODATA = -1,
//		DUPLICATION,	// (로그인) 아이디 중복, (가입) 아이디 중복
//		IDERROR,			// (로그인) 아이디 틀림
//		PWERROR,			// (로그인) 비번 틀림 - 아이디맞아 
//		SUCCESS			// (로그인) 로그인 성공, (가입) 가입 성공
//	};
//#pragma endregion
//
//public:
//	void Function(Session* _session);
//	void JoinProcess(Session* _session);
//	void LoginProcess(Session* _session);
//	void LogoutProcess(Session* _session);
//	void DropProcess(Session* _session);
//	void ExitProcess(Session* _session);
//
//	void MainMenuProcess(Session* _session);
//	void LoginedMenuProcess(Session* _session);
//
//	void MenuSend(Session* _session);
//
//	void LoginInfoPush(TCHAR* _id, TCHAR* _password);
//
//#pragma region Packing&Unpacking
//	// packing
//	int ResultDataMake(BYTE* _data, E_RESULT _result, const TCHAR* _message);
//	int MsgDataMake(BYTE* _data, const TCHAR* _message);
//
//	// unpacking
//	void LoginInfoDataSplit(BYTE* _data, TCHAR* _id, TCHAR* _password);
//	void SelectInfoDataSplit(BYTE* _data, int* _selectInfo);
//#pragma endregion
//private:
//	CriticalKey m_criticalKey;
//
//	list<_LoginInfo*> m_loginList;					// 로그인한 유저의 계정 정보
//	list<_LoginInfo*> m_joinList;						// 가입된 유저들의 계정 정보
//
//	/*로그인전 메인메뉴*/const TCHAR* MAIN_MENU_MSG = L"\n<메뉴>\n1.[회원가입]\n2.[로그인]\n3.[종료]";
//	/*회원가입 메뉴*/const TCHAR* JOIN_MENU_MSG = L"\n<회원가입>\n ";
//	/*로그인 메뉴*/const TCHAR* LOGIN_MENU_MSG = L"\n<로그인>\n ";
//	/*로그인후 메뉴*/const TCHAR* LOGINED_MENU_MSG = L"\n<메뉴>\n1.[로그아웃]\n2.[회원탈퇴]\n3.[종료]";
//	/*로그아웃 메뉴*/const TCHAR* LOGOUT_MENU_MSG = L"\n<로그아웃>\nEnter로 로그아웃";
//	/*회원탈퇴 메뉴*/const TCHAR* DROP_MENU_MSG = L"\n<회원탈퇴>\nEnter로 회원탈퇴";
//
//	/*로그인 성공 E_RESULT::SUCCESS*/const TCHAR* LOGIN_SUCESS_MSG = L"\n로그인 성공\0";
//	/*로그아웃 성공 E_RESULT::SUCCESS*/const TCHAR* LOGOUT_SUCESS_MSG = L"\n로그아웃 성공\0";
//	/*회원가입 성공 E_RESULT::SUCCESS*/const TCHAR* JOIN_SUCESS_MSG = L"\n가입 성공\0";
//	/*회원탈퇴 성공 E_RESULT::SUCCESS*/const TCHAR* DROP_SUCESS_MSG = L"\n탈퇴 성공\0";
//	/*아이디 오류 E_RESULT::IDERROR*/const TCHAR* ID_ERROR_MSG = L"\n없는 아이디입니다\0";
//	/*패스워드 오류 E_RESULT::PWERROR*/const TCHAR* PW_ERROR_MSG = L"\n패스워드가 틀렸습니다\0";
//	/*아이디 중복 E_RESULT::DUPLICATION*/const TCHAR* ALREADY_EXIST_MSG = L"\n이미 존재하는 아이디입니다.\0";
//	/*로그인 중복 E_RESULT::DUPLICATION*/const TCHAR* ANOTHER_LOGINED_MSG = L"\n이미 다른 이용자가 로그인하고 있습니다.\0";
//	/*프로그램 종료*/const TCHAR* END_MSG = L"\n프로그램을 종료합니다.";
//};
//
//
//
