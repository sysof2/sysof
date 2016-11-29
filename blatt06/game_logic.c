
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define buflen 256
#define MAX_NAME_LEN 256

int word_count;


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
    printf ("%s\n\n", liste->city);
    while (liste->next != NULL) {
        liste=liste->next;
        printf ("%s\n\n", liste->city);
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
                //printf("\n");
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
        
        //printf("index word: %d\n",(*lst)->value);
    }
}

void einlesen(struct word **lst, char *name){
    //ersten und letzten Buchstaben identifizieren
    //printf("cityname: %s\n",name);
    int ersterB = (int)name[0]-'A';
    int letzterB = (int)name[strlen(name)-1]-'a';
    //printf("ersterB: %d\n", ersterB);
    //printf("letzterB: %d\n", letzterB);
   
    if((*lst)!=NULL){
        struct word *temp;
        temp = *lst;
        
        //checken ob der erste Buchstabe schon in word_liste endhalten ist
        while((temp !=NULL) && (temp->value != ersterB)){
            temp = temp->next;
        }
        if(temp==NULL){
            //printf("erster buchstabe neu einfuegen in word_list\n");
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
            //printf("letzter buchstabe neu einfuegen in word_list\n");
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


/*void delete(struct wordSet **lst){
    struct wordSet *tmp;
    tmp = (struct wordSet*) calloc(1,sizeof(*tmp));
    tmp->value=(*lst)->value;
    tmp->next=(*lst)->next;
    (*lst)->next=(*lst)->next->next;
    
    free(tmp);		
}*/

int read_file(struct word **word_list);
int player_turns(struct word **word_list);
int printf_final_list(struct wordSet **lst);
void prepend(struct wordSet **lst, char *city);
int matching_words_exist(struct word **word_list, char firstchar, char lastchar);
int word_in_list(struct word **word_list, char *buf);

int main(){
    int return_value;
    
    struct word *word_list;
    word_list = NULL;
    
    //char *test1 = "Ulm";
    //char *test2 = "Stuttgart";
    //char *test3 = "Muenchen";
    //char *test4 = "Berlin";
    //char *test5 = "Hamburg";
    //char *test6 = "Mannheim";
    
    //einlesen(&word_list,test1);
    //einlesen(&word_list,test2);
    //einlesen(&word_list,test3);
    //einlesen(&word_list,test4);
    //einlesen(&word_list,test5);
    //einlesen(&word_list,test6);    
    
    return_value = read_file(&word_list);
    
    player_turns(&word_list);
    
    //printlisteGesamt(&word_list);
    
    return return_value;
}


int read_file(struct word **word_list) {
    FILE *file;
    
    file = fopen("cities2.txt","r");
    if(file==NULL) {
        printf("Fehler beim oeffnen der Datei!!!\n");
        return 1;
    }

    char buf[buflen];
    while(fgets(buf, buflen, file)){
        //printf("%s\n",buf);
        //overwrite '\n' with '\0'
        size_t p=strlen(buf);
        buf[p-1]='\0';
        einlesen(word_list,buf);
        word_count++;
    }

    fclose(file);

    return 0;
}

int player_turns(struct word **word_list){
    int word_chain_count = 0;
    char firstchar = 0;
    char lastchar= 0;
    
    struct wordSet *word_list_final;
    word_list_final = NULL;
    
    for(;;){
    
        char buf[buflen];
        
        printf("%d words left\n", word_count);
        printf("Next word: ");
        
        fgets(buf, buflen, stdin);
        //overwrite '\n' with '\0'
        size_t p=strlen(buf);
        buf[p-1]='\0';
        //printf("%s\n",buf);
        
        if(!strcmp(buf,"Bye")){
            return 0;
        }else if(strlen(buf)==0){
            printf("Print help: \n");
            //INSERT FUNCTION PRINT ALL CITIES LEFT HERE
            continue;
        }
        
        
        if(word_in_list(word_list,buf)){
        
            if(word_list_final == NULL){
                append(&word_list_final, buf);
                word_chain_count++;
                
            } else if(buf[strlen(buf)-1] == firstchar){
                prepend(&word_list_final, buf);
                word_chain_count++;
                
            } else if (buf[0]-'A'+'a' == lastchar){
                append(&word_list_final, buf);
                word_chain_count++;
                
            }else{
                printf("Word is not fitting to the current chain, starting with new chain\n");
                word_list_final = NULL;
            }
        
        }else{
            printf("Word is not in list, try again.\n");
        }
        
        if(word_list_final != NULL){
            
            firstchar = word_list_final->city[0];
        
            struct wordSet *lst = word_list_final;
            while (lst->next != NULL ){ // suche das letzte Element
                lst=lst->next;
            }   
            lastchar = lst->city[strlen(word_list_final->city)-1];
            
        }
        
        //printf("%c\n",firstchar);
        //printf("%c\n",lastchar);
        
        
        if(!matching_words_exist(word_list, firstchar,lastchar)&&word_list_final != NULL){
            //new chain
            printf("No candidates left for old chain, starting with new chain.\n");
            word_list_final = NULL;
        }else{
            //printf("matching candidates\n");
        }
        
        
        printf("Current chain of %d words: ", word_chain_count);
        
        
        printf_final_list(&word_list_final);
    
    }
    
    
    return 0;
}

int word_in_list(struct word **word_list, char *buf){
    struct word *lst = *word_list; 
       
    while (lst!= NULL ){
        
        if(lst->value==buf[0]-'A'){
            struct wordSet *wordsetlst = lst->erster_buchstabe;
            
            while (wordsetlst != NULL ){
                
                if(!strcmp(wordsetlst->city,buf)){
                    return 1;
                }
                
                wordsetlst=wordsetlst->next;
            }
                
        }
        lst=lst->next;
    }
    return 0;
}


int matching_words_exist(struct word **word_list, char firstchar, char lastchar){
    
    struct word *lst = *word_list;
    
    /*
    if((firstchar==0||lastchar==0) && lst != NULL){
        return 1;
    }
    */

    //printf("firstchar: %d\n", firstchar-'A');
    //printf("lastchar: %d\n", lastchar-'a');
    
    while (lst != NULL ){
        /*
        printf("lstvalue: %d\n",lst->value);
        
        if(lst->erster_buchstabe){
            printf("erster_buchstabe: %s\n",lst->erster_buchstabe->city);
        }
        if(lst->letzter_buchstabe != NULL){
            printf("letzter_buchstabe: %s\n",lst->letzter_buchstabe->city);
        }
        */
        if(((lst->value == firstchar-'A') && (lst->letzter_buchstabe != NULL) )|| ((lst->value == lastchar-'a') && (lst->erster_buchstabe != NULL))){
            return 1;
        }
        lst=lst->next;
    }
        
    
    return 0;   
}

//einfuegen eines neuen Elements am Anfang einer wordSet Liste
void prepend(struct wordSet **lst, char *city) {
    
    struct wordSet *neuesElement;
    neuesElement = (struct wordSet*) calloc(1,sizeof(*neuesElement)); // erzeuge ein neues Element
    strncpy(neuesElement -> city,city, MAX_NAME_LEN);
    
    neuesElement->next = *lst;
    *lst = neuesElement;
}

//druckt die wordSet liste im format: Ulm ... München
int printf_final_list(struct wordSet **lst){
    
    struct wordSet *liste;
    liste=*lst;
    
    if(liste == NULL) return 1;
    
    printf ("%s", liste->city);
    
    if(liste->next == NULL){
        printf("\n");
        return 1;
    }
    
    int i=0;
    while (liste->next->next != NULL) {
        liste=liste->next;
        i++;
    }
    
    if(i>0){
        printf(" ...");
    }
    
    printf (" %s\n", liste->next->city);
    
    return 0;
}



