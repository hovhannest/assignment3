#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>


int main(int argc , char *argv[]) {
    char host[100];
    char ip[1000];
    int port = 0;
    int sock;
    struct sockaddr_in server;    
    
    port = 6666;
    strncpy(host, "127.0.0.1", 9);

    if(argc==2) {
    	port = atoi(argv[1]);
    } else if(argc==3) {
    	strncpy(host, argv[1], 100);
    	port = atoi(argv[2]);
    }
 
    sock = socket(AF_INET , SOCK_STREAM , 0);
    if (sock == -1) {
        printf("No socket created");
        return 1;
    }
    puts("Socket created");
     
    server.sin_addr.s_addr = inet_addr(host);
    server.sin_family = AF_INET;
    server.sin_port = htons(port);
 
    if (connect(sock , (struct sockaddr *)&server , sizeof(server)) < 0) {
        perror("connect failed. Error");
        return 1;
    }
     
    printf("Connection established\n");
     
    while(1) {
        char message[1000] , server_reply[2000];
        scanf("%s" , message);
        if( send(sock , message , strlen(message) , 0) < 0) {
            printf("Send failed");
            return 1;
        }
        
        if( recv(sock , server_reply , 2000 , 0) < 0) {
            printf("Server Error: ");
            return 1;
        }

        if(strcmp(server_reply, "Server is disconnected")==0) {
            printf("Server is disconnected");
			return 1;
		}
        
        printf("Reply :");
        printf("%s\n", server_reply);
    }
        
    return 0;
}