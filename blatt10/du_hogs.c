#include <sys/stat.h>
#include <unistd.h> //rmdir
#include <dirent.h> //opendir, closedir, readdir
#include <stdio.h>
#include <stdlib.h>
#include<string.h> //strcat()

#define PATHLENGTH 200

struct file{
    char pathname[PATHLENGTH];
    off_t size;
    struct datei *next;
}file_list;

int main(int argc, char *argv[]){
    DIR *dir;
    struct dirent *dirzeiger;
    struct stat attribut;
    
    if(argc == 1) {
        printf("Benutzung : %s Directory\n", argv[0]);
        return 1;
    }
    
    if((dir = opendir(argv[1])) == NULL) {
        printf("Fehler bei opendir....\n");
        return 1;
    }else{
        
        printf("geoeffnet: %s\n", argv[1]);
    }
    
    //++argv;
    while((dirzeiger=readdir(dir)) != NULL){
        printf("while\n");
        if(!strchr((*dirzeiger).d_name, '.')){
        char *directory = (*dirzeiger).d_name;
        char *temp = argv[1];
        strcat(temp,directory);
        printf("1%s\n", temp);
        printf("2%s\n", (*dirzeiger).d_name);
        
        if(chdir(temp) == 0 && ((dirzeiger=readdir(dir)) != NULL)){
            printf("3:%s\n", (*dirzeiger).d_name);
        }
        }
        /*
        printf("%s\n", (*dirzeiger).d_name);
        
        chdir((*di);
        printf("2222:%s\n", (*dirzeiger).d_name);
        //printf("attribut: %hu\n", attribut.st_mode);
        //char **temp = argv;
        //while(argv[1]) {
            printf("2.%s = ",*argv);
            if(stat(*argv, &attribut) == -1) {
                fprintf(stderr,"Fehler bei stat (2)...\n");
                return EXIT_FAILURE;
            }
            if(attribut.st_mode & S_IFREG){
                printf("Reguläre Datei\n");
            }else if(attribut.st_mode & S_IFDIR){
                printf("Directory\n");
            }else if(attribut.st_mode & S_IFCHR){
                printf("Gerätedatei\n");
            }else{
                printf("Unbekannte Datei\n");
            }*/
        //}
    }
    
    if(closedir(dir) == -1){
        printf("Fehler beim Schließen von %s\n", argv[1]);
    }else{
        printf("geschlossen\n");
    }
    
    return 0;
}
