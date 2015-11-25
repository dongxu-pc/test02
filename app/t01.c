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

#include "debtools.h"

int main()
{
	FILE* fp;
	fp = fopen("src.ldb","r");
	if(fp == NULL){
		printf("error!\n");
		return 1;
	}
	char a[] = "src.ldb";
	Slice filename;
	Footer footer;
	setSlice(&filename,a,strlen(a));
	sequentialFile* psFile = (sequentialFile*)malloc(sizeof(sequentialFile));
	setSequentialFile(psFile,fp,&filename);
	
	readFooter(psFile,&footer);

	fclose(fp); 
	printf("Hello World!\n");

	return 0;
}
