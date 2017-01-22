
#ifndef hashfunction_h
#define hashfunction_h

#include <stdio.h>

static unsigned int hashadd(int hashval, unsigned char ch);

/*Calculates hashindex*/
static unsigned int compute_hash(const char* buf);

#endif /* hashfunction_h */
