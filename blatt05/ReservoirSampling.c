#include <stdio.h>
#include <string.h>
#include <time.h>
#include <stdlib.h>

#define MAX_NAME_LEN 100
#define LIST_COUNT 8
#define buflen 256

struct mountains{
    char name[MAX_NAME_LEN];
    int hight;
}mountain_list[LIST_COUNT];

void set_mountains(int list_number, char* name, int hight){
    strncpy(mountain_list[list_number].name,name, MAX_NAME_LEN);
    mountain_list[list_number].hight = hight;
}

int reservoirSampling(){
    FILE *datei;
    datei = fopen("berge1","r");
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

int main(){

    int finished = 0;

    finished = reservoirSampling();


    if(finished){
        printf("beendet, da zu wenig Berge eingelesen wurden...\n");
        return 0;
    }
    for(int k=0; k<8; k++){
        printf("Bergname: %s\n", mountain_list[k].name);
        printf("Berghoehe: %d\n", mountain_list[k].hight);
    }
    
    return 0;
}