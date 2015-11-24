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

int decodeFooter(Slice* input);

int readFooter(sequentialFile* psFile,Footer* footer)
{
	char footerSpace[48];
	long filesize;
	Slice r;
	filesize = getFilesize(psFile);
	readSFile(48,filesize-48,psFile,&r,footerSpace);
	decodeFooter(&r);
	
	
	printf("file size = %ld,footersize = %d\n",filesize,r.size_);
	
	return 0;
}

int decodeFooter(Slice* input)
{
	const char* magic_ptr = input->data_ + 40;
	const uint64_t magic_lo = (uint64_t)decodeFixed32(magic_ptr);
	const uint64_t magic_hi = (uint64_t)decodeFixed32(magic_ptr+4);
	const uint64_t magic = (magic_hi << 32) | (magic_lo);
	if(magic != MAGICNUMBER){
		printf("This is not a ldb file(bad magic).\n");
		exit(-1);
	}
	printf("Success magic!\n");
	
	return 0;
}
