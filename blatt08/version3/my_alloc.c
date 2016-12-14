#include <stdlib.h>
#include <stdio.h>
#include "my_alloc.h"
#include "my_system.h"
#include <unistd.h>

#define BLOCKSIZE 8192

typedef struct memnode {
	size_t size;
	struct memnode* next;
} memnode;

memnode *root;
memnode *node;

//int totalblockcount=1;

memnode* successor(memnode* p) {
	return (memnode*) ((char*)(p+1) + p->size);
}

void init_my_alloc() {
	root = (memnode *)get_block_from_system();
	//printf("root: \t\t%lu\n",(long)root);
	//printf("last+1: \t%lu\n",(long)((char *)root+BLOCKSIZE));
	root->size = 0;
	root->next = successor(root);
	root->next->size = BLOCKSIZE-2*sizeof(memnode);
	root->next->next = NULL;
	//printf("root next: \t%lu\n",(long)root->next);
	//printf("root next size: %lu\n",(long)root->next->size);
	node = root->next;
	//printf("node: \t\t%lu\n",(long)node);
	//printf("node size: %lu\n",(long)node->size);
	//printf("-------\n");
}

void* my_alloc(size_t size) {
	//printf("alloc size : %lu\n",(long)size);
	//printf("node: \t\t%lu\n",(long)node);
	//printf("node size: %lu\n",(long)node->size);
	//printf("node->next: \t%lu\n",(long)node->next);
	memnode *temp=node;
	memnode *prev=NULL;
	
	//printf("totalblockcount: %d\n",totalblockcount);
	//memnode *test=node;
	//printf("search start.. \n");
	//while(test!=NULL){
	//	test=test->next;
	//}
	//printf("search end\n");
	
	//search for free memory in list, starting with node
	while(temp!=NULL&&size>temp->size){
		//printf("temp: \t\t%lu\n",(long)temp);
		prev = temp;
		temp=temp->next;
		//printf("temp: \t\t%lu\n",(long)temp);
		//if(temp!=NULL) printf("temp->size: \t%lu\n",(long)temp->size);
		//usleep(20000);
	}
	//printf("prev: \t\t%lu\n",(long)prev);
	//printf("temp: \t\t%lu\n",(long)temp);
	//printf("temp->next: \t%lu\n",(long)(temp->next));
	//printf("temp->size: \t%lu\n",(long)temp->size);
	
	//not enough free memory left
	if(temp==NULL){
		printf("temp = NULL, get new block from system\n");
		memnode *retnode = (memnode *)get_block_from_system();
		//totalblockcount++;
		
		retnode->next = NULL;
		retnode->size = size;
		memnode *new = successor(retnode);
		new->size = BLOCKSIZE-2*sizeof(memnode)-size;
		new->next = NULL;
		
		//prev != NULL, node should never be NULL
		prev->next = new;
		
		char *ret = (char *)retnode + sizeof(memnode);
		return ret;
	}
	
	//if prev==NULL --> temp==node
	
	//printf("temp->size: %d\n",(int)temp->size);
	//printf("size: %d\n",(int)(temp->size-sizeof(memnode)-8));
	//enough free memory left for split
	if((long)size<=(long)(temp->size-sizeof(memnode)-8)){
		//printf("enough free memory left for split\n");
		//printf("temp: \t\t%lu\n",(long)temp);
		//printf("temp size: %lu\n",(long)temp->size);
		memnode *retnode = temp;
		
		//temp = new element in right "half" of the free memory block
		temp = (memnode *)((char *)temp+sizeof(memnode)+size);
		temp->size = retnode->size - sizeof(memnode) - size;
		temp->next = retnode->next;
		
		//printf("temp2: \t\t%lu\n",(long)temp);
		//printf("temp2 size: %d\n",(int)temp->size);
		
		if(prev!=NULL){
			prev->next = temp;
			//printf("prev->next: \t%lu\n",(long)prev->next);
		}else{
			node = temp;
		}
		
		retnode->size = size;
		retnode->next = NULL;
		
		//printf("prev: \t\t%lu\n",(long)prev);
		//printf("retnode: \t%lu\n",(long)retnode);
		//printf("retnode->size: %lu\n",(long)retnode->size);
		//printf("node: \t\t%lu\n",(long)node);
		//printf("node->size: \t%lu\n",(long)node->size);
		//printf("node->next: \t%lu\n",(long)node->next);
		
		char *ret = (char *)retnode + sizeof(memnode);
		//printf("ret: \t\t%lu\n",(long)ret);
		//printf("--------\n");
		return ret;
		
		//exact fit
	} else if(size == temp->size || size == temp->size-8 || size == temp->size-16){
		//printf("fits exactly\n");
		memnode *retnode = temp;
		//temp = get_block_from_system();
		
		if(temp->next!=NULL){
			if(prev!=NULL){
				prev->next = temp->next;
			}else{
				node = temp->next;
			}
		}else{
			if(prev!=NULL){
				prev->next=NULL;
			}else{
				//no prev, create new block
				printf("prev = NULL,next = NULL, get new block from system\n");
				node = (memnode *)get_block_from_system();
				
				//totalblockcount++;
				
				node->size = BLOCKSIZE;
				node->next = NULL;
			}
		}
		//printf("node: \t\t%lu\n",(long)node);
		char *ret = (char *)retnode + sizeof(memnode);
		return ret;
		
	}else{
		//printf("does not fit exactly\n");
		
		//printf("temp: \t\t%lu\n",(long)temp);
		//printf("temp->next: \t%lu\n",(long)temp->next);
		//printf("temp->size: \t%lu\n",(long)temp->size);
		
		memnode *retnode = (memnode *)get_block_from_system();
		retnode->next = NULL;
		retnode->size = size;
		memnode *new = successor(retnode);
		new->size = BLOCKSIZE-2*sizeof(memnode)-size;
		new->next = temp->next;
		temp->next = new;
		
		char *ret = (char *)retnode + sizeof(memnode);
		return ret;
		
	}
	
	printf("should never reach this\n");
	return NULL;
	
}

void my_free(void* ptr) {
	//printf("free: \t\t%lu\n",(long)ptr);
	
	memnode *temp = node->next;
	node->next = (memnode *)(ptr-sizeof(memnode));
	node->next->next = temp;
	
	//printf("node: \t\t%lu\n",(long)node);
	//printf("node->next: \t%lu\n",(long)node->next);
	//printf("node->n->n: \t%lu\n",(long)node->next->next);
	
	//printf("---\n");
}
