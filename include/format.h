/*************************************************************************
	> File Name: format.h
	> Author: dong xu
	> Mail: gwmxyd@163.com 
	> Created Time: 2015年11月21日 星期六 10时54分33秒
 ************************************************************************/

#include "stdint.h"
#include "stddef.h"

#ifndef __SSTABLE_CUDA_TABLE_FORMAT_H__
#define __SSTABLE_CUDA_TABLE_FORMAT_H__

typedef struct{
	const unsigned char* value_;
	int size_;
} varint;

inline uint32_t decodeFixed32(const unsigned char* ptr);
inline uint64_t decodeFixed64(const unsigned char* ptr);
inline uint64_t  varToint64(varint* vint);

#endif

