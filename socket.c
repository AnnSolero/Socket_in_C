#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 12345
#define BUFFER_SIZE 1024

int main() {
    int server_socket, client_socket;
    struct sockaddr_in server_addr, client_addr;
    socklen_t addr_size;
    char buffer[BUFFER_SIZE];

    /* server socket */
    server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket == -1) {
        perror("Error creating server socket");
        exit(EXIT_FAILURE);
    }

    /* Configuring server address */
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(PORT);

    /*Bind server socket to the specified port */
    if (bind(server_socket, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1) {
        perror("Error binding server socket");
        exit(EXIT_FAILURE);
    }

    /* Listen for incoming connections */
    if (listen(server_socket, 1) == -1) {
        perror("Error listening for connections");
        exit(EXIT_FAILURE);
    }

    printf("Server listening on port %d...\n", PORT);

    /* Accepting incoming connection */
    addr_size = sizeof(client_addr);
    client_socket = accept(server_socket, (struct sockaddr *)&client_addr, &addr_size);
    if (client_socket == -1) {
        perror("Error accepting connection");
        exit(EXIT_FAILURE);
    }

    printf("Client connected...\n");

    /* Communication with the client */
    while (1) {
        // Receive message from client
        ssize_t bytes_received = recv(client_socket, buffer, BUFFER_SIZE, 0);
        if (bytes_received <= 0) {
            printf("Client disconnected.\n");
            break;
        }

        buffer[bytes_received] = '\0';
        printf("Received message from client: %s\n", buffer);

        // Example: Send acknowledgment back to the client
        const char *acknowledgment = "Message received!";
        send(client_socket, acknowledgment, strlen(acknowledgment), 0);
    }

    // Close sockets
    close(client_socket);
    close(server_socket);

    return 0;
}
