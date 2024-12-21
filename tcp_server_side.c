#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <unistd.h>

int main(){

    int ServerSocketFD = socket(AF_INET, SOCK_STREAM, 0);
    
    char* ip = "127.0.0.1";
    struct sockaddr_in ServerAddress;
    ServerAddress.sin_family = AF_INET;
    ServerAddress.sin_port = htons(2004);
    inet_pton(AF_INET, ip, &ServerAddress.sin_addr.s_addr);
    //to bind the incoming connections with the upcomming connections
    int result = bind(ServerSocketFD, (struct sockaddr *)&ServerAddress, sizeof(ServerAddress));
    if(result == 0){
        printf("Server socket was found successfully");
    }
    else{
        printf("Failed to bind the socket");
        return 1;
    }
    //specifying the socket and max number of connections
    int listenResult = listen(ServerSocketFD, 10);

    struct sockaddr_in ClientAddress;
    int ClientAddressSize = sizeof(ClientAddress);
    int ClientSocketFD = accept(ServerSocketFD, (struct sockaddr *)&ClientAddress, &ClientAddressSize);
    
    char buffer[1024];
    recv(ClientSocketFD,buffer, 1024,0);
    printf("response was %s\n", buffer);
    
    char* message;
    message = "heyyyy, I Love You too!";
    send(ClientSocketFD, message, strlen(message), 0);

    close(ClientSocketFD);
    close(ServerSocketFD);
    return 0;
}