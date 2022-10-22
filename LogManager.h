#pragma once
#include "Global.h"
class LogManager
{
#pragma region Singleton
public:
	static bool CreateInstance();
	static void DestroyInstance();
	bool Initialize();
	void Release();
	static LogManager* GetInstance();
private:
	static LogManager* m_instance;

	LogManager();
	virtual ~LogManager();
#pragma endregion
public:
	void LogWrite(int _errorCode);
	//LogManager::GetInstance()->LogWrite();

	TCHAR* GetFilename();
	void SetFilename(const TCHAR* _filename);
private:
	TCHAR m_fileName[BUFSIZE];
	bool SearchFile(const TCHAR* _filename);
};