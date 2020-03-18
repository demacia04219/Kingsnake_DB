#include "LatencyManager.h"
#include "GlobalProtocol.h"
#include "User.h"

LatencyManager* LatencyManager::Instance = nullptr;

void LatencyManager::CreateInstance() {
	if (Instance == nullptr)
		Instance = new LatencyManager();
}
LatencyManager* LatencyManager::GetInstance() {
	return Instance;
}
void LatencyManager::DestroyInstance() {
	if (Instance != nullptr)
		delete Instance;
}

LatencyManager::LatencyManager() {

}
LatencyManager::~LatencyManager() {

}

void LatencyManager::recv(User* user) {
	
}
void LatencyManager::send(User* user) {

}