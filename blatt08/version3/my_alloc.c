#include <stdlib.h>
#include "my_alloc.h"
#include "my_system.h"
#include <stdio.h>

#define BLOCKSIZE 8192
#define MINBLOCKSIZE 8

typedef struct memnode {
	size_t size;
	struct memnode* next;
} memnode;

memnode *node;

//prints the complete list of all free elements
void printList(){
	memnode *temp = node;
	printf("list of free elements: ");
	while(temp!=NULL){
		//long ptrval = (long)temp%10000;
		int size = temp->size;
		//printf("%ld,%d --- ",ptrval,size);
		printf("%d - ",size);
		temp=temp->next;
	}
	printf("\n");
}

//returns the directly next element, it may be used or free
memnode* successor(memnode* p) {
	return (memnode*) ((char*)(p+1) + p->size);
}

void init_my_alloc() {
	node = (memnode *)get_block_from_system();
	node->size = BLOCKSIZE-2*sizeof(memnode);
	node->next = NULL;
	
	//printf("node: \t\t%ld\n",(long)node);
	//printf("node: \t\t%ld\n",(long)node%10000);
	//printf("node->next: \t%ld\n",(long)node->next);
	//printf("node->size: %ld\n",(long)node->size);
	//printf("-------\n");
}

void* my_alloc(size_t size) {
	//printf("alloc size : %ld\n",(long)size);
	//printList();
	
	memnode *prev = NULL;
	memnode *temp = node;
	
	//searches for element with enough space or end of list, starting with node
	while(temp!=NULL&&size>temp->size){
		prev = temp;
		temp=temp->next;
	}
	
	//printf("prev: \t\t%ld\n",(long)prev%10000);
	//printf("temp: \t\t%ld\n",(long)temp%10000);
	//printf("temp->size: %ld\n",(long)temp->size);
	//printf("temp->next: \t%ld\n",(long)temp->next%10000);
	
	
	//not enough free memory left, add new block at beginning of list
	if(temp==NULL){
		//printf("alloc size : %ld\n",(long)size);
		//printf("not enough free memory left, getting new block\n");
		//printList();
		
		memnode *newelement = (memnode *)get_block_from_system();
		newelement->size=BLOCKSIZE-sizeof(memnode);
		newelement->next = node;
		node = newelement;
		temp = node;
		prev = NULL;
		//printList();
	}
	
	//enough free memory left for splitting one memory block into two
	if((long)size<=(long)(temp->size-sizeof(memnode)- MINBLOCKSIZE)){
		//printf("enough free memory left for split\n");
		
		//printf("temp: %p\n",temp);
		//printf("temp: %ld\n",(long)temp%10000);
		memnode *newelement = (memnode *)((char *)temp + sizeof(memnode) + size);
		//printf("newnode: %ld\n",(long)newelement%10000);
		
		newelement->next = temp->next;
		newelement->size = temp->size - sizeof(memnode) - size;
		
		temp->size = size;
		temp->next = NULL;
		
		if(prev!=NULL){
			prev->next = newelement;
			
		}else{
			node = newelement;
			
		}
		
		//printList();
		//printf("return ptr: %p\n",(temp+1));
		//printf("return ptr: %ld\n",(long)(temp+1));
		return (void *)(temp+1);
	}
	
	//fits exactly or too small to split
	else{
		//printf("fits exactly\n");
		
		if(prev!=NULL){
			prev->next = temp->next;
			
		}else{
			if(temp->next != NULL){
				node = temp->next;
				
			}else{
				node = (memnode *)get_block_from_system();
				node->size = BLOCKSIZE - sizeof(memnode);
				node->next = NULL;
			}
		}
		temp->next = NULL;
		
		//printList();
		//printf("return ptr: %ld\n",(long)(temp+1));
		return (void *)(temp+1);
	}
	
}

void my_free(void* ptr) {
	//printf("free: \t\t%ld\n",(long)(ptr-1));
	
	//set free memnode at the beginning of the list
	memnode *temp = node;
	node = (memnode *)((char *)ptr-sizeof(memnode));
	node->next = temp;
	
	/*
	memnode *temp = node->next;
	node->next = (memnode *)((char *)ptr-sizeof(memnode));
	node->next->next = temp;
	*/
}
