#include "stdafx.h"
#include "../include/datamanager.h"
#include "../include/sqlite3.h"
#include "../NewMonitorDoc.h"
#include "../resource.h"

#include <exception>
#include <utility>

char DataManager::msg_buf[1024];

bool DataManager::OpenDataFile(const std::wstring& filename)
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

	return open_result;
}

void DataManager::CloseDataFile(const std::wstring& filename)
{
	_sql_connections.erase(filename);
	CNewMonitorDoc *pDoc = CNewMonitorDoc::GetDoc();
	pDoc->_explore_status._file_map.erase(filename);

	CFrameWnd * pFrame = (CFrameWnd *)(AfxGetApp()->m_pMainWnd);
	pFrame->SendMessage(WM_COMMAND, MAKELONG(IDU_FRESHEXPLORER, 0), NULL);
}

void DataManager::CreateDataTable(const std::wstring& filename, const std::wstring& display_name, const std::wstring& description)
{
	_sql_connections[filename]->CreateTableData(display_name, description);
	ParseDataFile(filename);
}

void DataManager::AlterDataTable(const std::wstring& filename, int table_id, const std::wstring& display_name, const std::wstring& description)
{
	_sql_connections[filename]->AlterTableData(table_id, display_name, description);
	ParseDataFile(filename);
}

void DataManager::DeleteDataTable(const std::wstring& filename, int table_id, const std::wstring& table_name)
{
	_sql_connections[filename]->DeleteTableData(table_id, table_name);
	ParseDataFile(filename);
}

void DataManager::InsertData(const std::wstring& filename, const std::wstring& table_name, int seg_id, std::tm data_time, const std::vector<char>& data)
{
	_sql_connections[filename]->InsertData(table_name, seg_id, data_time, data);

	ParseData();
}

void DataManager::ParseDataFile(const std::wstring& filename)
{
	SQLiteWrapper::TableInfo table_info = _sql_connections[filename]->EnumTableInfo();
	CNewMonitorDoc *pDoc = CNewMonitorDoc::GetDoc();

	pDoc->_explore_status._file_map[filename] = std::map<int, std::tuple<std::wstring/*displayname*/, std::wstring/*description*/, std::wstring/*table_name*/>>{};
	for (auto item : table_info){
		pDoc->_explore_status._file_map[filename][item.first] = std::tuple<std::wstring, std::wstring, std::wstring>{std::get<1>(item.second), std::get<2>(item.second), std::get<0>(item.second)};
	}

	CFrameWnd * pFrame = (CFrameWnd *)(AfxGetApp()->m_pMainWnd);
	pFrame->SendMessage(WM_COMMAND, MAKELONG(IDU_FRESHEXPLORER, 0), NULL);
}

void DataManager::ParseData()
{
	CNewMonitorDoc *pDoc = CNewMonitorDoc::GetDoc();
	if (pDoc->_explore_status._sel_status == 2){			//选中文件
		std::wstring filename = pDoc->_explore_status._sel_filename;
		std::wstring table_name = std::get<2>(pDoc->_explore_status._file_map[filename][pDoc->_explore_status._sel_tableid]);
		pDoc->_explore_status._data_info = _sql_connections[filename]->EnumDataInfo(table_name);
	}
	
	CFrameWnd * pFrame = (CFrameWnd *)(AfxGetApp()->m_pMainWnd);
	pFrame->SendMessage(WM_COMMAND, MAKELONG(IDU_FRESHDATA, 0), NULL);
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