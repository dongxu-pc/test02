/*************************************************************************
	> File Name: dbase.c
	> Author: Yandong Xu
	> Mail: gwmxyd@163.com 
	> Created Time: 2015年12月12日 星期六 10时24分10秒
 ************************************************************************/

#include "stdio.h"
#include "stdlib.h"
#include "stdint.h"
#include "stddef.h"

#include "dbase.h"
#include "block.h"

inline int initDBhandle(DBhandle* dbhandle)
{
	initSlice(&dbhandle->key_,20);
	initSlice(&dbhandle->value_,20);
	dbhandle->type = 0;
	
	return 20;
}

inline int freeDBhandle(DBhandle* dbhandle)
{
	freeSlice(&dbhandle->key_);
	freeSlice(&dbhandle->value_);
	
	return dbhandle->type;
}

inline void showDBhandle(const DBhandle* dbhandle){
	printf("\nType:%d",dbhandle->type);
	printf("  key:");
	showKey(&dbhandle->key_);
	printf("  value:");
	showSlice(&dbhandle->value_);
	printf("\n");
}

/*
 * 通过key,定位该项的位置
 * 如果indexBlock为data index block，则是用于定位key在哪一个block中，得到的index是block在blockarray中的下标
 * 如果indexBlock为data block,则用于定位key在block中的哪一个重启段中，得到的index是restart
 */
inline int blockFix(size_t* index,const Block* indexBlock,const Slice* key)
{
	uint32_t low,mid,high;
	printf("step in,restartNum(%u)\n",indexBlock->restartNum);
	int result = -1;
	Slice midkey;
	initSlice(&midkey,20);
	low = 0;
	high = indexBlock->restartNum-1;
	printf("step in,restartNum(%u)\n",indexBlock->restartNum);
	while(low < high){
		mid = low + high;
		mid = mid >> 1;
		printf("blow=%u,high=%u,mid=%u\n",low,high,mid);
		getBlockEntryKey(indexBlock->data_+indexBlock->restart_[mid],&midkey);
		showSlice(&midkey);
		printf("\n");
		result = compareKey(midkey,*key);
		if(result == 1){
			high = mid;
		}else if(result == 2){
			low = mid+1;
			mid = low;
		}else {
			low = mid;
			high = mid;
		}
	}
	*index = low;
	printf("low=%u,high=%u,mid=%u\n",low,high,mid);
	printf("index = %zd\n",*index);
	freeSlice(&midkey);
	return 1;
}

/*
 * 数据文件操作指令句柄
 * 0,无操作
 * 1,读操作
 * 2，3,写操作
 * 4,删除操作
 */
int dbcmd(Block* blockArray,Block* dataIndexBlock,DBhandle* dbhandle)
{
	switch(dbhandle->type){
		case 0: showDBhandle(dbhandle);return 0;
		case 1: return getKV(blockArray,dataIndexBlock,dbhandle);
		case 2: return putKV(blockArray,dataIndexBlock,dbhandle);
		case 3: return putKV(blockArray,dataIndexBlock,dbhandle);
		case 4: return deleteKV(blockArray,dataIndexBlock,dbhandle);
		default: printf("dbcmd error:dbhandle type(%d)\n",dbhandle->type);return -1; 
	}
}

int getKV(Block* blockArray,Block* dataIndexBlock,DBhandle* dbhandle)
{
	uint64_t block = 0,restart = 0;
	Segment segment;
	uint32_t offset;
	printf("getKV\n");
	showKey(&dbhandle->key_);
	blockFix(&block,dataIndexBlock,&dbhandle->key_);
	printf("Block Fixed,block(%zd).\n",block);
	segmentFix(&segment,&blockArray[block],&dbhandle->key_);
	printf("Segment Fixed,offset=%u,size_=%u.\n",segment.start_,segment.size_);
	offset = scanSegment(&blockArray[block],&segment,dbhandle);
	if(1 != offset){
		printf("found:");
		showDBhandle(dbhandle);
		return 1;
	}else{
		printf("Not Found.\n");
		return 0;
	}
	
}


/**
 * 实现对key-vlaue的添加或修改操作
 */
int putKV(Block* blockArray,Block* dataIndexBlock,DBhandle* dbhandle)
{
	printf("Put action!\n");
	return 0;
}

/**
 * 实现对key-value对的删除操作
 * 如果需要删除的kv项的共享键长度大于或着等于,则其删除后不需要修改其临近项的内容
 * 如果需要删除的kv项的共享键长度小于其后临项，则说明该临近项的共享部分包含了其非公享部分，故需要对其邻近向的nosharedkey进行修改
 * 以保证信息不会丢失
 * 最后修改block中的内容以使得删除是生效的，
 */
int deleteKV(Block* blockArray,Block* dataIndexBlock,DBhandle* dbhandle)
{
	uint64_t block = 0,i = 0,size = 0;
	size_t offset = 0;
	unsigned char* tmp;
	unsigned char* tmp1;
	uint32_t* restart;
	Segment segment;
	Entrykey prekey;
	Entrykey nextkey;
	initEntrykey(&prekey);
	initEntrykey(&nextkey);
	
	
	printf("getKV\n");
	showKey(&dbhandle->key_);
	blockFix(&block,dataIndexBlock,&dbhandle->key_);/* 获取key应该在哪一个block中 */
	printf("Block Fixed,block(%zd).\n",block);
	segmentFix(&segment,&blockArray[block],&dbhandle->key_);/* 获取key应该在block中的哪一个restart段中 */
	printf("Segment Fixed,offset=%u,size_=%u.\n",segment.start_,segment.size_);
	offset = scanSegment(&blockArray[block],&segment,dbhandle);/* 获取key应该在段中的偏移量(作为返回值)，病取出该项 *//* 此处offset为在restart段中的偏移量 */
	if(1 != offset){/* 定位成功，该block中含有该值 */
		/* 获取当前项和下一项的信息，分别记录在prekey和nextkey中 */
		tmp = blockArray[block].data_+segment.start_+offset;/* tmp记录当前项的位置 */
		getBlockEntryInfo(tmp,&prekey);
		tmp1 = tmp+prekey.entrysize;/* tmp1记录下一项的位置 */
		getBlockEntryInfo(tmp1,&nextkey);
		
		if(prekey.sharedkeylen >= nextkey.sharedkeylen){
			/* 删除当前项不影响下一项的内容，则只需将其后的所有内容前移即可，同时修改block中start的内容 */
			size = blockArray[i].restart_offset-prekey.entrysize-segment.start_-offset;
			for(i = 0;i < size;i++){
				tmp[i] = tmp1[i];
			}
			restart = (uint32_t*)(tmp + size);
			blockArray[i].restart_offset -= prekey.entrysize;
			for(i = 0;i < blockArray[block].restartNum;i++){
				if(blockArray[block].restart_[i]<segment.start_)
					restart[i] = blockArray[block].restart_[i];
				else{
					restart[i] = blockArray[block].restart_[i]-prekey.entrysize;
				}
			}
			
		}else{
			/* 此时删除当前项对其下一项的非共享key有影响，故需要对下一项的内容做出修改 */
			size = blockArray[i].restart_offset-prekey.entrysize-segment.start_-offset;
			encodeVarint32(tmp,prekey.sharedkeylen);
			encodeVarint32(tmp,nextkey.nosharedkey.size_+nextkey.sharedkeylen-prekey.sharedkeylen);
			encodeVarint32(tmp,nextkey.value)
		}
		
	}else{
		printf("The KV is not in database!\n    ");
		showDBhandle(dbhandle);
	}
	
	freeEntrykey(&prekey);
	freeEntrykey(&nextkey);
	
	printf("Delete action!\n");
	return 0;	
}