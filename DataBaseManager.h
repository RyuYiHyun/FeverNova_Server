//#pragma once
//#include "Global.h"
//#include <mysql.h>
//class DataBaseManager
//{
//#pragma region Singleton
//public:
//	static bool		CreateInstance();		// 생성
//	static void		DestroyInstance();		// 삭제
//	bool			Initialize();			// 초기화
//	void			Release();				// 후처리
//	static DataBaseManager* GetInstance();
//private:
//	static DataBaseManager* m_instance;		// 인스턴스 변수
//
//	DataBaseManager();
//	virtual ~DataBaseManager();
//#pragma endregion
//
//public:
//	bool DataBaseConnect(const TCHAR* _id, const TCHAR* _password, const TCHAR* _dataBaseName, const unsigned int _port);
//
//	void LoginInfoLoad();
//	void LoginInfoInsert(const TCHAR* _id, const TCHAR* _password); // 가입 DB에 저장
//	void LoginInfoDelete(const TCHAR* _id);							// 탈퇴 DB에 삭제
//
//private:
//	const TCHAR* HOSTIP = L"127.0.0.1";	// 루프백 주소
//	TCHAR* m_id;
//	const TCHAR* m_password;
//	TCHAR* m_databaseName;
//
//	MYSQL m_mysql;						// 연결 섹션 객체
//};