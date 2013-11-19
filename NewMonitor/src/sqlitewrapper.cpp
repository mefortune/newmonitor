#include "stdafx.h"

#include "../include/sqlitewrapper.h"
#include "../include/sql_template.h"
#include <stdexcept>
#pragma comment(lib, "sqlite3.lib")


SQLiteWrapper::SQLiteWrapper(std::wstring filename)
{
	Connect(filename);
}

void SQLiteWrapper::Connect(std::wstring filename)
{
	int rc;
	rc = sqlite3_open16(filename.c_str(), &_db);
	if (rc != SQLITE_OK){
		sqlite3_close(_db);
		throw std::runtime_error("can't open sqlite connection");
	}
	PrepareStmts();
}

void SQLiteWrapper::Disconnect()
{
	DestroyStmts();
	if (_db != nullptr){
		sqlite3_close(_db);
	}
}

bool SQLiteWrapper::InitTableInfo()
{
	bool exec_result = sqlite3_step(_sql_stmts[SQLCMD::INIT_TABLE_INFO]) == SQLITE_DONE;
	sqlite3_reset(_sql_stmts[SQLCMD::INIT_TABLE_INFO]);
	return exec_result;
}

auto SQLiteWrapper::EnumTableInfo()->TableInfo
{
	while (sqlite3_step(_sql_stmts[SQLCMD::ENUM_TABLE_INFO]) == SQLITE_ROW){
		int tbl_id = sqlite3_column_int(_sql_stmts[SQLCMD::ENUM_TABLE_INFO], 0);
		std::wstring tbl_displayname{ static_cast<const wchar_t *>(sqlite3_column_text16(_sql_stmts[SQLCMD::ENUM_TABLE_INFO], 2)) };
	}
	sqlite3_reset(_sql_stmts[SQLCMD::ENUM_TABLE_INFO]);
}


void SQLiteWrapper::PrepareStmts()
{
	sqlite3_prepare16_v2(_db, sql_create_tbl_info, sizeof(sql_create_tbl_info), &_sql_stmts[SQLCMD::INIT_TABLE_INFO], nullptr);
	sqlite3_prepare16_v2(_db, sql_enum_tbl_info, sizeof(sql_enum_tbl_info), &_sql_stmts[SQLCMD::ENUM_TABLE_INFO], nullptr);
}
void SQLiteWrapper::DestroyStmts()
{
	for (auto stmt_pair : _sql_stmts){
		sqlite3_finalize(stmt_pair.second);
	}
}