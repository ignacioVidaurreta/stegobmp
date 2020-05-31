#include <stdlib.h>
#include <stdio.h>
#include <openssl/des.h>

#include "../include/errors.h"

#define TRUE 1
#define FALSE 0

int store_key(DES_cblock* key) {
    int suitable = FALSE;
    int i = 0;

    // the probability a key is not suitable is 1 in 2^52
    // but just in case we set a restriction for how many times to run this
    while(!suitable && i < 500) {
        DES_random_key(key);
        DES_set_odd_parity(key);
        int is_weak = DES_is_weak_key(key);
        if(is_weak == FALSE) {
            suitable = TRUE;
        }
    }
    return (suitable == TRUE ? SUCCESS : FAILURE); 
}

int encrypt() {
    DES_cblock* key = malloc(sizeof(*key));
    store_key(key);
    printf("My key is %s", key);
}

int decrypt() {

}