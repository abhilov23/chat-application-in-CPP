#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>

int main(){
    
    //the socket version takes 3 parameters: IP-version, Protocol, TCP/IP layer 
    //after creating the socket, it returns a number, if it is negative number i.e. -1 then it is an error
    //otherwise if it returns positive number means it is successfully created
    int SocketFD = socket(AF_INET, SOCK_STREAM, 0);
    
    char* ip = "172.253.63.100";
    struct sockaddr_in address;
    address.sin_family = AF_INET;
    address.sin_port = htons(80);
    inet_pton(AF_INET, ip, &address.sin_addr.s_addr);
    
    


    //the connect method takes 3 parameters: 
    int result = connect(SocketFD,  (struct sockaddr*)&address, sizeof address);
    if(result == 0){
        printf("Connect to server");
    }
    char* message;
    message = "GET \\ HTTP/1.1\r\nHost: google.com\r\n\r\n";
    send(SocketFD, message, strlen(message), 0);

    char buffer[1024];
    recv(SocketFD,buffer, 1024,0);
    
    printf("response was %s\n", buffer);
    return 0;
}