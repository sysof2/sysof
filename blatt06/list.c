#include <stdio.h>
#include <stdlib.h>

struct element {
     int value; // der Wert des Elements
     struct element *next; // das nächste Element
};

void printliste(struct element *l) {
        struct element *liste;
        liste=l;
        printf ("%d\n", liste->value);
        while (liste->next != NULL) {
            liste=liste->next;
            printf ("%d\n", liste->value);
	}
}

void append(struct element **lst, int value) {
        struct element *neuesElement;
        struct element *lst_iter = *lst;
        neuesElement = (struct element*) calloc(1,sizeof(*neuesElement)); // erzeuge ein neues Element
        neuesElement->value = value;
        neuesElement->next = NULL; // Wichtig für das Erkennen des Listenendes
        if ( lst_iter != NULL ) { // sind Elemente vorhanden
            while (lst_iter->next != NULL ) // suche das letzte Element
                lst_iter=lst_iter->next;
            lst_iter->next=neuesElement; // Hänge das Element hinten an
        }
        else // wenn die liste leer ist, bin ich das erste Element
        *lst=neuesElement;
}

void delete(struct element **lst){
	struct element *tmp;
	tmp = (struct element*) calloc(1,sizeof(*tmp));
	tmp->value=(*lst)->value;
	tmp->next=(*lst)->next;
	(*lst)->next=(*lst)->next->next;

	free(tmp);		
}

void my_delete(struct element **lst){
	struct element *tmp = (*lst)->next;
        (*lst)->next=(*lst)->next->next;
	free(tmp);		
}

int main(){
        struct element *Liste;
        Liste = NULL; // init. die Liste mit NULL = leere liste
        append(&Liste, 123); // füge neues Element in die Liste ein
        append(&Liste, 456); // füge neues Element in die Liste ein
        append(&Liste, 789); // füge neues Element in die Liste ein
        
        //struct element *element1 = Liste;
        struct element *element2 = Liste->next;
        
        printf("Full List:\n");
        printliste(Liste); // zeige liste an
	printf("\n");
        
        printf("Delete second element\n");
        //delete(&Liste);
	my_delete(&Liste);
        
        printf("Current List:\n");
	printliste(Liste);
        
        printf("\nPrint second element: %d\n",element2->value);

	return 0;
}