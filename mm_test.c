/* A simple test harness for memory alloction. */

#include "mm_alloc.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char **argv)
{
    int *data1;
    char *data2;
    int *data3;
    int *data4;
    
    printf("size of: %d\n",sizeof(s_block));
	printf("break pt: %d\n",sbrk(0));
   	
   	data1 = (int*) mm_malloc(4);
   	printf("break pt: %d\n",sbrk(0));
   	
   	data4 = (int*) mm_malloc(4);
   	printf("break pt: %d\n",sbrk(0));
   	
   	mm_free(data4);
   	printf("break pt: %d\n",sbrk(0));
   	
   	data2 = (char*) mm_malloc(6);
   	printf("break pt: %d\n",sbrk(0));
   	
   	data3 = (int*) mm_malloc(4);
   	printf("break pt: %d\n",sbrk(0));
    
    *data1 = 1;
   	
   	data2="hello";
   	
   	*data3=10;
   	
   	printf("data1:%d ,data2:%s ,data3:%d \n",*data1,data2,*data3);
   	
    printf("malloc sanity test successful!\n");
    //void *pt=sbrk(0);
    //void *pt2=sbrk(1);
    //char a=*((char*) (pt+4095));
    //printf("success\n");
    //printf("sbrk ptr: %p\n",((char *)(sbrk(0))));
    //printf("sbrk ptr: %c\n",*((char *)(sbrk(1)+4094)));
    return 0;
}
