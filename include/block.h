/*************************************************************************
	> File Name: block.h
	> Author: dong xu
	> Mail: gwmxyd@163.com 
	> Created Time: 2015年11月21日 星期六 13时45分48秒
 ************************************************************************/

#include "stddef.h"
#include "stdint.h"

#include "slice.h"
#include "table.h"


#ifndef __SSTABLE_CUDA_INCLUDE_BLOCK_H__
#define __SSTABLE_CUDA_INCLUDE_BLOCK_H__

typedef struct {
	Slice key_;
	size_t length_;
} LastKey;
/*int readDataIndexBlock(sequentialFile* psFile,const Footer* pfooter);*/
size_t readBlockEntry(const Block* block,BlockEntry* blockEntry,size_t* offset,Slice* lastKey);

#endif