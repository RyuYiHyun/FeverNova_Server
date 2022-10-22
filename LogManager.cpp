#include "LogManager.h"

#pragma region Singleton
bool LogManager::CreateInstance()
{
	if (m_instance == nullptr)
	{
		m_instance = new LogManager();
	}
	else
	{
		return false;
	}
	return true; // 성공적 생성
}
//
void LogManager::DestroyInstance()
{
	if (m_instance != nullptr)
	{
		delete m_instance;
	}
}
//
bool LogManager::Initialize()
{
	SetFilename(L"LogTest.txt");
	return true;
}
//
void LogManager::Release()
{
}
//
LogManager* LogManager::GetInstance()
{
	if (m_instance != nullptr)
	{
		return m_instance;
	}
	else
	{
		return nullptr;
	}
}
//
LogManager::LogManager()
{
}
//
LogManager::~LogManager()
{
}
//
LogManager* LogManager::m_instance = nullptr; // Singleton 객체
#pragma endregion

void LogManager::LogWrite(int _errorCode)
{
	// 파일을 바이너리 쓰기 모드로 open
	int l_value;
	TCHAR l_logMessage[BUFSIZE];
	ZeroMemory(l_logMessage, BUFSIZE);
	time_t timer;
	tm* t;

	FILE* l_file = _tfopen(m_fileName, L"w+");
	// open 실패시
	if (l_file == NULL)
	{
		cout << "Log File Fail" << endl;
		return;
	}

	timer = time(NULL); // 1970년 1월 1일 0시 0분 0초부터 시작하여 현재까지의 초
	t = localtime(&timer);
	_stprintf(l_logMessage, L"ERRORCODE [%d] : [%d월%d일 - %d:%d:%d]", _errorCode, t->tm_mon + 1, t->tm_mday, t->tm_hour, t->tm_min, t->tm_sec);

	l_value = fwrite(l_logMessage, 1, 2*_tcslen(l_logMessage), l_file);
	fclose(l_file);
	return;
}

TCHAR* LogManager::GetFilename()
{
	return m_fileName;
}

void LogManager::SetFilename(const TCHAR* _filename)
{
	_tcscpy(m_fileName, _filename);
}

bool LogManager::SearchFile(const TCHAR* _filename)
{
	WIN32_FIND_DATAW FindFileData;// 찾은 파일의 정보

	// _filename의 파일이 있는지 검색
	HANDLE hFindFile = FindFirstFile(_filename, &FindFileData);
	if (hFindFile == INVALID_HANDLE_VALUE)
	{
		// 파일이 존재하지 않는경우
		return false;
	}
	else
	{
		// 파일이 존재하는 경우
		FindClose(hFindFile);
		return true;
	}
}