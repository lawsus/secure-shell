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
- **TODO**: ~~Fork in server for multi processing.~~

#### 10/14/23
- Forked in server for multi processing, allowing multiple users to connect.
- Implemented zombie reaping for completed processes.
- Created a cryptography utility file, completing functions for generating random numbers, checking if a number is prime, generating random prime numbers, and a power function (all for long long).
- Created a diffie-hellman file for computing public keys and computing private keys.
- Integrated diffie-hellman into client and server, generating session keys and values in server and sending them to the client.
- Added Makefile for server and client execs.
- **TODO**: ~~Finish diffie-hellman key exchange (sending client’s public key to server and computing shared secrets for both client and server) + learn more about how SSH keys work.~~
- **TODO**: ~~Overflow error in power function for long long during intermediate calculations (possible solutions: int128, multiplication func, custom struct).~~
- **FUTURE**: Change DH to elliptic curve DH with curve25519 for more secure key exchange (already have implementation in Python, just need to rewrite it in C).

#### 10/16/23
- Fixed overflow on power function using a multiplication function which works in the same way as the power function.
- Finished DH key exchange, both client and server accurately calculate the shared secret key.
- Added additional print statements in `format.c`.
- **TODO**: ~~`g` must be a primitive root modulo `p`.~~
- **TODO**: Make a key derivation function.
- **TODO**: Make a symmetric encryption function such as AES.
- **TODO**: Make a MAC function.

#### 10/17/23
- Fixed base point gen.
- **TODO**: Make my own integer data type.
- **TODO**: Implement nonce.

#### 10/19/23
- Learned about and started implementation of bignum (long long to big num and addition).
