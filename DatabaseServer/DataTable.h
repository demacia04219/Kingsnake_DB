#ifndef __CSVTABLE_H__
#define __CSVTABLE_H__

#include <iostream>
#include <ostream>
#include <vector>
#include <string>
#include <functional>

using namespace std;

struct Record
{
	vector<string> fields;
	
	string& operator[](const int _index);
	int getFieldCount();

	int getInteger(int pIndex);
	float getFloat(int pIndex);
	double getDouble(int pIndex);
	bool getBool(int pIndex);
};

class DataTable
{
public:
	vector<Record> sheet;

	Record& operator[](const int _index);
	DataTable& operator<<(Record _record);

	int getRecordCount();
	int getFieldCount();

	DataTable search(function<bool(Record&)>);
};

ostream &operator<<(ostream& stream, const DataTable& dbTable);

#endif