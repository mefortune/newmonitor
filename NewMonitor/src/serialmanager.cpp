#include "stdafx.h"
#include "../include/serialmanager.h"

#include "../include/AsyncSerial.h"
#include "../include/crc16wrapper.h"

SerialManager::SerialManager() :
_serial_wrapper{ new CallbackAsyncSerial{} }, _crc_wrapper{ new CRC16Wrapper{} }
{

}

SerialManager::SerialManager(std::string &devname) :
_serial_wrapper{ new CallbackAsyncSerial{ devname, baud_rate } }, _crc_wrapper{ new CRC16Wrapper{} }
{

}