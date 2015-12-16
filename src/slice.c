

#include "stdio.h"
#include "stdlib.h"
#include "assert.h"
#include "string.h"

#include "slice.h"

inline int initSlice(Slice* slice,size_t len)
{
    slice->size_ = 0;
    slice->data_ = (unsigned char*) malloc(len);
    slice->length = len;
    assert(slice->data_);
    if(slice->data_ == NULL)
        return 0;

    return slice->length;
}

int setSlice(Slice* pslice,unsigned char* data,size_t n)
{
    
    if(n <= pslice->length){
        memcpy(pslice->data_,data,n);
        pslice->size_ = n;
        return n;
    }

    if(pslice->data_ != NULL)
        free(pslice->data_);
    else
        printf("resetSlice NULL\n");
        
    pslice->data_ = (unsigned char*)malloc(n);
    assert(pslice->data_);
    pslice->length = n;
    memcpy(pslice->data_,data,n);
    pslice->size_ = n;

    return n;
}

int cpySlice(Slice* dst,const Slice* src)
{
    if(src->size_ <= dst->length){
        memcpy(dst->data_,src->data_,src->size_);
        dst->size_ = src->size_;
        return src->size_;
    }

    if(dst->data_ != NULL)
        free(dst->data_);
    else
        printf("resetSlice NULL\n");
        
    dst->data_ = (unsigned char*)malloc(src->size_);
    assert(dst->data_);
    dst->length = src->size_;

    memcpy(dst->data_,src->data_,src->size_);
    dst->size_ = src->size_;

    return src->size_;
}

int freeSlice(Slice* slice)
{
    if(slice->data_ != NULL)
        free(slice->data_);
    else
        printf("resetSlice NULL\n");
        
    return slice->length;
}

int resetSliceLength(Slice* slice,size_t n)
{
    if(n < slice->length)
        return n;
        
    if(slice->data_ != NULL)
        free(slice->data_);
    else
        printf("resetSlice NULL\n");
        
    slice->data_ = (unsigned char*)malloc(n);
    assert(slice->data_);
    slice->length = n;
    
    return n;
}

unsigned char* getSliceData(Slice* pslice)
{
    return pslice->data_;
}

void showSlice(const Slice* pslice)
{
    size_t i = 0;
    for(i = 0;i < pslice->size_;i++){
        printf("%c",pslice->data_[i]);
    }
}

void showKey(const Slice* pslice)
{
    size_t i = 0;
    for(i = 0;i < pslice->size_-8;i++){
        printf("%c",pslice->data_[i]);
    }
}

int sliceToFile(const Slice* pslice,const unsigned char* filename)
{
    FILE* fp;
    size_t i = 0;
    fp = fopen((const char *)filename,"r");
    if(fp == NULL){
        printf("sliceToFile open error!\n");
        exit(-1);
    }
    for(i = 0;i < pslice->size_;i++){
        fprintf(fp,"%c",pslice->data_[i]);
    }
    
    return 0;
}
