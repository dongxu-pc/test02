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
} Slice,*pslice_;

int setSlice(char* data,Slice* pslice,size_t n)
{
    pSlice->data_ = data;
    pslice->size_ = n;
    if(data == NULL || pslice == NULL)
        return -1;
    return n;
}

#endif
