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

inline uint64_t  varToint64(varint* vint)
{
	uint64_t value = 0;
	int i = 0;
	while((vint->value_[vint->size_] & 0x80) == 0x80){
		value += ((uint64_t)(vint->value_[vint->size_]&0x7F)) << (7*i);
		vint->size_++;
		i++;
		assert(i < 8);
	}
	value += ((uint64_t)(vint->value_[vint->size_]&0x7F)) << (7*i);
	vint->size_++;
	
	return value;
}

