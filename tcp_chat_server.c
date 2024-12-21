#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

int main() {
    // Step 1: Create socket
    int ServerSocketFD = socket(AF_INET, SOCK_STREAM, 0);
    if (ServerSocketFD < 0) {
        perror("Socket creation failed");
        return 1;
    }

    // Step 2: Define server address
    struct sockaddr_in ServerAddress;
    memset(&ServerAddress, 0, sizeof(ServerAddress)); // Clear structure
    ServerAddress.sin_family = AF_INET;
    ServerAddress.sin_port = htons(2004);
    inet_pton(AF_INET, "127.0.0.1", &ServerAddress.sin_addr.s_addr);

    // Step 3: Bind socket to the server address
    int result = bind(ServerSocketFD, (struct sockaddr*)&ServerAddress, sizeof(ServerAddress));
    if (result < 0) {
        perror("Failed to bind socket");
        close(ServerSocketFD);
        return 1;
    }
    printf("Server socket was bound successfully\n");

    // Step 4: Start listening for connections
    int listenResult = listen(ServerSocketFD, 10);
    if (listenResult < 0) {
        perror("Failed to start listening");
        close(ServerSocketFD);
        return 1;
    }
    printf("Server is listening...\n");

    // Step 5: Accept client connection
    struct sockaddr_in ClientAddress;
    socklen_t ClientAddressSize = sizeof(ClientAddress);
    int ClientSocketFD = accept(ServerSocketFD, (struct sockaddr*)&ClientAddress, &ClientAddressSize);
    if (ClientSocketFD < 0) {
        perror("Failed to accept client connection");
        close(ServerSocketFD);
        return 1;
    }
    printf("Client connected!\n");

    // Send welcome message
    char* message = "Let's chat, darling...\n";
    send(ClientSocketFD, message, strlen(message), 0);

    // Chat loop
    while (1) {
        char buffer[1024];
        memset(buffer, 0, sizeof(buffer));
        
        // Receive message from client
        int recvBytes = recv(ClientSocketFD, buffer, sizeof(buffer) - 1, 0);
        if (recvBytes <= 0) {
            printf("Connection closed by client or error.\n");
            break;
        }
        printf("Client: %s\n", buffer);

        // Check for "quit" keyword
        if (strcmp(buffer, "quit") == 0) {
            printf("Client has ended the chat.\n");
            break;
        }

        // Send message to client
        printf("Enter message: ");
        char message1[1024];
        fgets(message1, sizeof(message1), stdin); // Safely read input
        message1[strcspn(message1, "\n")] = '\0'; // Remove trailing newline
        send(ClientSocketFD, message1, strlen(message1), 0);
    }

    // Cleanup
    close(ClientSocketFD);
    close(ServerSocketFD);
    printf("Server shut down.\n");

    return 0;
}
