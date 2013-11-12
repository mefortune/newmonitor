#include "stdafx.h"
#include "../include/datamanager.h"

#include <utility>
bool DataManager::OpenDataFile(std::wstring filename)
{
	try{
		_sql_connections.insert(std::make_pair(
			filename, std::unique_ptr<SQLiteWrapper>{ new SQLiteWrapper(filename) }));
	}
	catch (SQLiteWrapper::SQLiteError){
		return false;
	}
	return true;
}

void DataManager::CloseDataFile(std::wstring filename)
{
	_sql_connections.erase(filename);
}