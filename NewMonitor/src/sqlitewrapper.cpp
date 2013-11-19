#include "stdafx.h"

#include "../include/sqlitewrapper.h"
#include "../include/sql_template.h"
#include <cstdint>
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
	TableInfo table_info;

	while (sqlite3_step(_sql_stmts[SQLCMD::ENUM_TABLE_INFO]) == SQLITE_ROW){
		int tbl_id{ sqlite3_column_int(_sql_stmts[SQLCMD::ENUM_TABLE_INFO], 0) };
		std::wstring tbl_tablename{ static_cast<const wchar_t *>(sqlite3_column_text16(_sql_stmts[SQLCMD::ENUM_TABLE_INFO], 1)) };
		std::wstring tbl_displayname{ static_cast<const wchar_t *>(sqlite3_column_text16(_sql_stmts[SQLCMD::ENUM_TABLE_INFO], 2)) };
		int64_t	tbl_create_time{ sqlite3_column_int64(_sql_stmts[SQLCMD::ENUM_TABLE_INFO], 3) };
		std::wstring tbl_description{ static_cast<const wchar_t *>(sqlite3_column_text16(_sql_stmts[SQLCMD::ENUM_TABLE_INFO], 4)) };

#pragma warning(push)
#pragma warning(disable:4996)
		std::tm *ptime_tm = std::localtime(&tbl_create_time);
#pragma warning(pop)
		table_info[tbl_id] = std::make_tuple(tbl_tablename, tbl_displayname, tbl_description, *ptime_tm);
	}
	sqlite3_reset(_sql_stmts[SQLCMD::ENUM_TABLE_INFO]);

	return table_info;
}

bool CreateTableData(std::wstring display_name, std::wstring description)
{
	std::time_t cur_time = std::time(nullptr);
#pragma warning(push)
#pragma warning(disable:4996)
	std::tm *ptime_tm = std::localtime(&cur_time);
#pragma warning(pop)


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