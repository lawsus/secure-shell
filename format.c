#include <stdio.h>
#include <stdlib.h>
// #include "bignum.h"

void print_fork_failed() {
    fprintf(stderr, "Fork Failed!\n");
    exit(1);
}

// Updated print statements for bignums
// void print_received(char* name, Bignum* val) {
//     printf("Received %s: ", name);
//     print_bignum(val);
// }

// void print_sent(char* name, Bignum* val) {
//     printf("Sent %s: ", name);
//     print_bignum(val);
// }

void print_received(char* name, long long val) {
    printf("Received %s: %lld\n", name, val);
}

void print_sent(char* name, long long val) {
    printf("Sent %s: %lld\n", name, val);
}

void print_shared_secret(long long val) {
    printf("Shared secret: %lld\n", val);
}

void print_disconnected(char *component) {
    printf("%s disconnected or error occurred.\n", component);
}