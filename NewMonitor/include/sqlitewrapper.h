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
	explicit SQLiteWrapper(std::wstring filename);
	~SQLiteWrapper(){ Disconnect(); }

	SQLiteWrapper(const SQLiteWrapper&) = delete;
	SQLiteWrapper& operator=(const SQLiteWrapper&) = delete;

	// should be invoked before any other functions;
	
	bool InitTableInfo();

	using TableInfo = std::map <int/*table_id*/, std::tuple<std::wstring/*table_name*/, std::wstring/*display_name*/, std::wstring/*description*/, std::tm/*create_time*/>>;
	TableInfo EnumTableInfo();
	using DataInfo = std::map<int/*table_id*/, std::vector<std::tuple<int/*data_id*/, int/*seg_id*/, std::tm/*data_time*/, std::vector<char>/*data*/>>>;
	DataInfo EnumDataInfo(int table_id);

	bool CreateTableData(std::wstring display_name, std::wstring description);
private:
	void Connect(std::wstring filename);
	void Disconnect();

	enum class SQLCMD{
		INIT_TABLE_INFO,
		ENUM_TABLE_INFO,
		ADD_TABLE_INFO,
	};
	void DestroyStmts();
	
private:
	sqlite3     *_db{ nullptr };
	std::map<SQLCMD, sqlite3_stmt*> _sql_stmts;
	std::map<std::wstring/*tbl_name*/, sqlite3_stmt*> _sql_data_stmts;
};

#endif