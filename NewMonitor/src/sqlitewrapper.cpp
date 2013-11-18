#include "stdafx.h"

#include "../include/sqlitewrapper.h"

#pragma comment(lib, "sqlite3.lib")
SQLiteWrapper::SQLiteWrapper(std::wstring filename) : _filename(filename)
{
	Connect();
}

void SQLiteWrapper::Connect()
{
	int rc;
	rc = sqlite3_open16(_filename.c_str(), &_db);
	if (rc != SQLITE_OK){
		sqlite3_close(_db);
		throw SQLiteError("can't open sqlite connection");
	}
}

void SQLiteWrapper::Disconnect()
{
	if (_db != nullptr){
		sqlite3_close(_db);
	}
}

bool SQLiteWrapper::Exec(const std::string sql, int(*callback)(void*, int, char**, char**), void *param, std::string& errmsg)
{
	char *zErrMsg = 0;
	int rc = sqlite3_exec(_db, sql.c_str(), callback, param, &zErrMsg);
	if (zErrMsg != nullptr){
		errmsg.insert(0, zErrMsg);
		sqlite3_free(zErrMsg);
	}
	if (rc != SQLITE_OK){
		return false;
	}
	return true;
}