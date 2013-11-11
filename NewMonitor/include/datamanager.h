#ifndef DATAMANAGER_H
#define DATAMANAGER_H

#include "sqlitewrapper.h"
#include <map>
#include <memory>
#include <string>

class DataManager{
public:
	bool OpenDataFile(std::wstring);
	void CloseDataFile(std::wstring);
private:
	std::map<std::wstring, std::unique_ptr<SQLiteWrapper>> _sql_connections;
};

#endif