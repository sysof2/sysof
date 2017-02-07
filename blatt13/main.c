#include <stdlib.h>
#include <stdio.h> 
#include <string.h>
#include "filefunctions.h"

int main(int argc, char* argv[]) {
	 
	if(--argc !=2){
		printf("Only 2 arguments allowed: path, username\n");
		exit(1);
	}
	
	argv++;
	const char* path = *argv++; argc--;
	const char* username = *argv++; argc--;
	
	if(check_username(path,username)){
		printf("Username already taken\n");
		//exit(1);
	}
	
	int current_bid=0;
	char current_user[20];
	
	while(1){
		
		if(read_bid(path,&current_bid,current_user)){
			printf("Something went wrong\n");
			exit(1);
		}
		
		if(current_bid==0){
			printf("No bid yet\n");
		}else{
			printf("%d by %s\n",current_bid,current_user); 
		}
		
		printf("Your bid: ");
		char buf[20];
		int bid=0;
		if(!fgets(buf,20,stdin)){
			printf("error rading line\n");
			exit(1);
		}
		if(sscanf(buf,"%d",&bid)!=1){
			printf("error reading line\n");
			exit(1);
		}
		
		if(set_bid(path,bid,username)){
			printf("Sorry, someone else was faster\n");
		}else{
			//printf("%d\n",bid);
			printf("Ok\n");
		}
		
	}
}
