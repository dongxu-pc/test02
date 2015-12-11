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
	Slice filename;
	initSlice(&filename,10);
	Slice lastKey;
	initSlice(&lastKey,20);
	Footer footer;
	Block dataIndexBlock;
	Block* blockArray;
	BlockEntry blockEntry;
	initBlockEntry(&blockEntry);
	
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
	
	for(i = 0;i < dataIndexBlock.restartNum;i++){
		offset = 0;
		lastKey.size_ = 0;
		do{
			if(0 == readBlockEntry(&(blockArray[i]),&blockEntry,&offset,&lastKey))
			    break;
			printf("offset=%zd  ",offset);
			showBlockEntry(&blockEntry);
		}while(offset < blockArray[i].restart_offset);
	}
	
	fclose(fp); 
	printf("Hello World!\n");

	return 0;
}
