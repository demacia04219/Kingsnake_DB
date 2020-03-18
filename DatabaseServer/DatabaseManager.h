#ifndef __DATABASE_MANAGER_H__
#define __DATABASE_MANAGER_H__

#include <list>
#include <functional>
#include <unordered_map>

#define TOTAL_DBSTRATEGY 8

class User;

using namespace std;

using GoogleID = string;

class DatabaseManager
{
	unordered_map<int, function<void(User&)>> functionMap;

	unordered_map<string, string> tableMap;

	static DatabaseManager Instance;

	DatabaseManager();
	~DatabaseManager();
	
public:
	static DatabaseManager& GetInstance();

	void proceed(User&);

	/* PROTOCOL FUNCTIONS */
	/* Give Data */
	void reqBasicInfo(User&);
	void reqMatchInfo(User&);
	void reqCharacterDack(User&);
	void reqArtifactDack(User&);
	void reqCharacterInventory(User&);
	void reqArtifactInventory(User&);
	void reqCostumeInventory(User&);
	/* ChangeData */
	void reqSetBasicInfo(User&);
	void reqSetMatchInfo(User&);
	void reqSetCharacterDack(User&);
	void reqSetArtifactDack(User&);
	void reqSetSubCharaInventory(User&);
	void reqSetArtifactInventory(User&);
	void reqSetCostumeInventory(User&);
	/* Register & Withdraw */
	void reqRegister(User&);
	void reqWithdraw(User&);
	/* CheckIDExist */
	void reqCheckIDExist(User&);
	/* CollectMatchResult */
	void reqCollectMatchResult(User&);

	/* Add item into inventory */
	void reqAddItem(User&);
	/* Get whole table */
	void reqGetDB(User&);
	/* Set whole table */
	void reqSetDB(User&);


private:
	/* CUSTOM FUNCTIONS */
	void EnterCS();
	void LeaveCS();
};

#endif