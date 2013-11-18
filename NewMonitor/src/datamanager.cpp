#include "stdafx.h"
#include "../include/datamanager.h"

#include <utility>
#include <ctime>
#include <locale>

const std::string DataManager::sql_create_tbl_info = R"sql(
CREATE TABLE IF NOT EXISTS "main"."TableInfo" (
"id"  INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL,
"table_name"  TEXT(100) NOT NULL,
"display_name"  TEXT(100) NOT NULL,
"create_time"  INTEGER NOT NULL,
"description"  TEXT(100)
)
)sql";

const std::string DataManager::sql_template_addnew = R"sql(
INSERT INTO "main"."TableInfo"
(table_name, display_name, create_time, description)
VALUES
(%s, %s, %I64d, %s)
)sql";

const std::string DataManager::sql_enum_tbl_info = R"sql(
SELECT
TableInfo.id,
TableInfo.table_name,
TableInfo.display_name,
TableInfo.create_time,
TableInfo.description
FROM
TableInfo
)sql";
bool DataManager::OpenDataFile(std::wstring filename)
{
	try{
		_sql_connections.insert(std::make_pair(
			filename, std::unique_ptr<SQLiteWrapper>{ new SQLiteWrapper(filename) }));
	}
	catch (SQLiteWrapper::SQLiteError){
		return false;
	}
	EstablishTables(filename);
	

	return true;
}

void DataManager::CloseDataFile(std::wstring filename)
{
	_sql_connections.erase(filename);
}

void DataManager::EstablishTables(std::wstring filename)
{
	std::string errmsg{};
	_sql_connections[filename]->Exec(sql_create_tbl_info, nullptr, nullptr, errmsg);
	if (errmsg.size() != 0){
		LogEventSql("创建INFO表失败", errmsg);
	}
}

void DataManager::AddDataTable(std::wstring filename, const std::wstring table_name, const std::wstring description)
{
	char sql_buf[256];
	std::time_t cur_time = std::time(nullptr);
	sprintf_s(sql_buf, sql_template_addnew.c_str(), table_name.c_str(), table_name.c_str(), cur_time, description.c_str());
http://stackoverflow.com/questions/4804298/how-to-convert-wstring-into-string;
	std::string errmsg{};
	_sql_connections[filename]->Exec(std::string{ sql_buf }, nullptr, nullptr, errmsg);
	if (errmsg.size() != 0){
		LogEventSql("新建数据表失败", errmsg);
	}
}
void DataManager::LogEventSql(const std::string info, const std::string description)
{
	std::string temp{ "error:" };
	temp = temp + info + "," + "description:" + description;
	OutputDebugStringA(temp.c_str());
}

void DataManager::EnumTable(const std::wstring filename)
{
	std::string errmsg{};
	_sql_connections[filename]->Exec(sql_enum_tbl_info, &OnEnumTable, nullptr, errmsg);
}
int DataManager::OnEnumTable(void*, int, char**, char**)
{

}