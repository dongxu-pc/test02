/*************************************************************************
	> File Name: format.h
	> Author: dong xu
	> Mail: gwmxyd@163.com 
	> Created Time: 2015年11月21日 星期六 10时54分33秒
 ************************************************************************/

#include "stdio.h"
#include "stdlib.h"
#include "stdint.h"
#include "stddef.h"
#include "string.h"
#include "assert.h"

#include "format.h"
#include "slice.h"
#include "coding.h"



/* 将key解析为key1和key2（sequence）两个部分那 */
inline size_t parseKey(const Slice* key,KeySeq* keyseq)
{
	if(key == NULL){
		return -1;
	}
	keyseq->seq_ = decodeFixed64(key->data_+key->size_-8);
	setSlice(&keyseq->key_,key->data_,key->size_-8);

	return 0;
}

/* 在key中添加序列号8个字节 */
inline size_t appendKey(const KeySeq* keyseq,Slice* key)
{
	size_t len = keyseq->key_.size_+8;
	unsigned char b[8];
	if(len < key->length){
		cpySlice(key,&keyseq->key_);
		encodeFixed64(b,keyseq->seq_);
		printf("encode %lu is %lu\n",keyseq->seq_,decodeFixed64(b));
		memcpy(key->data_+keyseq->key_.size_,b,8);
		key->size_ = keyseq->key_.size_+8;
		return key->size_;
	}else{
		/*分配存储空间*/
		if(key->data_ != NULL)
        	free(key->data_);
    	else
        	printf("resetSlice NULL\n");
    	key->data_ = (unsigned char*)malloc(len);
    	assert(key->data_);
    	key->length = len;
		/*拷贝数据*/
		cpySlice(key,&keyseq->key_);
		encodeFixed64(b,keyseq->seq_);
		printf("encode %lu is %lu\n",keyseq->seq_,decodeFixed64(b));
		memcpy(key->data_+keyseq->key_.size_,b,8);
		key->size_ = keyseq->key_.size_+8;
		return key->size_;
	}
}


inline size_t encodePrefix(const Slice* prekey,Slice* key,uint64_t sharedlen)
{
	size_t offset = 0;
	//uint64_t i = 0;
	uint64_t sharedKeyLen = decodeVarint(key->data_,&offset);
	uint64_t nosharedKeyLen = decodeVarint(key->data_,&offset);
	uint64_t valueLen = decodeVarint(key->data_,&offset);
	uint64_t i = 0;
	if(sharedKeyLen <= sharedlen){
		return sharedlen;
	}else{
		for(i = sharedKeyLen;i < prekey->size_;i++){
			if(prekey->data_[i] == key->data_);
		}
		
	}
	
}


/* key1 > key2 则为1，反之为2，相等则为0 */
inline size_t compareKey(Slice key1,Slice key2)
{
	size_t min_size = (key1.size_ > key2.size_) ? key2.size_ : key1.size_;
	size_t i;
	/* 比较key */
	for(i = 0;i < (min_size-8);i++){
		if(key1.data_[i] > key2.data_[i]){
			return 1;
		}else if(key1.data_[i] < key2.data_[i]){
			return 2;
		}
	}
	
	if(key1.size_ < key2.size_){
		return 1;
	}else if(key1.size_ > key2.size_){
		return 2;
	}
	
	return 0;
}

/* sequenceNum是随着数据项增加，越大说明越新 */
inline size_t comparaKeySequence(Slice key1,Slice key2)
{
	if(decodeFixed64(key1.data_+key1.size_-8) > decodeFixed64(key2.data_+key2.size_-8)){
		return 1;
	}else{
		return -1;
	}
}
