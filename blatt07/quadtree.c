#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

struct quad_tree{
	char colour; //Entweder w oder b oder m für mixed
	int size; //gibt größe des Teilbildes an
	struct quad_tree *child[4]; //child[0] = links oben, child[0] = rechts oben, child[0] = links unten, child[0] = rechts unten,
};

typedef struct quad_tree node;

void initTree(node **root, int size, char initcolour);
void insert(node **quad_tree, int size, char colour, int row, int col);
void printTreeImage(node *tree, int size);
void printImage(node *tree, int row, int col, int size);
void printTree(node *tree);

int main(){
	node *root = NULL;
	
	initTree(&root, 4, 'b');
	
	
	insert(&root,4,'w',2,0);
	insert(&root,4,'w',1,2);
	insert(&root,4,'w',2,1);
	
	//printf("%c\n",root->child[1]->child[2]->colour);
	
	printTreeImage(root,4);
	printTree(root);
	printf("\n");
	
	insert(&root,4,'b',2,0);
	insert(&root,4,'b',1,2);
	
	printTreeImage(root,4);
	printTree(root);
	printf("\n");
	
	return 0;
}

void printTree(node *tree){
	
	if(!tree->child[0]){
		printf("%c",tree->colour);
		return;
	}else{
		printf("[");
		
		for(int i=0;i<4;i++){
			printTree(tree->child[i]);
		}
		
		printf("]");
		return;
	}
}

//druckt den kompletten Baum aus
void printTreeImage(node *tree, int size){
	for(int row=0;row<size;row++){
		for(int col=0; col<size; col++){
			//printf("Koordinaten: r:%d c:%d",row,col);
			printImage(tree, row, col, size);
		}
		printf("\n");
	}
}


//druckt 1 blatt des Baumes
void printImage(node *tree, int row, int col, int size){
	
	//Fall:Baum ist leer
	if(!tree){
		printf("Error\n");
		return;
	}
	//Fall: Man befindet sich in Blatt(=Ziel)
	if(tree->child[0]==NULL){
		if(tree->colour == 'b'){
			printf("#");
			return;
		}
		if(tree->colour == 'w'){
			printf(" ");
			return;
		}
		if(tree->colour == 'm'){
			printf("M");
			return;
		}
	}else{
		
		int pos = (row>=(size/2))*2+(col>=(size/2)); //Bestimmt in welchem Quadranten sich row/col befindet
		
		printImage(tree->child[pos], row%(size/2), col%(size/2), size/2);
		return;
	}
}

//erstellt die wurzel des baumes
void initTree(node **root, int size, char initcolour){
	(*root) = (node *)malloc(sizeof(node));
	(*root)->size = size;
	(*root)->colour = initcolour;
}

//setzt die gegebene Farbe an die Position row/col
void insert(node **quad_tree, int size, char colour, int row, int col){
	
	node *temp = *quad_tree;
	
	//Dieser Fall sollte nicht mehr auftreten
	/*if(temp==NULL){
		temp = (node *)malloc(sizeof(node));
		temp->size=size;
	}*/
	
	//insert setzt colour immer an blätter der size = 1
	if(size==1){
		temp->size=size;
		temp->colour=colour;
		//printf("set size = %d, set colour = %c\n",temp->size,temp->colour);
	}else{
		//printf("size = %d\n",temp->size);
		int pos = (row>=(size/2))*2+(col>=(size/2)); //Bestimmt in welchem Quadranten sich row/col befindet
		//printf("pos = %d\n",pos);
		
		for(int i=0;i<4;i++){
			
			//Fügt neue Blätter hinzu, falls noch keine existieren, mit der Farbe der übergeordneten Node
			if(temp->child[i]==NULL){
				node *temp2 = (node *)malloc(sizeof(node));
				temp2->size=size/2;
				temp2->colour=temp->colour;
				temp->child[i]=temp2;
			}
			
			//Geht weiter in den Teilbaum
			if(i==pos){
				insert(&(temp->child[i]),size/2,colour,row%(size/2),col%(size/2));
			}
			
		}
		
		//Überprüft ob 4 gleiche Kinder existieren und ersetzt sie gegebenenfalls
		int b=0,w=0;
		for(int i=0;i<4;i++){
			if(temp->child[i]!=NULL&&temp->child[i]->colour=='b'){
				b++;
			}else if(temp->child[i]!=NULL&&temp->child[i]->colour=='w'){
				w++;
			}
		}
		
		if(b==4){
			temp->colour='b';
			//printf("current size = %d, current colour = %c\n",temp->size,temp->colour);
			
			for(int i=0;i<4;i++){
				free(temp->child[i]);
				temp->child[i]=NULL;
			}
			
		}else if(w==4){
			temp->colour='w';
			//printf("current size = %d, current colour = %c\n",temp->size,temp->colour);
			
			for(int i=0;i<4;i++){
				free(temp->child[i]);
				temp->child[i]=NULL;
			}
			
		}else{
			temp->colour='m';
		}
		
		//printf("current size = %d, current colour = %c\n",temp->size,temp->colour);
	}
	
	*quad_tree=temp; //Übernimmt Neue Baumstruktur
}