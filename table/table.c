/*************************************************************************
	> File Name: table.c
	> Author: dong xu
	> Mail: gwmxyd@163.com 
	> Created Time: 2015年11月21日 星期六 12时29分40秒
 ************************************************************************/

#include<stdio.h>
#include "slice.h"
#include "env.h"
#ifndef __SSTABLE_CUDA_INCLUDE_TABLE_C__
#define __SSTABLE_CUDA_INCLUDE_TABLE_C__

int readFooter(struct sequentialFile* psFile,struct Footer* footer)
{
	char footerSpace[48];
	readSFile(48,psFile,)
	
}

#endif
