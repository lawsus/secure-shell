#pragma once

#define BASE 1000000000 // 10^9
#define SIZE 100

typedef struct {
    int digits[SIZE];
    int size; // max num of blocks
    int sign; // 1 is positive, -1 is negative
} Bignum;

// /**
//  * Initialize a bignum to 0.
//  * @return Initialized bignum.
//  */
// Bignum init_bignum();

// /**
//  * Convert a long long to a bignum.
//  * @param value The long long value to convert.
//  * @return The converted bignum.
//  */
// Bignum num_to_bignum(long long value);

// /**
//  * Subtract two bignums.
//  * @param a First bignum.
//  * @param b Second bignum.
//  * @return The difference of the two bignums.
//  */
// Bignum sub_bignum(const Bignum* a, const Bignum* b);

// /**
//  * Add two bignums.
//  * @param a First bignum.
//  * @param b Second bignum.
//  * @return The sum of the two bignums.
//  */
// Bignum add_bignum(const Bignum* a, const Bignum* b);

// /**
//  * Multiply two bignums.
//  * @param a First bignum.
//  * @param b Second bignum.
//  * @return The product of the two bignums.
//  */
// Bignum mult_bignum(const Bignum* a, const Bignum* b);

// /**
//  * ++.
//  * @param num First bignum.
//  * @return bignum + 1.
//  */
// Bignum pp_bignum(const Bignum* num);

// /**
//  * --.
//  * @param num First bignum.
//  * @return bignum - 1.
//  */
// Bignum mm_bignum(const Bignum* num);

// /**
//  * Multiply two bignums with modulo.
//  * @param a First bignum.
//  * @param b Second bignum.
//  * @param mod The modulo bignum.
//  * @return The product of the two bignums modulo mod.
//  */
// Bignum multmod_bignum(const Bignum* a, const Bignum* b, const Bignum* mod);

// /**
//  * Power two bignums with modulo.
//  * @param base The base bignum.
//  * @param exp The exponent bignum.
//  * @param mod The modulo bignum.
//  * @return The result of (base ^ exp) % mod.
//  */
// Bignum powermod_bignum(const Bignum* base, const Bignum* exp, const Bignum* mod);

// /**
//  * Print out a bignum.
//  * @param num The bignum to print.
//  */
// void print_bignum(const Bignum* num);

// int equal_bignum(const Bignum* a, const Bignum* b);

Bignum init_bignum();
Bignum num_to_bignum(long long value);
int is_zero(const Bignum* num);
Bignum _sub_bignum(const Bignum* a, const Bignum* b);
int equal_bignum(const Bignum* a, const Bignum* b);
int is_greater_than(const Bignum* a, const Bignum* b);
int is_greater_than_or_equal(const Bignum* a, const Bignum* b);
Bignum sub_bignum(const Bignum* a, const Bignum* b);
Bignum add_bignum(const Bignum* a, const Bignum* b);
void print_bignum(const Bignum* num);
Bignum mult_bignum(const Bignum* a, const Bignum* b);
Bignum pp_bignum(const Bignum* num);
Bignum mm_bignum(const Bignum* num);
Bignum mod_bignum(const Bignum* a, const Bignum* b);
Bignum div_bignum(const Bignum* a, const Bignum* b);
void shift_left(Bignum* num);
void divmod_bignum(const Bignum* a, const Bignum* b, Bignum* quotient, Bignum* remainder);
long long bignum_to_num(const Bignum* num);