#include <stdlib.h>
#include <stdio.h>
#include "my_alloc.h"
#include "my_system.h"

#define BLOCKSIZE 8192

typedef struct memnode {
	size_t size;
	struct memnode* next;
} memnode;

memnode *root;
memnode *node;

memnode* successor(memnode* p) {
   return (memnode*) ((char*)(p+1) + p->size);
}

void init_my_alloc() {
	root = (memnode *)get_block_from_system();
	printf("root: \t\t%lu\n",(long)root);
	printf("last+1: \t%lu\n",(long)((char *)root+BLOCKSIZE));
	root->size = 0;
	root->next = successor(root);
	root->next->size = BLOCKSIZE-2*sizeof(memnode);
	root->next->next = NULL;
	printf("root next: \t%lu\n",(long)root->next);
	printf("root next size: %lu\n",(long)root->next->size);
	node = root->next;
	printf("node: \t\t%lu\n",(long)node);
	printf("node size: %lu\n",(long)node->size);
}

void* my_alloc(size_t size) {
	printf("alloc size : %lu\n",(long)size);
	printf("node: \t\t%lu\n",(long)node);
	printf("node size: %lu\n",(long)node->size);
	
	memnode *temp=node;
	memnode *prev=NULL;
	
	//search for free memory in list, starting with node
	while(temp!=NULL&&size<temp->size){
		//printf("temp: \t\t%lu\n",(long)temp);
		prev = temp;
		temp=temp->next;
		//printf("temp: \t\t%lu\n",(long)temp);
	}
	printf("node: \t\t%lu\n",(long)node);
	
	//not enough free memory left
	if(temp==NULL){
		temp = get_block_from_system();
		temp->size = BLOCKSIZE-sizeof(memnode);
		temp->next=NULL;
		if(prev==NULL){		
			//prev != NULL, node should never be NULL
		}else{
			prev->next = temp;
		}
		char *ret = (char *)temp + sizeof(memnode);
		return ret;
	}
	
	//if prev==NULL --> temp==node
	
	//enough free memory left for split
	if(size<=(temp->size-sizeof(memnode)-8)){
		//printf("temp: \t\t%lu\n",(long)temp);
		//printf("temp size: %lu\n",(long)temp->size);
		memnode *retnode = temp;
		
		//temp = new element in right "half" of the free memory block
		temp = successor(retnode);
		temp->size = retnode->size - sizeof(memnode) - size;
		temp->next = retnode->next;
		
		if(prev!=NULL){
			prev->next = temp;
		}
		
		retnode->size = size;
		retnode->next = NULL;
		
		//printf("next: \t\t%lu\n",(long)temp->next);
		//printf("next->size: %lu\n",(long)temp->next->size);
		
		char *ret = (char *)retnode + sizeof(memnode);
		return ret;
	
	//exact fit
	} else if(size == temp->size){
		printf("fits exactly\n");
		memnode *retnode = temp;
		//temp = get_block_from_system();
		
		if(temp->next!=NULL){
			if(prev!=NULL){
				prev->next = temp->next;
			}
		}else{
			if(prev!=NULL){
				prev->next=NULL;
			}else{
				//no prev, create new block
				node = (memnode *)get_block_from_system();
				node->size = BLOCKSIZE;
				node->next = NULL;
			}
		}
		
		char *ret = (char *)retnode + sizeof(memnode);
		return ret;
		
	}
	
	printf("should never reach this\n");
	return NULL;
	
}

void my_free(void* ptr) {
	
	
}
