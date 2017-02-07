#ifndef filefunctions_h
#define filefunctions_h

int check_username(const char* path, const char* username);
int read_bid(const char* path, int* current_bid, char* current_user);
int set_bid(const char* path, int bid, const char* user);

#endif /* filefunctions_h */