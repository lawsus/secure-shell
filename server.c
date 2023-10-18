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
    char buffer[1024] = {0};
    // dh key exchange
    srand(time(NULL));
    long long p = generate_random_prime(LOWER, UPPER);
    // g must be a primitive root modulo p
    long long g = find_primitive_root(p);
    long long server_private_key = generate_random_long_long(LOWER, UPPER);
    // printf("server private key: %lld\n", server_private_key);
    long long server_public_key = compute_public_key(g, server_private_key, p);

    // send prime p, base point g, and public session key
    sprintf(buffer, "%lld %lld %lld", p, g, server_public_key);
    send(client_socket, buffer, strlen(buffer), 0);
    print_sent("p", p);
    print_sent("G", g);
    print_sent("server public key", server_public_key);

    recv(client_socket, buffer, 1024, 0);
    long long client_public_key;
    sscanf(buffer, "%lld", &client_public_key);
    print_received("client public key", client_public_key);

    *shared_secret = compute_shared_secret(client_public_key, server_private_key, p);
}

// 9934778517649^656574425985 % 6612879338959

void handle_client(int client_socket) {
    long long shared_secret;
    key_exchange(client_socket, &shared_secret);
    print_shared_secret(shared_secret);

    char buffer[1024] = {0};
    while (1) {
        ssize_t bytes_read = read(client_socket, buffer, 1024);
        if (bytes_read <= 0) {
            print_disconnected("Client");
            break;
        }
        printf("Message received: %s\n", buffer);
        send(client_socket, buffer, strlen(buffer), 0);
        printf("Message sent: %s\n", buffer);
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

    printf("Server is listening...\n");
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