/*************************************************************************
	> File Name: table.h
	> Author: dong xu
	> Mail: gwmxyd@163.com 
	> Created Time: 2015年11月21日 星期六 10时21分46秒
 ************************************************************************/

#include "stdio.h"
#include "stdint.h"
#include "stdlib.h"

#include "env.h"
#include "format.h"

#ifndef __SSTABLE_CUDA_TABLE_TABLE_H__
#define __SSTABLE_CUDA_TABLE_TABLE_H__

/*
 * blockHandle的分为文件中的偏移，大小和类型
 * type 0,1,2,3分别代表datablock，metablock，dataindexblock，metaindexblock
 */
typedef struct {
	uint64_t offset_;
	uint64_t size_;
	uint8_t type_;
} BlockHandle;

typedef struct {
	BlockHandle metaIndexHandle;
	BlockHandle dataIndexHandle;
} Footer;

typedef struct {
	const char* data_;
	size_t size_;
	uint32_t restart_offset;
	uint32_t restartNum;
} Block;

typedef struct{
	char* key;
	varint kenlen;
	uint64_t offset_;/* block offset */
} IndexBlockEntry;

typedef struct{
	char* key;
	varint kenlen;
	uint64_t offset_;
} RestartEntry;

int readFooter(sequentialFile* psFile,Footer* pfooter);

void showFooter(const Footer* pfooter);

int readBlock(sequentialFile* psFile,Block* block,BlockHandle blockHandle);
int decodeBlock(Block* pblock);

#endif

