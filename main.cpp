#define MAIN
#include "Global.h"
#include <time.h>
#include "MainManager.h"

int main(int argc, BYTE* argv[])
{
	srand(time(NULL));
	if (!MainManager::CreateInstance())
	{
		return 0;
	}

	MainManager* manager = MainManager::GetInstance();
	if (manager->Initialize())
	{
		manager->Run();
		manager->Release();
	}
	MainManager::DestroyInstance();
	return 0;
}

// SetConsoleCtrlHandler