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

	static void EnableSQLErrorLog();
private:
	void ParseDataFile(std::wstring);

	static void errorLogCallback(void *pArg, int iErrCode, const char *zMsg);

private:
	std::map<std::wstring, std::unique_ptr<SQLiteWrapper>> _sql_connections;
	static char msg_buf[1024];
};

#endif