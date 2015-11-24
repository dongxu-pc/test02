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

#ifndef __SSTABLE_CUDA_TABLE_TABLE_H__
#define __SSTABLE_CUDA_TABLE_TABLE_H__

typedef struct {
	uint64_t offset;
	uint64_t size;
} BlockHandle;

typedef struct {
	char magicNum[8];
	BlockHandle metaIndexHandle;
	BlockHandle dataIndexHandle;
} Footer,*pFooter;

typedef struct {
	const char* data_;
	size_t size;
	uint32_t restart_offset;
	size_t restartNum;
} Block, *pBlock;
int readFooter(sequentialFile* psFile,Footer* footer);
#endif

