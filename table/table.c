/*************************************************************************
	> File Name: table.c
	> Author: dong xu
	> Mail: gwmxyd@163.com 
	> Created Time: 2015年11月21日 星期六 12时29分40秒
 ************************************************************************/

#include<stdio.h>

#include "table/table.h"
#include "env/env.h"

int readFooter(sequentialFile* psFile,Footer* footer)
{
	char footerSpace[48];
	long filesize;
	//readSFile(48,psFile,)
	filesize = getFilesize(psFile);
	
	//readSFile = 
	printf("file size = %ld\n",filesize);
	
	return 0;
}

