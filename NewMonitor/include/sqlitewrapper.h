#ifndef SQLITEWRAPPER_H
#define SQLITEWRAPPER_H

#include "../include/sqlite3.h"
#include <string>
#include <stdexcept>

class SQLiteWrapper{
public:
	explicit SQLiteWrapper(std::wstring filename);
	~SQLiteWrapper(){ Disconnect(); }
	SQLiteWrapper(const SQLiteWrapper&) = delete;
	SQLiteWrapper& operator=(const SQLiteWrapper&) = delete;

	bool Exec(std::string sql, int(*callback)(void*, int, char**, char**), void *param, std::string& errmsg);

	class SQLiteError : public std::runtime_error{
	public:
		SQLiteError(const char * msg) : runtime_error(msg){}
	};
private:
	void Connect();
	void Disconnect();

private:
	std::wstring _filename;
	sqlite3     *_db{ nullptr };
};

#endif