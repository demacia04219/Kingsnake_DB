#include "DataTable.h"
#include "Global.h"

string &Record::operator[](const int _index)
{
	return fields[_index];
}
int Record::getFieldCount()
{
	return static_cast<int>(fields.size());
}

ostream &operator<<(ostream &stream, const DataTable &dbTable)
{
	for (auto record : dbTable.sheet)
	{
		for (int idx = 0; idx < record.getFieldCount(); idx++)
		{
			if (record[idx] == "")
				os << "[\t]"
				   << "\t";
			else
				os << record[idx] << "\t";
		}
		os << endl;
	}
	return stream;
}

Record &DataTable::operator[](const int _index)
{
	return sheet[_index];
}
DataTable &DataTable::operator<<(Record _record)
{
	sheet.push_back(_record);
	return *this;
}

int DataTable::getRecordCount()
{
	return static_cast<int>(sheet.size());
}
int DataTable::getFieldCount()
{
	return sheet[0].getFieldCount();
}
DataTable DataTable::search(function<bool(Record &)> pCondition)
{
	DataTable resultTable;

	for (Record &itrRecord : sheet)
	{
		if (pCondition(itrRecord))
		{
			resultTable << itrRecord;
		}
	}

	return resultTable;
}

int Record::getInteger(int pIndex)
{
	return stoi(fields[pIndex]);
}
float Record::getFloat(int pIndex)
{
	return stof(fields[pIndex]);
}
double Record::getDouble(int pIndex)
{
	return stod(fields[pIndex]);
}
bool Record::getBool(int pIndex)
{
	return (fields[pIndex] == "true");
}