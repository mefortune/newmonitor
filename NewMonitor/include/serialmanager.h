#include <memory>
#include <string>

class CallbackAsyncSerial;
class CRC16Wrapper;

class SerialManager{
public:
	SerialManager();
	SerialManager(std::string &devname);
	static SerialManager* GetInstance()
	{
		static SerialManager _serial_manager;
		return &_serial_manager;
	}

	bool SyncTime();
private:
	std::unique_ptr<CallbackAsyncSerial> _serial_wrapper;
	std::unique_ptr<CRC16Wrapper>	_crc_wrapper;
	static const unsigned int baud_rate = 19200;
};