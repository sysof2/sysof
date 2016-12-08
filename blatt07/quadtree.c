#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define BUFLEN 256

//Auskommentieren und Dateinamen anpassen, falls die Eingabe per Datei erfolgen soll, sonst erfolgt sie über die Standardeingabe
//#define FILENAME "Meerfahrt.txt"

struct quad_tree{
	char colour; //Entweder w oder b oder m für mixed
	int size; //gibt größe des Teilbildes an
	struct quad_tree *child[4]; //child[0] = links oben, child[1] = rechts oben, child[2] = links unten, child[3] = rechts unten,
};

typedef struct quad_tree node;

void initTree(node **root, int size, char initcolour);
void insert(node **quad_tree, int size, char colour, int row, int col);
void printTreeImage(node *tree);
void printImage(node *tree, int row, int col, int size);
void printTree(node *tree);
void read_input(node **root,FILE *file);

int main(){
	node *root = NULL;
	
	#ifdef FILENAME
		FILE *file;
		
		file = fopen(FILENAME,"r");
		
		if(file==NULL) {
			printf("Fehler beim oeffnen der Datei!!!\n");
			return 1;
		}
		
		read_input(&root,file);
		
		fclose(file);
	#endif
		
	#ifndef FILENAME
		read_input(&root,stdin);
	#endif
	
	return 0;
}

//liest die befehle entweder von einer datei oder der standardeingabe ein
void read_input(node **root, FILE *file){
	
	char buf[BUFLEN];
	
	if(fgets(buf, BUFLEN, file)){
		int size = (int) strtol(buf,NULL,10);
		//printf("size: %d", size);
		if(size!=0){
			initTree(root,size,'b'); //initialisiert den Baum als komplett schwarz
		}else{
			printf("input not correct\n");
			return;
		}
	}
	
	while(fgets(buf, BUFLEN, file)){
		//printf("%s",buf);
		if(strncmp(buf, "PUT",3)==0){
			unsigned int x=0,y=0;
			if(strlen(buf)>=7){
				char *ptr;
				ptr = strtok(buf, " ");
				ptr = strtok(NULL, " ");
				
				if(ptr==NULL) continue;
				x = (unsigned int) strtol(ptr,NULL,10);

				ptr = strtok(NULL, " ");
				
				if(ptr==NULL) continue;
				y = (unsigned int) strtol(ptr,NULL,10);

				//printf("x= %u, y=%u\n",x,y);
				insert(root,(*root)->size,'w',x,y);
			}
		}
		else if(strncmp(buf, "PRINT",5)==0){
			if(strlen(buf)>=7){
				if(strncmp(&buf[6],"IMAGE",5)==0){
					//printf("Printed Image!!\n");
					#ifdef FILENAME
						system("clear");
					#endif
					printTreeImage(*root);
					#ifdef FILENAME
						usleep(200000);
					#endif
				}
				else if(strncmp(&buf[6],"TREE",4)==0){
					//printf("Printed Tree!!\n");
					//Die Stringausgabe bei den Bildern wird sehr groß, zur Übersichtlichkeit wird sie daher hier nicht ausgegeben
					#ifndef FILENAME
						printTree(*root);
						printf("\n");
					#endif
				}
			}
		}
		else {
			printf("Invalid Command!!\n");
		}
	}
}

//druckt den baum als string aus
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
void printTreeImage(node *tree){
	for(int row=0;row<tree->size;row++){
		for(int col=0; col<tree->size; col++){
			//printf("Koordinaten: r:%d c:%d",row,col);
			printImage(tree, row, col, tree->size);
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
				for(int k=0;k<4;k++){
					temp->child[i]->child[k]=NULL;
				}
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