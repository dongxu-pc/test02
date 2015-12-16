/*************************************************************************
	> File Name: format.h
	> Author: dong xu
	> Mail: gwmxyd@163.com 
	> Created Time: 2015年11月21日 星期六 10时54分33秒
 ************************************************************************/

#include "stdint.h"
#include "stddef.h"

#include "slice.h"

#ifndef __SSTABLE_CUDA_TABLE_FORMAT_H__
#define __SSTABLE_CUDA_TABLE_FORMAT_H__

typedef struct{
	Slice key_;
	uint64_t seq_;
} KeySeq;

/* 将key解析为key1和key2（sequence）两个部分那 */
inline size_t parseKey(const Slice* key,KeySeq* keyseq);

/* 在key中添加序列号8个字节 */
inline size_t appendKey(const KeySeq* keyseq,Slice* key);

/* 用prekey的共享前缀生成key的共享前缀 */
inline size_t encodePrefix(const Slice prekey,Slice* key,uint64_t sharedlen);

/* key1 > key2 则为1，反之为2，相等则为0 */
inline size_t compareKey(Slice key1,Slice key2);

/* sequenceNum是随着数据项增加，越大说明越新 */
inline size_t comparaKeySequence(Slice key1,Slice key2);

#endif
