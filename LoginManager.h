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
//		MAINMENU,			// ���� -> Ŭ�� : ���θ޴� ���, Ŭ�� -> ���� : ���� �޴� ���ð�
//		LOGINEDMENU,		// ���� -> Ŭ�� : �α��θ޴� ���, Ŭ�� -> ���� : �α��� �޴� ���ð�
//
//		START,					// Ŭ�� -> ���� : �ʱ� �޴� ��� ��û
//		CRYPTOKEY,			// ���� -> Ŭ�� : ��ĪŰ ����
//		JOIN,						// ���� -> Ŭ�� : ȸ������ �޴� ���, Ŭ�� -> ���� : ȸ������ ��û
//		LOGIN,					// ���� -> Ŭ�� : �α��� �޴� ���, Ŭ�� -> ���� : �α��� ��û
//		LOGOUT,				//  ���� -> Ŭ�� : �α׾ƿ� �޴� ���, Ŭ�� -> ���� : �α׾ƿ� ��û
//		DROP,					//  ���� -> Ŭ�� : ȸ��Ż�� �޴� ���, Ŭ�� -> ����  : Ż�� ��û
//		EXIT,						// ���� -> Ŭ�� :  ���� ��ȣ
//
//		RESULT					//���� -> Ŭ��
//	};
//	//���� -> �̱��� , static ���
//
//	enum class E_SUB1_PROTOCOL
//	{
//		//�޴���û
//		//�޴� ����
//		//�Է¿�û
//		//�Է�
//	};
//
//	enum class E_SUB2_PROTOCOL
//	{
//
//	};
//
//	enum class E_DETAIL_PROTOCOL
//	{
//		//�Է°�
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
//		DUPLICATION,	// (�α���) ���̵� �ߺ�, (����) ���̵� �ߺ�
//		IDERROR,			// (�α���) ���̵� Ʋ��
//		PWERROR,			// (�α���) ��� Ʋ�� - ���̵�¾� 
//		SUCCESS			// (�α���) �α��� ����, (����) ���� ����
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
//	list<_LoginInfo*> m_loginList;					// �α����� ������ ���� ����
//	list<_LoginInfo*> m_joinList;						// ���Ե� �������� ���� ����
//
//	/*�α����� ���θ޴�*/const TCHAR* MAIN_MENU_MSG = L"\n<�޴�>\n1.[ȸ������]\n2.[�α���]\n3.[����]";
//	/*ȸ������ �޴�*/const TCHAR* JOIN_MENU_MSG = L"\n<ȸ������>\n ";
//	/*�α��� �޴�*/const TCHAR* LOGIN_MENU_MSG = L"\n<�α���>\n ";
//	/*�α����� �޴�*/const TCHAR* LOGINED_MENU_MSG = L"\n<�޴�>\n1.[�α׾ƿ�]\n2.[ȸ��Ż��]\n3.[����]";
//	/*�α׾ƿ� �޴�*/const TCHAR* LOGOUT_MENU_MSG = L"\n<�α׾ƿ�>\nEnter�� �α׾ƿ�";
//	/*ȸ��Ż�� �޴�*/const TCHAR* DROP_MENU_MSG = L"\n<ȸ��Ż��>\nEnter�� ȸ��Ż��";
//
//	/*�α��� ���� E_RESULT::SUCCESS*/const TCHAR* LOGIN_SUCESS_MSG = L"\n�α��� ����\0";
//	/*�α׾ƿ� ���� E_RESULT::SUCCESS*/const TCHAR* LOGOUT_SUCESS_MSG = L"\n�α׾ƿ� ����\0";
//	/*ȸ������ ���� E_RESULT::SUCCESS*/const TCHAR* JOIN_SUCESS_MSG = L"\n���� ����\0";
//	/*ȸ��Ż�� ���� E_RESULT::SUCCESS*/const TCHAR* DROP_SUCESS_MSG = L"\nŻ�� ����\0";
//	/*���̵� ���� E_RESULT::IDERROR*/const TCHAR* ID_ERROR_MSG = L"\n���� ���̵��Դϴ�\0";
//	/*�н����� ���� E_RESULT::PWERROR*/const TCHAR* PW_ERROR_MSG = L"\n�н����尡 Ʋ�Ƚ��ϴ�\0";
//	/*���̵� �ߺ� E_RESULT::DUPLICATION*/const TCHAR* ALREADY_EXIST_MSG = L"\n�̹� �����ϴ� ���̵��Դϴ�.\0";
//	/*�α��� �ߺ� E_RESULT::DUPLICATION*/const TCHAR* ANOTHER_LOGINED_MSG = L"\n�̹� �ٸ� �̿��ڰ� �α����ϰ� �ֽ��ϴ�.\0";
//	/*���α׷� ����*/const TCHAR* END_MSG = L"\n���α׷��� �����մϴ�.";
//};
//
//
//
