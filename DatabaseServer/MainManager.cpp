#include "IOCPServer.h"
#include "User.h"
#include "MYSQLManager.h"
#include "MainManager.h"

MainManager* MainManager::Instance = nullptr;

MainManager::MainManager()
{
	server = new IOCPServer();
}
MainManager::~MainManager()
{
	delete server;
}

MainManager* MainManager::CreateInstance()
{
	if (Instance == nullptr)
	{
		Instance = new MainManager();

		MYSQLManager::CreateInstance();
	}
	printf("\n");
	return Instance;
}
MainManager* MainManager::GetInstance()
{
	return Instance;
}
void MainManager::DestroyInstance()
{
	if (Instance != nullptr)
	{
		MYSQLManager::DestroyInstance();

		delete Instance;
		Instance = nullptr;
	}
}

void MainManager::run()
{
	server->start(SERVERPORT);
	//server->setNagle(false);
	while (true)
	{
		server->acceptClient(new User());
	}

	SocketInit::stop();
	printf("server close\n");
	system("pause");
}