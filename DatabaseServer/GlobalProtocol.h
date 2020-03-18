#ifndef __GLOBALPROTOCOL_H__
#define __GLOBALPROTOCOL_H__

enum LICENCE_PROTOCOL {
	CLIENT_PASSWORD,				// 클라에서 비밀번호를 받음[ 16바이트 ]
	SERVER_RESULT,					// 비밀번호 검사 결과 [ bool ]
};
enum HEAD_PROTOCOL
{
	/* 테이블 변경 */
	GAMESERVER_REQ_SET_BASICINFO, // [ string ID, string nickname, int LV, int EXP, int gold, int gem, int gemOfVictory, int costume ]
	GAMESERVER_REQ_SET_MATCHINFO, // [ string ID, int matchingCount, int victoryCount, int victoryStreak, int loseStreak, int score ]
	GAMESERVER_REQ_SET_CHARACTER_DACK, // [ string ID, int leaderCharacter, int subCharacter1, int subCharacter2, int subCharacter3 ]
	GAMESERVER_REQ_SET_ITEM_DACK, // [ string ID, bool isItem1Exist, bool isItem2Exist, bool isItem3Exist ]
	GAMESERVER_REQ_SET_EMOTION_DACK,
	GAMESERVER_REQ_SET_CHARACTER_INVENTORY, // [string ID][vector<int>...]
	GAMESERVER_REQ_SET_ITEM_INVENTORY, // [string ID][vector<int>...]
	GAMESERVER_REQ_SET_COSTUME_INVENTORY, // [string ID][vector<int>...]
	GAMESERVER_REQ_SET_EMOTION_INVENTORY, // [string ID][vector<int>...]

	DBSERVER_RES_SET_BASICINFO, // [ string ID, bool isSuccess ]
	DBSERVER_RES_SET_MATCHINFO, // [ string ID, bool isSuccess ]
	DBSERVER_RES_SET_CHARA_DACK, // [ string ID, bool isSuccess ]
	DBSERVER_RES_SET_ITEM_DACK, // [ string ID, bool isSuccess ]
	DBSERVER_RES_SET_EMOTION_DACK,
	DBSERVER_RES_SET_SUBCHARACTER_INVENTORY, // [ string ID, bool isSuccess ]
	DBSERVER_RES_SET_ARTIFACT_INVENTORY, // [ string ID, bool isSuccess ]
	DBSERVER_RES_SET_COSTUME_INVENTORY, // [ string ID, bool isSuccess ]
	DBSERVER_RES_SET_EMOTION_INVENTORY,

	/* 테이블 탐색 */
	GAMESERVER_REQ_BASICINFO, // [ string ID ]
	GAMESERVER_REQ_MATCHINFO, // [ string ID ]
	GAMESERVER_REQ_CHARACTER_DACK, // [ string ID ]
	GAMESERVER_REQ_ARTIFACT_DACK, // [ string ID ]
	GAMESERVER_REQ_EMOTION_DACK,
	GAMESERVER_REQ_COSTUME_INVENTORY, // [ string ID ]
	GAMESERVER_REQ_ARTIFACT_INVENTORY, // [ string ID ]
	GAMESERVER_REQ_EMOTION_INVENTORY,
	GAMESERVER_REQ_CHARACTER_INVENTORY, // [ string ID ]

	DBSERVER_RES_BASICINFO, // [ string ID, bool isGoogleID, string nickname, int LV, int EXP, int gold, int gem, int gemOfVictory, int costume ]
	DBSERVER_RES_MATCHINFO, // [ string ID, int matchingCount, int victoryCount, int victoryStreak, int loseStreak, int score ]
	DBSERVER_RES_CHARACTER_DACK, // [ string ID, int leaderCharacter, int subCharacter1, int subCharacter2, int subCharacter3 ]
	DBSERVER_RES_ITEM_DACK, // [ string ID, int itemSlot1, int itemSlot2, int itemSlot3 ]
	DBSERVER_RES_CHARACTER_INVENTORY, //
	DBSERVER_RES_ARTIFACT_INVENTORY, //
	DBSERVER_RES_COSTUME_INVENTORY, //
	DBSERVER_RES_EMOTION_INVENTORY,

	/* 계정 유무 확인 */
	GAMESERVER_REQ_CHECK_USEREXIST, // [ string ID ]

	DBSERVER_RES_CHECK_USEREXIST, // [ string ID, bool isExist ]

	/* 계정 변경 */
	// REGISTER(가입)
	GAMESERVER_REQ_REGISTER = 9000, // [ string ID, bool isGoogleID, string nickname ]
	DBSERVER_RES_REGISTER, // [ string ID, bool isSuccess ]
	// Withdraw(탈퇴)
	GAMESERVER_REQ_WITHDRAW, // [ string ID ]
	DBSERVER_RES_WITHDRAW, // [ string ID, bool isSuccess ]

	GAMESERVER_MATCHEND = 12345,

	/* ADD ITEM INTO INVENTORY */
	GAMESERVER_REQ_ADDITEM = 20000, // [string ID][string Table][int index]
	DBSERVER_RES_ADDITEM, // [bool isSuccess]

	/* GET DB */
	GAMESERVER_REQ_GETDB = 30000, // [string ID][string Table]
	DBSERVER_RES_GETDB, // [string ID][string Table][data...]

	/* SET DB */
	GAMESERVER_REQ_SETDB = 40000, // [string ID][string Table][data...]
	DBSERVER_RES_SETDB, // [bool isSuccess]
};

enum SERVICE_PROTOCOL {

};

#endif
