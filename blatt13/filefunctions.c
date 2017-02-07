#include "filefunctions.h"
#include <dirent.h>
#include <stdlib.h>
#include <stdio.h> 
#include <string.h>
#include <unistd.h>
#include <stdbool.h>
#include <fcntl.h>
#include <ctype.h>
#include <errno.h>


char* cmdname;
//stralloc tmpfile = {0};
char* tempfile = "current_bid.tmp";
char* bidfile = "current_bid";
bool tmpfile_created = false;

/* print an out of memory message to standard error and exit */
void memerr() {
	static char memerrmsg[] = "out of memory error\n";
	write(2, memerrmsg, sizeof(memerrmsg) - 1);
	//if (tmpfile_created) unlink(tmpfile.s);
	if(tmpfile_created) unlink(tempfile);
	exit(1);
}

/* print a error message to standard error and exit;
 *  include "message" in the output message, if not 0,
 *  otherwise strerror(errno) is being used
 */
void die(char* filename, char* message) {
	/*
	stralloc msg = {0};
	if (stralloc_copys(&msg, cmdname) &&
		stralloc_cats(&msg, ": ") &&
		(message? stralloc_cats(&msg, message):stralloc_cats(&msg, strerror(errno))) &&
		stralloc_cats(&msg, ": ") &&
		stralloc_cats(&msg, filename) &&
		stralloc_cats(&msg, "\n")) {
		write(2, msg.s, msg.len);
	} else {
		memerr();
	}
		*/
	//if (tmpfile_created) unlink(tmpfile.s);
	fprintf(stderr,"%s\n",strerror(errno));
	if(tmpfile_created) unlink(tempfile);
	exit(1);
}

/* checks for an already existing username, if not
 * creates a new file with the username as filename
 * the access is not locked, so a sync failure may occur!!
 */
int check_username(const char* path, const char* username){
	DIR *dir;
	struct dirent *dp;
	
	//printf("strlength: %lu\n",strlen(path));
	
	if((dir = opendir(path)) == NULL) {
		printf("failed to open path\n");
		exit(1);
	}
	
	while((dp=readdir(dir)) != NULL){
		//printf("%s\n", (*dp).d_name);
		if(!strcmp((*dp).d_name,username)){
			if (chdir(path) < 0) {
				perror(path);
				exit(1);
			}
			return 1;
		}
	}
	
	if(closedir(dir) == -1){
		printf("failed to close path\n");
		exit(1);
	}
	
	if (chdir(path) < 0) {
		perror(path);
		exit(1);
	}
	
	FILE *fp = fopen(username,"w");
	if(fp == NULL) {
		fprintf(stderr, "Could not open file.\n");
		exit(1);
	}
	if(fclose(fp)){
		fprintf(stderr, "Could not close file.\n");
		exit(1);
	}
	
	return 0;
}

/* reads the current bid and the user from the current_bid file 
 * returns 0 if successfull
 */
int read_bid(const char* path, int* current_bid, char* current_user){
	
	/*
	// try to open the temporary file which also serves as a lock
	int outfd;
	for (int tries = 0; tries < 10; ++tries) {
		outfd = open(tempfile, O_WRONLY|O_CREAT|O_EXCL, 0666);
		if (outfd >= 0) break;
		if (errno != EEXIST) break;
		sleep(1);
	}
	if (outfd < 0) die(tempfile, 0);
	tmpfile_created = true;
	*/
	// determine current value of the counter
	char content[100];
	bool fileexists=false;
	
	int infd = open(bidfile, O_RDONLY);
	if (infd >= 0) {
		char buf[512];
		ssize_t nbytes = read(infd, buf, sizeof buf);
		if (nbytes <= 0) die(bidfile, 0);
		int i=0;
		for (char* cp = buf; cp < buf + nbytes; ++cp) {
			content[i]=*cp;
			i++;
		}
		fileexists=true;
	} else if (errno != ENOENT) {
		die(bidfile, 0);
	} else {
		*current_bid = 0;
		current_user = "\0";
	}
	
	if(sscanf(content,"%d by %s",current_bid,current_user)!=2){
		if(fileexists){
			printf("error reading file\n");
			exit(1);
		}
	}
	//printf("current_bid: %d\n",*current_bid);
	//printf("current_user: %s\n",current_user);
	
	return 0;
}

int set_bid(const char* path, int bid, const char* user){
	
	int current_bid = 0;
	char current_user[20];
	
	// try to open the temporary file which also* serves as a lock
	int outfd;
	for (int tries = 0; tries < 10; ++tries) {
		outfd = open(tempfile, O_WRONLY|O_CREAT|O_EXCL, 0666);
		if (outfd >= 0) break;
		if (errno != EEXIST) break;
		sleep(1);
	}
	if (outfd < 0) die(tempfile, 0);
	tmpfile_created = true;
	
	// determine current value of the counter
	char content[100];
	bool fileexists=false;
	
	int infd = open(bidfile, O_RDONLY);
	if (infd >= 0) {
		char buf[512];
		ssize_t nbytes = read(infd, buf, sizeof buf);
		if (nbytes <= 0) die(bidfile, 0);
		int i=0;
		for (char* cp = buf; cp < buf + nbytes; ++cp) {
			content[i]=*cp;
			i++;
		}
		fileexists=true;
	} else if (errno != ENOENT) {
		die(bidfile, 0);
	} else {
		current_bid = 0;
		//current_user = "\0";
	}
	
	if(sscanf(content,"%d by %s",&current_bid,current_user)!=2){
		if(fileexists){
			printf("error reading file\n");
			exit(1);
		}
	}
	//printf("current_bid: %d\n",current_bid);
	//printf("current_user: %s\n",current_user);
	
	//is the bid still valid?
	if(bid<=current_bid){
		/* update counter file atomically by a rename */
		//if (rename(bidfile, tempfile) < 0) die(bidfile, 0);
		//if (rename(tempfile, bidfile) < 0) die(bidfile, 0);
		//tmpfile_created = false;
		if(tmpfile_created) unlink(tempfile);
		return 1;
	}
	
	char str[100];
	snprintf(str,100, "%d", bid);
	strcat(str," by ");
	strcat(str,user);
	
	int nbytes = write(outfd, str, strlen(str)+1);
	if (nbytes < strlen(str)+1) die(tempfile, 0);
	if (fsync(outfd) < 0) die(tempfile, 0);
	if (close(outfd) < 0) die(tempfile, 0);
	
	/* update counter file atomically by a rename */
	if (rename(tempfile, bidfile) < 0) die(bidfile, 0);
	tmpfile_created = false;
	
	
	return 0;
}



