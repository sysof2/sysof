#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>

int main(int argc, const char* argv[]){
    const char* cmdname = *argv++; argc--;
    
    if(argc !=3){
        printf("zu wenig Argumente! Es werden 3 benoetigt!\n");
        exit(1);
    }
    
    const char* reg1 = *argv++; argc--;
    const char*reg2 = *argv++; argc--;
    const char* file = *argv++; argc--;
    printf("reg1: %s, reg2: %s, filename:%s\n", reg1, reg2, file);
    
    int fd;
    if((fd=open(file,O_RDWR))<0){
        perror("open");
        exit(1);
    }
    
    struct stat statbuf;
    
    if(fstat(fd,&statbuf)<0){
        perror("fstat");
        exit(1);
    }
    
    off_t nbytes = statbuf.st_size;
    
    unsigned char* buf = (unsigned char*) mmap(0, nbytes, PROT_READ||PROT_WRITE, MAP_SHARED, fd, 0);
    if(buf == MAP_FAILED) {
        printf("mmap failed!!\n");
    }
    
    for(unsigned char* cp=buf; cp<buf+nbytes; ++cp) {
        //hier findet dann das translate statt - fehlt noch...
        unsigned char ch = *cp;
        const char* temp1 = reg1;
        const char* temp2 = reg2;
        
        for
        
        printf("%c",ch);
    }
    
    munmap(buf,nbytes);
    close(fd);
    
    return 0;
}
