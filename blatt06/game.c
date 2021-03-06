
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define buflen 256
#define MAX_NAME_LEN 256


struct word {
    int value; // der Wert des Elements
    struct word *next;
    struct wordSet *letzter_buchstabe;
    struct wordSet *erster_buchstabe;
};

struct wordSet{
    char city[buflen];
    struct wordSet *next;
};

void printliste(struct wordSet **l) {
    struct wordSet *liste;
    liste=*l;
    printf ("%s\n", liste->city);
    while (liste->next != NULL) {
        liste=liste->next;
        printf ("%s\n", liste->city);
    }
}

void printlisteGesamt(struct word **lst){
    
    for(int value=0; value<26; value++){
        
        struct word *temp;
        temp = *lst;
        while((temp !=NULL) && (temp->value != value)){
            temp = temp->next;
        }
        if(temp != NULL){
            printf("Buchstabe %d\n", value);
            if((temp->erster_buchstabe)!=NULL){
                printf("erster buchstabe: \n");
                printliste(&(temp->erster_buchstabe));
            }
            
            if((temp->letzter_buchstabe)!=NULL){
                printf("\n");
                printf("letzter buchstabe:\n");
                printliste(&(temp->letzter_buchstabe));
            }
        }
    }
}

//einfuegen eines neuen Elements in eine wordSet Liste
void append(struct wordSet **lst, char *city) {
    
    struct wordSet *neuesElement;
    struct wordSet *lst_h = *lst;
    neuesElement = (struct wordSet*) calloc(1,sizeof(*neuesElement)); // erzeuge ein neues Element
    strncpy(neuesElement -> city,city, MAX_NAME_LEN);
    neuesElement->next = NULL; // Wichtig für das Erkennen des Listenendes
    
    if ( lst_h != NULL ) { // sind Elemente vorhanden
        while (lst_h->next != NULL ){ // suche das letzte Element
            lst_h=lst_h->next;
        }
        lst_h->next=neuesElement; // Hänge das Element hinten an
    }
    else{ // wenn die liste leer ist, bin ich das erste Element
        *lst=neuesElement;
    }
}

//einfuegen eines neuen Elements in die word Liste
void append2(struct word **lst, int value) {
    
    struct word *neuesElement;
    struct word *lst_h = *lst;
    neuesElement = (struct word*) calloc(1,sizeof(*neuesElement)); // erzeuge ein neues Element
    neuesElement -> value = value;
    neuesElement->next = NULL; // Wichtig für das Erkennen des Listenendes
    neuesElement->letzter_buchstabe = NULL;
    neuesElement->erster_buchstabe = NULL;
    if ( lst_h != NULL ) { // sind Elemente vorhanden
        while (lst_h->next != NULL ){ // suche das letzte Element
            lst_h=lst_h->next;
        }
        lst_h->next=neuesElement; // Hänge das Element hinten an
    }
    else{ // wenn die liste leer ist, bin ich das erste Element
        *lst=neuesElement;
    }
}

void einlesen(struct word **lst, char *name){
    //ersten und letzten Buchstaben identifizieren
    int ersterB = (int)name[0]-'A';
    int letzterB = (int)name[strlen(name)-1]-'a';
   
    if((*lst)!=NULL){
        struct word *temp;
        temp = *lst;
        
        //checken ob der erste Buchstabe schon in word_liste endhalten ist
        while((temp !=NULL) && (temp->value != ersterB)){
            temp = temp->next;
        }
        if(temp==NULL){
            append2(&(*lst), ersterB);
            //stadt einfuegen
            struct word *temp2;
            temp2 = *lst;
            while(temp2->value != ersterB){
                temp2 = temp2->next;
            }
            append(&(temp2->erster_buchstabe), name);
            
        }else{
            append(&(temp->erster_buchstabe), name);
        }
        
        temp = *lst;
        
        //checken ob der letzte Buchstabe schon in word_liste endhalten ist
        while((temp !=NULL) && (temp->value != letzterB)){
            temp = temp->next;
        }
        if(temp == NULL){
            append2(&(*lst), letzterB);
            //stadt einfuegen
            struct word *temp2;
            temp2 = *lst;
            while(temp2->value != letzterB){
                temp2 = temp2->next;
            }
            append(&(temp2->letzter_buchstabe), name);
        }else{
            append(&(temp->letzter_buchstabe), name);
        }

    }else{
        append2(&(*lst), ersterB);
        append(&((*lst)->erster_buchstabe), name);
        append2(&(*lst), letzterB);
        struct word *temp;
        temp = (*lst)->next;
        append(&(temp->letzter_buchstabe), name);
    }
}


void delete2(struct wordSet **lst){
    struct wordSet *tmp = (*lst)->next;
    (*lst)->next=(*lst)->next->next;
    free(tmp);
}

void delete3(struct word ***lst, char *name, int b){
    //b gibt an ob ersterB oder letzterB
    int variabelB;
    
    if(b==0){
        variabelB = (int)name[0]-'A';
    }else{
        variabelB = (int)name[strlen(name)-1]-'a';
    }
    
    struct word *temp = **lst;
    
    while((temp !=NULL) && (temp->value != variabelB)){
        temp = temp->next;
    }
    
    if(temp != NULL){
        struct wordSet *h;
        if(b==0){
            h = temp->erster_buchstabe;
        }else{
            h = temp->letzter_buchstabe;
        }
        
        struct wordSet *after = h->next;
        char *save = h->city;
        char *save2 = after->city;
        
        while(h !=NULL && (after !=NULL) && (strcmp(save2,name)!=0) && (strcmp(save,name)!=0)){
            h = h->next;
            after = after->next;
            save = h->city;
            save2 = after->city;
        }
        
        if(strcmp(save,name)==0){
            //erstes Element in der Liste
            if(b==0){
                temp->erster_buchstabe = h->next;
            }else{
                temp->letzter_buchstabe = h->next;
            }
            free(h);
        }else{
            delete2(&h);
        }
    }

    
}

void delete(char *name, struct word **lst){
    int b = 0;
    delete3(&lst,name,b);
    b = 1;
    delete3(&lst,name,b);
}

int main(){
    struct word *word_list;
    word_list = NULL;
    
    char *test1 = "Ulm";
    char *test2 = "Stuttgart";
    char *test3 = "Muenchen";
    char *test4 = "Berlin";
    char *test5 = "Hamburg";
    
    einlesen(&word_list,test1);
    einlesen(&word_list,test2);
    einlesen(&word_list,test3);
    einlesen(&word_list,test4);
    einlesen(&word_list,test5);
    
    
    printlisteGesamt(&word_list);
    
    printf("\n");
    printf("\n");
    printf("\n");
    printf("**********delete***************\n");
    printf("\n");
    printf("\n");
    printf("\n");
    
    
    delete(test1, &word_list);
    
    printlisteGesamt(&word_list);
    return 0;
}
