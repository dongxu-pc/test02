/*************************************************************************
	> File Name: table.h
	> Author: dong xu
	> Mail: gwmxyd@163.com 
	> Created Time: 2015年11月21日 星期六 10时21分46秒
 ************************************************************************/

#include "stdio.h"
#include "stdint.h"
#include "stdlib.h"
#include "assert.h"

#include "env.h"
#include "format.h"

#ifndef __SSTABLE_CUDA_TABLE_TABLE_H__
#define __SSTABLE_CUDA_TABLE_TABLE_H__

/*
 * blockHandle的分为文件中的偏移，大小和类型
 * type 1,2,3,4分别代表datablock，metablock，dataindexblock，metaindexblock
 *      10,20,30,40表示该block已经读取到内存中
 */
typedef struct {
	uint64_t offset_;/* block offset */
	uint64_t size_; /* block size = actual file size - 1(type) - 4(CRC32) */
} BlockHandle;

typedef struct {
	BlockHandle metaIndexHandle;
	BlockHandle dataIndexHandle;
} Footer;

/*
 * 是block的信息
 * data_表示数据内容
 * restart_offset:表示首个重启点的偏移量，可以看做Block数据本分的结尾
 * restartNum:表示重启点的个数
 * restart_：其占用的是data_部分的存储空间，位置是data_+restart_offset
 * status: 1,2,3,4分别代表datablock，metablock，dataindexblock，metaindexblock
 *      10,20,30,40表示该block已经读取到内存中
*/
typedef struct {
	unsigned char* data_;
	size_t size_;
	uint32_t restart_offset;
	uint32_t restartNum;
	uint32_t* restart_;
	uint8_t status;/* 表示 */
} Block;

/*
 * Block的数据项，解析过的。
 */
typedef struct{
	Slice key_;
	Slice value_;
	uint8_t type;
}BlockEntry;

/*
 * IndexBlock的数据项，即对BlockEntry的value进行了进一步的解析
 */
typedef struct{
	Slice key_;
	BlockHandle blockhandle;
} IndexBlockEntry;

/*
 * 重启点
 */
typedef struct{
	Slice* key;
	uint64_t offset_;
} RestartEntry;

int readFooter(sequentialFile* psFile,Footer* pfooter);

void showFooter(const Footer* pfooter);

int readBlock(sequentialFile* psFile,Block* block,BlockHandle blockHandle);
inline int decodeBlockHandle(BlockHandle* blockHandle,const Slice* slice);
int readAllBlock(sequentialFile* psFile,Block* blockArray,const Block* dataIndexBlock);
inline size_t decodeBlockEntry(BlockEntry* blockEntry,const unsigned char* data,Slice* lastKey);
inline int getBlockEntryKey(const unsigned char* data,Slice* key);

//int decodeBlock(Block* pblock);
inline int decodeBlock();
void showBlock(const Block* block);
void showBlockEntry(const BlockEntry* blockEntry);
void showIndexBlockEntry(const BlockEntry* blockEntry);
inline int initBlockEntry(BlockEntry* blockEntry);
inline int freeBlockEntry(BlockEntry* blockEntry);

#endif
