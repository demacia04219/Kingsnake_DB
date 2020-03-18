#ifndef __DBMANAGER_H__
#define __DBMANAGER_H__

#pragma comment(lib, "libmariadb.lib")

#include <mysql/mysql.h>
#include "Global.h"
#include <vector>

class DataTable;

class MYSQLManager
{
private:
	static MYSQLManager* instance;
protected:
	MYSQLManager();
	~MYSQLManager();

	MYSQL *mysql;
	const char* host;
	const char* user;
	const char* pw;
	const char* dbname;
	int port;

public:
	static void CreateInstance();
	static MYSQLManager* GetInstance();
	static void DestroyInstance();

	bool Initialize();

	bool getSQLResult(DataTable& out_table, const char* _sql);
	bool changeDB(const char* _query);

	bool startTransaction();
	bool commitTransaction();
	bool rollbackTransaction();
};

#endif
