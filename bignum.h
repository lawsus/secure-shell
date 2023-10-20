#define BASE 1000000000 // 10^9
#define SIZE 100

typedef struct {
    int digits[SIZE];
    int size; // number of blocks
} Bignum;

/**
 * Initialize a bignum to 0.
 */
void init_bignum(Bignum* num);

/**
 * Convert a long long to a bignum.
 */
void num_to_bignum(Bignum* res, long long value);

/**
 * Add two bignums.
 */
void add_bignum(Bignum* res, const Bignum* a, const Bignum* b);

/**
 * Subtract two bignums.
 */
// void sub_bignum(Bignum* res, const Bignum* a, const Bignum* b);

/**
 * Multiply two bignums.
 */
void mult_bignum(Bignum* res, const Bignum* a, const Bignum* b);

/**
 * Multiply two bignums. (a * b) % mod
 */
void multmod_bignum(Bignum* res, const Bignum* a, const Bignum* b, const Bignum* mod);

/**
 * Power two bignums. (base ^ exp) % mod
 */
void powermod_bignum(Bignum* res, const Bignum* base, const Bignum* exp, const Bignum* mod);

/**
 * Print out a bignum.
 */
void print_bignum(const Bignum* num);
