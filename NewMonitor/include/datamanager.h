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

	static DataManager* GetInstance()
	{
		static DataManager _data_manager;
		return &_data_manager;
	}

	void AddDataTable(std::wstring filename, const std::wstring table_name, const std::wstring description);
	void RenameDataTable(const std::string table_name, const std::string new_name);
private:
	void EstablishTables(std::wstring);
	void LogEventSql(const std::string info, const std::string description);

	void EnumTable(const std::wstring filename);
	static int OnEnumTable(void*, int, char**, char**);
private:
	std::map<std::wstring, std::unique_ptr<SQLiteWrapper>> _sql_connections;

	static const std::string sql_create_tbl_info;
	static const std::string sql_template_addnew;
	static const std::string sql_enum_tbl_info;
};

#endif