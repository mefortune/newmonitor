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
	bool open_result = _sql_connections[filename]->InitTableInfo();
	ParseDataFile(filename);

	return open_result;
}

void DataManager::CloseDataFile(std::wstring filename)
{
	_sql_connections.erase(filename);
}

void DataManager::EnableSQLErrorLog()
{
	sqlite3_config(SQLITE_CONFIG_LOG, errorLogCallback, nullptr);
}

void DataManager::ParseDataFile(std::wstring filename)
{
	SQLiteWrapper::TableInfo table_info = _sql_connections[filename]->EnumTableInfo();

	std::wstring a;
	for (auto item : table_info){
		a = a + std::get<0>(item.second) + std::get<1>(item.second) + std::get<2>(item.second) + L"\n";
	}
	OutputDebugString(a.c_str());
}
void DataManager::errorLogCallback(void *pArg, int iErrCode, const char *zMsg)
{
	sprintf_s(msg_buf, "error_code:%d, description:%s", iErrCode, zMsg);
	OutputDebugStringA(msg_buf);
}