#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include <netdb.h>
#include <sys/socket.h>
#include <netinet/in.h>


#define buff_size 1024
char buffer[buff_size];
char bye[5];

int main(int argc, char *argv[])
{
    int welcomeSocket, newSocket;
    struct sockaddr_in serverAddr;
    struct sockaddr_storage serverStorage;
    socklen_t addr_size;
    
    if(argc != 2) {
        printf("wrong number of arguments\n");
        exit(0);
    }
    
    welcomeSocket = socket(AF_INET, SOCK_STREAM, 0);
    
    int socket_port = atoi(argv[1]);
    if(socket_port >= (1<<16) || socket_port < 1) {
        printf("wrong port\n");
        exit(0);
    }
    
    printf("port = %d\n", socket_port);
    
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(socket_port);
    serverAddr.sin_addr.s_addr = INADDR_ANY;
    memset(serverAddr.sin_zero, '\0', sizeof serverAddr.sin_zero);  

    if(bind(welcomeSocket, (struct sockaddr *) &serverAddr, sizeof(serverAddr)) < 0) {
        printf("error in binding\n");
        exit(1);
    }
    
    listen(welcomeSocket, 10);
    addr_size = sizeof serverStorage;
    
    while(1){
        newSocket = accept(welcomeSocket, (struct sockaddr *) &serverStorage, &addr_size);
        if(newSocket == -1)    {
            printf("Error in accepting");
            exit(1);
        }
        
        int len;
        pid_t cpid;
        switch (cpid = fork()) {
            case 0: len = read(newSocket, buffer, buff_size - 1);
                if (len < 0) {
                    printf("socket reading error");
                    exit(1);
                }
                printf("server received: %s\n", buffer);
                
                len = write(newSocket, "message received", 16);
                if (len < 0) {
                    printf("ERROR writing to socket\n");
                    exit(1);
                }
                
                strncpy(bye, buffer, 3);
                bye[4] = '\0';
                
                if(strcmp(bye, "bye") == 0){ //if the first 3 letters = bye; sometimes w/ \n
                    printf("bye received, child exiting\n");
                    close(newSocket);//if fails exits anyway
                    exit(0);
                }
                break;
            case -1: printf("fork error\n");
                break;
            default:  printf("this is parent\n");
                break;
        }
    }
    
    return 0;
}