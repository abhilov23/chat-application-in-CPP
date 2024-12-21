#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <unistd.h>

int main(){
    
    //the socket version takes 3 parameters: IP-version, Protocol, TCP/IP layer 
    //after creating the socket, it returns a number, if it is negative number i.e. -1 then it is an error
    //otherwise if it returns positive number means it is successfully created
    int SocketFD = socket(AF_INET, SOCK_STREAM, 0);
    
    char* ip = "127.0.0.1";
    struct sockaddr_in address;
    address.sin_family = AF_INET;
    address.sin_port = htons(2004);
    inet_pton(AF_INET, ip, &address.sin_addr.s_addr);
    
    


    //the connect method takes 3 parameters: 
    int result = connect(SocketFD,  (struct sockaddr*)&address, sizeof address);
    if(result == 0){
        printf("Connect to server");
    }
    char* message;
    message = "heyyyy, I Love You!";
    send(SocketFD, message, strlen(message), 0);

    char buffer[1024];
    recv(SocketFD,buffer, 1024,0);
    
    printf("response was %s\n", buffer);
    
    close(SocketFD);
    return 0;
}