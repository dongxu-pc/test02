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

/* 将key解析为key1和key2（sequence）两个部分那 */
inline size_t parseKey(const Slice* key,Slice* key1,Slice* key2);

/* 在key中添加序列号8个字节 */
inline size_t appendKey();

/* key1 > key2 则为1，反之为-1，相等则为0 */
inline size_t comparaKey(Slice key1,Slice key2);

/* sequenceNum是随着数据项增加，越大说明越新 */
inline size_t comparaKeySequence(Slice key1,Slice key2);

#endif
