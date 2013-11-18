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
	if (_data_vec.size() < 7){
		return std::vector<char>{};
	}
	char cmd_type = _data_vec[2];
	size_t cmd_size = 0;
	switch (cmd_type){
	case data_start_cmd_flag:
		cmd_size = 13;
		break;
	case data_length_cmd_flag:
		cmd_size = 9;
		break;
	case data_trans_cmd_flag:
		{
			size_t data_count = static_cast<size_t>(static_cast<unsigned char>(_data_vec[6]));
			cmd_size = 10 + data_count * 2;
		}
		break;
	case data_end_cmd_flag:
		cmd_size = 7;
		break;
	}
	if (cmd_size == 0){
		LogEvent("命令提取错误", Event_Data_Error, HexToString(_data_vec).c_str(), Event_Log);
		_data_vec.erase(_data_vec.begin()); // should not happen, so remove the 0xFF;
		return std::vector<char>{};
	}
	if (_data_vec.size() < cmd_size){
		LogEvent("命令提取错误", Event_Data_Error, HexToString(_data_vec).c_str(), Event_Log);
		return std::vector<char>{};
	}
	if (_data_vec[cmd_size - 1] != data_tail){
		LogEvent("命令提取错误", Event_Data_Error, HexToString(_data_vec).c_str(), Event_Log);
		_data_vec.erase(_data_vec.begin()); // should not happen, so remove the 0xFF;
		return std::vector<char>{};
	}

	std::vector<char> temp{ _data_vec.begin(), _data_vec.begin() + cmd_size };
	_data_vec.erase(_data_vec.begin(), _data_vec.begin() + cmd_size);
	return temp;
}

void SerialManager::HandleCommand(std::vector<char>& cmd)
{
	std::vector<char> cmd_data{ cmd.begin() + 1, cmd.end() - 1 };
	if (!_crc_wrapper->VerifyCRCData(cmd_data)){
		LogEvent("数据校验错误", Event_Data_Error, HexToString(cmd).c_str(), Event_Log);
		return;
	}

	switch (cmd_data[1]){
	case data_start_cmd_flag:
		{
			_data_transmission._data_no = static_cast<unsigned short>(static_cast<unsigned char>(cmd_data[2]));
			_data_transmission._data_time.tm_year = static_cast<int>(static_cast<unsigned char>(cmd_data[3])) + 100;
			_data_transmission._data_time.tm_mon = static_cast<int>(static_cast<unsigned char>(cmd_data[4])) - 1;
			_data_transmission._data_time.tm_mday = static_cast<int>(static_cast<unsigned char>(cmd_data[5]));
			_data_transmission._data_time.tm_hour = static_cast<int>(static_cast<unsigned char>(cmd_data[6]));
			_data_transmission._data_time.tm_min = static_cast<int>(static_cast<unsigned char>(cmd_data[7]));
			_data_transmission._data_time.tm_sec = static_cast<int>(static_cast<unsigned char>(cmd_data[8]));

			_data_transmission._data_count = 0;
			_data_transmission._data_transfered.clear();
		}
		break;
	case data_length_cmd_flag:
		{
			if (cmd_data[2] != _data_transmission._data_no){
				LogEvent("数据段索引不匹配", Event_CMD2_Error, HexToString(cmd_data).c_str(), Event_Log);
				return;
			}
			_data_transmission._data_count = static_cast<unsigned short>(static_cast<unsigned char>(cmd_data[3]))
				| static_cast<unsigned short>(static_cast<unsigned char>(cmd_data[4])) << 8;
		}
		break;
	case data_trans_cmd_flag:
		{
			if (cmd_data[2] != _data_transmission._data_no){
				LogEvent("数据段索引不匹配", Event_CMD3_Error, HexToString(cmd_data).c_str(), Event_Log);
				return;
			}
			size_t data_index = static_cast<unsigned short>(static_cast<unsigned char>(cmd_data[3]))
				| static_cast<unsigned short>(static_cast<unsigned char>(cmd_data[4])) << 8;
			size_t data_count = static_cast<size_t>(static_cast<unsigned char>(cmd_data[5]));

			_data_transmission._data_transfered.insert(std::make_pair(data_index, std::make_pair(data_count, std::vector<char>(cmd_data.begin() + 6, cmd_data.end() - 2))));
		}
		break;
	case data_end_cmd_flag:
		{
			if (cmd_data[2] != _data_transmission._data_no){
				LogEvent("数据段索引不匹配", Event_CMD4_Error, HexToString(cmd_data).c_str(), Event_Log);
				return;
			}
			HandleCompleteData();
		}
		break;
	}
}

void SerialManager::HandleCompleteData()
{
	size_t data_count = 0;
	std::vector<char> data;
	for (auto data_pair : _data_transmission._data_transfered){
		data_count += data_pair.second.first;
		data.insert(data.end(), data_pair.second.second.begin(), data_pair.second.second.end());
	}

	// verify if data_count match
	char msg_buf[256];
	if (data_count != _data_transmission._data_count){
		sprintf_s(msg_buf, "段起始声明了%lu字数据,实际收到%lu数据", _data_transmission._data_count, data_count);
		LogEvent("数据段长度不匹配", Event_CMD_Varify_Error, msg_buf, Event_Log);
		return;
	}
	if (data.size() != data_count * 2){
		sprintf_s(msg_buf, "段起始声明了%lu字数据,实际收到%lu数据", _data_transmission._data_count, data.size() / 2);
		LogEvent("数据段长度不匹配", Event_CMD_Varify_Error, msg_buf, Event_Log);
		return;
	}
	

	OutputDebugStringA(HexToString(data).c_str());
}

std::string HexToString(const std::vector<char> hex)
{
	std::string temp;
	for (auto c : hex){
		char low = static_cast<short>(static_cast<unsigned char>(c & 0x0F)) >= 10 ?
			'A' + static_cast<short>(static_cast<unsigned char>(c & 0x0F)) - 10 : '0' + static_cast<short>(static_cast<unsigned char>(c & 0x0F));
		char high = static_cast<short>(static_cast<unsigned char>(c & 0xF0) >> 4) >= 10 ?
			'A' + static_cast<short>(static_cast<unsigned char>(c & 0xF0) >> 4) - 10 : '0' + static_cast<short>(static_cast<unsigned char>(c & 0xF0) >> 4);

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