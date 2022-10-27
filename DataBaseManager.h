//#pragma once
//#include "Global.h"
//#include <mysql.h>
//class DataBaseManager
//{
//#pragma region Singleton
//public:
//	static bool		CreateInstance();		// ����
//	static void		DestroyInstance();		// ����
//	bool			Initialize();			// �ʱ�ȭ
//	void			Release();				// ��ó��
//	static DataBaseManager* GetInstance();
//private:
//	static DataBaseManager* m_instance;		// �ν��Ͻ� ����
//
//	DataBaseManager();
//	virtual ~DataBaseManager();
//#pragma endregion
//
//public:
//	bool DataBaseConnect(const TCHAR* _id, const TCHAR* _password, const TCHAR* _dataBaseName, const unsigned int _port);
//
//	void LoginInfoLoad();
//	void LoginInfoInsert(const TCHAR* _id, const TCHAR* _password); // ���� DB�� ����
//	void LoginInfoDelete(const TCHAR* _id);							// Ż�� DB�� ����
//
//private:
//	const TCHAR* HOSTIP = L"127.0.0.1";	// ������ �ּ�
//	TCHAR* m_id;
//	const TCHAR* m_password;
//	TCHAR* m_databaseName;
//
//	MYSQL m_mysql;						// ���� ���� ��ü
//};