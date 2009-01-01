#include <memory>
#include <string>

class CallbackAsyncSerial;
class CRC16Wrapper;

class SerialManager{
public:
	SerialManager();

	void open(const std::string &devname);
	void close();

	static SerialManager* GetInstance()
	{
		static SerialManager _serial_manager;
		return &_serial_manager;
	}

	bool SyncTime();
private:
	std::shared_ptr<CallbackAsyncSerial> _serial_wrapper;
	std::shared_ptr<CRC16Wrapper>	_crc_wrapper;

	static const unsigned int baud_rate		= 19200;

	static const char data_head = '\xFF';
	static const char data_tail = '\x0D';

	// frame bein flag
	static const char time_frame_flag = '\x24';
	static const char data_frame_flag = '\x40';
	// cmd flag
	static const char time_cmd_flag = '\xC0';
};