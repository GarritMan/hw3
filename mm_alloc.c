/*
 * mm_alloc.c
 *
 * Stub implementations of the mm_* routines. 
 * 
 * The Design of this memory allocator is in essence a first fit model. However there are some twists.
 * 
 * The general routine is as follows:
 * 
 * 1. The user asks for some memory by calling mm_malloc(size). If this is the first time memory is being asked
 * for then extend_heap is called and the memory is given with the exact asked for size. If it is not the first 
 * time, then mm_malloc will search through the linked list of previously allocated memory blocks. If it finds a 
 * block that is free and big enough for what the user wants, then is will attempt to split the block. If the 
 * block is not big enough as is to provide a reasonable free portion once split (this means enough space for the
 * struct data and atleast one words length of space for user data) then the block is given to the user as is even
 * if it is bigger than what was asked for). If mm_malloc cannot find an appropriate free block, then it will 
 * extend the heap.
 * 
 * 2. When the user frees a block, the mm_free function will find the block corresponding to the pointer given
 * and when it finds it; if it is the very last block that was allocated, then it will release the block back to
 * the operating system by pushing back the heap break pointer. If is not the last block allocated, then is will 
 * free the block and it will look at the block before , and after it's self and if either or both of those are
 * it will merge them into one big free block.
 * 
 * 3. When the user asks to reallocate a previously allocated memory block; if they want to increase its size, then
 * a new block is allocated that is the appropriate size and the old block is freed and its contents copied to the 
 * new block. If they want to decrease size and the size as is, is big enough to split the the block effectively 
 * then it will split the block and return the correct size block. If the block is not big enough to split, then
 *  it is given back to the user as is.
 * 
 */

#include "mm_alloc.h"

#include <stdlib.h>

/* Your final implementation should comment out this macro. */
//#define MM_USE_STUBS

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
	if( (b->next)->free ==1 ){
		b->size=b->size+sizeof(s_block)+(b->next)->size;
		b->next=(b->next)->next;
		(b->next)->prev=b;
	}
	
	if( (b->prev)->free ==1 ){
		
		(b->prev)->next=b->next;
		(b->next)->prev=b->prev;
		(b->prev)->free=b->free;
		(b->prev)->size=(b->prev)->size + sizeof(s_block) + b->size;
		
		return b->prev;
	}
	
	return b;
}

void split_block (s_block_ptr b, size_t s){
	if(b && s >= sizeof(void *)){
		if(  b->size - s  >= sizeof(s_block) + sizeof(void*) ){
			s_block_ptr newP = (s_block_ptr) (b->block + s);
			newP->next = b->next;
			(newP->next)->prev=newP;
			b->next=newP;
			newP->prev=b;
			newP->size=b->size - s - sizeof(s_block);
			b->size = s;
			newP->block= b->block + s + sizeof(s_block);
			
			mm_free(newP->block);
			
		}
	}
}


void* mem_copy(s_block_ptr oldB, s_block_ptr newB){
	if( oldB && newB){
		char * oStart= (char *) oldB->block;
		char * nStart=(char *) newB->block;
		int i;
		for(i=0;i<oldB->size;i++){
			*(nStart + i)=*(oStart + i);
		}
		
		return newB->block;
		
	}
	
	return NULL;
}


void* mm_malloc(size_t size){
	
	s_block_ptr head=HeadPtr;
	s_block_ptr prev=NULL;
	
	while(head){
		if(head->free==1 && head->size >=size){
			mm_realloc(head->block,size);
			head->free=0;
			return head->block;
			
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

/*mm_realloc:
 * If want bigger space then acts the same as malloc but copies over contents of old block to a new, more
 * approproate block.
 * If smaller block is needed but current block isnt big enough to split into two viable blocks, then nothing is
 * done.
 * If current clock is big enough then the block is split accordingly
 */
void* mm_realloc(void* ptr, size_t size){
		s_block_ptr curr=get_block(ptr);
		
		if(curr){
			if(size > curr->size){
				void* p=mm_malloc(size);
				s_block_ptr newP=get_block(p);
				if(newP){
					p=mem_copy(curr,newP);
					mm_free(curr->block);
					return p;
				}
				
				
					
			}else if(size < curr->size){
				split_block(curr,size);
				return curr->block;
			}else{
				return curr->block;
			}
		}
		return NULL;
}

void mm_free(void* ptr){
	s_block_ptr curr=get_block(ptr);
	
	if(curr){
		if(curr->next==NULL){
			
			if(curr->prev){
				
				(curr->prev)->next=NULL;
			}else{
				HeadPtr=NULL;
			}
			
			sbrk(-(curr->size + sizeof(s_block)));
		}else{
			
			curr->free=1;
			fusion(curr);
		}
	}
}













