#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#include "diffie_hellman.h"
#include "crypto_utils.h"

void start_client() {
    struct sockaddr_in address;
    int sock = 0;

    sock = socket(AF_INET, SOCK_STREAM, 0);
    address.sin_family = AF_INET;
    address.sin_port = htons(8080);
    inet_pton(AF_INET, "127.0.0.1", &address.sin_addr);

    connect(sock, (struct sockaddr *)&address, sizeof(address));

    char buffer[1024] = {0};

    // dh key exchange
    recv(sock, buffer, 1024, 0);
    long long g, p, received_public_key;
    sscanf(buffer, "%lld %lld %lld", &g, &p, &received_public_key);
    printf("Received %lld, %lld, %lld\n", g, p, received_public_key);

    long long lower = 1000000000000;
    long long upper = 9999999999999;
    long long private_session_key = generate_random_long_long(lower, upper);
    long long public_session_key = compute_public_key(g, private_session_key, p);

    long long shared_session_secret = compute_shared_secret(public_session_key, private_session_key, p);
    
    memset(buffer, 0, sizeof(buffer));

    while (1) {
        printf("> ");
        fgets(buffer, 1024, stdin);
        buffer[strcspn(buffer, "\n")] = 0;
        
        send(sock, buffer, strlen(buffer), 0);
        printf("Message sent: %s\n", buffer);

        ssize_t bytes_received = read(sock, buffer, 1024);
        if (bytes_received <= 0) {
            printf("Server disconnected or error occurred.\n");
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

// gcc -o client client.c diffie_hellman.c crypto_utils.c