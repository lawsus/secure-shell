#include <stdio.h>
#include "bignum.h"
#include "crypto_utils.h"

#define LOWER 1000000000000
#define UPPER 9999999999999

int main() {
    int i = 10;
    while (i > 0) {
        Bignum a, b, result;

        long long l1 = generate_random_long_long(LOWER, UPPER);
        long long l2 = generate_random_long_long(LOWER, UPPER);

        num_to_bignum(&a, l1);
        num_to_bignum(&b, l2);
        
        // printf("A: ");
        // print_bignum(&a);
        
        // printf("B: ");
        // print_bignum(&b);

        add_bignum(&result, &a, &b);
        printf("A + B: ");
        print_bignum(&result);

        Bignum ans;
        num_to_bignum(&ans, l1 + l2);
        printf("A + B: ");
        print_bignum(&ans);
        
        i--;
    }

    return 0;
}

// gcc -o bignum_test bignum.c bignum_test.c crypto_utils.h
