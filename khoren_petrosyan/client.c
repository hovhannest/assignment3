#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h> 
#include <arpa/inet.h>

int main(int argc, char *argv[]){

    struct sockaddr_in server_adder;
    struct hostent *server;
    
    if(argc != 3){ 
        printf("Error: Invalid arguments passed!!! \n\n Please provide valid IP and port number!!!");
        return -1;
    }

    int num_port = atoi(argv[2]);
    int socket_fd = socket(AF_INET, SOCK_STREAM, 0);

    if (socket_fd < 0) {
            printf("Error: Invalid socket < 0");
            return -1;
        }
    
    if((server = gethostbyname(argv[1])) == NULL){
            printf("Error: Invalid host");
            return -1;
    }

    bzero((char *) &server_adder, sizeof(server_adder));
    server_adder.sin_family = AF_INET;
    bcopy((char *)server->h_addr, 
         (char *)&server_adder.sin_addr.s_addr,
         server->h_length);
    server_adder.sin_port = htons(num_port);
    
    if (connect(socket_fd,(struct sockaddr *) &server_adder, sizeof(server_adder)) < 0) {
        printf("Error: can't connect to server");
        return -1;
    }
    else{
        printf("\nConnection with server established!!!");
    }

    char message_to_server[500];
    char message_from_server[500];

    while(1)
    {
        printf("\n\nEnter message: (max 500 chars)");
        fgets(message_to_server, 500, stdin);
        send(socket_fd, message_to_server, sizeof(message_to_server), 0);
        printf("Sent.");
        int x = read(socket_fd, message_from_server, sizeof(message_from_server));
        if(x > 0)
            printf("\nServer response: %s", message_from_server);
    }
        return 0;
}

