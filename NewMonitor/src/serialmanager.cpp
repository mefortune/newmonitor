#include "stdafx.h"
#include "../include/serialmanager.h"

#include "../include/AsyncSerial.h"
#include "../include/crc16wrapper.h"

#include <ctime>

SerialManager::SerialManager() :
_serial_wrapper{ new CallbackAsyncSerial{} }, _crc_wrapper{ new CRC16Wrapper{} }
{

}

void SerialManager::open(const std::string &devname)
{
	_serial_wrapper->open(devname, baud_rate);
}

void SerialManager::close()
{
	_serial_wrapper->close();
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
	sync_time_cmd.push_back(static_cast<char>(0));
	sync_time_cmd.push_back(static_cast<char>(0));
	//sync_time_cmd.push_back(static_cast<char>(pcur_time_tm->tm_min));
	//sync_time_cmd.push_back(static_cast<char>(pcur_time_tm->tm_sec));

	_crc_wrapper->GenerateCRCData(sync_time_cmd);
	sync_time_cmd.insert(sync_time_cmd.begin(), data_head);
	sync_time_cmd.push_back(data_tail);

	_serial_wrapper->write(sync_time_cmd);

	return _serial_wrapper->errorStatus();
}