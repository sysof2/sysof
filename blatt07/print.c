#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

struct tree{
    char colour; //Entweder W oder B oder M fuer Mixed
    int size; //gibt groesse des Teilbildes an
    struct tree *left_up, *left_down, *right_up, *right_down;
    //int numberW; //speichert die Anzahl an weissen Pixeln, welche sich in diesem teilbaum befinden(bei 'M' interessant)
};

typedef struct tree node;

void insert(node **tree, char colour, int size) {
    node *temp = NULL;
    if(!(*tree)){
        temp = (node *)malloc(sizeof(node));
        temp->left_up = temp->left_down = temp->right_up = temp->right_down = NULL;
        temp->colour = colour;
        temp->size = size;
        //printf("%c\n",temp->colour);
        *tree = temp;
        return;
    }else{
        
    }
    
}

void printImage(node *tree, int row, int column, int size){
    //Fall:Baum ist leer
    if(!tree){
        printf("Error\n");
        return;
    }
    //Fall: Man befindet sich in Blatt(=Ziel)
    if(tree->left_down==NULL){
        if(tree->colour == 'B'){
            printf("#");
            return;
        }
        if(tree->colour == 'W'){
            printf(" ");
            return;
        }
        if(tree->colour == 'M'){
            printf("M");
            return;
        }
    }
    //Fall:Man befindet sich noch nicht in Blatt...
    if(tree->left_down !=NULL){
        //links oben
        if(row<size/2 && column<size/2){
            printImage(tree->left_up, row, column, size/2);
            return;
        }
        //rechts oben
        if(row<size/2 && column>=size/2){
            printImage(tree->right_up, row, column-size/2, size/2);
            return;
        }
        //links unten
        if(row>=size/2 && column<size/2){
            printImage(tree->left_down, row-size/2, column, size/2);
            return;
        }
        //rechts unten
        if(row>=size/2 && column>=size/2){
            printImage(tree->right_down, row-size/2, column-size/2, size/2);
            return;
        }
    }
}

void printTree(node *tree, int row, int column, int size){
//i
}

int main(){
    node *root = NULL;
    /*
       ##
        #
     ####
     ####
     */
    int size = 4;
    insert(&root,'B',4);
    insert(&(root->left_up),'W',2);
    insert(&(root->right_up),'M',2);
    insert(&(root->left_down),'B',2);
    insert(&(root->right_down),'B',2);
    node *temp = root->right_up;
    insert(&(temp->left_up),'B',1);
    insert(&(temp->right_up),'B',1);
    insert(&(temp->left_down),'W',1);
    insert(&(temp->right_down),'B',1);
    
    //printf("size von root: %d\n", root->size);
    for(int row=0;row<size;row++){
        for(int column=0; column<size; column++){
            //printf("Koordinaten: r:%d c:%d",row,column);
            printImage(root, row, column, size);
        }
        printf("\n");
    }
    
    printf("\n");
    return 0;
}
