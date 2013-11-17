#include "stdafx.h"
#include "../include/serialmanager.h"

#include "../include/AsyncSerial.h"
#include "../include/crc16wrapper.h"


#include <algorithm>

SerialManager::SerialManager() :
_serial_wrapper{ new CallbackAsyncSerial{} }, _crc_wrapper{ new CRC16Wrapper{} }
{

}

void SerialManager::open(const std::string &devname)
{
	_serial_wrapper->open(devname, baud_rate);
	_serial_wrapper->setCallback(boost::bind(&SerialManager::ReadDataCallback, this, _1, _2));
}

void SerialManager::close()
{
	_serial_wrapper->close();
}

bool SerialManager::is_open()
{
	return _serial_wrapper->isOpen();
}

bool SerialManager::SyncTime()
{
	std::time_t cur_time = std::time(nullptr);
	#pragma warning(push)
	#pragma warning(disable:4996)
	std::tm *   pcur_time_tm = std::localtime(&cur_time);
#pragma warning(pop)

	std::vector<char> sync_time_cmd{ time_frame_flag, time_cmd_flag };
	sync_time_cmd.push_back(static_cast<char>(pcur_time_tm->tm_year - 100));
	sync_time_cmd.push_back(static_cast<char>(pcur_time_tm->tm_mon + 1));
	sync_time_cmd.push_back(static_cast<char>(pcur_time_tm->tm_mday));
	sync_time_cmd.push_back(static_cast<char>(pcur_time_tm->tm_hour));
	sync_time_cmd.push_back(static_cast<char>(pcur_time_tm->tm_min));
	sync_time_cmd.push_back(static_cast<char>(pcur_time_tm->tm_sec));
	
	_crc_wrapper->GenerateCRCData(sync_time_cmd);
	sync_time_cmd.insert(sync_time_cmd.begin(), data_head);
	sync_time_cmd.push_back(data_tail);

	_serial_wrapper->write(sync_time_cmd);

	return _serial_wrapper->errorStatus();
}

void SerialManager::ReadDataCallback(const char* data, size_t sz)
{
	// insert incoming data into _data_vec;
	_data_vec.insert(_data_vec.end(), data, data + sz);

	std::vector<char> cmd = PickCommand();
	while (cmd.size() != 0){
		HandleCommand(cmd);
		cmd = PickCommand();
	}
}

std::vector<char> SerialManager::PickCommand()
{
	// the first data in _data_vec must be 0xFF
	std::vector<char>::iterator cmd_head = std::find(_data_vec.begin(), _data_vec.end(), data_head);
	_data_vec.erase(_data_vec.begin(), cmd_head);
	std::vector<char>::iterator cmd_tail = std::find(_data_vec.begin(), _data_vec.end(), data_tail);
	if (cmd_tail != _data_vec.end()){
		std::vector<char> temp{_data_vec.begin(), cmd_tail + 1};
		_data_vec.erase(_data_vec.begin(), cmd_tail + 1);
		return temp;
	}

	return std::vector<char>{};
}

void SerialManager::HandleCommand(std::vector<char>& cmd)
{
	if (cmd.size() < 4){
		LogEvent("数据长度错误", Event_Data_Error, HexToString(cmd).c_str(), Event_Log);
		return;
	}

	std::vector<char> cmd_data{ cmd.begin() + 1, cmd.end() - 1 };
	if (!_crc_wrapper->VerifyCRCData(cmd_data)){
		LogEvent("数据校验错误", Event_Data_Error, HexToString(cmd).c_str(), Event_Log);
		return;
	}

	switch (cmd_data[1]){
	case data_start_cmd_flag:
		if (cmd_data.size() != 11){
			LogEvent("命令长度错误", Event_CMD1_Error, HexToString(cmd_data).c_str(), Event_Log);
		}
		_data_transmission._data_no = cmd_data[2];
		_data_transmission._data_time.tm_year = static_cast<int>(static_cast<unsigned char>(cmd_data[3])) + 100;
		_data_transmission._data_time.tm_mon = static_cast<int>(static_cast<unsigned char>(cmd_data[4])) - 1;
		_data_transmission._data_time.tm_mday = static_cast<int>(static_cast<unsigned char>(cmd_data[5]));
		_data_transmission._data_time.tm_hour = static_cast<int>(static_cast<unsigned char>(cmd_data[6]));
		_data_transmission._data_time.tm_min = static_cast<int>(static_cast<unsigned char>(cmd_data[7]));
		_data_transmission._data_time.tm_sec = static_cast<int>(static_cast<unsigned char>(cmd_data[8]));
		break;
	case data_length_cmd_flag:
		if (cmd_data.size() != 7){
			LogEvent("命令长度错误", Event_CMD2_Error, HexToString(cmd_data).c_str(), Event_Log);
		}
		if (cmd_data[2] != _data_transmission._data_no){
			LogEvent("数据段索引不匹配", Event_CMD2_Error, HexToString(cmd_data).c_str(), Event_Log);
		}
		_data_transmission._data_count = (static_cast<unsigned short>(static_cast<unsigned char>(cmd_data[3]))) 
			| (static_cast<unsigned short>(static_cast<unsigned char>(cmd_data[4])) << 8);


		char msg[256];
		sprintf_s(msg, "len:%d",_data_transmission._data_count);
		OutputDebugStringA(msg);

		break;
	case data_trans_cmd_flag:
		break;
	case data_end_cmd_flag:
		break;
	}
}

std::string HexToString(const std::vector<char> hex)
{
	std::string temp;
	for (auto c : hex){
		char low = static_cast<short>(c & 0x0F) >= 10 ? 
			'A' + static_cast<short>(c & 0x0F) - 10 : '0' + static_cast<short>(c & 0x0F);
		char high = static_cast<short>((c & 0xF0) >> 4) >= 10 ?
			'A' + static_cast<short>((c & 0xF0) >> 4) - 10 : '0' + static_cast<short>((c & 0xF0) >> 4);

		temp += high;
		temp += low;
		temp += ' ';
	}
	return temp;
}
void LogEvent(const char *event_name, const int value, const char *event_description, unsigned int mask)
{
	char msg_buf[256];
	// Alert User About Event
	if (mask & Event_Alert){
		sprintf_s(msg_buf, "error_code:%d\nerror_description:%s", value, event_description);
		MessageBoxA(NULL, msg_buf, event_name, MB_OK);
	}

	if(mask & Event_Log){
		char msg_buf[256];
		sprintf_s(msg_buf, "%s--error_code:%d\nerror_description:%s", event_name, value, event_description);
		OutputDebugStringA(msg_buf);
	}
}