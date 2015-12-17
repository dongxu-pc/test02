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
#include "dbase.h"
#include "format.h"


#ifndef __SSTABLE_CUDA_INCLUDE_BLOCK_H__
#define __SSTABLE_CUDA_INCLUDE_BLOCK_H__

typedef struct {
	Slice key_;
	size_t length_;
} LastKey;

/* 保存restart段,指的是在block中的起始位置和偏移位置,type为段的类型，1代表data block，2代表data index block */
typedef struct {
	uint32_t start_;
	uint32_t size_;
	uint8_t type;
} Segment;

typedef struct{
	uint32_t sharedkeylen;/* 记录key的共享部分的长度 */
	uint32_t entrysize;/* 记录kv项的总长度 */
	uint32_t offset;/* 记录nosharedkey的偏移量，即sharedkeylen，nosharedkey len，valuelen的总长度 */
	Slice nosharedkey;/* key非共享部分的内容 */
} Entrykey;

/*int readDataIndexBlock(sequentialFile* psFile,const Footer* pfooter);*/
size_t readBlockEntry(const Block* block,BlockEntry* blockEntry,size_t* offset,Slice* lastKey);

uint32_t scanSegment(const Block* block,const Segment* segment,DBhandle* dbhandle);

int segmentFix(Segment* segment,const Block* block,const Slice* key);

/* 获取数据项的非共享键，和整个kv项的长度,共享键的长度 */
int getBlockEntryInfo(const unsigned char* data,Entrykey* entrykey);

void initEntrykey(Entrykey* entrykey);
void freeEntrykey(Entrykey* entrykey);

void showBlokRestart(const Block* block);

void showBlockData(const Block* block);

#endif
