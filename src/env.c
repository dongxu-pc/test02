/*************************************************************************
	> File Name: env.c
	> Author: dong xu
	> Mail: gwmxyd@163.com 
	> Created Time: 2015年11月22日 星期六 09时36分48秒
 ************************************************************************/
 
 #include "env.h"
 
 /**
   *初始化sequentialFile
   **/
 int setSequentialFile(sequentialFile* psFile,FILE* file,Slice* fileName){
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
 int readSFile(size_t n,long start,const sequentialFile* psFile,unsigned char* scratch)
 {
	 fseek(psFile->file_,start,SEEK_SET);
	 printf("Read Block,start %ld,size %ld\n",start,n);
	 size_t r = fread_unlocked(scratch, 1, n, psFile->file_);
     /*setSlice(result,scratch, r);*/
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

/**
 * 获取文件的大小
 */
long getFilesize(const sequentialFile* psFile)
{
	if(psFile->file_ == NULL){
		printf("getFileSize Failed!\n");
		exit(-1);
	}
	long current,filesize;
	current = ftell(psFile->file_);
	fseek(psFile->file_,0L,SEEK_END);
	filesize = ftell(psFile->file_);
	fseek(psFile->file_,current,SEEK_SET);
	
	return filesize;
}


