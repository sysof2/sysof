#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdint.h>
#include <string.h>

#define MAX_FILENAMELENGTH 200
#define MAX_DIRNAMELENGTH 200
int count;

struct filelist{
    char filename[MAX_FILENAMELENGTH];
    intmax_t size;
    struct filelist *next;
};

struct dirlist{
    char dirname[MAX_DIRNAMELENGTH];
    struct dirlist *next;
};

void print_dir(struct dirlist *directory_list);
void apend_dir(struct dirlist **directory_list, char *dirname);
void prepend_dir(struct dirlist **directory_list, char *dirname);
void delete_first_dir(struct dirlist **directory_list);
void delete_last_dir(struct dirlist **directory_list);
void prepend_file(struct filelist **file_list, char *filename, intmax_t size);
void print_file(struct filelist *file_list);
void sort_filelist(struct filelist **file_list);
void print_file2(struct filelist *file_list);

int main(int argc, char* argv[]) {
    count = 0;
    char* cmdname = *argv++; --argc;
    char usage[] = "Usage: %s [directory]\n";
    if (argc > 1) {
        fprintf(stderr, usage, cmdname);
        exit(1);
    }
    char* dirname;
    if (argc > 0) {
        dirname = *argv;
    } else {
        dirname = ".";
    }
    
    //printf("directory used: %s\n",dirname);
    
    struct filelist *file_list = NULL;
    struct dirlist *directory_list = NULL;
    
    prepend_dir(&directory_list,"..");
    prepend_dir(&directory_list,dirname);
    
    while(directory_list!=NULL){
        
        if (chdir(directory_list->dirname) < 0) {
            perror(directory_list->dirname);
            exit(1);
        }
        
        //print_dir(directory_list);
        if(!strcmp(directory_list->dirname,"..")){
            //printf("going up\n");
            delete_first_dir(&directory_list);
            continue;
        }else{
            //printf("going down:\n");
        }
        
        
        DIR* dir = opendir(".");
        if (!dir) {
            perror(dirname);
            exit(1);
        }
        
        delete_first_dir(&directory_list);
        
        struct dirent* entry;
        
        while ((entry = readdir(dir))) {
            
            //printf("%s: ", entry->d_name);
            
            struct stat statbuf;
            if (lstat(entry->d_name, &statbuf) < 0) {
                perror(entry->d_name); exit(1);
            }
            
            if (S_ISREG(statbuf.st_mode)) {
                //printf("\tregular file with %jd bytes\n",(intmax_t) statbuf.st_size);
                prepend_file(&file_list,entry->d_name, (intmax_t) statbuf.st_size);
                count++;
                
            } else if (S_ISDIR(statbuf.st_mode)) {
                //puts("\tdirectory");
                if(strcmp(entry->d_name,".") && strcmp(entry->d_name,"..")){
                    prepend_dir(&directory_list,"..");
                    prepend_dir(&directory_list,entry->d_name);
                }
                
            } else if (S_ISLNK(statbuf.st_mode)) {
                char buf[1024];
                ssize_t len = readlink(entry->d_name, buf, sizeof buf);
                if (len < 0) {
                    perror(entry->d_name); exit(1);
                }
                //printf("\tsymbolic link pointing to %.*s\n", (int)len, buf);
                
            } else {
                //puts("\tspecial");
            }
            
        }
        
        closedir(dir);
        
        //print_dir(directory_list);
    }
    
    //print_file(file_list);
    
    //printf("\n");
    
    sort_filelist(&file_list);
    return 0;
}

void print_dir(struct dirlist *directory_list){
    printf("\n---------------------------\nList of Directories:\n");
    struct dirlist *temp = directory_list;
    while(temp!=NULL){
        printf("%s\n",temp->dirname);
        temp = temp->next;
    }
    printf("---------------------------\n");
}

void prepend_dir(struct dirlist **directory_list, char *dirname){
    struct dirlist *new;
    new = (struct dirlist*) calloc(1,sizeof(*new));
    strncpy(new->dirname,dirname, MAX_DIRNAMELENGTH);
    new->next = *directory_list;
    *directory_list = new;
}

void apend_dir(struct dirlist **directory_list, char *dirname){
    struct dirlist *new;
    new = (struct dirlist*) calloc(1,sizeof(*new));
    strncpy(new->dirname,dirname, MAX_DIRNAMELENGTH);
    new->next = NULL;
    
    if((*directory_list) == NULL){
        *directory_list = new;
        return;
    }
    
    struct dirlist *temp = *directory_list;
    while(temp->next!=NULL){
        temp = temp->next;
    }
    temp->next = new;
}

void delete_first_dir(struct dirlist **directory_list){
    struct dirlist *temp = *directory_list;
    *directory_list = (*directory_list) -> next;
    free(temp);
}

void delete_last_dir(struct dirlist **directory_list){
    struct dirlist *temp = *directory_list;
    
    if((*directory_list)->next == NULL){
        free(*directory_list);
        *directory_list = NULL;
        return;
    }
    
    while(temp->next->next!=NULL){
        temp = temp->next;
    }
    free(temp->next);
    temp->next = NULL;
}

void print_file(struct filelist *file_list){
    printf("\n---------------------------\nList of Files:\n");
    struct filelist *temp = file_list;
    while(temp!=NULL){
        printf("%s: %jd\n",temp->filename, temp->size);
        temp = temp->next;
    }
}

void print_file2(struct filelist *file_list){
    printf("\n---------------------------\nList of Files Result:\n");
    struct filelist *temp = file_list;
    int x = 0;
    while(temp!=NULL && x<10){
        printf("%s: %jd\n",temp->filename, temp->size);
        temp = temp->next;
        x++;
    }
}

void prepend_file(struct filelist **file_list, char *filename, intmax_t size){
    struct filelist *new;
    new = (struct filelist*) calloc(1,sizeof(*new));
    strncpy(new->filename,filename, MAX_FILENAMELENGTH);
    new->size = size;
    new->next = *file_list;
    *file_list = new;
}

void sort_filelist(struct filelist **file_list){
   
    struct filelist *sorted[10] = {NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL} ;
    struct filelist *temp = *file_list;
    int countArray = 0;
    intmax_t smallestSize;
    int pos;
    
    
    
    while(temp!=NULL){
        if(countArray<10){
            sorted[countArray] = temp;
            countArray++;
        }else{
            pos = 0;
            smallestSize = sorted[0]->size;
            for(int x=1; x<10;x++){
                if(smallestSize>sorted[x]->size){
                    smallestSize = sorted[x]->size;
                    pos = x;
                }
            }
            if(temp->size>sorted[pos]->size){
                sorted[pos] = temp;
            }
        }
        temp=temp->next;
    }
    for(int k=0; k<10; k++){
        for(int j=0; j<9; j++){
            if(sorted[j]!=NULL && sorted[j+1]!=NULL && sorted[j]->size<sorted[j+1]->size){
                temp = sorted[j];
                sorted[j] = sorted[j+1];
                sorted[j+1]=temp;
            }
        }
    }
    printf("Result:\n");
    for(int y=0;y<10;y++){
        if(sorted[y]!=NULL){
            printf("%s: %jd\n", sorted[y]->filename, sorted[y]->size);
        }
        
    }
}
