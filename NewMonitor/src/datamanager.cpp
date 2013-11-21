#include "stdafx.h"
#include "../include/datamanager.h"
#include "../include/sqlite3.h"
#include "../NewMonitorDoc.h"
#include "../resource.h"

#include <exception>
#include <utility>

char DataManager::msg_buf[1024];

bool DataManager::OpenDataFile(std::wstring filename)
{
	try{
		_sql_connections.insert(std::make_pair(
			filename, std::unique_ptr<SQLiteWrapper>{ new SQLiteWrapper(filename) }));
	}
	catch (std::runtime_error){
		return false;
	}
	bool open_result = _sql_connections[filename]->InitTableInfo();

	ParseDataFile(filename);

	CFrameWnd * pFrame = (CFrameWnd *)(AfxGetApp()->m_pMainWnd);
	pFrame->SendMessage(WM_COMMAND, MAKELONG(IDU_FRESHEXPLORER, 0), NULL);
	return open_result;
}

void DataManager::CloseDataFile(std::wstring filename)
{
	_sql_connections.erase(filename);
	CNewMonitorDoc *pDoc = CNewMonitorDoc::GetDoc();
	pDoc->_explore_status._file_map.erase(filename);

	CFrameWnd * pFrame = (CFrameWnd *)(AfxGetApp()->m_pMainWnd);
	pFrame->SendMessage(WM_COMMAND, MAKELONG(IDU_FRESHEXPLORER, 0), NULL);
}

void DataManager::CreateDataTable(std::wstring filename, std::wstring display_name, std::wstring description)
{
	_sql_connections[filename]->CreateTableData(display_name, description);
	ParseDataFile(filename);

	CFrameWnd * pFrame = (CFrameWnd *)(AfxGetApp()->m_pMainWnd);
	pFrame->SendMessage(WM_COMMAND, MAKELONG(IDU_FRESHEXPLORER, 0), NULL);
}
void DataManager::ParseDataFile(std::wstring filename)
{
	SQLiteWrapper::TableInfo table_info = _sql_connections[filename]->EnumTableInfo();
	CNewMonitorDoc *pDoc = CNewMonitorDoc::GetDoc();

	pDoc->_explore_status._file_map[filename] = std::map<int, std::tuple<std::wstring/*displayname*/, std::wstring/*description*/>>{};
	for (auto item : table_info){
		pDoc->_explore_status._file_map[filename][item.first] = std::tuple<std::wstring, std::wstring>{std::get<1>(item.second), std::get<2>(item.second)};
	}
}

void DataManager::EnableSQLErrorLog()
{
	sqlite3_config(SQLITE_CONFIG_LOG, errorLogCallback, nullptr);
}

void DataManager::errorLogCallback(void *pArg, int iErrCode, const char *zMsg)
{
	sprintf_s(msg_buf, "error_code:%d, description:%s\n", iErrCode, zMsg);
	OutputDebugStringA(msg_buf);
}