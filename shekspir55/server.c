#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <pthread.h> 

int main(int argc , char *argv[]) {

	char str[80];
    int read_size;    
    int socket_desc , client_sock , c, port = 0;
    struct sockaddr_in server , client;
    //Create socket
	socket_desc = socket(AF_INET , SOCK_STREAM , 0);
    
    if(argv[1]) {
        port = atoi(argv[1]);
    }

    if(!port) {
        //default port
        port = 6544;
    }

    if(port<1 || port>65535) {
        printf("%s %d %s\n", "Sorry port", port, "is out of range");
        return 1;
    }

    if (socket_desc == -1) {
        printf("Could not create socket");
        return 0;
	} 

    printf("socket created\n");

    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons( port );

    if( bind(socket_desc,(struct sockaddr *)&server , sizeof(server)) < 0) {
        perror("bind failed. Error");
        return 1;
    }
	printf("server listening on port %d\n", port);
  
    listen(socket_desc , 3);
     
    puts("Waiting for sockets");
    c = sizeof(struct sockaddr_in);

    while( (client_sock = accept(socket_desc, (struct sockaddr *)&client, (socklen_t*)&c)) ){
        if (client_sock < 0)
        {
            printf("%s\n", "accept failed");
            return 1;
        }

        printf("Connection accepted \n");
        
        pid_t pid = fork();
        
        if (pid == 0) {
            while(1) {
                char client_message[10000];
                read_size = recv(client_sock , client_message, 2000 , 0);
                
                if(!(read_size > 0)) {
                    break;
                }
                
                client_message[read_size] = '\0';

                if(strcmp(client_message, "bye") == 0) {
                    write(client_sock, "Server is disconnected", 22);
                    break;
                }
                
                printf("%s\n", client_message);
                write(client_sock , client_message , strlen(client_message));
            }

            if(read_size == 0) {
                printf("Client disconnected \n");
            } else if(read_size == -1) {
                printf("Some error accured\n");
                return(1);
            }
   
		}
	}
     
    return 0;
}
