

/*************************************************************************
	> File Name: coding.c
	> Author: dong xu
	> Mail: gwmxyd@163.com 
	> Created Time: 2015年11月21日 星期六 13时45分48秒
 ************************************************************************/
#include "stdlib.h"

#include "stdint.h"
#include "assert.h"

#include "coding.h"

inline uint64_t decodeVarint(const unsigned char* ptr,size_t* offset)
{
	varint vint;
	uint64_t value;
	
	vint.size_ = 0;
	vint.value_ = ptr+(*offset);
	
	value = varToint64(&vint);
	
	(*offset) += vint.size_;
	//printf("value = %llu,offset = %d\n",value,*offset);
	
	return value;
}


inline size_t decodeVarint32(const unsigned char* ptr,uint32_t* value)
{
	size_t i = 0;
	*value = 0;
	for(i = 0;i < 4;i++){
		if((ptr[i] & 0x80) != 0x80)
			break;
		value += ((uint32_t)(ptr[i]&0x7F)) << (7*i);	
	}
	assert(i < 4);
	value += ((uint32_t)(ptr[i]&0x7F)) << (7*i);
	
	return i;/* 返回这个整数所占用的字节数 */
}

inline size_t decodeVarint64(const unsigned char* ptr,uint64_t* value)
{
	size_t i = 0;
	*value = 0;
	for(i = 0;i < 8;i++){
		if((ptr[i] & 0x80) != 0x80)
			break;
		value += ((uint64_t)(ptr[i]&0x7F)) << (7*i);	
	}
	assert(i < 8);
	value += ((uint64_t)(ptr[i]&0x7F)) << (7*i);
	
	return i;/* 返回这个整数所占用的字节数 */
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


inline size_t getVarint32Size(uint32_t value)
{
	/* 7,14,21,28,35 */
	if(value < 0x7F)
		return 1;
	else if(value < 0x3FFF)
		return 2;
	else if(value < 0x1FFFFF)
		return 3;
	else if(value < 0xFFFFFFF)
		return 4;
	else if(value < 0xFFFFFFFF)
		return 5;
	else
		return 0;/* 错误 */
}

inline size_t getVarint64Size(uint64_t value)
{
	/* 7,14,21,28,35,42,49,56,63,70 */
	if(value < 0x7F)
		return 1;
	else if(value < 0x3FFF)
		return 2;
	else if(value < 0x1FFFFF)
		return 3;
	else if(value < 0xFFFFFFF)
		return 4;
	else if(value < 0x7FFFFFFFF)
		return 5;
	else if(value < 0x3FFFFFFFFFF)
		return 6;
	else if(value < 0x1FFFFFFFFFFFF)
		return 7;
	else if(value < 0xFFFFFFFFFFFFFF)
		return 8;
	else if(value < 0x7FFFFFFFFFFFFFFF)
		return 9;
	else if(value < 0xFFFFFFFFFFFFFFFF)
		return 10;
	else
		return 0;/* 错误 */
}

inline size_t encodeVarint32(unsigned char* ptr,uint32_t value)
{
	size_t size = getVarint32Size(value);
	size_t i = 0;
	if(ptr == NULL)
		ptr = (unsigned char*)malloc(size);
	assert(ptr);
	for(i = 0;i < size;i++){
		                                                       
	}
	
}

inline size_t encodeVarint64(unsigned char* ptr,uint64_t value)；



inline uint64_t uint64Tovarint(varint* vint,uint64_t in)
{
	
}

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

void encodeFixed32(unsigned char* buf,uint32_t value)
{
	buf[0] = (unsigned char) value & 0xff;
	buf[1] = (unsigned char) (value >> 8) & 0xff;
	buf[2] = (unsigned char) (value >> 16) & 0xff;
	buf[3] = (unsigned char) (value >> 24) & 0xff;
}

void encodeFixed64(unsigned char* buf,uint64_t value)
{
	buf[0] = (unsigned char) value & 0xff;
	buf[1] = (unsigned char) (value >> 8) & 0xff;
	buf[2] = (unsigned char) (value >> 16) & 0xff;
	buf[3] = (unsigned char) (value >> 24) & 0xff;
	buf[4] = (unsigned char) (value >> 32) & 0xff;
	buf[5] = (unsigned char) (value >> 40) & 0xff;
	buf[6] = (unsigned char) (value >> 48) & 0xff;
	buf[7] = (unsigned char) (value >> 56) & 0xff;
}

void putFixed32(unsigned char* dst,uint32_t value)
{
	if(dst == NULL)
	    dst = (unsigned char*)malloc(4);
	encodeFixed32(dst,value);
}

void putFixed64(unsigned char* dst,uint64_t value)
{
	if(dst == NULL)
		dst = (unsigned char*)malloc(8);
	encodeFixed64(dst,value);
}