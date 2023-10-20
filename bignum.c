#include <stdio.h>

#include "bignum.h"

void init_bignum(Bignum* num) {
    for (int i = 0; i < SIZE; i++) {
        num->digits[i] = 0;
    }
}

// 123456789
// 0:789, 1:456, 2:123
void num_to_bignum(Bignum* res, long long value) {
    init_bignum(res);
    int index = 0;
    while (value > 0 && index < SIZE) {
        res->digits[index] = value % BASE; // take 9 digits
        value /= BASE;
        index++;
    }
}

void add_bignum(Bignum* res, const Bignum* a, const Bignum* b) {
    int carry = 0;
    for (int i = 0; i < SIZE; i++) {
        int sum = a->digits[i] + b->digits[i] + carry;
        res->digits[i] = sum % BASE;
        carry = sum / BASE;
    }
}

void print_bignum(const Bignum* num) {
    int start = 0;
    for (int i = SIZE - 1; i >= 0; i--) {
        if (num->digits[i] != 0) {
            start = 1;
        }
        if (start) {
            printf("%d", num->digits[i]);
        }
    }
    if (!start) {
        printf("0");
    }
    printf("\n");
}