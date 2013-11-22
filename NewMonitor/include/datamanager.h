#ifndef DATAMANAGER_H
#define DATAMANAGER_H

#include "sqlitewrapper.h"
#include <map>
#include <memory>
#include <string>

class DataManager{
public:
	bool OpenDataFile(const std::wstring&);
	void CloseDataFile(const std::wstring&);

	void CreateDataTable(const std::wstring& filename, const std::wstring& display_name, const std::wstring& description);
	void AlterDataTable(const std::wstring& filename, int table_id, const std::wstring& display_name, const std::wstring& description);
	void DeleteDataTable(const std::wstring& filename, int table_id, const std::wstring& table_name);

	void InsertData(const std::wstring& filename, const std::wstring& table_name, int seg_id, std::tm data_time, const std::vector<char>& data);

	void ParseData();
	static DataManager* GetInstance()
	{
		static DataManager _data_manager;
		return &_data_manager;
	}

	static void EnableSQLErrorLog();
private:
	void ParseDataFile(const std::wstring& filename);
	
	static void errorLogCallback(void *pArg, int iErrCode, const char *zMsg);
private:
	std::map<std::wstring, std::unique_ptr<SQLiteWrapper>> _sql_connections;
	static char msg_buf[1024];
};

#endif