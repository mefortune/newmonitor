#ifndef CRC16WRAPPER_H
#define CRC16WRAPPER_H


#include <string>
#include <cstdint>
class CRC16Wrapper{
public:
	CRC16Wrapper(){ InitPolyTbl(); }
	uint16_t CalcCRC16(std::string str);

	std::string GenerateCRCData(std::string str);
private:
	void InitPolyTbl();
private:
	static const uint16_t POLY = 0xA001;
	uint16_t _poly_tbl[256];
};

#endif