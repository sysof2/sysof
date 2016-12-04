#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

struct tree{
    char colour; //Entweder W oder B oder M fuer Mixed
    int size; //gibt groesse des Teilbildes an
    struct tree *left_up, *left_down, *right_up, *right_down;
    int numberW; //speichert die Anzahl an weissen Pixeln, welche sich in diesem teilbaum befinden(bei 'M' interessant)
};

typedef struct tree node;

void deltree(node * tree){
    if (tree){
        deltree(tree->left_up);
        deltree(tree->left_down);
        deltree(tree->right_up);
        deltree(tree->right_down);
        free(tree);
    }
}

void insert(node **tree, char colour, int size) {
    node *temp = NULL;
    if(!(*tree)){
        temp = (node *)malloc(sizeof(node));
        temp->left_up = temp->left_down = temp->right_up = temp->right_down = NULL;
        temp->colour = colour;
        temp->size = size;
        if(colour == 'W'){
            temp->numberW = 1;
        }else{
            temp->numberW = 0;
        }
        *tree = temp;
        return;
    }
    
    
}

void insert2(node **tree, int row, int column, int size, char colour){
    node *temp = NULL;
    int number = 0;
    /*if(!(*tree)){
        temp = (node *)malloc(sizeof(node));
        temp->left_up = temp->left_down = temp->right_up = temp->right_down = NULL;
        temp->colour = 'W';
        temp->size = size;
        *tree = temp;
        return;
    }*/
    
    //Fall: size=1 ->Man hat das schwarze Pixel erreicht, welches man in weiss umwandeln will
    if(size==1){
        (*tree)->colour = 'W';
        (*tree)->numberW = 1;
        return;
     }
    
    //Fall Endknoten(Blatt) erreicht. Dieser muss komplett schwarz sein
    if((*tree)->left_down == NULL && (*tree)->colour == 'B'){
        (*tree)->colour = 'M';
        if(row<size/2 && column<size/2){
            insert(&(*tree)->right_down, 'B', size/2);
            insert(&(*tree)->right_up, 'B', size/2);
            insert(&(*tree)->left_up, 'W', size/2);
            insert(&(*tree)->left_down, 'B', size/2);
            if(size>2){
                number = (*tree)->numberW;
                (*tree)->numberW = number + 1;
                insert2(&(*tree)->left_up, row, column, size/2, 'W');
            }
        }
        if(row<size/2 && column>size/2){
            insert(&(*tree)->right_down, 'B', size/2);
            insert(&(*tree)->right_up, 'W', size/2);
            insert(&(*tree)->left_up, 'B', size/2);
            insert(&(*tree)->left_down, 'B', size/2);
            if(size>2){
                number = (*tree)->numberW;
                (*tree)->numberW = number + 1;
                insert2(&(*tree)->right_up, row, column%size, size/2, 'W');
            }
        }
        if(row>size/2 && column<size/2){
            insert(&(*tree)->right_down, 'B',size/2);
            insert(&(*tree)->right_up, 'B', size/2);
            insert(&(*tree)->left_up, 'B', size/2);
            insert(&(*tree)->left_down, 'W', size/2);
            if(size>2){
                number = (*tree)->numberW;
                (*tree)->numberW = number + 1;
                insert2(&(*tree)->left_down, row%size, column, size/2, 'W');
            }
        }
        if(row>size/2 && column>size/2){
            insert(&(*tree)->right_down, 'W', size/2);
            insert(&(*tree)->right_up, 'B', size/2);
            insert(&(*tree)->left_up, 'B', size/2);
            insert(&(*tree)->left_down, 'B', size/2);
            if(size>2){
                number = (*tree)->numberW;
                (*tree)->numberW = number + 1;
                insert2(&(*tree)->right_up, row%size, column%size, size/2, 'W');
            }
        }
        return;
    }
    //Fall: In dem Teil des Bildes sind alle Pixel weiss, bis auf das welches man ersetzen will -> Teilbaeume loeschen
    if(size*size-1==(*tree)->numberW){
        (*tree)->colour = 'W';
        number = (*tree)->numberW;
        (*tree)->numberW = number + 1;
        
        deltree((*tree)->left_up);
        deltree((*tree)->left_down);
        deltree((*tree)->right_up);
        deltree((*tree)->right_down);
        (*tree)->left_up = NULL;
        (*tree)->left_down = NULL;
        (*tree)->right_up = NULL;
        (*tree)->right_down = NULL;
        return;
    }
    
    //Fall: es ist nur ein Teil der Pixel weiss
    if(size*size-1!=(*tree)->numberW){
        if(row<size/2 && column<size/2){
            number = (*tree)->numberW;
            (*tree)->numberW = number + 1;
            insert2(&(*tree)->left_up, row, column, size/2, 'W');
        }
        if(row<size/2 && column>size/2){
            number = (*tree)->numberW;
            (*tree)->numberW = number + 1;
            insert2(&(*tree)->right_up, row, column%size, size/2, 'W');
        }
        if(row>size/2 && column<size/2){
            number = (*tree)->numberW;
            (*tree)->numberW = number + 1;
            insert2(&(*tree)->left_down, row%size, column, size/2, 'W');
        }
        if(row>size/2 && column>size/2){
            number = (*tree)->numberW;
            (*tree)->numberW = number + 1;
            insert2(&(*tree)->right_up, row%size, column%size, size/2, 'W');
        }
        return;
    }
}

int main(){
    
    
    return 0;
}
