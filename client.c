#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#include "diffie_hellman.h"
#include "crypto_utils.h"
#include "format.h"

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
    long long client_public_key = compute_public_key(g, client_private_key, p);

    sprintf(buffer, "%lld", client_public_key);
    send(sock, buffer, strlen(buffer), 0);
    print_sent("client public key", client_public_key);

    *shared_secret = compute_shared_secret(server_public_key, client_private_key, p);
}

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

    char buffer[1024] = {0};

    while (1) {
        printf("> ");
        fgets(buffer, 1024, stdin);
        buffer[strcspn(buffer, "\n")] = 0;
        
        send(sock, buffer, strlen(buffer), 0);
        printf("Message sent: %s\n", buffer);

        ssize_t bytes_received = read(sock, buffer, 1024);
        if (bytes_received <= 0) {
            print_disconnected("Server");
            break;
        }
        buffer[bytes_received] = '\0';
        printf("Message received: %s\n", buffer);
    }
    close(sock);
}

int main() {
    start_client();
    return 0;
}