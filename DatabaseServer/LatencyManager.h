#ifndef __LATENCYMANAGER_H__
#define __LATENCYMANAGER_H__

class User;

class LatencyManager
{
private:
	static LatencyManager* Instance;

	LatencyManager();
	~LatencyManager();
public:
	static void CreateInstance();
	static LatencyManager* GetInstance();
	static void DestroyInstance();

	void recv(User*);
	void send(User*);
};

#endif