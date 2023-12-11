#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <signal.h>
#include <time.h>

#include "format.h"
#include "diffie_hellman.h"
#include "crypto_utils.h"

#define LOWER 1000000000000
#define UPPER 9999999999999

#define MAX_CHUNK_SIZE 1024

static int server_fd;

void reap_zombies(int signal_number) {
    (void) signal_number;
    // Reap zombie processes
    while (waitpid(-1, NULL, WNOHANG) > 0);
}

void handle_sigint(int sig) {
    (void)sig;  // To avoid unused parameter warning
    print_disconnected("Server");
    close(server_fd);
    exit(0);
}

void key_exchange(int client_socket, long long *shared_secret) {
    // printf("start key exchange\n");
    char buffer[1024] = {0};
    // dh key exchange
    srand(time(NULL));
    long long p = generate_random_prime(LOWER, UPPER);
    // g must be a primitive root modulo p
    long long g = find_primitive_root(p);
    long long server_private_key = generate_random_long_long(LOWER, UPPER);
    // printf("SERVER PRIVATE KEY FOR TESTING: %lld\n", server_private_key);
    // printf("server private key: %lld\n", server_private_key);
    long long server_public_key = compute_public_key(g, server_private_key, p);

    // send prime p, base point g, and public session key
    sprintf(buffer, "%lld %lld %lld", p, g, server_public_key);
    send(client_socket, buffer, strlen(buffer), 0);
    print_sent("p", p);
    print_sent("G", g);
    print_sent("server public key", server_public_key);

    memset(buffer, 0, 1024);

    recv(client_socket, buffer, 1024, 0);
    long long client_public_key;
    sscanf(buffer, "%lld", &client_public_key);
    print_received("client public key", client_public_key);

    *shared_secret = compute_shared_secret(client_public_key, server_private_key, p);
}

// 9934778517649^656574425985 % 6612879338959

void handle_client(int client_socket) {
    // printf("handling client\n");
    long long shared_secret;
    key_exchange(client_socket, &shared_secret);
    print_shared_secret(shared_secret);

    unsigned char aes_key[KEY_LENGTH];
    derive_aes_key_from_shared_secret(shared_secret, aes_key, sizeof(aes_key));

    // printf("aes_key: %s\n", aes_key);
    printf("Derived Key: ");
    for (int i = 0; i < KEY_LENGTH; ++i) {
        printf("%02x", aes_key[i]);
    }
    printf("\n");

    // char buffer[1024] = {0};
    while (1) {
        ssize_t bytes_read = 0;
        unsigned char received_iv[AES_BLOCK_SIZE];
        bytes_read = recv(client_socket, received_iv, AES_BLOCK_SIZE, 0); // receive iv
        printf("Receive IV: ");
        for (int i = 0; i < AES_BLOCK_SIZE; ++i) {
            printf("%02x", received_iv[i]);
        }
        printf("\n");

        if (bytes_read <= 0) {
            print_disconnected("Client");
            break;
        }
        unsigned char received_encrypted_message[1024];
        bytes_read = recv(client_socket, received_encrypted_message, 1024, 0); // receive encrypted message
        if (bytes_read <= 0) {
            print_disconnected("Client");
            break;
        }
        printf("Encrypted message received: ");
        for (int i = 0; i < bytes_read; ++i) {
            printf("%02x", received_encrypted_message[i]);
        }
        printf("\n");

        unsigned char decrypted_message[1024];
        int decrypted_size = aes_decrypt(received_encrypted_message, bytes_read, aes_key, received_iv, decrypted_message); // decrypt message

        printf("Decrypted message: %s\n", decrypted_message);

        // Execute the command and capture the output
        char command_output[4096]; // Buffer for command output
        FILE *fp = popen((char *)decrypted_message, "r");
        if (fp == NULL) {
            printf("Error.\n");
        }

        // int end = 0;
        // unsigned char response_chunk[MAX_CHUNK_SIZE];
        // while ((bytes_read = fread(response_chunk, 1, MAX_CHUNK_SIZE, fp)) > 0) {
        //     // if (bytes_read <= 0) break;
        //     if (bytes_read < MAX_CHUNK_SIZE) {
        //         response_chunk[bytes_read] = '\0';
        //         printf("terminate\n");
        //         end = 1;
        //     }
        //     unsigned char iv[AES_BLOCK_SIZE];
        //     RAND_bytes(iv, AES_BLOCK_SIZE); // gen iv
        //     printf("send iv\n");
        //     send(client_socket, iv, AES_BLOCK_SIZE, 0); // send iv
        //     unsigned char encrypted_response_chunk[MAX_CHUNK_SIZE];
        //     int encrypted_response_size = aes_encrypt(response_chunk, aes_key, iv, encrypted_response_chunk);
        //     printf("send response\n");
        //     send(client_socket, encrypted_response_chunk, encrypted_response_size, 0); // send encrypted response chunk
        //     printf("check end\n");
        //     if (end == 1) { 
        //         unsigned char end_output[1024] = "END_OUTPUT";
        //         unsigned char iv[AES_BLOCK_SIZE];
        //         RAND_bytes(iv, AES_BLOCK_SIZE); // gen iv
        //         printf("send iv\n");
        //         send(client_socket, iv, AES_BLOCK_SIZE, 0); // send iv
        //         unsigned char encrypted_end_response[1024];
        //         int encrypted_end_response_size = aes_encrypt((unsigned char *)end_output, aes_key, iv, encrypted_end_response);
        //         send(client_socket, encrypted_end_response, encrypted_end_response_size, 0); // send encrypted end response
        //         printf("break\n");
        //         break; 
        //     }
        // }
        // printf("exit loop\n");
        // pclose(fp);

        bytes_read = fread(command_output, 1, sizeof(command_output) - 1, fp);
        command_output[bytes_read] = '\0';
        pclose(fp);

        unsigned char iv[AES_BLOCK_SIZE];
        RAND_bytes(iv, AES_BLOCK_SIZE); // gen iv
        send(client_socket, iv, AES_BLOCK_SIZE, 0); // send iv
        unsigned char encrypted_response[4096];
        int encrypted_response_size = aes_encrypt((unsigned char *)command_output, aes_key, iv, encrypted_response);
        send(client_socket, encrypted_response, encrypted_response_size, 0); // send encrypted response
    }
    close(client_socket);
}

void start_server() {
    int new_socket;
    struct sockaddr_in address;
    int addrlen = sizeof(address);

    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(8080);

    bind(server_fd, (struct sockaddr *)&address, sizeof(address));
    listen(server_fd, 3);

    signal(SIGCHLD, reap_zombies);
    signal(SIGINT, handle_sigint);

    printf("Server is listening...\n\n");
    while (1) {
        new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen);
        printf("Client connected.\n");

        pid_t pid = fork();
        if (pid < 0) {
            print_fork_failed();
            exit(1);
        } else if (pid > 0) {
            close(new_socket); // parent doesn't need new_socket
        } else {
            // child process
            close(server_fd); // child no longer needs listening scoket
            handle_client(new_socket);
            exit(0); // exit after handling client
        }
    }
    close(server_fd);
}

int main() {
    start_server();
    return 0;
}