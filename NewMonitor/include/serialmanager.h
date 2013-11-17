#include <memory>
#include <string>
#include <vector>
#include <map>
#include <cstddef>
#include <ctime>

class CallbackAsyncSerial;
class CRC16Wrapper;

#define	Event_Alert	0x0000000001L
#define Event_Log	0x0000000010L

// Event_Codes
#define Event_Data_Error       1
#define Event_CMD0_Error       2
#define Event_CMD1_Error       3
#define Event_CMD2_Error       4
#define Event_CMD3_Error       5
#define Event_CMD4_Error       6
class SerialManager{
public:
	SerialManager();

	void open(const std::string &devname);
	void close();

	bool is_open();
	static SerialManager* GetInstance()
	{
		static SerialManager _serial_manager;
		return &_serial_manager;
	}

	bool SyncTime();
	
private:
	void ReadDataCallback(const char*, size_t);
	// pick commands from _data_vec, returns empty if no command available
	std::vector<char> PickCommand();
	void HandleCommand(std::vector<char>& cmd);
private:
	std::shared_ptr<CallbackAsyncSerial>	_serial_wrapper;
	std::shared_ptr<CRC16Wrapper>			_crc_wrapper;
	std::vector<char>						_data_vec;

	// data structure to indicate a transmition
	struct DATA_TRANSMISSION{
		unsigned short	_data_no{ 0 };
		std::tm	_data_time;
		size_t	_data_count{ 0 };			// word
		std::map<size_t, std::pair<size_t, std::vector<char>>> _data_transfered;
	}_data_transmission;
	

	static const unsigned int baud_rate		= 19200;

	static const char data_head				= '\xFF';
	static const char data_tail				= '\x0D';

	// frame bein flag
	static const char time_frame_flag		= '\x24';
	static const char data_frame_flag		= '\x40';
	// cmd flag
	static const char time_cmd_flag			= '\xC0';
	static const char data_start_cmd_flag	= '\xC1';
	static const char data_length_cmd_flag	= '\xC2';
	static const char data_trans_cmd_flag	= '\xC3';
	static const char data_end_cmd_flag		= '\xC4';
};

void LogEvent(const char *event_name, const int value, const char *event_description, unsigned int mask);
std::string HexToString(const std::vector<char> hex);