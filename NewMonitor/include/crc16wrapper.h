#ifndef CRC16WRAPPER_H
#define CRC16WRAPPER_H


#include <vector>
#include <cstdint>
class CRC16Wrapper{
public:
	CRC16Wrapper(){ InitPolyTbl(); }
	uint16_t CalcCRC16(const std::vector<char> data);
	void GenerateCRCData(std::vector<char> &data);
	bool VerifyCRCData(const std::vector<char> data);
private:
	void InitPolyTbl();
private:
	static const uint16_t POLY = 0xA001;
	uint16_t _poly_tbl[256];
};

#endif