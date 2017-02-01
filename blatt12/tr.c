#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>

int main(int argc, const char* argv[]){
	//const char* cmdname = *argv++; argc--;
	argv++;
	
	if(--argc !=3){
		printf("zu wenig Argumente! Es werden 3 benoetigt!\n");
		exit(1);
	}
	
	const char* reg1 = *argv++; argc--;
	const char* reg2 = *argv++; argc--;
	const char* file = *argv++; argc--;
	printf("reg1: %s, reg2: %s, filename:%s\n", reg1, reg2, file);
	
	char srclist[256];
	char destlist[256];
	
	if(reg1[0]=='-'||reg1[strlen(reg1)-1]=='-'){
		printf("incorrect use of '-'\n");
		exit(1);
	}
	
	//expand '-' syntax to full vector
	
	int pos=0;
	for(int i=0;i<strlen(reg1);i++){
		if(reg1[i]=='-'){
			if(reg1[i-1]>=reg1[i+1]){
				printf("incorrect use of '-'\n");
				exit(1);
			}
			
			for(char ch=reg1[i-1]+1;ch<reg1[i+1];ch++){
				srclist[pos]=ch;
				pos++;
				if(pos>=256){
					printf("incorrect arguments\n");
					return 1;
				}
			}
			
		}else{
			srclist[pos]=reg1[i];
			pos++;
			if(pos>=256){
				printf("incorrect arguments\n");
				return 1;
			}
		}
	}
	srclist[pos]=0;
	
	pos=0;
	for(int i=0;i<strlen(reg2);i++){
		if(reg2[i]=='-'){
			if(reg2[i-1]>=reg2[i+1]){
				printf("incorrect use of '-'\n");
				exit(1);
			}
			
			for(char ch=reg2[i-1]+1;ch<reg2[i+1];ch++){
				destlist[pos]=ch;
				pos++;
				if(pos>=256){
					printf("incorrect arguments\n");
					return 1;
				}
			}
			
		}else{
			destlist[pos]=reg2[i];
			pos++;
			if(pos>=256){
				printf("incorrect arguments\n");
				return 1;
			}
		}
	}
	destlist[pos]=0;
	
	if(strlen(srclist)!=strlen(destlist)){
		printf("arguments must be the same length\n");
	}
	
	//printf("%s\n",srclist);
	//printf("%s\n",destlist);
	
	
	int fd = open(file,O_RDWR);
	if(fd<0){
		perror("open");
		exit(1);
	}
	
	struct stat statbuf;
	
	if(fstat(fd,&statbuf)<0){
		perror("fstat");
		exit(1);
	}
	
	off_t nbytes = statbuf.st_size;
	
	unsigned char* buf = (unsigned char*) mmap(0, nbytes, PROT_READ|PROT_WRITE, MAP_SHARED, fd, 0);
	if(buf == MAP_FAILED) {
		printf("mmap failed!!\n");
	}
	
	for(unsigned char* cp=buf; cp<buf+nbytes; ++cp) {
		unsigned char ch = *cp;
		
		for(int i=0;i<strlen(srclist);i++){
			if(srclist[i]==ch){
				ch=destlist[i];
			}
		}
		
		//Change the file:
		*cp = ch;
		
		//printf("%c",*cp);
	}
	
	munmap(buf,nbytes);
	close(fd);
	
	printf("success\n");
	
	return 0;
} 
