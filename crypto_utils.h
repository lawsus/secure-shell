#pragma once

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
 * This function calculates base^exp % mod.
 */
long long power(long long base, long long exp, long long mod);