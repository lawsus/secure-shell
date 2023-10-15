#include "crypto_utils.h"
#include <stdlib.h>

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
// long long power(long long base, long long exp, long long mod) {
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


// TODO: Fix overflow in intermediate calculations (res * base) and (base * base).

// return (base ^ exp) % mod
// (a * b) % p == ((a % p) * (b % p)) mod p (necessary ?)
long long power(long long base, long long exp, long long mod) {
    long long res = 1;
    base = base % mod;
    while (exp > 0) {
        if (exp % 2 == 1) {
            // res = ((res % mod) * (base % mod)) % mod;
            res = (res * base) % mod;
        }
        exp = exp >> 1;
        // base = ((base % mod) * (base % mod)) % mod;
        base = (base * base) % mod;
    }
    return res;
}