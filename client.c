#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#include "diffie_hellman.h"
#include "crypto_utils.h"
#include "format.h"

#include <errno.h>

#define LOWER 1000000000000
#define UPPER 9999999999999

void key_exchange(int sock, long long *shared_secret) {
    char buffer[1024] = {0};
    // dh key exchange
    recv(sock, buffer, 1024, 0);
    long long p, g, server_public_key;
    sscanf(buffer, "%lld %lld %lld", &p, &g, &server_public_key);
    print_received("p", p);
    print_received("G", g);
    print_received("server public key", server_public_key);

    long long client_private_key = generate_random_long_long(LOWER, UPPER);
    // printf("client private key: %lld\n", client_private_key);
    long long client_public_key = compute_public_key(g, client_private_key, p);

    memset(buffer, 0, 1024);

    sprintf(buffer, "%lld", client_public_key);
    send(sock, buffer, strlen(buffer), 0);
    print_sent("client public key", client_public_key);

    *shared_secret = compute_shared_secret(server_public_key, client_private_key, p);
}
// 656574425985^993477851764 % 6612879338959

void start_client() {
    struct sockaddr_in address;
    int sock = 0;

    sock = socket(AF_INET, SOCK_STREAM, 0);
    address.sin_family = AF_INET;
    address.sin_port = htons(8080);
    inet_pton(AF_INET, "127.0.0.1", &address.sin_addr);

    connect(sock, (struct sockaddr *)&address, sizeof(address));

    long long shared_secret;
    key_exchange(sock, &shared_secret);
    print_shared_secret(shared_secret);

    unsigned char aes_key[KEY_LENGTH];
    derive_aes_key_from_shared_secret(shared_secret, aes_key, sizeof(aes_key));

    // printf("aes_key: %s\n", aes_key);
    printf("Derived Key: ");
    for (int i = 0; i < KEY_LENGTH; ++i) {
        printf("%02x", aes_key[i]);
    }
    printf("\n");

    unsigned char buffer[1024] = {0};

    while (1) {
        ssize_t bytes_read = 0;
        printf("> ");
        fgets((char*)buffer, 1024, stdin);
        buffer[strcspn((char*)buffer, "\n")] = 0;

        unsigned char iv[AES_BLOCK_SIZE];
        RAND_bytes(iv, AES_BLOCK_SIZE); // gen iv
        
        // printf("Send IV: ");
        // for (int i = 0; i < AES_BLOCK_SIZE; ++i) {
        //     printf("%02x", iv[i]);
        // }
        // printf("\n");

        send(sock, iv, AES_BLOCK_SIZE, 0); // send iv

        unsigned char encrypted_message[1024];
        int encrypted_size = aes_encrypt(buffer, aes_key, iv, encrypted_message); // encrypt message

        // printf("Send encrypted message: ");
        // for (int i = 0; i < encrypted_size; ++i) {
        //     printf("%02x", encrypted_message[i]);
        // }
        // printf("\n");

        send(sock, encrypted_message, encrypted_size, 0); // send encrypted message
        
        // printf("Message sent: %s\n", buffer);

        // while (1) {
        //     unsigned char received_iv[AES_BLOCK_SIZE] = {0};
        //     ssize_t bytes_read = recv(sock, received_iv, AES_BLOCK_SIZE, 0);

        //     unsigned char received_encrypted_message[1024] = {0};
        //     bytes_read = recv(sock, received_encrypted_message, 1024, 0);

        //     unsigned char decrypted_message[1024] = {0};
        //     int decrypted_size = aes_decrypt(received_encrypted_message, bytes_read, aes_key, received_iv, decrypted_message);
        //     printf("%.*s", decrypted_size, decrypted_message);

        //     if (strncmp((char*) decrypted_message, "END_OUTPUT", strlen("END_OUTPUT")) == 0) {
        //         printf("\n");
        //         break;
        //     }
        // }

        unsigned char received_iv[AES_BLOCK_SIZE];
        bytes_read = recv(sock, received_iv, AES_BLOCK_SIZE, 0); // receive iv
        unsigned char received_encrypted_message[1024];
        bytes_read = recv(sock, received_encrypted_message, 1024, 0); // receive encrypted response
        unsigned char decrypted_message[1024] = {0};
        int decrypted_size = aes_decrypt(received_encrypted_message, bytes_read, aes_key, received_iv, decrypted_message); // decrypt response
        printf("%.*s\n", decrypted_size, decrypted_message);
    }
    close(sock);
}

int main() {
    start_client();
    return 0;
}