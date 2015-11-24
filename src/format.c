/*************************************************************************
	> File Name: format.h
	> Author: dong xu
	> Mail: gwmxyd@163.com 
	> Created Time: 2015年11月21日 星期六 10时54分33秒
 ************************************************************************/

#include "stdint.h"
#include "stddef.h"
#include "assert.h"

#include "format.h"


inline uint32_t decodeFixed32(const char* ptr)
{
	return (
			((uint32_t)(ptr[0]))
			|(((uint32_t)(ptr[1])) <<8)
			|(((uint32_t)(ptr[2])) << 16)
			|(((uint32_t)(ptr[3])) << 24)
			);
}

inline uint64_t decodeFixed64(const char* ptr)
{
	uint64_t lo = decodeFixed32(ptr);
	uint64_t hi = decodeFixed32(ptr+4);
	return ((hi << 32)|lo);
}

inline uint64_t  varToint64(struct varint vint)
{
	uint64_t value = 0;
	vint.size = 0;
	while((vint.value_[vint.size] & 0x80) == 0x80){
		value += value*128 + (uint64_t)(vint.value_[vint.size]&0x7F);
		vint.size++;
		assert(vint.size < 8);
	}

	return value;
}

