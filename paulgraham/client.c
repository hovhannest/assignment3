#include <stdio.h>
#include <stdlib.h>
#include <netdb.h>
#include <netinet/in.h>
#include <string.h>


#define buff_size 1024
char buffer[buff_size];

int main(int argc, char *argv[]) {
    int sockfd, socket_port, n;
    struct sockaddr_in serv_addr;
    struct hostent *server;
    
    
    if (argc != 3) {
        printf("Usage: %s hostname port\n", argv[0]);
        exit(0);
    }
	
    socket_port = atoi(argv[2]);
    if(socket_port >= (1<<16) || socket_port < 1) {
        printf("wrong port\n");
        exit(0);
    }
    
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        printf("ERROR opening socket\n");
        exit(1);
    }
	
    server = gethostbyname(argv[1]);
    
    if (server == NULL) {
        printf("ERROR, no such host\n");
        exit(0);
    }
    
    bzero((char *) &serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    bcopy((char *)server->h_addr, (char *)&serv_addr.sin_addr.s_addr, server->h_length);
    serv_addr.sin_port = htons(socket_port);
    
    if (connect(sockfd, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) < 0) {
        printf("ERROR connecting\n");
        exit(1);
    }
    
    printf("Please enter the message: ");
    bzero(buffer, buff_size);
    fgets(buffer,buff_size - 1,stdin);
    
    n = write(sockfd, buffer, strlen(buffer));
    
    if (n < 0) {
        printf("ERROR writing to socket\n");
        exit(1);
    }
    
    bzero(buffer,buff_size);
    n = read(sockfd, buffer, buff_size - 1);
    
    if (n < 0) {
        printf("ERROR reading from socket\n");
        exit(1);
    }
	
    printf("%s\n",buffer);
    return 0;
}