/*************************************************************************
	> File Name: t01.c
	> Author: dong xu
	> Mail: gwmxyd@163.com 
	> Created Time: 2015年11月21日 星期六 12时13分56秒
 ************************************************************************/

#include <stdio.h>
#include "stdlib.h"
#include "c.h"

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
	setSlice(&filename,a,strlen(a));
	struct sequentialFile* psFile = (struct sequentialFile*)malloc(sizeof(struct sequentialFile));
	setSequentialFile(psFile,fp,&filename);

	fclose(fp); 
	printf("Hello World!\n");

	return 0;
}
