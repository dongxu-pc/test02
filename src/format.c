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

/* 将key解析为key1和key2（sequence）两个部分那 */
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

/* 在key中添加 */
inline size_t appendKey()
/* key1 > key2 则为1，反之为-1，相等则为0 */
inline size_t comparaKey(Slice key1,Slice key2){
	size_t min_size = (key1.size_ > key2.size_t) ? key2.size_ : key1.size_;
	size_t i;
	/* 比较key */
	for(i = 0;i < (min_size-8);i++){
		if(key1.data_[i] > key2.data_[i]){
			return 1;
		}else if(key1.data_[i] < key2.data_[i]){
			return -1;
		}
	}
	
	if(key1.size_ < key2.size_){
		return 1;
	}else if(key1.size_ > key2.size_){
		return -1;
	}
	
	return 0;
}

/* sequenceNum是随着数据项增加，越大说明越新 */
inline size_t comparaKeySequence(Slice key1,Slice key2){
	if(decodeFixed64(key1.data_+key1.size_-8) > decodeFixed64(key2.data_+key2.size_-8)){
		return 1;
	}else{
		return -1;
	}
}
