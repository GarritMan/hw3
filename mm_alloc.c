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

s_block_ptr get_block (void *p){
	s_block_ptr head=HeadPtr;
	//printf("in get_block\n");
	while(head){
		if(head->block==p){
			return head;
		}
		head=head->next;
	}
	//null if we cant find it.
	return NULL;
	
}

s_block_ptr fusion(s_block_ptr b){
	if((b->next)->free==1){
		b->size=b->size+sizeof(s_block)+(b->next)->size;
		b->next=(b->next)->next;
		
	}
	if((b->prev)->free==1){
		(b->prev)->next=b->next;
		(b->prev)->free=b->free;
		(b->prev)->size=(b->prev)->size + sizeof(s_block) + b->size;
		
		return b->prev;
	}
	
	return b;
}

void split_block (s_block_ptr b, size_t s){
	
}


void* mm_malloc(size_t size){
	//printf("in malloc 1\n");
	s_block_ptr head=HeadPtr;
	s_block_ptr prev=NULL;
	//printf("in malloc 2\n");
	while(head){
		//printf("in malloc 3\n");
		if(head->free==1 && head->size >=size){
			
			head->free=0;
			return head->block;
			//do Something
			return NULL;
		}else{
			prev=head;
			head=head->next;
		}
	}
	
	//If we exited the loop, we know that we didnt find a suitable space, and so we need to move the break pt.
	head=extend_heap(prev,size);
	if(!head){
		exit(EXIT_FAILURE);
	}
	return head->block;
	
}

void* mm_realloc(void* ptr, size_t size){
	
#ifdef MM_USE_STUBS
    return realloc(ptr, size);
#else
#error Not implemented.
#endif
}

void mm_free(void* ptr){
	s_block_ptr curr=get_block(ptr);
	//printf("in free 1\n");
	if(curr){
		if(curr->next==NULL){
			//printf("in free 2\n");
			if(curr->prev){
				//printf("in free 4\n");
				(curr->prev)->next=NULL;
			}else{
				HeadPtr=NULL;
			}
			//printf("in free 5\n");
			sbrk(-(curr->size + sizeof(s_block)));
		}else{
			//printf("in free 3\n");
			curr->free=1;
			fusion(curr);
		}
	}
}













