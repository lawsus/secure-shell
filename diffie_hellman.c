#include "diffie_hellman.h"
#include "crypto_utils.h"

// A = g^a % p or B = g^b % p
long long compute_public_key(long long g, long long private_key, long long p) {
    return power(g, private_key, p);
}

// (g^a % p)^b == (g^b % p)^a
// A^b % p == B^a % p
long long compute_shared_secret(long long public_key, long long private_key, long long p) {
    return power(public_key, private_key, p);
}