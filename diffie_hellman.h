#pragma once

/**
 * This function computes the public key.
 */
long long compute_public_key(long long g, long long private_key, long long p);

/**
 * This function computes the shared secret.
 */
long long compute_shared_secret(long long public_key, long long private_key, long long p);


// TODO: Implement elliptic-curve diffie-hellman