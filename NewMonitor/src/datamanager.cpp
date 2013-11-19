#include "stdafx.h"
#include "../include/datamanager.h"
#include "../include/sqlite3.h"

#include <exception>
#include <utility>

char DataManager::msg_buf[1024];

bool DataManager::OpenDataFile(std::wstring filename)
{
	try{
		_sql_connections.insert(std::make_pair(
			filename, std::unique_ptr<SQLiteWrapper>{ new SQLiteWrapper(filename) }));
	}
	catch (std::runtime_error){
		return false;
	}
	return _sql_connections[filename]->InitTableInfo();
}

void DataManager::CloseDataFile(std::wstring filename)
{
	_sql_connections.erase(filename);
}

void DataManager::EnableSQLErrorLog()
{
	sqlite3_config(SQLITE_CONFIG_LOG, errorLogCallback, nullptr);
}

void DataManager::errorLogCallback(void *pArg, int iErrCode, const char *zMsg)
{
	sprintf_s(msg_buf, "error_code:%d, description:%s", iErrCode, zMsg);
	OutputDebugStringA(msg_buf);
}