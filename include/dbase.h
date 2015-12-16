/*************************************************************************
	> File Name: dbase.h
	> Author: Yandong Xu
	> Mail: gwmxyd@163.com 
	> Created Time: 2015年12月12日 星期六 09时54分00秒
 ************************************************************************/

#include "stdio.h"
#include "stdlib.h"

#include "slice.h"
#include "table.h"

#ifndef __SSTABLE_CUDA_INCLUDE_DB_H__
#define __SSTABLE_CUDA_INCLUDE_DB_H__


/*
typedef struct{
	Slice key_;
	Slice value_;
	uint8_t type;	
} DBhandle;
*/
/* type:0代表无含义，1代表查找，2代表插入，3代表删除，4代表修改*/
typedef BlockEntry DBhandle;


inline int initDBhandle(DBhandle* dbhandle);
inline int freeDBhandle(DBhandle* dbhandle);

int dbcmd(Block* blockArray,Block* dataIndexBlock,DBhandle* dbhandle);

inline int blockFix(size_t* index,const Block* indexBlock,const Slice* key);

/**
 * 根据key从数据库文件中获取value
 */
int getKV(Block* blockArray,Block* dataIndexBlock,DBhandle* dbhandle);

/**
 * 实现对key-vlaue的添加或修改操作
 */
int putKV(Block* blockArray,Block* dataIndexBlock,DBhandle* dbhandle);

/**
 * 实现对key-value对的删除操作
 */
int deleteKV(Block* blockArray,Block* dataIndexBlock,DBhandle* dbhandle);

#endif
