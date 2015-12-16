/*************************************************************************
	> File Name: table.c
	> Author: dong xu
	> Mail: gwmxyd@163.com 
	> Created Time: 2015年11月21日 星期六 12时29分40秒
 ************************************************************************/

#include "stdio.h"
#include "stdlib.h"
#include "stdint.h"
#include "stddef.h"
#include "assert.h"
#include "string.h"

#include "table.h"
#include "env.h"
#include "c.h"
#include "coding.h"
#include "format.h"
#include "debtools.h"
#include "crc32c.h"

int decodeFooter(Footer* pfooter,const Slice* input);
inline int checkBlockCrc32(const uint8_t* data,size_t n);
inline int blockEntryToIndexBlock(IndexBlockEntry* index,const BlockEntry* entry);


/*
 * 读取sstable文件的Footer
 */
int readFooter(sequentialFile* psFile,Footer* pfooter)
{
	unsigned char footerSpace[48];
	long filesize;
	Slice r;
	initSlice(&r,20);
	int i;
	filesize = getFilesize(psFile);
	readSFile(48,filesize-48,psFile,footerSpace);
	setSlice(&r,footerSpace,48);
	decodeFooter(pfooter,&r);
	for(i = 0;i < 48;i++){
		printXchar(r.data_[i]);
	}
	freeSlice(&r);
	printf("file size = %ld,footersize = %zd\n",filesize,r.size_);
	return 0;
}

/*
 * 将Footer字符串进行解析
 * 验证magic值
 * 读取metaIndexBlock和dataIndexBlock的句柄
 */
int decodeFooter(Footer* pfooter,const Slice* input)
{
	const unsigned char* magic_ptr = input->data_;
	const uint64_t magic = decodeFixed64(magic_ptr+40);
	if(magic != MAGICNUMBER){
		printf("This is not a ldb file(bad magic).\n");
		exit(-1);
	}
	printf("Success magic!\n");
	varint varint;
	varint.value_ = magic_ptr;
	varint.size_ = 0;
	pfooter->metaIndexHandle.offset_ = varToint64(&varint);
	printf("vint offset = %zd\n",varint.size_);
	pfooter->metaIndexHandle.size_ = varToint64(&varint);
	printf("vint offset = %zd\n",varint.size_);
	pfooter->dataIndexHandle.offset_ = varToint64(&varint);
	printf("vint offset = %zd\n",varint.size_);
	pfooter->dataIndexHandle.size_ = varToint64(&varint);
	printf("vint offset = %zd\n",varint.size_);
	
	showFooter(pfooter);
	
	return 0;
}

/*
 * 输出Footer的基本信息
 */
void showFooter(const Footer* pfooter)
{
	printf("dateIndexHandle(%lu,%lu).\n",pfooter->dataIndexHandle.offset_,
	   pfooter->dataIndexHandle.size_);
	printf("metaIndexHandle(%lu,%lu).\n",pfooter->metaIndexHandle.offset_,
	   pfooter->metaIndexHandle.size_);
}

/*
 * 读取Block数据，结果存储在Block中
 */
int readBlock(sequentialFile* psFile,Block* block,BlockHandle blockHandle)
{
	//printf("Read Block,offset %llu,size %llu\n",blockHandle.offset_,blockHandle.size_);
	uint32_t i;
	block->data_ = (unsigned char*) malloc(blockHandle.size_);
	assert(block->data_);
	
	readSFile(blockHandle.size_+5,blockHandle.offset_,psFile,block->data_);
	block->size_ = blockHandle.size_;

	checkBlockCrc32(block->data_,blockHandle.size_+5);
	
	/*decodeBlock(block);*/
	block->restartNum = decodeFixed32( (unsigned char*)(block->data_ + block->size_ - 4) );
	block->restart_offset = block->size_ - (block->restartNum+1)*4;
	block->restart_ = (uint32_t*)(block->data_ + block->restart_offset);
	/* 计算restart点信息 */
	for(i = 0;i < block->restartNum;i++){
		block->restart_[i] = decodeFixed32((unsigned char*)(block->data_ + block->restart_offset + 4*i));
	}
	//printf("Read Block\n");
	
	return 0;
}



/*
 * 批量读取block数据
 */
int readAllBlock(sequentialFile* psFile,Block* blockArray,const Block* dataIndexBlock)
{
	printf("Read All Block.\n");
	
	//printf("%s(%d)-<%s>: ",__FILE__, __LINE__, __FUNCTION__);
	uint32_t i = 0;
	BlockHandle blockHandle;
	BlockEntry blockEntry;
	unsigned char* data;
	Slice lastKey;
	initSlice(&lastKey,20);
	initBlockEntry(&blockEntry);
	
	for(i = 0;i < dataIndexBlock->restartNum;i++){
		data =(unsigned char*) (dataIndexBlock->data_+dataIndexBlock->restart_[i]);
		decodeBlockEntry(&blockEntry,data,&lastKey);
		decodeBlockHandle(&blockHandle,&blockEntry.value_);
		readBlock(psFile,&blockArray[i],blockHandle);
		//showBlock(&blockArray[i]);
	}
	freeBlockEntry(&blockEntry);
	freeSlice(&lastKey);
	return 0;
}

/*
 * 解析Block项
 */
inline size_t decodeBlockEntry(BlockEntry* blockEntry,const unsigned char* data,Slice* lastKey)
{
	size_t offset = 0;
	//uint64_t i = 0;
	uint64_t sharedKeyLen = decodeVarint(data,&offset);
	uint64_t nosharedKeyLen = decodeVarint(data,&offset);
	uint64_t valueLen = decodeVarint(data,&offset);
	//printf("sharedKeyLen = %llu,nosharedKeyLen = %llu,valueLen = %llu\n",sharedKeyLen,nosharedKeyLen,valueLen);
	
	if(lastKey->size_ < sharedKeyLen){
		printf("lastKey length is wrong!\n");
		return 0;
	}
	
	/* 读取key */
	blockEntry->key_.size_ = sharedKeyLen + nosharedKeyLen;
	resetSliceLength(&blockEntry->key_,blockEntry->key_.size_);
	
	memcpy(blockEntry->key_.data_,lastKey->data_,sharedKeyLen);
	memcpy(blockEntry->key_.data_+sharedKeyLen,data+offset,nosharedKeyLen);
	offset += nosharedKeyLen;
	
	/* 更新lastkey */
	resetSliceLength(lastKey,blockEntry->key_.size_);
	lastKey->size_ = blockEntry->key_.size_;
	memcpy(lastKey->data_,blockEntry->key_.data_,blockEntry->key_.size_);
	
	/*读取value*/
	blockEntry->value_.size_ = valueLen;
	resetSliceLength(&blockEntry->value_,valueLen);
	memcpy(blockEntry->value_.data_,data+offset,valueLen);
	offset+=valueLen;

	//showIndexBlockEntry(blockEntry);
	/* 返回此blockEntry占用的字节数 */
	return offset;
	
}

/*
 * 获取重启点的key值
 */
inline int getBlockEntryKey(const unsigned char* data,Slice* key)
{
	size_t offset = 0;
	//uint64_t i = 0;
	uint64_t sharedKeyLen = decodeVarint(data,&offset);
	uint64_t nosharedKeyLen = decodeVarint(data,&offset);
	uint64_t valueLen = decodeVarint(data,&offset);
	printf("sharedKeyLen(%lu),nosharedKeyLen(%lu),valueLen(%lu).\n",sharedKeyLen,nosharedKeyLen,valueLen);
	if(sharedKeyLen > 0 && key->size_ == 0){
		printf("error:This is not a restart point.\n");
		return 0;
	}
	resetSliceLength(key,nosharedKeyLen);
	memcpy(key->data_,data+offset,nosharedKeyLen);
	key->size_ = nosharedKeyLen;
	
	return 1;
}

/*
 * 输出Block额基本信息
 */
void showBlock(const Block* block)
{
	uint32_t i;
	printf("Block size is %zd\nrestartNum %u,restart offset %u\n",block->size_,block->restartNum,block->restart_offset);
	for(i = 0;i < block->restartNum;i++){
		printf("Restart %d is %u;\n",i,block->restart_[i]);
	}
}

/*
 * 解析block数据
 */
//inline int decodeBlock(Block* pblock)
inline int decodeBlock()
{

	return 0;
}


/*
 * 校验Block的CRC值
 * 校验和包括blockentry，restarts，restartNum，type
 * 故CRC校验的内容是block.size + 1
 */
inline int checkBlockCrc32(const uint8_t* data,size_t n)
{
	const uint32_t crc = crcUnmask(decodeFixed32(data+n-4));
	const uint32_t actual = crcValue(data,n-4);
	
	if(actual != crc){
		printf("Check Crc,error!\n");
		return 1;
	}
	return 0;
}

/*
 * 将BlockEntry转化成IndexBlockEntry
 */
inline int blockEntryToIndexBlock(IndexBlockEntry* index,const BlockEntry* entry)
{
	size_t offset = 0;
	index->key_ = entry->key_;
	index->blockhandle.offset_ = decodeVarint(entry->value_.data_,&offset);
	index->blockhandle.size_ = decodeVarint(entry->value_.data_,&offset);
	
	return entry->value_.size_;
}

inline int decodeBlockHandle(BlockHandle* blockHandle,const Slice* slice)
{
	size_t offset = 0;
	blockHandle->offset_ = decodeVarint(slice->data_,&offset);
	blockHandle->size_ = decodeVarint(slice->data_,&offset);
	
	return slice->size_;
}

void showBlockEntry(const BlockEntry* blockEntry)
{
	printf("key:");
	showKey(&blockEntry->key_);
	printf("  Value:");
	showSlice(&blockEntry->value_);
	printf("\n");
}

void showIndexBlockEntry(const BlockEntry* blockEntry)
{
	printf("key:");
	showKey(&blockEntry->key_);
	printf("\n");
}

inline int initBlockEntry(BlockEntry* blockEntry)
{
	initSlice(&blockEntry->key_,20);
	initSlice(&blockEntry->value_,20);
	
	return 1;
}

inline int freeBlockEntry(BlockEntry* blockEntry)
{
	freeSlice(&blockEntry->key_);
	freeSlice(&blockEntry->value_);
	
	return 1;
}
