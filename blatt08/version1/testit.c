#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include <unistd.h>
#include "my_alloc.h"
#include "my_system.h"

//sysblocksize: 8192

struct control{
	char bit_alloc[124];
	char bit_size[124];
	char *next;
};

typedef struct control memnode;

void printBits(size_t const size, void const * const ptr)
{
	unsigned char *b = (unsigned char*) ptr;
	unsigned char byte;
	int i, j;
	
	for (i=size-1;i>=0;i--)
	{
		for (j=7;j>=0;j--)
		{
			byte = (b[i] >> j) & 1;
			printf("%u", byte);
		}
	}
	puts("");
}

int main (){
	
	init_my_alloc();
	
	char *ptr1 = my_alloc(8);
	
	printf("pos got: %lu\n",(long)ptr1);
	
	//*ptr = 5;
	//printf("%d\n",*ptr);
	
	memnode *control_block = (memnode *)((char *)ptr1-256);

	//printf("pos1got: %lu\n",(long)control_block);
	
	char firstseg = control_block->bit_alloc[0];
	
	printBits(sizeof(firstseg), &firstseg);
	
	
	char *ptr2 = my_alloc(8);
	
	printf("pos got: %lu\n",(long)ptr2);
	firstseg = control_block->bit_alloc[0];
	printBits(sizeof(firstseg), &firstseg);
	
	
	char *ptr3 = my_alloc(8);
	
	printf("pos got: %lu\n",(long)ptr3);
	firstseg = control_block->bit_alloc[0];
	printBits(sizeof(firstseg), &firstseg);
	
	//printf("ptr2: %p\n",ptr2);
	my_free(ptr2);
	
	firstseg = control_block->bit_alloc[0];
	printBits(sizeof(firstseg), &firstseg);
	
	
	my_alloc(8);
	my_alloc(8);
	my_alloc(8);
	my_alloc(8);
	my_alloc(8);	
	my_alloc(8);
	my_alloc(8);
	char * ptr4 = my_alloc(8);
	my_alloc(8);
	
	printf("firstseg: ");
	firstseg = control_block->bit_alloc[0];
	printBits(sizeof(firstseg), &firstseg);
	printf("lastseg:  ");
	firstseg = control_block->bit_alloc[1];
	printBits(sizeof(firstseg), &firstseg);
	
	my_free(ptr4);
	
	printf("firstseg: ");
	firstseg = control_block->bit_alloc[0];
	printBits(sizeof(firstseg), &firstseg);
	printf("lastseg:  ");
	firstseg = control_block->bit_alloc[1];
	printBits(sizeof(firstseg), &firstseg);
	
	
	return 0;
}
