#include <stdio.h>
#include "bignum.h"

Bignum init_bignum() {
    Bignum num;
    for (int i = 0; i < SIZE; i++) {
        num.digits[i] = 0;
    }
    num.sign = 1;
    return num;
}

Bignum num_to_bignum(long long value) {
    Bignum res = init_bignum();
    if (value == 0) { return res; }

    if (value < 0) {
        value = -1 * value;
        res.sign = -1;
    } else {
        res.sign = 1;
    }

    int index = 0;
    while (value > 0 && index < SIZE) {
        res.digits[index] = value % BASE;
        value /= BASE;
        index++;
    }
    return res;
}

int is_zero(const Bignum* num) {
    for (int i = 0; i < SIZE; i++) {
        if (num->digits[i] != 0) {
            return 0; // False
        }
    }
    return 1; // True
}

// Helper function for a - b where a >= b
Bignum _sub_bignum(const Bignum* a, const Bignum* b) {
    Bignum res = init_bignum();

    int borrow = 0;
    for (int i = 0; i < SIZE; i++) {
        int diff = a->digits[i] - b->digits[i] - borrow;
        if (diff < 0) {
            diff += BASE;
            borrow = 1;
        } else {
            borrow = 0;
        }
        res.digits[i] = diff;
    }

    return res;
}

int equal_bignum(const Bignum* a, const Bignum* b) {
    if (a->sign != b->sign) { return 0; }

    for (int i = SIZE - 1; i >= 0; i--) {
        if (a->digits[i] != b->digits[i]) {
            return 0; // not equal
        }
    }
    return 1; // equal
}

int is_greater_than(const Bignum* a, const Bignum* b) {
    // a > 0, b < 0
    if (a->sign == 1 && b->sign == -1) {
        return 1;
    }
    // a < 0, b > 0
    else if (a->sign == -1 && b->sign == 1) {
        return 0;
    }
    // a > 0, b > 0
    else if (a->sign == 1 && b->sign == 1) {
        for (int i = SIZE - 1; i >= 0; i--) {
            if (a->digits[i] > b->digits[i]) {
                return 1;
            } else if (a->digits[i] < b->digits[i]) {
                return 0;
            }
        }
        return 0; // equal
    }
    // a < 0, b < 0
    else {
        for (int i = SIZE - 1; i >= 0; i--) {
            if (a->digits[i] < b->digits[i]) {
                return 1;
            } else if (a->digits[i] > b->digits[i]) {
                return 0;
            }
        }
        return 0; // equal
    }
}


int is_greater_than_or_equal(const Bignum* a, const Bignum* b) {
    // a > 0 = 0 > b
    if (a->sign == 1 && b->sign == -1) {
        return 1;
    }
    // a < 0 = 0 < b
    else if (a->sign == -1 && b->sign == 1) {
        return 0;
    }
    // a > 0, b > 0
    else if (a->sign == 1 && b->sign == 1) {
        for (int i = SIZE - 1; i >= 0; i--) {
            if (a->digits[i] > b->digits[i]) {
                return 1;
            } else if (a->digits[i] < b->digits[i]) {
                return 0;
            }
        }
        return 1; // equal
    }
    // a < 0, b < 0
    else {
        for (int i = SIZE - 1; i >= 0; i--) {
            if (a->digits[i] < b->digits[i]) {
                return 1;
            } else if (a->digits[i] > b->digits[i]) {
                return 0;
            }
        }
        return 1; // equal
    }
}

Bignum sub_bignum(const Bignum* a, const Bignum* b) {
    Bignum res;
    Bignum abs_a = *a;
    Bignum abs_b = *b;
    abs_a.sign = 1;
    abs_b.sign = 1;

    // (-a) - (-b) = b - a
    if (a->sign == -1 && b->sign == -1) {
        if (is_greater_than_or_equal(&abs_b, &abs_a)) {
            res = _sub_bignum(&abs_b, &abs_a);
        } else {
            res = _sub_bignum(&abs_a, &abs_b);
            res.sign = -1;
        }
    }
    // a - (-b) = a + b
    else if (a->sign == 1 && b->sign == -1) {
        res = add_bignum(a, &abs_b);
    }
    // (-a) - b = - (a + b)
    else if (a->sign == -1 && b->sign == 1) {
        res = add_bignum(&abs_a, b);
        res.sign = -1;
    }
    // a - b
    else {
        if (is_greater_than_or_equal(a, b)) {
            res = _sub_bignum(a, b);
        } else {
            res = _sub_bignum(b, a);
            res.sign = -1;
        }
    }

    return res;
}

Bignum add_bignum(const Bignum* a, const Bignum* b) {
    Bignum abs_a = *a;
    Bignum abs_b = *b;
    abs_a.sign = 1;
    abs_b.sign = 1;

    // a + (-b)
    if (a->sign == 1 && b->sign == -1) {
        return sub_bignum(a, &abs_b);
    } 
    // (-a) + b
    else if (a->sign == -1 && b->sign == 1) {
        return sub_bignum(b, &abs_a);
    }

    // add two positive numbers or two negative numbers
    Bignum res = init_bignum();

    if (a->sign == 1 && b->sign == 1) {
        res.sign = 1;
    } else if (a->sign == -1 && b->sign == -1) {
        res.sign = -1;
    }

    int carry = 0;
    for (int i = 0; i < SIZE; i++) {
        int sum = a->digits[i] + b->digits[i] + carry;
        res.digits[i] = sum % BASE;
        carry = sum / BASE;
    }

    return res;
}

void print_bignum(const Bignum* num) {
    if (is_zero(num)) {
        printf("0\n");
        return;
    }

    if (num->sign == -1) {
        printf("-");
    }

    int start = 0;
    for (int i = SIZE - 1; i >= 0; i--) {
        if (num->digits[i] != 0) {
            start = 1;
        }
        if (start) {
            printf("%d", num->digits[i]);
        }
    }
    printf("\n");
}

// void print_bignum(const Bignum* num) {

//     if (num->sign == -1) {
//         printf("-");
//     }
//     int start = 0;
//     for (int i = SIZE - 1; i >= 0; i--) {
//         if (num->digits[i] != 0) {
//             start = 1;
//         }
//         if (start) {
//             printf("%d", num->digits[i]);
//         }
//     }
//     if (!start) {
//         printf("0");
//     }
//     printf("\n");
// }

// chat gpt generated, needs to be optimized, currently just long multiplication
Bignum mult_bignum(const Bignum* a, const Bignum* b) {
    Bignum res = init_bignum();
    for (int i = 0; i < SIZE; i++) {
        int carry = 0;
        for (int j = 0; j < SIZE; j++) {
            if (i + j < SIZE) {
                long long product = (long long)a->digits[i] * b->digits[j] + carry + res.digits[i + j];
                res.digits[i + j] = product % BASE;
                carry = product / BASE;
            }
        }
    }
    return res;
}

Bignum pp_bignum(const Bignum* num) {
    Bignum one = num_to_bignum(1);
    return add_bignum(num, &one);
}

Bignum mm_bignum(const Bignum* num) {
    Bignum one = num_to_bignum(1);
    return sub_bignum(num, &one);
}

Bignum mod_bignum(const Bignum* a, const Bignum* b) {
    Bignum quotient = init_bignum();
    Bignum remainder = init_bignum();
    divmod_bignum(a, b, &quotient, &remainder);
    return remainder;
}

Bignum div_bignum(const Bignum* a, const Bignum* b) {
    // printf("started div\n");
    Bignum quotient = init_bignum();
    Bignum remainder = init_bignum();
    divmod_bignum(a, b, &quotient, &remainder);
    // printf("finished div\n");
    return quotient;
}

// chat gpt generated
void shift_left(Bignum* num) {
    for (int i = SIZE - 1; i > 0; i--) {
        num->digits[i] = num->digits[i - 1];
    }
    num->digits[0] = 0;
}

// void divmod_bignum(const Bignum* a, const Bignum* b, Bignum* quotient, Bignum* remainder) {
//     // This function implements the basic long division algorithm.
//     // It's a placeholder and might not be the most efficient for very large numbers.
//     // You might want to optimize this further for better performance.

//     Bignum temp = init_bignum();
//     for (int i = SIZE - 1; i >= 0; i--) {
//         shift_left(remainder); // Multiply remainder by BASE
//         remainder->digits[0] = a->digits[i];
//         int q = 0;
//         while (is_greater_than_or_equal(remainder, b)) {
//             *remainder = sub_bignum(remainder, b);
//             q++;
//         }
//         temp.digits[i] = q;
//     }
//     *quotient = temp;
// }

// improved efficiency for dividing by 2 as it is used frequently
void divmod_bignum(const Bignum* a, const Bignum* b, Bignum* quotient, Bignum* remainder) {
    Bignum temp = init_bignum();

    // Special case for dividing by 2
    Bignum two = num_to_bignum(2);
    if (equal_bignum(b, &two)) {
        int carry = 0;
        for (int i = SIZE - 1; i >= 0; i--) {
            int current = a->digits[i] + carry * BASE;
            temp.digits[i] = current / 2;
            carry = current % 2;
        }
        *remainder = num_to_bignum(carry);
        *quotient = temp;
        return;
    }

    // General case
    for (int i = SIZE - 1; i >= 0; i--) {
        shift_left(remainder); // Multiply remainder by BASE
        remainder->digits[0] = a->digits[i];
        int q = 0;
        while (is_greater_than_or_equal(remainder, b)) {
            *remainder = sub_bignum(remainder, b);
            q++;
        }
        temp.digits[i] = q;
    }
    *quotient = temp;
}

long long bignum_to_num(const Bignum* num) {
    long long value = 0;
    for (int i = SIZE - 1; i >= 0; i--) {
        value = value * BASE + num->digits[i];
    }
    return value * num->sign;
}
