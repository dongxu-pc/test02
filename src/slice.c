
#include "slice.h"

int setSlice(Slice* pslice,char* data,size_t n)
{
    pslice->data_ = data;
    pslice->size_ = n;
    if(data == NULL || pslice == NULL)
        return -1;
    return n;
}