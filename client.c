#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <unistd.h>

void die_with_error(char *error_msg){
    perror(error_msg);
    exit(EXIT_FAILURE);
}

int main(int argc, char *argv[]) {
    int client_sock, port_no;
    struct sockaddr_in server_addr;
    struct hostent *server;

    char buffer[256];
    if (argc < 3) {
        fprintf(stderr, "Usage: %s hostname port_no\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    printf("Client starting...\n");

    // Create a socket using TCP
    client_sock = socket(AF_INET, SOCK_STREAM, 0);
    if (client_sock < 0)
        die_with_error("Error: socket() failed.");

    printf("Looking for host '%s'...\n", argv[1]);
    server = gethostbyname(argv[1]);
    if (server == NULL)
        die_with_error("Error: No such host.");
    printf("Host found!\n");

    // Establish a connection to server
    port_no = atoi(argv[2]);
    bzero((char *) &server_addr, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    bcopy((char *)server->h_addr, (char *)&server_addr.sin_addr.s_addr, server->h_length);
    server_addr.sin_port = htons(port_no);

    printf("Connecting to server at port %d...\n", port_no);
    if (connect(client_sock, (struct sockaddr *) &server_addr, sizeof(server_addr)) < 0)
        die_with_error("Error: connect() failed.");

    printf("Connection successful!\n");

    // Communicate
    printf("Please type a message: ");
    bzero(buffer, sizeof(buffer));
    fgets(buffer, sizeof(buffer) - 1, stdin);

    printf("Sending message to server...\n");
    ssize_t n = send(client_sock, buffer, strlen(buffer), 0);
    if (n < 0)
        die_with_error("Error: send() failed.");

    printf("Message sent! Awaiting reply...\n");
    bzero(buffer, sizeof(buffer));
    n = recv(client_sock, buffer, sizeof(buffer), 0);
    if (n < 0)
        die_with_error("Error: recv() failed.");
    printf("Server says: %s\n", buffer);

    close(client_sock);

    return 0;
}