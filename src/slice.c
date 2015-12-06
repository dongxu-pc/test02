
#include "slice.h"
#include "stdio.h"
#include "stdlib.h"

int setSlice(Slice* pslice,unsigned char* data,size_t n)
{
    pslice->data_ = data;
    pslice->size_ = n;
    if(data == NULL || pslice == NULL)
        return -1;
    return n;
}

char* getSliceData(Slice* pslice)
{
    return pslice->data_;
}

void showSlice(const Slice* pslice)
{
    int i = 0;
    for(i = 0;i < pslice->size_;i++){
        printf("%c ",pslice->data_[i]);
    }
}

int sliceToFile(const Slice* pslice,const unsigned char* filename)
{
    FILE* fp;
    fp = fopen(filename,"r");
    if(fp == NULL){
        printf("sliceToFile open error!\n");
        exit(-1);
    }
    
}