/*************************************************************************
	> File Name: format.h
	> Author: dong xu
	> Mail: gwmxyd@163.com 
	> Created Time: 2015年11月21日 星期六 10时54分33秒
 ************************************************************************/

#include "stdint.h"
#include "stddef.h"
#include "string.h"
#include "assert.h"

#include "format.h"


inline uint64_t  varToint64(varint* vint)
{
	uint64_t value = 0;
	int i = 0;
	while((vint->value_[vint->size_] & 0x80) == 0x80){
		value += ((uint64_t)(vint->value_[vint->size_]&0x7F)) << (7*i);
		vint->size_++;
		i++;
		assert(i < 8);
	}
	value += ((uint64_t)(vint->value_[vint->size_]&0x7F)) << (7*i);
	vint->size_++;
	
	return value;
}

inline size_t parseKey(const Slice* key,Slice* key1,Slice* key2)
{
	int i = 0;
	if(key == NULL){
		return -1;
	}
	key1->data_ = (unsigned char *)malloc(key->size_ - 8);
	key1->size_ = key->size_ - 8;
	key2->data_ = (unsigned char *)malloc(8);
	key2->data_ = 8;
	for(i ;i < key1->size_;i++){
		key1->data_[i] = key->data_[i];
	}
	for(i = 0;i < 8;i++){
		key2->data_[i] = key->data_[key1->size_ + i];
	}
	
	return 0;
}

inline size_t comparaKey(Slice key1,Slice key2){
	if(key1.size_>key2.size_){
		if(strcmp(key1.data_,key2.data_))
	}
	
}