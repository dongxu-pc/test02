/*************************************************************************
	> File Name: coding.h
	> Author: dong xu
	> Mail: gwmxyd@163.com 
	> Created Time: 2015年11月21日 星期六 13时45分48秒
 ************************************************************************/
#include "stdlib.h"

#include "stdint.h"

#ifndef __SSTABLE_CUDA_INCLUDE_CODING_H__
#define __SSTABLE_CUDA_INCLUDE_CODING_H__

typedef struct{
	const unsigned char* value_;
	size_t size_;
} varint;


inline uint64_t  varToint64(varint* vint);

inline uint64_t decodeVarint(const unsigned char* ptr,size_t* offset);

inline uint32_t decodeFixed32(const unsigned char* ptr);

inline uint64_t decodeFixed64(const unsigned char* ptr);

void encodeFixed32(unsigned char* buf,uint32_t value);

void encodeFixed64(unsigned char* buf,uint64_t value);

void putFixed32(unsigned char* dst,uint32_t value);

#endif
