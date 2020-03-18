#include "MainManager.h"
#include <Windows.h>
 
int main()
{
	MainManager::CreateInstance();
	MainManager::GetInstance()->run();
	MainManager::DestroyInstance();

	return 0;
}