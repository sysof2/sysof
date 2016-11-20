#include <stdio.h>
#include <string.h>

#define MAX_NAME_LEN 100
#define LIST_COUNT 8

struct mountains{
    char name[MAX_NAME_LEN];
    int hight;
}mountain_list[LIST_COUNT];

void set_mountains(int list_number, char* name, int hight){
    strncpy(mountain_list[list_number].name,name, MAX_NAME_LEN);
    mountain_list[list_number].hight = hight;
}

int main(){

    set_mountains(0,"Matterhorn",5);
    
    printf("Bergname = %s\n",mountain_list[0].name);
    printf("Höhe = %d\n",mountain_list[0].hight);
    
    set_mountains(1,"Eiger",42);
    
    printf("Bergname = %s\n",mountain_list[1].name);
    printf("Höhe = %d\n",mountain_list[1].hight);
    
    return 0;
}
