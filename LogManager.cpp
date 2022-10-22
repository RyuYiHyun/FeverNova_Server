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
	return true; // ������ ����
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
LogManager* LogManager::m_instance = nullptr; // Singleton ��ü
#pragma endregion

void LogManager::LogWrite(int _errorCode)
{
	// ������ ���̳ʸ� ���� ���� open
	int l_value;
	TCHAR l_logMessage[BUFSIZE];
	ZeroMemory(l_logMessage, BUFSIZE);
	time_t timer;
	tm* t;

	FILE* l_file = _tfopen(m_fileName, L"w+");
	// open ���н�
	if (l_file == NULL)
	{
		cout << "Log File Fail" << endl;
		return;
	}

	timer = time(NULL); // 1970�� 1�� 1�� 0�� 0�� 0�ʺ��� �����Ͽ� ��������� ��
	t = localtime(&timer);
	_stprintf(l_logMessage, L"ERRORCODE [%d] : [%d��%d�� - %d:%d:%d]", _errorCode, t->tm_mon + 1, t->tm_mday, t->tm_hour, t->tm_min, t->tm_sec);

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
	WIN32_FIND_DATAW FindFileData;// ã�� ������ ����

	// _filename�� ������ �ִ��� �˻�
	HANDLE hFindFile = FindFirstFile(_filename, &FindFileData);
	if (hFindFile == INVALID_HANDLE_VALUE)
	{
		// ������ �������� �ʴ°��
		return false;
	}
	else
	{
		// ������ �����ϴ� ���
		FindClose(hFindFile);
		return true;
	}
}