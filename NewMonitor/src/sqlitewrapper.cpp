#include "stdafx.h"

#include "../include/sqlitewrapper.h"
#include "../include/sql_template.h"
#include <cstdint>
#include <stdexcept>
#pragma comment(lib, "sqlite3.lib")


SQLiteWrapper::SQLiteWrapper(const std::wstring& filename)
{
	Connect(filename);
}

void SQLiteWrapper::Connect(const std::wstring& filename)
{
	int rc;
	rc = sqlite3_open16(filename.c_str(), &_db);
	if (rc != SQLITE_OK){
		sqlite3_close(_db);
		throw std::runtime_error("can't open sqlite connection");
	}
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
	sqlite3_prepare16_v2(_db, sql_create_tbl_info, sizeof(sql_create_tbl_info), &_sql_stmts[SQLCMD::INIT_TABLE_INFO], nullptr);

	bool exec_result = sqlite3_step(_sql_stmts[SQLCMD::INIT_TABLE_INFO]) == SQLITE_DONE;
	sqlite3_reset(_sql_stmts[SQLCMD::INIT_TABLE_INFO]);

	sqlite3_prepare16_v2(_db, sql_enum_tbl_info, sizeof(sql_enum_tbl_info), &_sql_stmts[SQLCMD::ENUM_TABLE_INFO], nullptr);
	sqlite3_prepare16_v2(_db, sql_template_addnew, sizeof(sql_template_addnew), &_sql_stmts[SQLCMD::ADD_TABLE_INFO], nullptr);
	sqlite3_prepare16_v2(_db, sql_alter_tbl_info, sizeof(sql_alter_tbl_info), &_sql_stmts[SQLCMD::ALTER_TABLE_INFO], nullptr);
	sqlite3_prepare16_v2(_db, sql_delete_tbl_info, sizeof(sql_delete_tbl_info), &_sql_stmts[SQLCMD::DELETE_TABLE_INFO], nullptr);
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

auto SQLiteWrapper::EnumDataInfo(const std::wstring& table_name)->DataInfo
{
	// prepare statements to enum data of that data_tbl;
	if (_sql_data_enum_stmts.find(table_name) == _sql_data_enum_stmts.end()){
		wchar_t sql_temp[300];
		swprintf_s(sql_temp, sql_enum_tbl_data, table_name.c_str(), table_name.c_str(), table_name.c_str(), table_name.c_str(), table_name.c_str());
		sqlite3_prepare16_v2(_db, sql_temp, sizeof(sql_temp), &_sql_data_enum_stmts[table_name], nullptr);
	}

	DataInfo data_info;
	while (sqlite3_step(_sql_data_enum_stmts[table_name]) == SQLITE_ROW){
		int data_id{ sqlite3_column_int(_sql_data_enum_stmts[table_name], 0) };
		int seg_id{ sqlite3_column_int(_sql_data_enum_stmts[table_name], 1) };
		int64_t	data_time{ sqlite3_column_int64(_sql_data_enum_stmts[table_name], 2) };
		const char *pdata = static_cast<const char *>(sqlite3_column_blob(_sql_data_enum_stmts[table_name], 3));
		size_t	data_sz = sqlite3_column_bytes(_sql_data_enum_stmts[table_name], 3);

#pragma warning(push)
#pragma warning(disable:4996)
		std::tm *ptime_tm = std::localtime(&data_time);
#pragma warning(pop)
		data_info[data_id] = std::make_tuple(seg_id, std::vector<char>{pdata, pdata + data_sz}, *ptime_tm);
	}
	sqlite3_reset(_sql_data_enum_stmts[table_name]);

	return data_info;
}
bool SQLiteWrapper::CreateTableData(const std::wstring& display_name, const std::wstring& description)
{
	std::time_t cur_time = std::time(nullptr);
#pragma warning(push)
#pragma warning(disable:4996)
	std::tm *ptime_tm = std::localtime(&cur_time);
#pragma warning(pop)
	wchar_t	tablename[50];
	swprintf_s(tablename, L"TableData_%d%d%d", ptime_tm->tm_hour, ptime_tm->tm_min, ptime_tm->tm_sec);

	sqlite3_bind_text16(_sql_stmts[SQLCMD::ADD_TABLE_INFO], 1, tablename, -1, nullptr);
	sqlite3_bind_text16(_sql_stmts[SQLCMD::ADD_TABLE_INFO], 2, display_name.c_str(),-1, nullptr);
	sqlite3_bind_int64(_sql_stmts[SQLCMD::ADD_TABLE_INFO], 3, cur_time);
	sqlite3_bind_text16(_sql_stmts[SQLCMD::ADD_TABLE_INFO], 4, description.c_str(), -1, nullptr);

	bool exec_result = sqlite3_step(_sql_stmts[SQLCMD::ADD_TABLE_INFO]) == SQLITE_DONE;
	sqlite3_reset(_sql_stmts[SQLCMD::ADD_TABLE_INFO]);

	// create the real data tbl;
	wchar_t sql_temp[300];
	swprintf_s(sql_temp, sql_create_tbl_data, tablename);
	sqlite3_stmt* create_tbl_stmt;
	sqlite3_prepare16_v2(_db, sql_temp, sizeof(sql_temp), &create_tbl_stmt, nullptr);
	sqlite3_step(create_tbl_stmt);
	sqlite3_finalize(create_tbl_stmt);

	return exec_result;
}

bool SQLiteWrapper::DeleteTableData(int table_id, const std::wstring& table_name)
{
	sqlite3_bind_int64(_sql_stmts[SQLCMD::DELETE_TABLE_INFO], 1, table_id);
	sqlite3_bind_text16(_sql_stmts[SQLCMD::DELETE_TABLE_INFO], 2, table_name.c_str(), -1, nullptr);
	bool exec_result = sqlite3_step(_sql_stmts[SQLCMD::DELETE_TABLE_INFO]) == SQLITE_DONE;
	sqlite3_reset(_sql_stmts[SQLCMD::DELETE_TABLE_INFO]);

	//delete the real data tbl;
	wchar_t sql_temp[300];
	swprintf_s(sql_temp, sql_delete_tbl_data, table_name.c_str());
	sqlite3_stmt* delete_tbl_stmt;
	sqlite3_prepare16_v2(_db, sql_temp, sizeof(sql_temp), &delete_tbl_stmt, nullptr);
	sqlite3_step(delete_tbl_stmt);
	sqlite3_finalize(delete_tbl_stmt);

	// delete statement to insert data into that data_tbl;
	_sql_data_insert_stmts.erase(table_name);
	_sql_data_enum_stmts.erase(table_name);

	return exec_result;
}

bool SQLiteWrapper::AlterTableData(int table_id, const std::wstring& display_name, const std::wstring& description)
{
	sqlite3_bind_text16(_sql_stmts[SQLCMD::ALTER_TABLE_INFO], 1, display_name.c_str(), -1, nullptr);
	sqlite3_bind_text16(_sql_stmts[SQLCMD::ALTER_TABLE_INFO], 2, description.c_str(), -1, nullptr);
	sqlite3_bind_int64(_sql_stmts[SQLCMD::ALTER_TABLE_INFO], 3, table_id);
	bool exec_result = sqlite3_step(_sql_stmts[SQLCMD::ALTER_TABLE_INFO]) == SQLITE_DONE;
	sqlite3_reset(_sql_stmts[SQLCMD::ALTER_TABLE_INFO]);

	return exec_result;
}

bool SQLiteWrapper::InsertData(const std::wstring& table_name, int seg_id, std::tm data_time, const std::vector<char>& data)
{
	// prepare statements to insert data into that data_tbl;
	if (_sql_data_insert_stmts.find(table_name) == _sql_data_insert_stmts.end()){
		wchar_t sql_temp[300];
		swprintf_s(sql_temp, sql_insert_tbl_data, table_name.c_str());
		sqlite3_prepare16_v2(_db, sql_temp, sizeof(sql_temp), &_sql_data_insert_stmts[table_name], nullptr);
	}
	sqlite3_bind_int(_sql_data_insert_stmts[table_name], 1, seg_id);
	sqlite3_bind_int64(_sql_data_insert_stmts[table_name], 2, std::mktime(&data_time));
	sqlite3_bind_blob(_sql_data_insert_stmts[table_name], 3, data.data(), data.size(), nullptr);
	bool exec_result = sqlite3_step(_sql_data_insert_stmts[table_name]) == SQLITE_DONE;
	sqlite3_reset(_sql_data_insert_stmts[table_name]);

	return exec_result;
}

void SQLiteWrapper::DestroyStmts()
{
	for (auto stmt_pair : _sql_stmts){
		sqlite3_finalize(stmt_pair.second);
	}

	for (auto stmt_pair : _sql_data_insert_stmts){
		sqlite3_finalize(stmt_pair.second);
	}

	for (auto stmt_pair : _sql_data_enum_stmts){
		sqlite3_finalize(stmt_pair.second);
	}
}