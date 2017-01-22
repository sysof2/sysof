
#include "hashfunction.h"

#define HASHSTART 5381

static unsigned int hashadd(int hashval, unsigned char ch) {
    hashval += hashval << 5;
    return hashval ^ ch;
}

static unsigned int compute_hash(const char* buf) {
    int hashval = HASHSTART;
    while (*buf) {
        hashval = hashadd(hashval, *buf++);
    }
    return hashval;
}
