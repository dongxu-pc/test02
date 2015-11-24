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
 
 typedef struct sequentialFile{
	 const struct Slice* filename_;
	 FILE* file_;
 } *psFile_;
 
 /**
   *初始化sequentialFile
   **/
 int setSequentialFile(struct sequentialFile* psFile,FILE* file,struct Slice* fileName){
	 if(file == NULL || fileName == NULL){
		 return 1;
	 }
	 psFile->file_ = file;
	 psFile->filename_ = fileName;
	 return 0;
 }
 
 /**
   * 自psFile中读取n字节的数据，结果存储在result中，
   **/
 
 int readSFile(size_t n,struct sequentialFile* psFile,struct Slice* result,char* scratch)
 {
	 size_t r = fread_unlocked(scratch, 1, n, psFile->file_);
     setSlice(scratch, result,r);
	 if (r < n) {
      if (feof(psFile->file_)) {
        // We leave status as ok if we hit the end of the file
      } else {
        // A partial read with an error: return a non-ok status
       printf("Error!,readFile\n");
	   exit(-1);
      }
    }
	return r;
 }
 
#endif