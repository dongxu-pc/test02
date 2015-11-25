/*************************************************************************
	> File Name: table.c
	> Author: dong xu
	> Mail: gwmxyd@163.com 
	> Created Time: 2015年11月21日 星期六 12时29分40秒
 ************************************************************************/

#include<stdio.h>

#include "table.h"
#include "env.h"
#include "c.h"
#include "format.h"
#include "debtools.h"

int decodeFooter(Footer* pfooter,const Slice* input);
void showFooter(const Footer* pfooter);

int readFooter(sequentialFile* psFile,Footer* pfooter)
{
	char footerSpace[48];
	long filesize;
	Slice r;
	int i;
	filesize = getFilesize(psFile);
	readSFile(48,filesize-48,psFile,&r,footerSpace);
	decodeFooter(pfooter,&r);
	for(i = 0;i < 48;i++){
		printXchar(r.data_[i]);
	}
	
	printf("file size = %ld,footersize = %d\n",filesize,r.size_);
	
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