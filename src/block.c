


#include "stddef.h"
#include "stdint.h"
#include "stdio.h"
#include "stdlib.h"

#include "block.h"

#include "format.h"
#include "slice.h"
#include "table.h"
#include "dbase.h"

/*
int readDataIndexBlock(sequentialFile* psFile,const Footer* pfooter)
{
	
}*/
size_t readBlockEntry(const Block* block,BlockEntry* blockEntry,size_t* offset,Slice* lastKey)
{
	size_t tmp;
	if((*offset) < block->restart_offset){
		tmp = decodeBlockEntry(blockEntry,block->data_+(*offset),lastKey);
		*offset += tmp;
		return *offset;
	}
	return 0;
}

uint32_t getBlockEntryInfo(const unsigned char* data,Entrykey* entrykey)
{
	size_t offset = 0;
	//uint64_t i = 0;
	uint32_t sharedKeyLen = decodeVarint32(data,&offset);
	uint32_t nosharedKeyLen = decodeVarint32(data,&offset);
	uint32_t valueLen = decodeVarint32(data,&offset);
	if(entrykey->nosharedkey.data_ == NULL){
		entrykey->nosharedkey.data_ = (unsigned char*)malloc(nosharedKeyLen);
		assert(entrykey->nosharedkey.data_);
		entrykey->nosharedkey.length = nosharedKeyLen;
		memcpy(entrykey->nosharedkey.data_,data+offset,nosharedKeyLen);
		entrykey->nosharedkey.size_ = nosharedKeyLen;
		entrykey->offset = offset;
		return valueLen;
	}
	setSlice(&entrykey->nosharedkey,data+offset,nosharedKeyLen);
	entrykey->entrysize = offset+nosharedKeyLen+valueLen;
	entrykey->sharedkeylen = sharedKeyLen;
	entrykey->offset = offset;
	
	return valueLen;
}

void initEntrykey(Entrykey* entrykey){
	entrykey->sharedkeylen = 0;
	entrykey->entrysize = 0;
	initSlice(&entrykey->nosharedkey,20);
}

void freeEntrykey(Entrykey* entrykey){
	freeSlice(&entrykey->nosharedkey);
}
/* 成功则返回在segment中的偏移量，失败则返回1，应为offset不可能等于1 */
uint32_t scanSegment(const Block* block,const Segment* segment,DBhandle* dbhandle)
{
	Slice lastKey;
	BlockEntry blockEntry;
	size_t tmp;
	size_t result;
	uint32_t offset = 0;
	unsigned char* b = block->data_ + segment->start_;
	printf("scanSegment:start_ = %u\n",segment->start_);
	initSlice(&lastKey,20);
	lastKey.size_ = 0;
	initBlockEntry(&blockEntry);
	
	while(offset < segment->size_){
		tmp = decodeBlockEntry(&blockEntry,b+offset,&lastKey);
		//showBlockEntry(&blockEntry);
		printf("keysize = %zd,valuesize = %zd\n",blockEntry.key_.size_,blockEntry.value_.size_);
		//break;
		//showBlockEntry(&blockEntry);
		//showIndexBlockEntry(&blockEntry);
		
		result = compareKey(blockEntry.key_,dbhandle->key_);
		if(0 == result){
			cpySlice(&dbhandle->value_,&blockEntry.value_);
			freeSlice(&lastKey);
			freeBlockEntry(&blockEntry);
			return offset;
		}else if(1 == result){
			printf("Waring:Not Found!\n");
			freeSlice(&lastKey);
			freeBlockEntry(&blockEntry);
			return 1;
		}
		offset += tmp;
	}
	
	freeSlice(&lastKey);
	freeBlockEntry(&blockEntry);
	
	return 1;
}

int segmentFix(Segment* segment,const Block* block,const Slice* key)
{
	uint32_t low,mid,high;
	int result = -1;
	Slice midkey;
	initSlice(&midkey,20);
	low = 0;
	high = block->restartNum-1;
	while(low < high){
		mid = low + high;
		mid = mid >> 1;
		printf("slow=%u,high=%u,mid=%u\n",low,high,mid);
		getBlockEntryKey(block->data_+block->restart_[mid],&midkey);
		result = compareKey(midkey,*key);
		if(result == 1){
			high = mid;
		}else if(result == 2){
			low = mid+1;
			mid = low;
		}else{
			high = mid;
			low = mid;
		}
	}
	if(mid == 0){
		printf("Waring: the first key of the block is bigger than key.\n");	
	}
	if(mid != 0){
		mid--;
	}
	if(mid == (block->restartNum-1)){
		segment->start_ = block->restart_[mid];
		segment->size_ = block->restart_offset - block->restart_[mid];		
	}else{
		segment->start_ = block->restart_[mid];
		segment->size_ = block->restart_[mid+1] - block->restart_[mid]; 
	}
	printf("Fixed finished!\nmid = %u,start_ = %u,size_ = %u\n",mid,segment->start_,segment->size_);
	midkey.size_ = 0;
	getBlockEntryKey(block->data_+segment->start_,&midkey);
	showKey(&midkey);
	printf("FIXFIX\n");
	freeSlice(&midkey);
	return 1;
}


void showBlokRestart(const Block* block)
{
	size_t i;
	size_t offset = 0;
	Slice lastKey;
	BlockEntry blockEntry;
	initBlockEntry(&blockEntry);
	initSlice(&lastKey,20);
	
	for(i = 0;i < block->restartNum;i++){
		offset = 0;
		lastKey.size_ = 0;
		offset = block->restart_[i];
		readBlockEntry(block,&blockEntry,&offset,&lastKey);
		showIndexBlockEntry(&blockEntry);
	}
	
	freeBlockEntry(&blockEntry);
	freeSlice(&lastKey);
}

void showBlockData(const Block* block)
{
	size_t i;
	size_t offset = 0;
	Slice lastKey;
	BlockEntry blockEntry;
	initBlockEntry(&blockEntry);
	initSlice(&lastKey,20);
	
	do{
		readBlockEntry(block,&blockEntry,&offset,&lastKey);
		showBlockEntry(&blockEntry);
	}while(offset < block->restart_offset);
	
	freeBlockEntry(&blockEntry);
	freeSlice(&lastKey);
}
