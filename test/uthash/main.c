#include "uthash.h"
#include <stdio.h>
#include <string.h>

/*
void test_hashmap(){
    void *chunk_ptr = malloc(sizeof(100));
    strcpy(chunk_ptr, "Hello");
    hashed_link h = { .seq_num = 1 , .dl_ptr = chunk_ptr  };
    add_hashl(&h);

    hashed_link *result = (hashed_link *)(find_hashl(1));
    hashed_link *result1 = (hashed_link *)(find_hashl(2));
    DBG("Name = %s\n", (char*)(result->dl_ptr));
    DBG("Name = %p\n", (result1));
}
*/



typedef struct hash {
    uint16_t seq_num;
    char *dl_ptr;
    char *nl_ptr;
    UT_hash_handle hh;
} hashl;

hashl *hl = NULL;

void add_hashl(hashl *s){
    HASH_ADD_INT( hl, seq_num, s);
}

hashl *find_hashl(int seq_num){
    hashl *h;
    HASH_FIND_INT( hl, &seq_num, h);
    return h;
}

int main(int argc, char *argv[]){
    void *chunk_ptr = malloc(sizeof(100));
    strcpy(chunk_ptr, "Hello");
    hashl h = { .seq_num = 1 , .dl_ptr = chunk_ptr  };
    add_hashl(&h);

    hashl *result = find_hashl(1);
    printf("Name = %s\n", (result->dl_ptr));
    return 0;
}
