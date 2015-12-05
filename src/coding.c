

/*************************************************************************
	> File Name: coding.c
	> Author: dong xu
	> Mail: gwmxyd@163.com 
	> Created Time: 2015年11月21日 星期六 13时45分48秒
 ************************************************************************/
#include "stdlib.h"

#include "stdint.h"

#include "coding.h"

inline uint32_t decodeFixed32(const unsigned char* ptr)
{
	return (
			((uint32_t)(ptr[0]))
			|(((uint32_t)(ptr[1])) << 8)
			|(((uint32_t)(ptr[2])) << 16)
			|(((uint32_t)(ptr[3])) << 24)
			);
}

inline uint64_t decodeFixed64(const unsigned char* ptr)
{
	uint64_t lo = decodeFixed32(ptr);
	uint64_t hi = decodeFixed32(ptr+4);
	return ((hi << 32)|lo);
}

void encodeFixed32(char* buf,uint32_t value)
{
	buf[0] = value & 0xff;
	buf[1] = (value >> 8) & 0xff;
	buf[2] = (value >> 16) & 0xff;
	buf[3] = (value >> 24) & 0xff;
}

void encodeFixed64(char* buf,uint64_t value)
{
	buf[0] = value & 0xff;
	buf[1] = (value >> 8) & 0xff;
	buf[2] = (value >> 16) & 0xff;
	buf[3] = (value >> 24) & 0xff;
	buf[4] = (value >> 32) & 0xff;
	buf[5] = (value >> 40) & 0xff;
	buf[6] = (value >> 48) & 0xff;
	buf[7] = (value >> 56) & 0xff;
}

void putFixed32(char* dst,uint32_t value)
{
	char buf[sizeof(value)];
	encodeFixed32(buf,value);
	dst = buf;
}
