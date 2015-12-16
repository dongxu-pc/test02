/*************************************************************************
	> File Name: t01.c
	> Author: dong xu
	> Mail: gwmxyd@163.com 
	> Created Time: 2015年11月21日 星期六 12时13分56秒
 ************************************************************************/

#include <stdio.h>
#include "stdlib.h"

#include "env.h"
#include "slice.h"
#include "c.h"
#include "table.h"
#include "block.h"
#include "dbase.h"

#include "debtools.h"

int main()
{
	FILE* fp;
	size_t i = 0;
	size_t offset;
	fp = fopen("src.ldb","r");
	if(fp == NULL){
		printf("error!\n");
		return 1;
	}
	unsigned char a[] = "src.ldb";
	
	unsigned char b[] = "1-959-250-4279";
	//"1-114-560-9305";
	
	Slice filename;
	Slice lastKey;/* 用于存储最后一次读取到的key */
	Slice srckey; /* 存储要查找、删除、修改、添加的key */
	DBhandle dbhandle;
	
	Footer footer;
	Block dataIndexBlock;
	Block* blockArray;
	BlockEntry blockEntry;
	
	initSlice(&filename,10);
	initSlice(&lastKey,20);
	initBlockEntry(&blockEntry);
	initDBhandle(&dbhandle);
	setSlice(&dbhandle.key_,b,22);

	
	setSlice(&filename,a,strlen((char*)a));
	sequentialFile* psFile = (sequentialFile*)malloc(sizeof(sequentialFile));
	setSequentialFile(psFile,fp,&filename);
	
	readFooter(psFile,&footer);
	
	showFooter(&footer);
	
	/* 读取data index block，存储在dataBlock中 */
	readBlock(psFile,&dataIndexBlock,footer.dataIndexHandle);
	
	blockArray = (Block*)malloc(sizeof(Block)*dataIndexBlock.restartNum);
	if(blockArray == NULL){
		printf("error:blockArray is NULL.\n");
		return 1;
	}
	readAllBlock(psFile,blockArray,&dataIndexBlock);

	/*设置操作为Read*/
	dbhandle.type = 1;	
	dbcmd(blockArray,&dataIndexBlock,&dbhandle);
	
	/* 
	 * 至此，所有的block都已经读取到内存中,data block的内容存储在blockarray中，
	 * data block的索引信息存储在dataIndexBlock中 
	 */
	
	/*
	 * 
	 */
	 //showBlokRestart(&dataIndexBlock);
	 
	
	for(i = 0;i < dataIndexBlock.restartNum;i++){
		//showBlockData(&(blockArray[i]));
	}
	
	
	freeBlockEntry(&blockEntry);
	freeDBhandle(&dbhandle);
	fclose(fp); 
	printf("Hello World!\n");

	return 0;
}
