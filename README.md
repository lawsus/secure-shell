# Secure Shell

A simple **secure shell** (SSH) implementation in C with a focus on building cryptographic features from scratch.

[Project Proposal](https://www.notion.so/Project-Proposal-8bac309c674b4210925ed08e682078cc)

---

#### File Overview
- `client`
- `server`
- `crypto utils`: Generating random numbers, primes, power functions.
- `diffie-hellman`: Functions for DH key exchange.
- `format`: Print statements for logs.
- `bignum`: Bignum datatype to prevent overflow.

---

### Development Log

#### 10/13/23
- Implemented basic message passing between client and server.
- Learned about how SSH works.

#### 10/14/23
- Forked in server for multi processing, allowing multiple users to connect.
- Implemented zombie reaping for completed processes.
- Created a cryptography utility file, completing functions for generating random numbers, checking if a number is prime, generating random prime numbers, and a power function (all for long long).
- Created a diffie-hellman file for computing public keys and computing private keys.
- Integrated diffie-hellman into client and server, generating session keys and values in server and sending them to the client.
- Added Makefile for server and client execs.

#### 10/16/23
- Fixed overflow on power function using a multiplication function which works in the same way as the power function.
- Finished DH key exchange, both client and server accurately calculate the shared secret key.
- Added additional print statements in `format.c`.

#### 10/17/23
- Fixed base point gen.

#### 10/19/23
- Learned about and started implementation of bignum (long long to big num and addition).

#### 10/26/23
- Finished implementation of bignum.
- Integrated bignum into crypto_util functions, allowing for accurate evalulation of keys without overflow.
- Tested bignum functionality.

#### 11/20/23
- Fixed issue with clearing buffers for key exchange.
- Learn more about key derivation functions + symmetric encryption functions.

#### 11/23/23
- Tried to code key derivation function and symmetric encryption functions, but it wasn't working out very well.
- Decided to use openssl implementations for aes and kdfs, using PKCS5_PBKDF2_HMAC and aes_cbc.

#### 11/24/23
- Finished encrypting and decrypting messages sent between client and server.
- Implemented command execution and response with popen.

#### TODO LIST
- **TODO**: ~~Fork in server for multi processing.~~
- **TODO**: ~~Finish diffie-hellman key exchange (sending client’s public key to server and computing shared secrets for both client and server) + learn more about how SSH keys work.~~
- **TODO**: ~~Overflow error in power function for long long during intermediate calculations (possible solutions: int128, multiplication func, custom struct).~~
- **TODO**: ~~`g` must be a primitive root modulo `p`.~~
- **TODO**: ~~Make my own bignum data type.~~
- **TODO**: ~~Run commands and get output.~~
- **TODO**: ~~Successfully encrypt and decrypt all messages between client and server.~~

#### Future TODO
- **FUTURE**: Change DH to elliptic curve DH with curve25519 for more secure key exchange (already have implementation in Python, just need to rewrite it in C).
- **FUTURE**: Improve efficiency of bignum / crypto util functions.
- **FUTURE**: Clean up documentation / header files
- **FUTURE**: Implement nonce.
- **FUTURE**: Implement my own kdf + symmetric encryption function.
- **FUTURE**: Fix issue with reading in large responses.
- **FUTURE**: Implement current directory.