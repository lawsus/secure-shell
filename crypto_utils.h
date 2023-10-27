#pragma once

#include "bignum.h"

/**
 * This function generates a random
 * num between lower and upper inclusive.
 */
long long generate_random_long_long(long long lower, long long upper);

/**
 * This function determines
 * if a number is prime.
 */
int is_prime(long long n);

/**
 * This function generates a random prime
 * number between lower and upper inclusive.
 */
long long generate_random_prime(long long lower, long long upper);

/**
 * This function calculates a*b % mod
 */
// long long mult_mod(long long a, long long b, long long mod);
Bignum mult_mod(const Bignum* a, const Bignum* b, const Bignum* mod);

/**
 * This function calculates base^exp % mod.
 */
long long power_mod(long long base, long long exp, long long mod);

/**
 * This function determines if a number g
 * is a primitive root modulo p.
 */
int is_primitive_root(long long g, long long p);

/**
 * This function finds a primitive root for
 * prime number p.
 */
long long find_primitive_root(long long p);