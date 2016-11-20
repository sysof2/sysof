#include <stdio.h>
#include <string.h>
#include <stdbool.h>

#define MAX_NAME_LEN 100
#define LIST_COUNT 3

struct mountains{
    char name[MAX_NAME_LEN];
    int hight;
}mountain_list[LIST_COUNT],sorted_list[LIST_COUNT];

void set_mountains(int list_number, char* name, int hight){
    strncpy(mountain_list[list_number].name,name, MAX_NAME_LEN);
    mountain_list[list_number].hight = hight;
}

void play_game();

int main(){

    set_mountains(0,"Mount Everest",8848);
    set_mountains(1,"Matterhorn",4478);
    set_mountains(2,"Eiger",3970);
    //set_mountains(3,"Watzmann",2713);
    //set_mountains(4,"Wildspitze",3768);
    //set_mountains(5,"Olymp",2918);
    //set_mountains(6,"Wetterhorn",3692);
    //set_mountains(7,"Tegelberg",1881);
    
    play_game();

    return 0;
}

void play_game(){
    bool finished = false;
    int src=0, dest=0;
    int points = -1;
    
    while(!finished){
        
        printf("\nCurrent state: \n");
        
        for(int i=0;i<LIST_COUNT;i++){
            if(sorted_list[i].name[0]!=0){
                printf("%d: %s\n",i,sorted_list[i].name);
            }
        }
        
        printf("\nStill to be sorted: \n");
        
        for(int i=0;i<LIST_COUNT;i++){
            if(mountain_list[i].name[0]!=0){
                printf("%d: %s\n",i,mountain_list[i].name);
            }
        }
        
        printf("\nWhat is to be inserted where? \n");
        
        if(scanf("%d",&src)==0 || src>=LIST_COUNT || src<0) {
            printf("Ungueltige Eingabe!!!\n");
            int c;
            while((c=getchar()) != '\n' && c!=EOF);
        }
        
        if(scanf("%d",&dest)==0 || dest>=LIST_COUNT || dest<0) {
            printf("Ungueltige Eingabe!!!\n");
            int c;
            while((c=getchar()) != '\n' && c!=EOF);
        }
        
        
        if(sorted_list[dest].name[0] != 0){
            finished = true;
            printf("\nPosition already used\n");
        }else{
            sorted_list[dest]=mountain_list[src];
            mountain_list[src].name[0]=0;
            
            for(int i=0; i<LIST_COUNT-1; i++){
                if(sorted_list[i].hight>sorted_list[i+1].hight && sorted_list[i+1].hight != 0 && sorted_list[i].hight != 0){
                    finished = true;
                    printf("\nItems no longer sorted: \n");
                    for(int i=0;i<LIST_COUNT;i++){
                        if(sorted_list[i].name[0]!=0){
                            printf("%d: %d, %s\n",i,sorted_list[i].hight,sorted_list[i].name);
                        }
                    }
                    break;
                }
            }
            
        }
        
        points++;
        
        if(points == LIST_COUNT-1){
            finished = true;
            printf("\nYou won!\n");
            for(int i=0;i<LIST_COUNT;i++){
                if(sorted_list[i].name[0]!=0){
                    printf("%d: %d, %s\n",i,sorted_list[i].hight,sorted_list[i].name);
                }
            }
        }
    }
    
    printf("\nYou got %d points\n",points);
}



