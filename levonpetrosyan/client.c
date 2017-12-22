#include <arpa/inet.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/socket.h>
#include <netdb.h>
#include <stdlib.h>
#include <netinet/in.h>

int main(int argc, char **argv)
{
    struct sockaddr_in address;
    struct addrinfo hints, *res;
    int sock = 0, valread;
    int addr;
    struct sockaddr_in serv_addr;
    char *hello = "Hello from client";
    char buffer[1024] = {0};
    memset(&hints, 0 , sizeof hints);
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;

    addr = getaddrinfo(argv[1], argv[2], &hints, &res);
    if(addr != 0){
        printf("connection failed");
        exit(1);
    }

    sock = socket(res->ai_family,res->ai_socktype,res->ai_protocol);
    if (sock < 0)
    {
        printf("\n Socket creation error \n");
        return -1;
    }

    memset(&serv_addr, '0', sizeof(serv_addr));

    serv_addr.sin_family = AF_INET;
    int clientport = atoi(argv[2]);
    serv_addr.sin_port = htons(clientport);

    if(inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr)<=0)
    {
        printf("\nInvalid address/ Address not supported \n");
        return -1;
    }


    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
    {
        printf("\nConnection Failed \n");
        return -1;
    }


    while(1){

        char name[10];
        printf("type your message? ");
        fgets(name, 20, stdin);

        send(sock , name , strlen(name) , 0 );
        printf("message sent\n");
        valread = read( sock , buffer, 1024);
        printf("server says: %s\n",buffer );
    }
    return 0;
}