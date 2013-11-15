#include <memory>
#include <string>
#include <vector>

class CallbackAsyncSerial;
class CRC16Wrapper;

#define	Event_Alert	0x0000000001L
#define Event_Log	0x0000000010L

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

	static void LogEvent(const char *event_name, const int value, const char *event_description, unsigned int mask);
	bool SyncTime();
	
private:
	void ReadDataCallback(const char*, size_t);
	// pick commands from _data_vec, returns empty if no command available
	std::vector<char> PickCommand();
	void HandleCommand(std::vector<char>& cmd);

private:
	std::shared_ptr<CallbackAsyncSerial> _serial_wrapper;
	std::shared_ptr<CRC16Wrapper>	_crc_wrapper;
	std::vector<char> _data_vec;


	static const unsigned int baud_rate		= 19200;

	static const char data_head = '\xFF';
	static const char data_tail = '\x0D';

	// frame bein flag
	static const char time_frame_flag = '\x24';
	static const char data_frame_flag = '\x40';
	// cmd flag
	static const char time_cmd_flag = '\xC0';
	static const char data_start_cmd_flag = '\xC1';
	static const char data_length_cmd_flag = '\xC2';
	static const char data_trans_cmd_flag = '\xC3';
	static const char data_end_cmd_flag = '\xC4';

};