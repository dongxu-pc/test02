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

void encodeFixed32(char* buf,uint32_t value);

void encodeFixed64(char* buf,uint64_t value);

void putFixed32(char* dst,uint32_t value);

#endif
