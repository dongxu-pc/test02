


#include "stddef.h"
#include "stdint.h"
#include "stdio.h"
#include "stdlib.h"

#include "block.h"

#include "slice.h"
#include "table.h"
/*
int readDataIndexBlock(sequentialFile* psFile,const Footer* pfooter)
{
	
}*/
size_t readBlockEntry(const Block* block,BlockEntry* blockEntry,size_t* offset,Slice* lastKey)
{
	size_t tmp;
	if((*offset) < block->restart_offset){
		tmp = decodeBlockEntry(blockEntry,block->data_+(*offset),lastKey);
		*offset += tmp;
		return *offset;
		
	}
	return 0;
}

