/* A simple test harness for memory alloction. */

#include "mm_alloc.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

void mem_visualiser(){
	s_block_ptr head=HeadPtr;
	while(head){
		size_t s=head->size;
		void * p=head->block;
		
		printf("#################\n");
		printf("size: %d\n",(int) s);
		printf("data ptr: %p\n",p);
		printf("free: %d\n",head->free);
		printf("-------------------\n");
		void* i;
		for(i=head->block; i < ( (head->next == NULL)?sbrk(0):head->next ) ;i++){
			printf("_____BYTE_____\n");
			
		}
		
		head=head->next;
	}
}

int main(int argc, char **argv)
{
    int *data1;
    char *data2;
    int *data3;
    int *data4;
    int *data5;
    int *data6;
    int *data7;
    
	//printf("break pt: %d\n",sbrk(0));
   	
   	data1= (int *) mm_malloc(8);
   	
   	data2= (char *) mm_malloc(5);
   	
   	data3=(int *) mm_malloc(60);
   	
   	data6= (int *) mm_malloc(4);
   	
   	data7= (int *) mm_malloc(4);
   	//data4=(int *) mm_realloc(data2,4);
   	
   	//mm_free(data2);
    
    data5=(int *) mm_realloc(data3,8);
    
    mm_free(data6);
    
    mem_visualiser();
    
    
   	
    printf("\n\nmalloc sanity test successful!\n");
   
    return 0;
}
