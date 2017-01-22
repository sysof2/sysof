#include <stralloc.h>
#include <stdio.h>
#include <errno.h>
#include <fcntl.h>
#include <stdlib.h>
#include <strings.h>
#include <unistd.h>
#include "hashfunction.h"

#define HASHLENGTH 10000
//#define HASHSTART 5381

const int BUFFER_SIZE = 8192;
const char* path = "gemeinden.txt";


struct communities{
    struct communities *next;
    double latidude; //Breitengrad
    double meridian; //Laengengrad
    stralloc city;
};

struct communities *hash[HASHLENGTH];

/*static unsigned int hashadd(int hashval, unsigned char ch) {
    hashval += hashval << 5;
    return hashval ^ ch;
}

static unsigned int compute_hash(const char* buf) {
    int hashval = HASHSTART;
    while (*buf) {
        hashval = hashadd(hashval, *buf++);
    }
    return hashval;
}*/

int readline(FILE* fp, stralloc* sa, stralloc* latit, stralloc* meri) {
    int count = 0;//gibt an ob man stadtnamen, laengengrad oder breitengrad einliest

    if (!stralloc_copys(sa, "")) return 0;
    if (!stralloc_copys(latit, "")) return 0;
    if (!stralloc_copys(meri, "")) return 0;
    for(;;) {
        stralloc temp = {0};
        //if (!stralloc_copys(&temp, "")) return 0;

        //readyplus: sorgt dafuer dass 1 byte mehr platz ist
        if(!stralloc_readyplus(&temp, 1)) return 0;
        
        //einlesen des naechsten zeichens
        if (fread(temp.s + temp.len, sizeof(char), 1, fp) <= 0){
            return 0;
        }
         ++temp.len;
        
        if(stralloc_diffs(&temp,":")==0){
            count++;
            continue;
        }
        
        if(count==0){
            //if(!stralloc_readyplus(sa, 1)) return 0;
            stralloc_cat(sa,&temp);
            continue;
        }
        if(count==1){
            //if(!stralloc_readyplus(latit, 1)) return 0;
            stralloc_cat(latit,&temp);
            continue;
        }
        
        if(count==2){
            //if(!stralloc_readyplus(meri, 1)) return 0;
            stralloc_cat(meri,&temp);
            if(meri->s[meri->len-1] == '\n') break;
        }

        
       
       
    }
    return 1;
}

void append(char* cityname, char *latid, char *meri, int hashindex){
    /*printf("test2\n");
    printf("cityname: %s\n", cityname);
    printf("latid: %s\n", latid);
    printf("meri: %s\n", meri);
    printf("hashindex: %d\n", hashindex);*/
    struct communities *new;
    new = (struct communities*) calloc(1,sizeof(*new));
    new->city.s=0;
    new->city.len=0;
    new->city.a=0;
    stralloc_copys(&new->city,cityname);
    new->latidude = atof(latid);
    new->meridian = atof(meri);
    if(hash[hashindex]==NULL){
        hash[hashindex] = new;
        return;
    }
    struct communities *temp = hash[hashindex];
    while(temp->next!=NULL){
        temp = temp->next;
    }
    temp->next = new;
}

void print_list(){
    for(int i=0; i<HASHLENGTH; i++){
        printf("i.%d", i);
        struct communities *temp = hash[i];
        while(temp!=NULL){
            printf("%s, ",temp->city.s);
            temp = temp->next;
        }
        printf("\n");
    }
    
    
}

int main(int argc, char** argv){
    stralloc sa = {0};
    stralloc latit = {0};
    stralloc meri = {0};
    FILE* fp = fopen(path, "r");
    int hashindex = 0;
    for(int k=0; k<HASHLENGTH;k++){
        hash[k]=NULL;
    }
    
    if(fp == NULL) {
        fprintf(stderr, "Could not open file.\n");
        return 1;
    }
    
    while(readline(fp, &sa, &latit, &meri)){
        //printf("name: %s, Laengengrad: %s, Breitengrad:%s\n",sa.s, latit.s, meri.s);
    
        hashindex = compute_hash(sa.s) % HASHLENGTH;
        append(sa.s,latit.s,meri.s, hashindex);
        
        meri.s=0;
        meri.len=0;
        meri.a=0;
        
        latit.s=0;
        latit.len=0;
        latit.a=0;
        
        sa.s=0;
        sa.len=0;
        sa.a=0;
    }
    fclose(fp);
    printf("\n");
    printf("***********Ausgabe************\n");
    printf("\n");
    print_list();
    printf("\n");
    
    return 0;
}
