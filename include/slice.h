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
    unsigned char* data_;
    size_t size_;
	size_t length;	
} Slice;

inline int initSlice(Slice* slice,size_t len);

int setSlice(Slice* pslice,unsigned char* data,size_t n);

int freeSlice(Slice* slice);

inline int resetSliceLength(Slice* slice,size_t n);

int cpySlice(Slice* dst,const Slice* src);

void showSlice(const Slice* pslice);

void showKey(const Slice* pslice);

int sliceToFile(const Slice* pslice,const unsigned char* filename);

#endif

