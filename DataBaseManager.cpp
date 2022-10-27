//#include "DataBaseManager.h"
//#include "LoginManager.h"
//#include "LogManager.h"
//
//#pragma region Singleton
//bool DataBaseManager::CreateInstance()
//{
//	if (m_instance == nullptr)
//	{
//		m_instance = new DataBaseManager();
//	}
//	else
//	{
//		return false;
//	}
//	return true; // 성공적 생성
//}
////
//void DataBaseManager::DestroyInstance()
//{
//	// 인스턴스가 null이 아닐떄
//	if (m_instance != nullptr)
//	{
//		delete m_instance;
//	}
//}
////
//bool DataBaseManager::Initialize()
//{
//	mysql_init(&m_mysql);
//	DataBaseConnect(L"root", L"1234", L"logindb", 3306);
//	return true;
//}
////
//void DataBaseManager::Release()
//{
//	mysql_close(&m_mysql);
//}
////
//DataBaseManager* DataBaseManager::GetInstance()
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
////
//DataBaseManager::DataBaseManager()
//{
//}
////
//DataBaseManager::~DataBaseManager()
//{
//}
////
//DataBaseManager* DataBaseManager::m_instance = nullptr; // Singleton 객체
//#pragma endregion
//
//bool DataBaseManager::DataBaseConnect(const TCHAR* _id, const TCHAR* _password, const TCHAR* _dataBaseName, const unsigned int _port)
//{
//	char l_hostIP[256];
//	char l_id[256];
//	char l_password[256];
//	char l_dataBaseName[256];
//
//	ZeroMemory(l_hostIP, 256);
//	ZeroMemory(l_id, 256);
//	ZeroMemory(l_password, 256);
//	ZeroMemory(l_dataBaseName, 256);
//
//	WideCharToMultiByte(CP_ACP, 0, HOSTIP, 2*_tcslen(HOSTIP), l_hostIP, 256, NULL, NULL);
//	WideCharToMultiByte(CP_ACP, 0, _id, 2*_tcslen(_id), l_id, 256, NULL, NULL);
//	WideCharToMultiByte(CP_ACP, 0, _password, 2*_tcslen(_password), l_password, 256, NULL, NULL);
//	WideCharToMultiByte(CP_ACP, 0, _dataBaseName, 2*_tcslen(_dataBaseName), l_dataBaseName, 256, NULL, NULL);
//
//	if (!mysql_real_connect(&m_mysql, l_hostIP, l_id, l_password, l_dataBaseName, _port, NULL, 0))
//	{
//		std::cout << "mysql connect error" << mysql_error(&m_mysql) << std::endl;
//		exit(-1);
//	}
//	// mysql 기본 UTF-8 (한글자에 4byte) , c언어는 2byte
//	mysql_query(&m_mysql, "set names 'euckr';");//euckr 2byte 로 결정
//	std::cout << "mysql connect ok" << std::endl;
//	return true;
//}
//
//void DataBaseManager::LoginInfoLoad()
//{
//	char l_query[BUFSIZE];
//	MYSQL_RES* l_result;
//	MYSQL_ROW l_row;
//	sprintf(l_query, "select * from jointable;");
//
//	if (mysql_query(&m_mysql, l_query))
//	{
//		std::cout << "mysql query error" << mysql_error(&m_mysql) << std::endl;
//	}
//	else
//	{
//		int i = 0;
//		l_result = mysql_store_result(&m_mysql);
//		while ((l_row = mysql_fetch_row(l_result)) != NULL)
//		{
//			TCHAR l_id[256];
//			TCHAR l_password[256];
//
//			ZeroMemory(l_id, 256);
//			ZeroMemory(l_password, 256);
//
//			MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, l_row[0], strlen(l_row[0]), l_id, 256);
//			MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, l_row[1], strlen(l_row[1]), l_password, 256);
//
//			// 결과를 ROW 단위로 받아오기
//			 
//			//LoginManager::GetInstance()->LoginInfoPush(l_id, l_password);
//		}
//		mysql_free_result(l_result);
//	}
//}
//
//void DataBaseManager::LoginInfoInsert(const TCHAR* _id, const TCHAR* _password)
//{
//	char l_query[BUFSIZE];
//	sprintf(l_query, "insert into jointable values('%s', '%s');", _id, _password);
//	// 쿼리문 실행
//	if (mysql_query(&m_mysql, l_query))
//	{
//		cout << "mysql query error" << mysql_error(&m_mysql) << endl;
//	}
//	else
//	{
//		cout << " 정상적으로 입력되었습니다." << endl;
//	}
//}
//
//void DataBaseManager::LoginInfoDelete(const TCHAR* _id)
//{
//	char l_query[BUFSIZE];
//	sprintf(l_query, "delete from jointable where userid = '%s';", _id);
//	// 쿼리문 실행
//	if (mysql_query(&m_mysql, l_query))
//	{
//		cout << "mysql query error" << mysql_error(&m_mysql) << endl;
//	}
//	else
//	{
//		cout << "정상적으로 삭제되었습니다." << endl;
//	}
//}