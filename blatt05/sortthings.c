#include <stdio.h>
#include <string.h>
#include <time.h>
#include <stdlib.h>
#include <stdbool.h>

#define MAX_NAME_LEN 100
#define LIST_COUNT 8
#define buflen 256

struct mountains{
    char name[MAX_NAME_LEN];
    int hight;
}mountain_list[LIST_COUNT], sorted_list[LIST_COUNT];

void set_mountains(int list_number, char* name, int hight){
    strncpy(mountain_list[list_number].name,name, MAX_NAME_LEN);
    mountain_list[list_number].hight = hight;
}

int reservoirSampling(){
    FILE *datei;
    datei = fopen("berge","r");
    int t = 0;
    srand(time(NULL));
    int m = 0;

    if(datei==NULL) {
        printf("Fehler beim Oeffnen der Datei!!!\n");
        return 0;
    }

    char buf[buflen];

    char *ptr;
    int i = 0;
    char* name;
    int hight;
    while(fgets(buf, buflen, datei) && i < 8){
        name = strtok(buf, ":");
        if(name==NULL) continue;
        //strncpy(mountain_list[i].name, ptr, MAX_NAME_LEN);
        //printf("Bergname = %s\n",mountain_list[i].name);

        ptr = strtok(NULL, ":");
        if(ptr==NULL) continue;
        hight = atoi(ptr);
        //printf("Berghoehe = %d\n",mountain_list[i].hight);
        set_mountains(i, name, hight);
        i++;

    }
    t = i;

    if(t<8){
        return 1;
    }

    printf("\n");
    printf("t: %d\n", t);
    printf("\n");
    

    do{
        t++;
        name = strtok(buf, ":");
        if(name==NULL) continue;
        
        m = rand()%(t-1); // Zahl zwischen 0 und t-1
        //printf("M= %d\n", m);
        if(m>=8) continue;

        //strncpy(mountain_list[m].name, ptr, MAX_NAME_LEN);
        //printf("Bergname = %s\n",mountain_list[m].name);

        ptr = strtok(NULL, ":");
        if(ptr==NULL) continue;
        hight = atoi(ptr);


        set_mountains(m, name, hight);
        //printf("Berghoehe = %d\n",mountain_list[m].hight);
        i++;
    }
    while(fgets(buf, buflen, datei));
    
    fclose(datei);
    
    return 0;

}

void play_game(){
    bool finished = false;
    int src=0, dest=0;
    int points = 0;
    
    while(!finished){
        
        printf("\nCurrent state: \n");
        
        for(int i=0; i<LIST_COUNT; i++){
            if(sorted_list[i].name[0]!=0){
                printf("%d: %s\n",i,sorted_list[i].name);
            }
        }
        
        printf("\nStill to be sorted: \n");
        
        for(int i=0; i<LIST_COUNT; i++){
            if(mountain_list[i].name[0]!=0){
                printf("%d: %s\n",i,mountain_list[i].name);
            }
        }
        
        printf("\nWhat is to be inserted where? \n");
        
        while(scanf("%d",&src)==0 || src>=LIST_COUNT || src<0) {
            printf("Ungueltige Eingabe der Quelle!!!\n");
            int c;
            while((c=getchar()) != '\n' && c!=EOF);
        }
        //printf("src: %d\n",src);
        
        while(scanf("%d",&dest)==0 || dest>=LIST_COUNT || dest<0) {
            printf("Ungueltige Eingabe des Ziels!!!\n");
            int c;
            while((c=getchar()) != '\n' && c!=EOF);
        }
        //printf("dest: %d\n",dest);
        
        if(sorted_list[dest].name[0] != 0){
            finished = true;
            printf("\nPosition already used\n");
        }else if(mountain_list[src].name[0] == 0){
            finished = true;
            printf("\nItem already assigned\n");
        }else{
            sorted_list[dest]=mountain_list[src];
            mountain_list[src].name[0]=0;
            points++;
            
            for(int i=0; i<LIST_COUNT-1; i++){
                if(sorted_list[i].hight>sorted_list[i+1].hight && sorted_list[i+1].hight != 0 && sorted_list[i].hight != 0){
                    finished = true;
                    points--;
                    printf("\nItems no longer sorted: \n");
                    for(int i=0; i<LIST_COUNT; i++){
                        if(sorted_list[i].name[0] != 0){
                            printf("%d: %d, %s\n",i,sorted_list[i].hight,sorted_list[i].name);
                        }
                    }
                    break;
                }
            }
            
        }
        
        if(points == LIST_COUNT){
            finished = true;
            printf("\nYou won!\n");
            for(int i=0; i<LIST_COUNT; i++){
                if(sorted_list[i].name[0] != 0){
                    printf("%d: %d, %s\n",i,sorted_list[i].hight,sorted_list[i].name);
                }
            }
        }
    }
    
    printf("\nYou got %d points\n",points);
}

int main(){

    int finished = 0;

    finished = reservoirSampling();

    if(finished){
        printf("beendet, da zu wenig Berge eingelesen wurden...\n");
        return 0;
    }
   
    play_game();
    
    return 0;
}