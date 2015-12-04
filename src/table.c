/*************************************************************************
	> File Name: table.c
	> Author: dong xu
	> Mail: gwmxyd@163.com 
	> Created Time: 2015年11月21日 星期六 12时29分40秒
 ************************************************************************/

#include<stdio.h>
#include "stdlib.h"
#include "stdint.h"
#include "stddef.h"

#include "table.h"
#include "env.h"
#include "c.h"
#include "format.h"
#include "debtools.h"

int decodeFooter(Footer* pfooter,const Slice* input);

int readFooter(sequentialFile* psFile,Footer* pfooter)
{
	char footerSpace[48];
	long filesize;
	Slice r;
	int i;
	filesize = getFilesize(psFile);
	readSFile(48,filesize-48,psFile,footerSpace);
	setSlice(&r,footerSpace,48);
	decodeFooter(pfooter,&r);
	for(i = 0;i < 48;i++){
		printXchar(r.data_[i]);
	}
	printf("file size = %ld,footersize = %d\n",filesize,r.size_);
	//free(r.data_);
	return 0;
}

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
	printf("vint offset = %d\n",varint.size_);
	pfooter->metaIndexHandle.size_ = varToint64(&varint);
	printf("vint offset = %d\n",varint.size_);
	pfooter->dataIndexHandle.offset_ = varToint64(&varint);
	printf("vint offset = %d\n",varint.size_);
	pfooter->dataIndexHandle.size_ = varToint64(&varint);
	printf("vint offset = %d\n",varint.size_);
	
	showFooter(pfooter);
	
	return 0;
}

void showFooter(const Footer* pfooter)
{
	printf("dateIndexHandle(%llu,%llu).\n",pfooter->dataIndexHandle.offset_,
	   pfooter->dataIndexHandle.size_);
	printf("metaIndexHandle(%llu,%llu).\n",pfooter->metaIndexHandle.offset_,
	   pfooter->metaIndexHandle.size_);
}

int readBlock(sequentialFile* psFile,Block* block,BlockHandle blockHandle)
{
	//char str_crc[];
	printf("Read Block,offset %llu,size %llu\n",blockHandle.offset_,blockHandle.size_);
	Slice r;
	int i;
	block->data_ = (uint8_t*) malloc(blockHandle.size_);
	if(block->data_ == NULL){
		block->data_ = (uint8_t*) malloc(blockHandle.size_);
	}
	readSFile(blockHandle.size_,blockHandle.offset_,psFile,block->data_);
	printf("Read Block,offset %llu,size %llu\n",blockHandle.offset_,blockHandle.size_);
	setSlice(&r,block->data_,blockHandle.size_);
	
	for(i = 0;i < blockHandle.size_;i++){
		printXchar(r.data_[i]);
	}
	
	printf("Read Block\n");
	
	return 0;
}
