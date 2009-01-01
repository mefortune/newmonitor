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

uint16_t CRC16Wrapper::CalcCRC16(std::vector<char> data)
{
	uint16_t reg = 0;
	for (size_t i = 0; i != data.size() + 2; ++i){
		uint16_t crc_mask = _poly_tbl[reg >> 8];
		reg = reg << 8 | (i < data.size() ? data[i] : 0);
		reg ^= crc_mask;
	}
	return reg;
}

void CRC16Wrapper::GenerateCRCData(std::vector<char> &data)
{
	uint16_t crc = CalcCRC16(data);
	data.push_back(static_cast<char>(crc & 0xFF));
	data.push_back(static_cast<char>((crc & 0xFF00) >> 8));
}