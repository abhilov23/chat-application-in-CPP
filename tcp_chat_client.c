#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdbool.h> // Include if you use 'true' and 'false'

int main() {
    // Step 1: Create a socket
    int SocketFD = socket(AF_INET, SOCK_STREAM, 0);
    if (SocketFD < 0) {
        perror("Socket creation failed");
        return 1;
    }

    // Step 2: Define the server address
    struct sockaddr_in address;
    memset(&address, 0, sizeof(address)); // Clear the structure
    address.sin_family = AF_INET;
    address.sin_port = htons(2004);
    inet_pton(AF_INET, "127.0.0.1", &address.sin_addr.s_addr);

    // Step 3: Connect to the server
    int result = connect(SocketFD, (struct sockaddr*)&address, sizeof(address));
    if (result == 0) {
        printf("Connected to the server.\n");
    } else {
        perror("Connection failed");
        close(SocketFD);
        return 1;
    }

    // Step 4: Chat loop
    while (1) {
        // Receiving message from the server
        char buffer[1024];
        memset(buffer, 0, sizeof(buffer)); // Clear the buffer
        int recvBytes = recv(SocketFD, buffer, sizeof(buffer) - 1, 0); // Leave space for null-terminator
        if (recvBytes <= 0) {
            printf("Server disconnected or error.\n");
            break;
        }
        buffer[recvBytes] = '\0'; // Ensure the string is null-terminated
        printf("Server: %s\n", buffer);

        // Check for "quit" to end the chat
        if (strcmp(buffer, "quit") == 0) {
            printf("Server ended the chat.\n");
            break;
        }

        // Sending a message to the server
        printf("Enter your message: ");
        char message1[1024];
        fgets(message1, sizeof(message1), stdin); // Use fgets for safe input
        message1[strcspn(message1, "\n")] = '\0'; // Remove trailing newline

        // Check if the client wants to quit
        if (strcmp(message1, "quit") == 0) {
            send(SocketFD, message1, strlen(message1), 0);
            printf("You ended the chat.\n");
            break;
        }

        send(SocketFD, message1, strlen(message1), 0);
    }

    // Step 5: Cleanup
    close(SocketFD);
    printf("Disconnected from the server.\n");

    return 0;
}
