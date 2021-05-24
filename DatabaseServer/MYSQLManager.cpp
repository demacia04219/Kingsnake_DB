#include "MYSQLManager.h"
#include "DataTable.h"
#include <Windows.h>
#include <WinNls.h>
#include <iostream>
#include <algorithm>
#include "Global.h"

const char* JHW_DB_HOST = NULL;
const char* JHW_DB_USER = "root";
const char* JHW_DB_PW = "1721";
const char* JHW_DB_DBNAME = "kingsnake";
const int	JHW_DB_PORT = 3306;

void changeQueryToUTF8(const char *pQuery, char* pUTFQuery)
{
	/* something spectaculous */
	wchar_t    awcUC[QUERY_MAX_LENGTH];
	int            iLength;

	iLength = MultiByteToWideChar(CP_ACP, 0, pQuery, -1, NULL, NULL);
	memset(awcUC, 0, sizeof(wchar_t) * iLength);
	MultiByteToWideChar(CP_ACP, 0, pQuery, -1, awcUC, iLength);

	iLength = WideCharToMultiByte(CP_UTF8, 0, awcUC, -1, NULL, 0, NULL, NULL);
	memset(pUTFQuery, 0, iLength);
	WideCharToMultiByte(CP_UTF8, 0, awcUC, -1, pUTFQuery, iLength, NULL, NULL);
	/* */
}

MYSQLManager* MYSQLManager::instance = nullptr;

MYSQLManager::MYSQLManager()
{
	mysql = 0;
}
MYSQLManager::~MYSQLManager()
{
	if (mysql != 0)
	{
		mysql_close(mysql);
	}
}
bool MYSQLManager::Initialize()
{
	host	= JHW_DB_HOST;
	user	= JHW_DB_USER;
	pw		= JHW_DB_PW;
	dbname	= JHW_DB_DBNAME;
	port	= JHW_DB_PORT;

	mysql = new MYSQL;
	mysql_init(mysql);

	unsigned int timeout = 864000;

	/* option change */
	if (mysql_options(mysql, MYSQL_SET_CHARSET_NAME, "utf8mb4") == 0)
	{
		std::cout << "MYSQL CHARSET SETTING COMPLETE" << std::endl;
	}
	else {
		std::cout << "ERROR: MYSQL_SET_CHARSET_NAME" << std::endl;
		std::cout << mysql_error(mysql) << std::endl;
	}
	if (mysql_options(mysql, MYSQL_INIT_COMMAND, "SET NAMES utf8mb4") == 0)
	{
		std::cout << "MYSQL COMMAND INIT COMPLETE" << std::endl;
	}
	else {
		std::cout << "ERROR: MYSQL_INIT_COMMAND" << std::endl;
		std::cout << mysql_error(mysql) << std::endl;
	}
	if (mysql_options(mysql, MYSQL_OPT_CONNECT_TIMEOUT, (const char*)&timeout) == 0)
	{
		std::cout << "MYSQL_OPT_CONNECT_TIMEOUT: " << timeout << std::endl;
		std::cout << mysql_error(mysql) << std::endl;
	}
	else {
		std::cout << "ERROR: MYSQL_OPT_CONNECT_TIMEOUT" << std::endl;
		std::cout << mysql_error(mysql) << std::endl;
	}

	/* REAL CONNECT */
	bool isInitSuccess = (mysql_real_connect(OUT mysql, host, user, pw, dbname, port, (char*)NULL, 0) != NULL);
	if (isInitSuccess) 
	{
		std::cout << "MYSQL Initialize: TRUE" << std::endl;
	}
	else {
		std::cout << "MYSQL Initialize: FALSE" << std::endl;
		std::cout << mysql_error(mysql) << std::endl;
	}
	return isInitSuccess;
}

void MYSQLManager::CreateInstance()
{
	if (instance == nullptr)
	{
		instance = new MYSQLManager();
		bool isInitSuccess = instance->Initialize();
		if (!isInitSuccess) {
			exit(0);
		}
	}
}
MYSQLManager* MYSQLManager::GetInstance()
{
	return instance;
}
void MYSQLManager::DestroyInstance()
{
	if (instance != nullptr)
	{
		delete instance;
		instance = nullptr;
	}
}

bool MYSQLManager::getSQLResult(DataTable& out_table, const char* _sql)
{
	printf("QUERY SEARCHED: %s\n", _sql);

	char UTF8Query[QUERY_MAX_LENGTH] = { 0, };
	changeQueryToUTF8(_sql, UTF8Query);

	int query_status = mysql_query(mysql, UTF8Query);
	if (query_status != 0)
	{
		std::cout << "QUERY ERROR: " << _sql << std::endl;
		std::cout << mysql_error(mysql) << std::endl;
		return false;
	}

	MYSQL_RES* result = mysql_store_result(mysql);

	MYSQL_ROW sql_row;
	int length_column = mysql_num_fields(result);
	while ((sql_row = mysql_fetch_row(result)) != NULL)
	{
		Record record;
		for (int idx = 0; idx < length_column; idx++)
		{
			record.fields.push_back(sql_row[idx]);
		}
		out_table << record;
	}

	mysql_free_result(result);

	return true;
}
bool MYSQLManager::changeDB(const char* _query)
{
	char UTFQuery[QUERY_MAX_LENGTH] = { 0, };
	changeQueryToUTF8(_query, UTFQuery);

	int query_status = mysql_query(mysql,UTFQuery);
	if (query_status != 0)
	{
		std::cout << "CHANGE QUERY ERROR: " << _query << std::endl;
		std::cout << mysql_error(mysql) << std::endl;
		return false;
	}

	std::cout << "CHANGED BY QUERY: " << _query << std::endl;

	return true;
}

bool MYSQLManager::startTransaction() {

	int query_status = mysql_query(mysql, "START TRANSACTION;");
	if (query_status != 0) {
		std::cout << "CANNOT START TRANSACTION" << std::endl;
		return false;
	}

	std::cout << "TRANSACTION STARTED." << std::endl;

	return true;
}
bool MYSQLManager::commitTransaction() {
	int query_status = mysql_query(mysql, "COMMIT;");
	if (query_status != 0) {
		std::cout << "CANNOT COMMIT TRANSACTION." << std::endl;
		return false;
	}

	std::cout << "COMMIT SUCCESSFULLY DONE." << std::endl;

	return true;
}
bool MYSQLManager::rollbackTransaction() {
	int query_status = mysql_query(mysql, "ROLLBACK;");
	if (query_status != 0) {
		std::cout << "CANNOT ROLLBACK TRANSACTION." << std::endl;
		return false;
	}

	std::cout << "ROLLBACK SUCCESSFULLY DONE." << std::endl;

	return true;
}
