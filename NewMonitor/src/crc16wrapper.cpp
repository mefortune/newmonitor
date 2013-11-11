#include "stdafx.h"
#include "../include/crc16wrapper.h"

void CRC16Wrapper::InitPolyTbl()
{
	for(uint16_t i = 0; i != 256; ++i){
		uint16_t reg = i << 8;
		_poly_tbl[i] = 0;

		for(int mask = 0; mask != 8; ++mask){
			if(reg & (0x8000 >> mask)){
				reg ^= POLY >> (mask + 1);
				_poly_tbl[i] ^= POLY << (7 - mask);
			}
		}
	}
}

uint16_t CRC16Wrapper::CalcCRC16(std::string str)
{
	uint16_t reg = 0;
	for(size_t i = 0; i != str.size() + 2; ++i){
		uint16_t crc_mask = _poly_tbl[reg >> 8];
		reg = reg << 8 | ( i < str.size() ? str[i] : 0);
		reg ^= crc_mask;
	}

	return reg;
}

std::string CRC16Wrapper::GenerateCRCData(std::string str)
{
	uint16_t crc = CalcCRC16(str);
	str.push_back(static_cast<char>(crc & 0xFF));
	str.push_back(static_cast<char>((crc & 0xFF00) >> 8));
	return str;
}