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

// int aes_encrypt(unsigned char *plaintext, unsigned char *key, unsigned char *iv, unsigned char *ciphertext) {
//     AES_KEY enc_key;

//     AES_set_encrypt_key(key, 128, &enc_key); // Set the encryption key
//     int size = AES_cbc_encrypt(plaintext, ciphertext, sizeof(plaintext), &enc_key, iv, AES_ENCRYPT);
//     return size;
// }

// int aes_encrypt(unsigned char *plaintext, unsigned char *key, unsigned char *iv, unsigned char *ciphertext) {
//     AES_KEY enc_key;

//     AES_set_encrypt_key(key, 128, &enc_key); // Set the encryption key

//     // Calculate the size of the plaintext with padding
//     int plaintext_len = strlen((char *)plaintext);
//     int padded_size = ((plaintext_len / AES_BLOCK_SIZE) + 1) * AES_BLOCK_SIZE;

//     // Perform the encryption
//     AES_cbc_encrypt(plaintext, ciphertext, padded_size, &enc_key, iv, AES_ENCRYPT);

//     return padded_size; // Return the size of the ciphertext
// }

// int aes_decrypt(unsigned char *ciphertext, unsigned char *key, unsigned char *iv, unsigned char *plaintext) {
//     AES_KEY dec_key;

//     AES_set_decrypt_key(key, 128, &dec_key); // Set the decryption key
//     int size = AES_cbc_encrypt(ciphertext, plaintext, sizeof(ciphertext), &dec_key, iv, AES_DECRYPT);
//     return size;
// }

// int aes_decrypt(unsigned char *ciphertext, unsigned char *key, unsigned char *iv, unsigned char *plaintext) {
//     AES_KEY dec_key;

//     AES_set_decrypt_key(key, 128, &dec_key); // Set the decryption key

//     // The size of ciphertext is assumed to be a multiple of AES_BLOCK_SIZE
//     int ciphertext_len = AES_BLOCK_SIZE * (strlen((char *)ciphertext) / AES_BLOCK_SIZE);

//     // Perform the decryption
//     AES_cbc_encrypt(ciphertext, plaintext, ciphertext_len, &dec_key, iv, AES_DECRYPT);

//     // Assuming the plaintext is null-terminated
//     return strlen((char *)plaintext); // Return the size of the plaintext
// }

// int aes_encrypt(unsigned char *plaintext, unsigned char *key, unsigned char *iv, unsigned char *ciphertext);
// int aes_decrypt(unsigned char *ciphertext, int ciphertext_len, unsigned char *key, unsigned char *iv, unsigned char *plaintext);

// int aes_encrypt(unsigned char *plaintext, unsigned char *key, unsigned char *iv, unsigned char *ciphertext) {
//     AES_KEY enc_key;
//     AES_set_encrypt_key(key, 128, &enc_key);

//     int plaintext_len = strlen((char *)plaintext);
//     int padding = AES_BLOCK_SIZE - (plaintext_len % AES_BLOCK_SIZE);
//     int padded_size = plaintext_len + padding;

//     // Copy plaintext to a new buffer and add padding
//     unsigned char padded_plaintext[padded_size];
//     memcpy(padded_plaintext, plaintext, plaintext_len);
//     memset(padded_plaintext + plaintext_len, padding, padding);

//     AES_cbc_encrypt(padded_plaintext, ciphertext, padded_size, &enc_key, iv, AES_ENCRYPT);
//     return padded_size;
// }

// implemented with the help of chat gpt for padding

int aes_encrypt(unsigned char *plaintext, unsigned char *key, unsigned char *iv, unsigned char *ciphertext) {
    AES_KEY enc_key;
    unsigned char temp_iv[AES_BLOCK_SIZE];  // Temporary IV buffer

    // Copy the original IV to the temporary buffer
    memcpy(temp_iv, iv, AES_BLOCK_SIZE);

    AES_set_encrypt_key(key, 128, &enc_key); // Set the encryption key

    // Calculate the size of the plaintext with padding
    int plaintext_len = strlen((char *)plaintext);
    int padded_size = ((plaintext_len / AES_BLOCK_SIZE) + 1) * AES_BLOCK_SIZE;

    // Perform the encryption using temp_iv instead of the original iv
    AES_cbc_encrypt(plaintext, ciphertext, padded_size, &enc_key, temp_iv, AES_ENCRYPT);

    return padded_size; // Return the size of the ciphertext
}

int aes_decrypt(unsigned char *ciphertext, int ciphertext_len, unsigned char *key, unsigned char *iv, unsigned char *plaintext) {
    AES_KEY dec_key;
    AES_set_decrypt_key(key, 128, &dec_key);

    AES_cbc_encrypt(ciphertext, plaintext, ciphertext_len, &dec_key, iv, AES_DECRYPT);

    // Remove padding
    int padding = plaintext[ciphertext_len - 1];
    int plaintext_len = ciphertext_len - padding;
    plaintext[plaintext_len] = '\0'; // Assuming plaintext is ASCII and null-terminated
    return plaintext_len;
}

void long_long_to_byte_array(long long num, unsigned char *byte_array, size_t byte_array_len) {
    if (byte_array_len < sizeof(num)) return;
    memset(byte_array, 0, byte_array_len); // zero out important !!!
    memcpy(byte_array, &num, sizeof(num));
    // if (byte_array_len < sizeof(num)) return;
    // long long num_be = OSSwapHostToBigInt64(num); // Convert to big-endian byte order
    // memcpy(byte_array, &num_be, sizeof(num_be));
}

void derive_aes_key_from_shared_secret(long long shared_secret, unsigned char *key, size_t key_len) {
    unsigned char shared_secret_bytes[sizeof(shared_secret)];
    long_long_to_byte_array(shared_secret, shared_secret_bytes, sizeof(shared_secret_bytes));

    printf("Shared Secret Bytes: ");
    for (int i = 0; i < (int)sizeof(shared_secret_bytes); ++i) {
        printf("%02x", shared_secret_bytes[i]);
    }
    printf("\n");

    // unsigned char salt[SALT_LENGTH] = "XXXX";
    unsigned char salt[SALT_LENGTH] = {'X', 'X', 'X', 'X'};
    
    PKCS5_PBKDF2_HMAC((char *)shared_secret_bytes, sizeof(shared_secret_bytes), salt, sizeof(salt), 10000, EVP_sha256(), key_len, key);

    // printf("Derived Key: ");
    // for (int i = 0; i < KEY_LENGTH; ++i) {
    //     printf("%02x", aes_key[i]);
    // }
    // printf("\n");
}