#include "DatabaseManager.h"
#include "User.h"
#include "MYSQLManager.h"
#include "GlobalProtocol.h"
#include "InitializeValue.h"
#include "DataTable.h"
#include "Encoding.h"
#include "CSVReader.h"

#include <cstdio>

#define COUNT_TABLE 6
#define MAX_LENGTH 1024

CRITICAL_SECTION cs;

DatabaseManager DatabaseManager::Instance = DatabaseManager();

void addFunction(unordered_map<int, function<void(User&)>> &pMap, int pKey, function<void(User&)> pValue);

DatabaseManager::DatabaseManager()
{
	InitializeCriticalSection(&cs);

	using std::placeholders::_1;
	/* DATABASE SET & CHANGE FUNCTIONS */
	addFunction(functionMap, GAMESERVER_REQ_SET_BASICINFO, bind(&DatabaseManager::reqSetBasicInfo, this, _1));
	addFunction(functionMap, GAMESERVER_REQ_SET_MATCHINFO, bind(&DatabaseManager::reqSetMatchInfo, this, _1));
	addFunction(functionMap, GAMESERVER_REQ_SET_CHARACTER_DACK, bind(&DatabaseManager::reqSetCharacterDack, this, _1));
	addFunction(functionMap, GAMESERVER_REQ_SET_ITEM_DACK, bind(&DatabaseManager::reqSetArtifactDack, this, _1));
	addFunction(functionMap, GAMESERVER_REQ_SET_CHARACTER_INVENTORY, bind(&DatabaseManager::reqSetSubCharaInventory, this, _1));
	addFunction(functionMap, GAMESERVER_REQ_SET_ITEM_INVENTORY, bind(&DatabaseManager::reqSetArtifactInventory, this, _1));
	addFunction(functionMap, GAMESERVER_REQ_SET_COSTUME_INVENTORY, bind(&DatabaseManager::reqSetCostumeInventory, this, _1));
	/* DATABASE SEARCH FUNCTIONS */
	addFunction(functionMap, GAMESERVER_REQ_BASICINFO, bind(&DatabaseManager::reqBasicInfo, this, _1));
	addFunction(functionMap, GAMESERVER_REQ_MATCHINFO, bind(&DatabaseManager::reqMatchInfo, this, _1));
	addFunction(functionMap, GAMESERVER_REQ_CHARACTER_DACK, bind(&DatabaseManager::reqCharacterDack, this, _1));
	addFunction(functionMap, GAMESERVER_REQ_ARTIFACT_DACK, bind(&DatabaseManager::reqArtifactDack, this, _1));
	addFunction(functionMap, GAMESERVER_REQ_CHARACTER_INVENTORY, bind(&DatabaseManager::reqCharacterInventory, this, _1));
	addFunction(functionMap, GAMESERVER_REQ_ARTIFACT_INVENTORY, bind(&DatabaseManager::reqArtifactInventory, this, _1));
	addFunction(functionMap, GAMESERVER_REQ_COSTUME_INVENTORY, bind(&DatabaseManager::reqCostumeInventory, this, _1));
	/* ACCOUNT CONTROL FUNCTIONS */
	addFunction(functionMap, GAMESERVER_REQ_REGISTER, bind(&DatabaseManager::reqRegister, this, _1));
	addFunction(functionMap, GAMESERVER_REQ_WITHDRAW, bind(&DatabaseManager::reqWithdraw, this, _1));
	addFunction(functionMap, GAMESERVER_REQ_CHECK_USEREXIST, bind(&DatabaseManager::reqCheckIDExist, this, _1));
	/* MATCH RESULT COLLECT FUNCTION */
	addFunction(functionMap, GAMESERVER_MATCHEND, bind(&DatabaseManager::reqCollectMatchResult, this, _1));
}
DatabaseManager::~DatabaseManager()
{
	DeleteCriticalSection(&cs);
}

DatabaseManager& DatabaseManager::GetInstance() { return Instance; }

void DatabaseManager::proceed(User& pUser) {

	int protocol;
	pUser >> protocol;

	printf("protocol: %d\n", protocol);

	// check map that it contains protocol as key
	auto target = functionMap.find(protocol);
	bool isProtocolExist = target != functionMap.end();

	if (isProtocolExist) {
		target->second(pUser);
	}
	else {
		printf("UNREGISTERED PROTOCOL ENTERED: [%d]\n", protocol);
	}
}

void DatabaseManager::reqBasicInfo(User& pUser) {

	enum {
		ID = 0,
		NICKNAME,
		USERLEVEL,
		EXP,
		GOLD,
		JEWEL,
		REWARD_DAILY,
		MAIN_COSTUME
	};
	EnterCS();
	/* Unpacking */
	std::string id;
	pUser >> id;

	/* Processing */
	DataTable table;
	char query[1024] = { 0, };
	sprintf(query, "select * from BASICINFO where id = \'%s\';", id.c_str());
	MYSQLManager::GetInstance()->getSQLResult(table, query);

	if (table.getRecordCount() == 0) {
		LeaveCS();
		return;
	}

	/* Packing */
	SendPacket sp;
	// protocol
	sp << HEAD_PROTOCOL::DBSERVER_RES_BASICINFO;
	Record& result = table[0];
	// ID : string
	sp << id;
	// nick: string
	sp << result[NICKNAME];
	// lv: int
	sp << result.getInteger(USERLEVEL);
	// exp: int
	sp << result.getInteger(EXP);
	// gold: int
	sp << result.getInteger(GOLD);
	// gem: int
	sp << result.getInteger(JEWEL);
	// gemOfVictory: int
	sp << result.getInteger(REWARD_DAILY);
	// costume: int
	sp << result.getInteger(MAIN_COSTUME);
	LeaveCS();
	pUser.sendMsg(sp);
}
void DatabaseManager::reqMatchInfo(User& pUser)
{
	enum {
		ID = 0,
		MATCHINGCOUNT,
		VICTORYCOUNT,
		VICTORYSTREAK,
		LOSESTREAK,
		MATCHSCORE,
	};

	EnterCS();
	string id;
	pUser >> id;

	DataTable table;
	char query[1024] = { 0, };
	sprintf(query, "select * from MATCHINFO where id = \'%s\';", id.c_str());
	MYSQLManager::GetInstance()->getSQLResult(table, query);

	if (table.getRecordCount() == 0) {
		LeaveCS();
		return;
	}

	SendPacket sp;
	sp << HEAD_PROTOCOL::DBSERVER_RES_MATCHINFO;
	Record& result = table[0];

	sp << id;
	sp << result.getInteger(MATCHINGCOUNT);
	sp << result.getInteger(VICTORYCOUNT);
	sp << result.getInteger(VICTORYSTREAK);
	sp << result.getInteger(LOSESTREAK);
	sp << result.getInteger(MATCHSCORE);
	LeaveCS();
	pUser.sendMsg(sp);
}
void DatabaseManager::reqCharacterDack(User& pUser)
{
	enum {
		ID = 0,
		LEADERCHARACTER,
		SUBCHARACTER1,
		SUBCHARACTER2,
		SUBCHARACTER3
	};
	EnterCS();
	/* Unpacking */
	std::string id;
	pUser >> id;

	/* Processing */
	DataTable table;
	char query[1024] = { 0, };
	sprintf(query, "select * from CHARA_DACK where id = \'%s\';", id.c_str());
	MYSQLManager::GetInstance()->getSQLResult(table, query);

	if (table.getRecordCount() == 0) {
		printf("no data...");
		LeaveCS();
		return;
	}

	/* Packing */
	SendPacket sp;
	// protocol
	sp << HEAD_PROTOCOL::DBSERVER_RES_CHARACTER_DACK;
	Record result = table[0];
	// ID : string
	sp << id;
	// leaderCharacter: int
	sp << result.getInteger(LEADERCHARACTER);
	// subCharacter1: int
	sp << result.getInteger(SUBCHARACTER1);
	// subCharacter2: int
	sp << result.getInteger(SUBCHARACTER2);
	// subCharacter3: int
	sp << result.getInteger(SUBCHARACTER3);
	LeaveCS();
	pUser.sendMsg(sp);
}
void DatabaseManager::reqArtifactDack(User& pUser)
{
	enum {
		ID = 0,
		ITEMSLOT1,
		ITEMSLOT2,
		ITEMSLOT3,
	};
	EnterCS();
	/* Unpacking */
	std::string id;
	pUser >> id;

	/* Processing */
	DataTable table;
	char query[1024] = { 0, };
	sprintf(query, "select * from ARTIFACT_DACK where id = \'%s\';", id.c_str());
	MYSQLManager::GetInstance()->getSQLResult(table, query);

	if (table.getRecordCount() == 0) {
		LeaveCS();
		return;
	}

	/* Packing */
	SendPacket sp;
	// protocol
	sp << HEAD_PROTOCOL::DBSERVER_RES_ITEM_DACK;
	Record targetRecord = table[0];
	// ID : string
	sp << id;
	// itemSlot1: int
	sp << targetRecord.getInteger(ITEMSLOT1);
	// itemSlot2: int
	sp << targetRecord.getInteger(ITEMSLOT2);
	// itemSlot3: int
	sp << targetRecord.getInteger(ITEMSLOT3);
	LeaveCS();
	pUser.sendMsg(sp);
}
void DatabaseManager::reqCharacterInventory(User& pUser)
{
	EnterCS();

	string id;
	pUser >> id;

	DataTable table;
	char query[1024] = { 0, };
	sprintf(query, "select * from SUBCHARACTER_INVENTORY where id = \'%s\';", id.c_str());

	if (MYSQLManager::GetInstance()->getSQLResult(table, query) == false) {
		LeaveCS();
		return;
	}

	Record& userRecord = table[0];
	
	SendPacket sp;
	sp << HEAD_PROTOCOL::DBSERVER_RES_CHARACTER_INVENTORY;
	sp << id;
	{
		list<int> existCharacterList;
		for (int idx = 1; idx < userRecord.getFieldCount(); idx++)
		{
			if (userRecord[idx] == "true")
			{
				existCharacterList.push_back(idx);
			}
		}

		sp << existCharacterList.size();
		for (int& itrCharacterIndex : existCharacterList)
		{
			sp << itrCharacterIndex;
		}
	}
	LeaveCS();
	pUser.sendMsg(sp);
}
void DatabaseManager::reqArtifactInventory(User& pUser)
{
	enum {
		IDX,
	};

	EnterCS();
	std::string id;
	pUser >> id;

	DataTable table;
	char query[1024] = { 0, };
	sprintf(query, "select * from ARTIFACT_INVENTORY where id = \'%s\';", id.c_str());
	
	if (MYSQLManager::GetInstance()->getSQLResult(table, query) == false) {
		LeaveCS();
		return;
	}

	Record& userRecord = table[0];

	SendPacket sp;
	sp << HEAD_PROTOCOL::DBSERVER_RES_ARTIFACT_INVENTORY;
	sp << id;
	{
		list<int> existArtifactList;
		for (int idx = 1; idx < userRecord.getFieldCount(); idx++)
		{
			if (userRecord[idx] == "true")
			{
				existArtifactList.push_back(idx);
			}
		}

		sp << existArtifactList.size();
		for (int& itrCharacterIndex : existArtifactList)
		{
			sp << itrCharacterIndex;
		}
	}
	LeaveCS();

	pUser.sendMsg(sp);
}
void DatabaseManager::reqCostumeInventory(User& pUser)
{
	EnterCS();
	std::string id;
	pUser >> id;

	DataTable table;
	char query[1024] = { 0, };
	sprintf(query, "select * from COSTUME_INVENTORY where id = \'%s\';", id.c_str());
	
	if (MYSQLManager::GetInstance()->getSQLResult(table, query) == false) {
		LeaveCS();
		return;
	}

	Record& userRecord = table[0];

	SendPacket sp;
	sp << HEAD_PROTOCOL::DBSERVER_RES_COSTUME_INVENTORY;
	sp << id;
	{
		list<int> existCostumeList;
		for (int idx = 1; idx < userRecord.getFieldCount(); idx++)
		{
			if (userRecord[idx] == "true")
			{
				existCostumeList.push_back(idx);
			}
		}

		sp << existCostumeList.size();
		for (int& itrCharacterIndex : existCostumeList)
		{
			sp << itrCharacterIndex;
		}
	}
	LeaveCS();
	pUser.sendMsg(sp);
}

void DatabaseManager::reqSetBasicInfo(User& pUser)
{
	EnterCS();
	string id = "";
	pUser >> id;
	// data unpack
	string nick;
	int lv, exp, gold, gem, gemOfVictory, costume;
	lv = exp = gold = gem = gemOfVictory = costume = 0;

	pUser >> nick >> lv >> exp >> gold >> gem >> gemOfVictory >> costume;

	// change db
	char query[1024] = { 0, };
	sprintf(query, "update basicinfo set nick = \'%s\', lv = %d, exp = %d, gold = %d, gem = %d, gemofvictory = %d, costume = %d where id = \'%s\';",
		nick.c_str(), lv, exp, gold, gem, gemOfVictory, costume, id.c_str());
	
	MYSQLManager::GetInstance()->changeDB(query);

	SendPacket sp;
	sp << HEAD_PROTOCOL::DBSERVER_RES_SET_BASICINFO;
	sp << id;
	sp << true;
	LeaveCS();
	pUser.sendMsg(sp);
}
void DatabaseManager::reqSetMatchInfo(User& pUser)
{
	EnterCS();
	string id = "";
	pUser >> id;

	int matchingCount, victoryCount, victoryStreak, loseStreak, matchscore;
	matchingCount = victoryCount = victoryStreak = loseStreak = matchscore = 0;

	pUser >> matchingCount >> victoryCount >> victoryStreak >> loseStreak >> matchscore;

	char query[1024] = { 0, };
	sprintf(query, "update MATCHINFO set matchingCount = %d, victoryCount = %d, victoryStreak = %d, loseStreak = %d, score = %d where id = \'%s\';",
		matchingCount, victoryCount, victoryStreak, loseStreak, matchscore, id.c_str());

	MYSQLManager::GetInstance()->changeDB(query);

	SendPacket sp;
	sp << HEAD_PROTOCOL::DBSERVER_RES_SET_MATCHINFO;
	sp << id;
	sp << true;
	LeaveCS();
	pUser.sendMsg(sp);
}
void DatabaseManager::reqSetCharacterDack(User& pUser)
{
	EnterCS();
	string id = "";
	pUser >> id;

	int leader, sub1, sub2, sub3;
	leader = sub1 = sub2 = sub3 = 0;

	pUser >> leader >> sub1 >> sub2 >> sub3;

	char query[1024] = { 0, };
	sprintf(query, "update CHARA_DACK set leaderCharacter = %d, subCharacter1 = %d, subCharacter2 = %d, subCharacter3 = %d where id = \'%s\';",
		leader, sub1, sub2, sub3, id.c_str());

	MYSQLManager::GetInstance()->changeDB(query);

	SendPacket sp;
	sp << HEAD_PROTOCOL::DBSERVER_RES_SET_CHARA_DACK;
	sp << id;
	sp << true;
	LeaveCS();
	pUser.sendMsg(sp);
}
void DatabaseManager::reqSetArtifactDack(User& pUser)
{
	EnterCS();
	string id = "";
	pUser >> id;

	int item1, item2, item3;
	item1 = item2 = item3 = 0;

	pUser >> item1 >> item2 >> item3;

	char query[1024] = { 0, };
	sprintf(query, "update ARTIFACT_DACK set a1 = %d, a2 = %d, a3 = %d where id = \'%s\';",
		item1, item2, item3, id.c_str());

	MYSQLManager::GetInstance()->changeDB(query);

	SendPacket sp;
	sp << HEAD_PROTOCOL::DBSERVER_RES_SET_ITEM_DACK;
	sp << id;
	sp << true;
	LeaveCS();
	pUser.sendMsg(sp);
}
void DatabaseManager::reqSetSubCharaInventory(User& pUser)
{
	EnterCS();

	MYSQLManager::GetInstance()->startTransaction();

	unordered_map<bool, string> boolToString;
	boolToString.insert(make_pair(true, "true"));
	boolToString.insert(make_pair(false, "false"));

	string id = "";
	pUser >> id;

	int recordCnt = 0;
	pUser >> recordCnt;

	std::vector<int> chararVec;
	chararVec.resize(recordCnt);

	for (int idx = 0; idx < recordCnt; idx++)
	{
		pUser >> chararVec[idx];
	}

	{
		char query[1024] = { 0, };
		array<bool, 3> existCheckArray;

		existCheckArray[0] = (find(chararVec.begin(), chararVec.end(), 0) != chararVec.end());
		existCheckArray[1] = (find(chararVec.begin(), chararVec.end(), 1) != chararVec.end());
		existCheckArray[2] = (find(chararVec.begin(), chararVec.end(), 2) != chararVec.end());

		sprintf(query, "UPDATE SUBCHARA_INVENTORY SET C1 = %s, C2 = %s, C3 = %s where id = \'%s\';",
			id.c_str(),
			boolToString[existCheckArray[0]].c_str(),
			boolToString[existCheckArray[1]].c_str(),
			boolToString[existCheckArray[2]].c_str());
		MYSQLManager::GetInstance()->changeDB(query);
	}

	SendPacket sp;
	sp << HEAD_PROTOCOL::DBSERVER_RES_SET_SUBCHARACTER_INVENTORY;
	sp << id;
	sp << true;

	MYSQLManager::GetInstance()->commitTransaction();

	LeaveCS();
	pUser.sendMsg(sp);
}
void DatabaseManager::reqSetArtifactInventory(User& pUser)
{
	EnterCS();

	string id = "";
	pUser >> id;

	unordered_map<bool, string> boolToString;
	boolToString.insert(make_pair(true, "true"));
	boolToString.insert(make_pair(false, "false"));

	int recordCnt = 0;
	pUser >> recordCnt;

	std::vector<int> artiVec;
	artiVec.resize(recordCnt);

	for (int idx = 0; idx < recordCnt; idx++)
	{
		pUser >> artiVec[idx];
	}

	{
		char query[1024] = { 0, };
		array<bool, 7> existCheckArray;

		existCheckArray[0] = (find(artiVec.begin(), artiVec.end(), 0) != artiVec.end());
		existCheckArray[1] = (find(artiVec.begin(), artiVec.end(), 1) != artiVec.end());
		existCheckArray[2] = (find(artiVec.begin(), artiVec.end(), 2) != artiVec.end());
		existCheckArray[3] = (find(artiVec.begin(), artiVec.end(), 3) != artiVec.end());
		existCheckArray[4] = (find(artiVec.begin(), artiVec.end(), 4) != artiVec.end());
		existCheckArray[5] = (find(artiVec.begin(), artiVec.end(), 5) != artiVec.end());
		existCheckArray[6] = (find(artiVec.begin(), artiVec.end(), 6) != artiVec.end());

		sprintf(query, "UPDATE ARTRIFACT_INVENTORY SET C1 = %s, C2 = %s, C3 = %s, C4 = %s, C5 = %s, C6 = %s, C7 = %s where id = \'%s\';",
			id.c_str(),
			boolToString[existCheckArray[0]].c_str(),
			boolToString[existCheckArray[1]].c_str(),
			boolToString[existCheckArray[2]].c_str(),
			boolToString[existCheckArray[3]].c_str(),
			boolToString[existCheckArray[4]].c_str(),
			boolToString[existCheckArray[5]].c_str(),
			boolToString[existCheckArray[6]].c_str());
		MYSQLManager::GetInstance()->changeDB(query);
	}

	SendPacket sp;
	sp << HEAD_PROTOCOL::DBSERVER_RES_SET_ARTIFACT_INVENTORY;
	sp << id;
	sp << true;

	LeaveCS();
	pUser.sendMsg(sp);
}
void DatabaseManager::reqSetCostumeInventory(User& pUser)
{
	EnterCS();

	unordered_map<bool, string> boolToString;
	boolToString.insert(make_pair(true, "true"));
	boolToString.insert(make_pair(false, "false"));

	string id = "";
	pUser >> id;

	int recordCnt = 0;
	pUser >> recordCnt;

	std::vector<int> costuVec;
	costuVec.resize(recordCnt);

	for (int idx = 0; idx < recordCnt; idx++)
	{
		pUser >> costuVec[idx];
	}

	{
		char query[1024] = { 0, };
		array<bool, 3> existCheckArray;

		existCheckArray[0] = (find(costuVec.begin(), costuVec.end(), 0) != costuVec.end());
		existCheckArray[1] = (find(costuVec.begin(), costuVec.end(), 1) != costuVec.end());
		existCheckArray[2] = (find(costuVec.begin(), costuVec.end(), 2) != costuVec.end());

		sprintf(query, "UPDATE COSTUME_INVENTORY SET C1 = %s, C2 = %s, C3 = %s where id = \'%s\';",
			id.c_str(),
			boolToString[existCheckArray[0]].c_str(),
			boolToString[existCheckArray[1]].c_str(),
			boolToString[existCheckArray[2]].c_str());
		MYSQLManager::GetInstance()->changeDB(query);
	}

	SendPacket sp;
	sp << HEAD_PROTOCOL::DBSERVER_RES_SET_COSTUME_INVENTORY;
	sp << id;
	sp << true;

	LeaveCS();
	pUser.sendMsg(sp);
}

void DatabaseManager::reqCheckIDExist(User& pUser)
{
	EnterCS();
	string id = "";
	pUser >> id;
	
	char query[1024] = { 0, };
	DataTable result;
	sprintf(query, "select * from BASICINFO where id = \'%s\';", id.c_str());
	MYSQLManager::GetInstance()->getSQLResult(result, query);

	SendPacket sp;
	sp << HEAD_PROTOCOL::DBSERVER_RES_CHECK_USEREXIST;
	sp << id;
	sp << (result.getRecordCount() != 0);
	LeaveCS();
	pUser.sendMsg(sp);
}

void DatabaseManager::reqRegister(User& pUser) {

	std::string ID;
	std::string nickname;
	pUser >> ID;
	pUser >> nickname;

	bool isSuccess = false;
	SendPacket sp;

	unordered_map<bool, string> boolToString;
	boolToString.insert(make_pair(true, "true"));
	boolToString.insert(make_pair(false, "false"));

	EnterCS();
	MYSQLManager::GetInstance()->startTransaction();

	char query[1024] = { 0, };
	sprintf(query, "CALL REGISTER(\'%s\', \'%s\');", ID.c_str(), nickname.c_str());
	isSuccess = MYSQLManager::GetInstance()->changeDB(query);
	
	if (!isSuccess)
	{
		MYSQLManager::GetInstance()->commitTransaction();
		sp << HEAD_PROTOCOL::DBSERVER_RES_REGISTER;
		sp << ID;
		sp << true;
	}
	else 
	{
		MYSQLManager::GetInstance()->commitTransaction();
		sp << HEAD_PROTOCOL::DBSERVER_RES_REGISTER;
		sp << ID;
		sp << true;
	}

	LeaveCS();
	pUser.sendMsg(sp);
}
void DatabaseManager::reqWithdraw(User& pUser) {

	std::string id = "";
	pUser >> id;

	bool isSuccess = true;
	SendPacket sp;

	EnterCS();
	MYSQLManager::GetInstance()->startTransaction();

	{
		char query[1024] = { 0, };
		sprintf(query, "CALL WITHDRAW(\'%s\');", id.c_str());

		isSuccess = MYSQLManager::GetInstance()->changeDB(query);
		if (!isSuccess) goto LABEL_WITHDRAW_ERROR;
	}

//LABEL_WITHDRAW_SUCCESS
	MYSQLManager::GetInstance()->commitTransaction();
	LeaveCS();
	sp << HEAD_PROTOCOL::DBSERVER_RES_WITHDRAW;
	sp << id;
	sp << true;

	pUser.sendMsg(sp);
	return;

LABEL_WITHDRAW_ERROR:
	MYSQLManager::GetInstance()->rollbackTransaction();
	LeaveCS();
	sp << HEAD_PROTOCOL::DBSERVER_RES_WITHDRAW;
	sp << id;
	sp << false;

	pUser.sendMsg(sp);
}

void DatabaseManager::reqCollectMatchResult(User& pUser) {
	enum {
		WIN_P1 = 0,
		WIN_P2,
		DRAW,
	};
	enum {
		ID = 0,
		LEADER,
		SUB1,
		SUB2, 
		SUB3
	};

	EnterCS();
	
	string user1ID = "";
	string user2ID = "";
	int matchResult = 0;

	pUser >> user1ID;
	pUser >> user2ID;
	pUser >> matchResult;

	/* 데이터 저장 */
	///* */
	//char query[QUERY_MAX_LENGTH] = { 0, };
	//DataTable charaDackInfo_user1;
	//sprintf(query, "select * from CHARACTER_DACK where id = \'%s\';",
	//	user1ID.c_str());
	//MYSQLManager::GetInstance()->getSQLResult(charaDackInfo_user1, query);

	///* */
	//memset(query, 0, QUERY_MAX_LENGTH);
	//DataTable charaDackInfo_user2;
	//sprintf(query, "select * from CHARACTER_DACK where id = \'%s\';",
	//	user2ID.c_str());
	//MYSQLManager::GetInstance()->getSQLResult(charaDackInfo_user2, query);

	///* */
	//Record &user1Rec = charaDackInfo_user1[0], &user2Rec = charaDackInfo_user2[0];
	//CSVReader csvReader;
	//
	//csvReader.read(MATCHLOG_FILENAME);
	//Table &matchLogTable = csvReader.table;
	//
	//Row resultRow;
	///* matchInfo */
	//resultRow.push_back(user1Rec[ID]);
	//resultRow.push_back(user2Rec[ID]);
	//resultRow.push_back(to_string(matchResult));
	///* user1 */
	//resultRow.push_back(user1Rec[LEADER]); 
	//resultRow.push_back(user1Rec[SUB1]); 
	//resultRow.push_back(user1Rec[SUB2]);  
	//resultRow.push_back(user1Rec[SUB3]);
	///* user2 */
	//resultRow.push_back(user2Rec[LEADER]);
	//resultRow.push_back(user2Rec[SUB1]);
	//resultRow.push_back(user2Rec[SUB2]);
	//resultRow.push_back(user2Rec[SUB3]);

	//matchLogTable.push_back(resultRow); 

	//csvReader.write(MATCHLOG_FILENAME);

	cout << "MATCH COMPLETED: [" << user1ID
		<< "] vs [" << "user2ID" << "]" << endl;

	LeaveCS();
}

void DatabaseManager::EnterCS()
{
	EnterCriticalSection(&cs);
}
void DatabaseManager::LeaveCS()
{
	LeaveCriticalSection(&cs);
}

void addFunction(unordered_map<int, function<void(User&)>> &pMap, int pKey, function<void(User&)> pValue)
{
	pMap.insert(make_pair(pKey, pValue));
}
