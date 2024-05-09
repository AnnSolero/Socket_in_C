#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>

void die_with_error(char *error_msg){
    perror(error_msg);
    exit(EXIT_FAILURE);
}

int main(int argc, char *argv[]) {
    int server_sock, client_sock, port_no;
    socklen_t client_size;
    char buffer[256];
    struct sockaddr_in server_addr, client_addr;

    if (argc < 2) {
        fprintf(stderr, "Usage: %s port_no\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    printf("Server starting...\n");

    // Create a socket for incoming connections
    server_sock = socket(AF_INET, SOCK_STREAM, 0);
    if (server_sock < 0)
        die_with_error("Error: socket() failed.");

    // Bind socket to a port
    bzero((char *) &server_addr, sizeof(server_addr));
    port_no = atoi(argv[1]);
    server_addr.sin_family = AF_INET; // Internet address
    server_addr.sin_addr.s_addr = INADDR_ANY; // Any incoming interface
    server_addr.sin_port = htons(port_no); // Local port

    if (bind(server_sock, (struct sockaddr *) &server_addr, sizeof(server_addr)) < 0)
        die_with_error("Error: bind() failed.");

    // Mark the socket so it will listen for incoming connections
    listen(server_sock, 5);
    printf("Server listening on port %d...\n", port_no);

    printf("Waiting for connection(s)...\n");

    // Accept new connection
    client_size = sizeof(client_addr);
    client_sock = accept(server_sock, (struct sockaddr *) &client_addr, &client_size);
    if (client_sock < 0)
        die_with_error("Error: accept() failed.");

    printf("Client successfully connected...\n");

    // Communicate
    bzero(buffer, sizeof(buffer));
    ssize_t n = recv(client_sock, buffer, sizeof(buffer), 0);
    if (n < 0)
        die_with_error("Error: recv() failed.");
    printf("Message received from client: %s", buffer);

    printf("Sending reply...\n");
    n = send(client_sock, "I got your message!", 18, 0);
    if (n < 0)
        die_with_error("Error: send() failed.");

    printf("Closing connection...\n");
    close(client_sock);
    close(server_sock);

    return 0;
}