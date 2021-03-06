
#include "hashfunction.h"

#define HASHSTART 5381

unsigned int hashadd(int hashval, unsigned char ch) {
    hashval += hashval << 5;
    return hashval ^ ch;
}

unsigned int compute_hash(const char* buf) {
    int hashval = HASHSTART;
    while (*buf) {
        hashval = hashadd(hashval, *buf++);
    }
    return hashval;
}
