#include <stdio.h>
#include "bignum.h"
#include "crypto_utils.h"
#include "format.h"
#include <time.h>
#include <stdlib.h>

#define LOWER 1000000000000
#define UPPER 9999999999999

int main() {
    srand(time(NULL));

    // long long l1 = generate_random_long_long(LOWER, UPPER);
    long long l1 = 2;
    long long l2 = generate_random_long_long(LOWER, UPPER);
    long long neg_l1 = -l1;
    long long neg_l2 = -l2;

    printf("l1: %llu\n", l1);
    printf("l2: %llu\n", l2);

    Bignum a = num_to_bignum(l1);
    Bignum b = num_to_bignum(l2);
    Bignum neg_a = num_to_bignum(neg_l1);
    Bignum neg_b = num_to_bignum(neg_l2);
    Bignum temp;

    // Test all combinations for addition
    Bignum result = add_bignum(&a, &b);
    print_Bignum("A + B: ", &result);
    temp = num_to_bignum(l1 + l2);
    if (!equal_bignum(&result, &temp)) {
        printf("Mismatch in A + B\n");
    }

    result = add_bignum(&a, &neg_b);
    print_Bignum("A + (-B): ", &result);
    temp = num_to_bignum(l1 - l2);
    if (!equal_bignum(&result, &temp)) {
        printf("Mismatch in A + (-B)\n");
    }

    result = add_bignum(&neg_a, &b);
    print_Bignum("(-A) + B: ", &result);
    temp = num_to_bignum(l2 - l1);
    if (!equal_bignum(&result, &temp)) {
        printf("Mismatch in (-A) + B\n");
    }

    result = add_bignum(&neg_a, &neg_b);
    print_Bignum("(-A) + (-B): ", &result);
    temp = num_to_bignum(neg_l1 + neg_l2);
    if (!equal_bignum(&result, &temp)) {
        printf("Mismatch in (-A) + (-B)\n");
    }

    // Test all combinations for subtraction
    result = sub_bignum(&a, &b);
    print_Bignum("A - B: ", &result);
    temp = num_to_bignum(l1 - l2);
    if (!equal_bignum(&result, &temp)) {
        printf("Mismatch in A - B\n");
    }

    result = sub_bignum(&a, &neg_b);
    print_Bignum("A - (-B): ", &result);
    temp = num_to_bignum(l1 + l2);
    if (!equal_bignum(&result, &temp)) {
        printf("Mismatch in A - (-B)\n");
    }

    result = sub_bignum(&neg_a, &b);
    print_Bignum("(-A) - B: ", &result);
    temp = num_to_bignum(neg_l1 - l2);
    if (!equal_bignum(&result, &temp)) {
        printf("Mismatch in (-A) - B\n");
    }

    result = sub_bignum(&neg_a, &neg_b);
    print_Bignum("(-A) - (-B): ", &result);
    temp = num_to_bignum(neg_l1 + l2);
    if (!equal_bignum(&result, &temp)) {
        printf("Mismatch in (-A) - (-B)\n");
    }

    // Test division and quotient for a / b
    Bignum quotient = div_bignum(&a, &b);
    Bignum remainder = mod_bignum(&a, &b);
    print_Bignum("A / B (quotient): ", &quotient);
    print_Bignum("A % B (remainder): ", &remainder);
    if (l2 != 0) {
        temp = num_to_bignum(l1 / l2);
        if (!equal_bignum(&quotient, &temp)) {
            printf("Mismatch in A / B\n");
        }
        temp = num_to_bignum(l1 % l2);
        if (!equal_bignum(&remainder, &temp)) {
            printf("Mismatch in A %% B\n");
        }
    }

    // Test division and quotient for b / a
    quotient = div_bignum(&b, &a);
    remainder = mod_bignum(&b, &a);
    print_Bignum("B / A (quotient): ", &quotient);
    print_Bignum("B % A (remainder): ", &remainder);
    if (l1 != 0) {
        temp = num_to_bignum(l2 / l1);
        if (!equal_bignum(&quotient, &temp)) {
            printf("Mismatch in B / A\n");
        }
        temp = num_to_bignum(l2 % l1);
        if (!equal_bignum(&remainder, &temp)) {
            printf("Mismatch in B %% A\n");
        }
    }

    return 0;
}

// gcc -o bignum_test bignum.c bignum_test.c crypto_utils.h
