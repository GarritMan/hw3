/*
 * mm_alloc.c
 *
 * Stub implementations of the mm_* routines. Remove this comment and provide
 * a summary of your allocator's design here.
 */

#include "mm_alloc.h"

#include <stdlib.h>

/* Your final implementation should comment out this macro. */
#define MM_USE_STUBS

s_block_ptr HeadPtr=NULL;

s_block_ptr extend_heap (s_block_ptr last , size_t s){
	void* p=sbrk(s+sizeof(s_block));
		
	if(!(p==(void *)-1)){
		
		s_block_ptr newBlock = (s_block_ptr) p;
		if(last){
			last->next=newBlock;
			
		}else{
			HeadPtr=newBlock;
		}
		
		
		
		newBlock->prev=last;
		newBlock->next=NULL;
		newBlock->size=s;
		newBlock->block=p+sizeof(s_block);
		//last->ptr=p;
		newBlock->free=0;
		
		return newBlock;
	}
	//Return Null on error
	return NULL;
}


void* mm_malloc(size_t size)
{
	s_block_ptr head=HeadPtr;
	s_block_ptr prev=NULL;
	
	while(head){
		if(head->free==1 && head->size >=size){
			//do Something
			return NULL;
		}else{
			prev=head;
			head=head->next;
		}
	}
	
	//If we exited the loop, we know that we didnt find a suitable space, and so we need to move the break pt.
	head=extend_heap(prev,size);
	
	return head->block;
	
}

void* mm_realloc(void* ptr, size_t size)
{
#ifdef MM_USE_STUBS
    return realloc(ptr, size);
#else
#error Not implemented.
#endif
}

void mm_free(void* ptr)
{
#ifdef MM_USE_STUBS
    free(ptr);
#else
#error Not implemented.
#endif
}
