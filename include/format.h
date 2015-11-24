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

typedef struct varint{
	char* value_;
	int size;
}*pvint;

inline uint32_t decodeFixed32(const char* ptr);
inline uint64_t decodeFixed64(const char* ptr);
inline uint64_t  varToint64(struct varint vint);

#endif

