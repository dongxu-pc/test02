/*************************************************************************
	> File Name: slice.h
	> Author: Yandong Xu
	> Mail: gwmxyd@163.com 
	> Created Time: 2015年11月18日 星期三 22时02分36秒
 ************************************************************************/

#include "stddef.h"
#include "assert.h"

#ifndef __SSTABLE_CUDA_INCLUDE_SLICE_H__
#define __SSTABLE_CUDA_INCLUDE_SLICE_H__

typedef struct{
    const char* data_;
    size_t size_;	
} Slice;

int setSlice(Slice* pslice,char* data,size_t n);

void showSlice(const Slice* pslice);

int sliceToFile(const Slice* pslice,const char* filename);

#endif

