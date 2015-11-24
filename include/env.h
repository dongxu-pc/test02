/*************************************************************************
	> File Name: env.h
	> Author: dong xu
	> Mail: gwmxyd@163.com 
	> Created Time: 2015年11月22日 星期六 09时36分48秒
 ************************************************************************/
 
#include "stdlib.h"
#include "string.h"
#include "stdio.h"

#include "slice.h"
 
#ifndef __SSTABLE_CUDA_INCLUDE_ENV_H__
#define __SSTABLE_CUDA_INCLUDE_ENV_H__

typedef struct{
	 const Slice* filename_;
	 FILE* file_;
 } sequentialFile,*psFile_;
 
 /**
   *初始化sequentialFile
   **/
 int setSequentialFile(sequentialFile* psFile,FILE* file,Slice* fileName);
 
 /**
   * 自psFile中读取n字节的数据，结果存储在result中，
   **/
 int readSFile(size_t n,long start,sequentialFile* psFile,Slice* result,char* scratch);

/**
 * 获取文件的大小
 */
long getFilesize(sequentialFile* psFile);
 
#endif

