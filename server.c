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

void reap_zombies(int signal_number) {
    (void) signal_number;
    // Reap zombie processes
    while (waitpid(-1, NULL, WNOHANG) > 0);
}

void handle_client(int client_socket) {
    char buffer[1024] = {0};

    // dh key exchange
    srand(time(NULL));
    long long lower = 1000000000000;
    long long upper = 9999999999999;
    long long g = generate_random_long_long(lower, upper);
    printf("Generating random prime...\n");
    long long p = generate_random_prime(lower, upper);
    printf("...%lld.\n", p);

    long long private_session_key = generate_random_long_long(lower, upper);
    printf("g: %lld, p: %lld\n", g, p);
    long long public_session_key = compute_public_key(g, private_session_key, p);
    printf("public session key: %lld\n", public_session_key);

    // send base point g, prime g, and public session key
    sprintf(buffer, "%lld %lld %lld", g, p, public_session_key);
    send(client_socket, buffer, strlen(buffer), 0);

    memset(buffer, 0, sizeof(buffer));

    while (1) {
        ssize_t bytes_read = read(client_socket, buffer, 1024);
        if (bytes_read <= 0) {
            printf("Client disconnected or error occurred.\n");
            break;
        }
        printf("Message received: %s\n", buffer);
        send(client_socket, buffer, strlen(buffer), 0);
        printf("Message sent: %s\n", buffer);
    }
    close(client_socket);
}

void start_server() {
    int server_fd, new_socket;
    struct sockaddr_in address;
    int addrlen = sizeof(address);

    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(8080);

    bind(server_fd, (struct sockaddr *)&address, sizeof(address));
    listen(server_fd, 3);

    signal(SIGCHLD, reap_zombies);

    printf("Server is listening...\n");
    while (1) {
        new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen);
        printf("Client connected.\n");

        pid_t pid = fork();
        if (pid < 0) {
            // fork failed
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

// gcc -o server server.c format.c diffie_hellman.c crypto_utils.c