
#include "User.h"
#include <stdio.h>
#include <algorithm>
#include "DatabaseManager.h"
#include <time.h>

int User::userAcceptCount = 0;

User::User()
{
	index = userAcceptCount;
	userAcceptCount++;
}
User::~User()
{
}

void User::recvProcess()
{
	DatabaseManager::GetInstance().proceed(*this);
}
void User::acceptProcess()
{
	printf("Game Server %d Connected: ", index);
}
void User::disconnectProcess()
{
	printf("Game Server %d Disconnected: ", index);
}
