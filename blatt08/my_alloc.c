#include <stdlib.h>
#include <stdio.h>
#include "my_alloc.h"
#include "my_system.h"

#define BLOCKSIZE 8192

typedef struct memnode {
  size_t size;
	struct memnode* next;
}memnode;

memnode *root;
memnode *node;

memnode* successor(memnode* p) {
	return (memnode*) ((char*)(p+1) + p->size);
}

bool is_allocated(memnode* ptr) {
	if (ptr == root) return false;
	if (ptr->next > ptr) return false;
	if (ptr->next != root) return true;
	if (root->next > ptr) return true;
	return root->next == root;
}

void init_my_alloc() {
	root = (memnode *)get_block_from_system();
	printf("root: %lu\n",(long)root);
	printf("last: %lu\n",(long)((char *)root+BLOCKSIZE));
	root->size = 0;
	root->next = successor(root);
	root->next->size = BLOCKSIZE-2*sizeof(memnode);
	root->next->next = root;
	printf("next: %lu\n",(long)root->next);
	printf("size: %lu\n",(long)root->next->size);
	
	node = root;
}

void* my_alloc(size_t size) {
	printf("size: %lu\n",(long)size);
	
	memnode* prev = node; memnode* ptr = prev->next;
	do {
		if (ptr->size >= size) break;
			prev = ptr; ptr = ptr->next;
	} while (ptr != node);
	
	if (ptr->size < size) return 0;
	if (ptr->size < size + 2*sizeof(memnode)) {
		node = ptr->next;
		prev->next = ptr->next;
		
		for (memnode* p = prev; p < ptr; p = successor(p)) {
			prev = p;
		}
		
		ptr->next = prev;
		//printf("ret:  %lu\n",(long)(ptr+1));
		return (void*) (ptr+1);
	}
		
	node = ptr;
	printf("ptr:  %lu\n",(long)ptr);
	memnode* newnode = (memnode*)((char*)ptr + ptr->size - size);
	newnode->size = size; newnode->next = ptr;
	memnode* next = successor(ptr);
	
	if (next < (memnode *)((char *)root + BLOCKSIZE) && next->next == ptr) {
		next->next = newnode;
	}
	
	ptr->size -= size + sizeof(memnode);
	printf("new:  %lu\n",(long)(newnode+1));
	return (void*) (newnode+1);
}

void my_free(void* ptr) {
	printf("free: %lu\n",(long)ptr);
	if (!ptr) return;
	memnode* node = (memnode*) ((char*) ptr - sizeof(memnode));
	
	memnode* prev = node->next;
	while (is_allocated(prev)) {
		prev = prev->next;
	}
	
	node->next = prev->next; prev->next = node;
	
	//join_if_possible(node, node->next);
	//join_if_possible(prev, node);
}
