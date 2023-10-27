#include "crypto_utils.h"
#include <stdlib.h>
#include <stdio.h>
#include "bignum.h"
#include "format.h"

long long generate_random_long_long(long long lower, long long upper) {
    int rand_num = rand();
    double normalized = (double) rand_num / RAND_MAX;
    double scaled = normalized * (upper - lower + 1);
    return lower + (long long) scaled;
}

// https://www.geeksforgeeks.org/prime-numbers/
int is_prime(long long n) {
    if (n <= 1) return 0;
    if (n <= 3) return 1;
    if (n % 2 == 0 || n % 3 == 0) return 0;
    for (long long i = 5; i * i <= n; i += 6) {
        if (n % i == 0 || n % (i + 2) == 0) return 0;
    }
    return 1;
}

long long generate_random_prime(long long lower, long long upper) {
    long long num;
    do {
        num = generate_random_long_long(lower, upper);
    } while (!is_prime(num));
    return num;
}

// Old leetcode implementation of power
// long long power_mod(long long base, long long exp, long long mod) {
//     long long res = 1;
//     base = base % mod;
//     while (exp != 0) {
//         if (exp % 2 == 1) {
//             res = (res * base)  % mod;
//             exp--;
//         }
//         base = (base * base) % mod;
//         exp = exp >> 1;
//     }
// }


// mult: Fix overflow in intermediate calculations (res * base) and (base * base).

// long long mult_mod(long long a, long long b, long long mod) {
//     long long res = 0;
//     a = a % mod;
//     while (b > 0) {
//         if (b % 2 == 1) {
//             res = (res + a) % mod;
//         }
//         a = (a * 2) % mod;
//         b /= 2;
//     }
//     return res % mod;
// }

// // return (base ^ exp) % mod
// // (a * b) % p == ((a % p) * (b % p)) mod p (necessary ?)
// long long power_mod(long long base, long long exp, long long mod) {
//     long long res = 1;
//     base = base % mod;
//     while (exp > 0) {
//         if (exp % 2 == 1) {
//             // res = ((res % mod) * (base % mod)) % mod;
//             res = mult_mod(res, base, mod);
//         }
//         exp = exp >> 1;
//         // base = ((base % mod) * (base % mod)) % mod;
//         base = mult_mod(base, base, mod);
//     }
//     return res;
// }

// long long mult_mod(long long a, long long b, long long mod) {
//     long long res = 0;
//     a = a % mod;
//     while (b > 0) {
//         if (b % 2 == 1) {
//             res = (res + a) % mod;
//         }
//         a = (a * 2) % mod;
//         b /= 2;
//     }
//     return res % mod;
// }

Bignum mult_mod(const Bignum* a, const Bignum* b, const Bignum* mod) {
    Bignum res_big = num_to_bignum(0);
    Bignum two = num_to_bignum(2);
    Bignum zero = num_to_bignum(0);

    Bignum a_big = mod_bignum(a, mod);
    Bignum b_big = *b;
    while (is_greater_than(&b_big, &zero)) {
        if (b_big.digits[0] % 2 == 1) {
            res_big = add_bignum(&res_big, &a_big);
            res_big = mod_bignum(&res_big, mod);
        }
        a_big = mult_bignum(&a_big, &two);
        a_big = mod_bignum(&a_big, mod);
        b_big = div_bignum(&b_big, &two);
    }
    return mod_bignum(&res_big, mod);
}

long long power_mod(long long base, long long exp, long long mod) {
    Bignum base_big = num_to_bignum(base);
    Bignum exp_big = num_to_bignum(exp);
    Bignum mod_big = num_to_bignum(mod);
    Bignum res_big = num_to_bignum(1);
    Bignum two = num_to_bignum(2);
    Bignum zero = num_to_bignum(0);

    base_big = mod_bignum(&base_big, &mod_big);
    while (is_greater_than(&exp_big, &zero)) {

        if (exp_big.digits[0] % 2 == 1) {
            res_big = mult_mod(&res_big, &base_big, &mod_big);
        }
        exp_big = div_bignum(&exp_big, &two);
        base_big = mult_mod(&base_big, &base_big, &mod_big);
    }
    // Convert back to long long
    return bignum_to_num(&res_big);
}


// is_primitive_root and find_primitive_root generated using chat gpt
int is_primitive_root(long long g, long long p) {
    long long phi = p - 1;  // Since p is prime
    // Find all prime factors of phi
    long long n = phi;
    for (long long i = 2; i * i <= n; i++) {
        if (n % i == 0) {
            // Check if g^phi/q is congruent to 1 mod p
            if (power_mod(g, phi/i, p) == 1) {
                return 0;
            }
            while (n % i == 0) {
                n /= i;
            }
        }
    }
    if (n > 1) {
        if (power_mod(g, phi/n, p) == 1) {
            return 0;
        }
    }
    return 1;
}

long long find_primitive_root(long long p) {
    for (long long g = 2; g < p; g++) {
        if (is_primitive_root(g, p)) {
            return g;
        }
    }
    return -1;  // Shouldn't reach here for prime p
}
