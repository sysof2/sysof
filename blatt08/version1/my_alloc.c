#include <stdio.h>
#include <stdlib.h>
#include "my_alloc.h"
#include "my_system.h"

//sizeof(memnode) = 256 (on 64bit system), 252 (on 32bit system)
#define MEMNODE_SIZE 256

//in on block (8192 bytes) from the system fit max 124*8 chunks, 1 chunk = (min) 8 byte
#define BITARRAY_LENGTH 124

struct control{
	char bit_alloc[BITARRAY_LENGTH];
	char bit_size[BITARRAY_LENGTH];
	char *next;
};

typedef struct control memnode;


char *root;

void init_my_alloc() {
	root = (char *)get_block_from_system ();
	
	printf("root:    %lu\n",(long)root);
	//printf("%lu\n",sizeof(memnode));
}

void* my_alloc(size_t size) {
   char *ret = NULL;
   
   memnode *control_block = (memnode *)root;
   
   char *pos = (char *)root+MEMNODE_SIZE;
   
   
   for(int i=0;i<BITARRAY_LENGTH;i++){
	   
	   for(int k=0;k<8;k++){
		   
		   if(!(control_block->bit_alloc[i]&(1<<k))){
			   ret = pos + 8*(k+8*i);
			   control_block->bit_alloc[i] |= (1<<k);
			   return ret;
		   }
		   
	   }
	   
   }
   
   
   return ret;
}

void my_free(void *ptr) {
	
	memnode *control_block = (memnode *)root;
	
	int i = ((long)((char *)ptr-(char *)root-256))/64;
	int k= (((long)((char *)ptr-(char *)root-256))%64)/8;
	
	printf("i: %d, k: %d\n",i,k);
	
	control_block->bit_alloc[i] &= ~(1<<k);
	
	
}
