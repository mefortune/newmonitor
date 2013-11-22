#ifndef SQLITEWRAPPER_H
#define SQLITEWRAPPER_H

#include "../include/sqlite3.h"
#include <string>
#include <tuple>
#include <map>
#include <vector>
#include <ctime>

class SQLiteWrapper{
public:
	explicit SQLiteWrapper(const std::wstring& filename);
	~SQLiteWrapper(){ Disconnect(); }

	SQLiteWrapper(const SQLiteWrapper&) = delete;
	SQLiteWrapper& operator=(const SQLiteWrapper&) = delete;

	// should be invoked before any other functions;
	
	bool InitTableInfo();

	using TableInfo = std::map <int/*table_id*/, std::tuple<std::wstring/*table_name*/, std::wstring/*display_name*/, std::wstring/*description*/, std::tm/*create_time*/>>;
	TableInfo EnumTableInfo();
	

	bool CreateTableData(const std::wstring& display_name, const std::wstring& description);
	bool AlterTableData(int table_id, const std::wstring& display_name, const std::wstring& description);
	bool DeleteTableData(int table_id, const std::wstring& table_name);

	bool InsertData(const std::wstring& table_name, int seg_id, std::tm data_time, const std::vector<char>& data);
	using DataInfo = std::map<int/*data_id*/, std::tuple<int/*seg_id*/, std::vector<char>/*data*/, std::tm/*data_time*/>>;
	DataInfo EnumDataInfo(const std::wstring& table_name);
private:
	void Connect(const std::wstring& filename);
	void Disconnect();

	enum class SQLCMD{
		INIT_TABLE_INFO,
		ENUM_TABLE_INFO,
		ADD_TABLE_INFO,
		ALTER_TABLE_INFO,
		DELETE_TABLE_INFO,
	};
	void DestroyStmts();
	
private:
	sqlite3     *_db{ nullptr };
	std::map<SQLCMD, sqlite3_stmt*> _sql_stmts;
	std::map<std::wstring/*tbl_name*/, sqlite3_stmt*> _sql_data_insert_stmts;
	std::map<std::wstring/*tbl_name*/, sqlite3_stmt*> _sql_data_enum_stmts;
};

#endif