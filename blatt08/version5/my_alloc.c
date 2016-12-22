#include <stdlib.h>
#include "my_alloc.h"
#include "my_system.h"
#include <stdio.h>

//8192-8
#define MAXBLOCKSIZE 8184
//#define MAXBLOCKSIZE 8176
#define MINBLOCKSIZE 8

//16 for 8Byte pointers, 8 for 4Byte pointers
#define NODE_SIZE 8

//set 1 to use the joining of elements
//set 0 to not use it
//joining of elements is not used, it is too preformance heavy to get points
//joining has an advantage in: (example) 1M increase cluster or 1M decrease cluster
const char use_join = 0;

typedef struct memnode {
	short size;			//size of memory in bytes
	char prev_pos;		//prev_pos*8-NODE_SIZE gives the position of the predecessor
	char is_free;		//5 != free, 42 = free, 0,1 error conditions
	struct memnode* next;
} memnode;

//34*4 + 4 +1 = 141 Bytes of global variables used
memnode *pool;
memnode *node[33];
int last_join_number;	//only needed for joining

//prints all elements in this list
void printList(memnode *temp){
	//printf("list of elements: ");
	while(temp!=NULL){
		printf("%ld,%d,%d,%d - ",(long)temp%10000,(int)temp->size,(int)temp->is_free,(int)temp->prev_pos);
		temp=temp->next;
	}
	printf("\n");
}

//prints all lists
void printall(){
	printf("-------------------\n");
	for(int i=0;i<33;i++){
		printf("node[%d]: ",i);
		printList(node[i]);
	}
}

//returns the directly next element, it may be used or free
memnode* successor(memnode* p) {
	return (memnode*) ((char*)(p+1) + p->size);
}

//returns the directly prev element, prev_pos has to be != 0
memnode* predecessor(memnode *p){
	return (memnode *)((char *)p-(p->prev_pos)*8-NODE_SIZE);
}

//returns a new chunk from the pool
memnode * get_chunk_from_pool(short size){
	
	memnode *temp = pool;
	
	//not enough free memory left, get new block from system
	if(size+NODE_SIZE+MINBLOCKSIZE>pool->size){
		pool = (memnode *)get_block_from_system();
		pool->size = MAXBLOCKSIZE;
		pool->next = NULL;
		pool->is_free = 5;
		pool->prev_pos = 0;
	}
	
	//enough free memory left in pool for splitting
	if(size+NODE_SIZE+MINBLOCKSIZE<=pool->size){
		
		temp = pool;
		memnode *newelement = (memnode *)((char *)pool + NODE_SIZE + size);
		
		newelement->next = NULL;
		newelement->size = pool->size - NODE_SIZE - size;
		newelement->is_free = 5;
		newelement->prev_pos = size/8;
		
		pool = newelement;
		
		temp->size = size;
		temp->next = NULL;
		temp->is_free = 5;
		
		return temp;
	}
	
	//fits exactly or too small to split
	else{
		return temp;
	}
}

//initializes the pool with a block from the system, pool will always be != NULL
void init_my_alloc() {
	pool = (memnode *)get_block_from_system();
	pool->size = MAXBLOCKSIZE-NODE_SIZE;
	pool->next = NULL;
	pool->is_free = 5;
	pool->prev_pos = 0;
}

void* my_alloc(size_t size) {
	
	//calculates the correspondending list
	int list_number = size/8-1;
	
	//if the current list is empty, search in the next higher list for a free element
	if(node[list_number]==NULL){
		
		memnode *temp=NULL;
		
		//list goes from 0 to 31 if joining not used, else from 0 to 32
		
		int i=list_number;	//start with current list number
		while(i<(32+use_join)&&node[i]==NULL){
			i++;
		}
		
		//free element at node[i]
		if(i<(32+use_join)){
			//enough free memory for splitting
			if(size+NODE_SIZE+MINBLOCKSIZE<=node[i]->size){
				
				temp = node[i];
				
				memnode *newelement = (memnode *)((char *)node[i] + NODE_SIZE + size);
				
				newelement->size = node[i]->size - NODE_SIZE - size;
				newelement->is_free = 42;
				newelement->prev_pos = size/8;
				
				if(use_join){
					successor(newelement)->prev_pos = newelement->size/8; //update successor
				}
				
				int new_list_number = newelement->size/8-1;
				
				//needed if joining of elements is used
				if(new_list_number>31){
					new_list_number=32;
				}
				
				node[i] = node[i]->next;
				
				//add newelement to the lists
				memnode *temp2 = node[new_list_number];
				node[new_list_number] = newelement;
				node[new_list_number]->next = temp2;
				
				temp->size = size;
				temp->next = NULL;
				temp->is_free = 5;
				
			//fits exactly or too small to split
			}else{
				temp = node[i];
				node[i] = node[i]->next;
				
				temp->is_free = 5;
			}
		
		//no free element is in one of the lists, so get new chunk from the pool
		}else{
			temp = get_chunk_from_pool((short)size);
		}
		
		return (void *)(temp+1);
	}
	//list has min 1 free element
	else{
		memnode *temp = node[list_number];
		node[list_number] = node[list_number]->next;
		
		temp->is_free = 5;
		
		return (void *)(temp+1);
	}
}

//joins the first element in node[list_number] with its successor if possible
void join_successor(int list_number){
	
	memnode *node1 = node[list_number];
	
	//successor not free, so no joining possible
	if(successor(node1)->is_free!=42) return;
	
	memnode *successor_node1 = successor(node1);
	
	int node2_list_number = successor_node1->size/8-1;
	
	if(node2_list_number>31){
		node2_list_number=32;
	}

	memnode *prev_node2 = NULL;
	//node2 is in the list of the successor
	memnode *node2 = node[node2_list_number];
	
	int new_list_number=0;
	
	//search for the successor node in the list
	while(node2!=NULL){
		
		if(successor_node1==node2){
			//remove node1, node1 is always at the beginning of the list
			node[list_number] = node1->next;
			
			//remove node2 from its list
			if(prev_node2!=NULL){
				if(prev_node2==node1){
					node[node2_list_number] = node2->next;
				}else{
					prev_node2->next = node2->next;
				}
			}else{
				node[node2_list_number] = node2->next;
			}
			
			//calculate new size
			node1->size = node1->size+node2->size+NODE_SIZE;
			node2->is_free = 5;
			node1->next = NULL;

			new_list_number = node1->size/8-1;
			if(new_list_number>31){
				new_list_number=32;
			}
			
			//adds the new connected elements to the list
			if(node[new_list_number]!=NULL){
				node1->next=node[new_list_number];
				node[new_list_number]=node1;
			}else{
				node[new_list_number]=node1;
			}
			
			successor(node1)->prev_pos = node1->size/8;	//update new successor
			
			last_join_number = new_list_number;
			
			//elements joint
			break;
		}
		
		prev_node2=node2;
		node2 = node2->next;
	}
}

//joins the first element in node[last_join_number] with its predecessor if possible
//node[last_join_number] is the same element who joined with its successor previously
void join_predecessor(){
	
	int list_number=last_join_number;
	memnode *node1 = node[list_number];
	
	memnode *predecessor_node1 = predecessor(node1);
	
	//no predecessor exists
	if(node1->prev_pos==0) return;
	
	//predecessor is not free, so no joining possible
	if(predecessor_node1->is_free!=42) return;
	
	//predecessor->size has to match with prev_pos
	if(predecessor_node1->size!=node1->prev_pos*8) return;

	int node2_list_number = predecessor_node1->size/8-1;
	
	if(node2_list_number>31){
		node2_list_number=32;
	}
	
	memnode *prev_node2 = NULL;
	//node2 is in the list of the successor
	memnode *node2 = node[node2_list_number];
	
	int new_list_number=0;
	
	//search for the predecessor node in the list
	while(node2!=NULL){
		
		if(predecessor_node1==node2){
			
			//remove node1, node1 is always at the beginning of the list
			node[list_number] = node1->next;
			
			//remove node2 from its list
			if(prev_node2!=NULL){
				if(prev_node2==node1){
					node[node2_list_number] = node2->next;
				}else{
					prev_node2->next = node2->next;
				}
			}else{
				node[node2_list_number] = node2->next;
			}
			
			//calculate new size
			node2->size = node1->size+node2->size+NODE_SIZE;
			node1->is_free = 5;
			
			node2->next = NULL;
			
			new_list_number = node2->size/8-1;
			if(new_list_number>31){
				new_list_number=32;
			}
			
			//adds the new connected elements to the list
			if(node[new_list_number]!=NULL){
				node2->next=node[new_list_number];
				node[new_list_number]=node2;
			}else{
				node[new_list_number]=node2;
			}
			
			//elements joint
			break;
		}
		prev_node2=node2;
		node2 = node2->next;
	}
}



void my_free(void* ptr) {
	
	memnode *temp = (memnode *)ptr - 1;
	temp->is_free = 42;
	
	//calculate correspondending list
	int list_number = temp->size/8 - 1;
	
	//add element to the list
	temp->next = node[list_number];
	node[list_number] = temp;
	
	last_join_number=list_number;
	
	if(use_join){
		join_successor(list_number);
		join_predecessor();
	}
}
