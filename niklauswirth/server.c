#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>

#define SOCK_PATH "socket"


int main( int argc, char *argv[] )
{

    int s, s2, t, len, read_size;
    char client_message[2000];
    struct sockaddr_un local, remote;


    for(;;){


        if ((s = socket(AF_UNIX, SOCK_STREAM, 0)) == -1) {
            perror("socket");
            exit(1);
        }

        local.sun_family = AF_UNIX;
        strcpy(local.sun_path, SOCK_PATH);
        unlink(local.sun_path);
        len = strlen(local.sun_path) + sizeof(local.sun_family);
        if (bind(s, (struct sockaddr *)&local, len) == -1) {
            perror("bind");
            exit(1);
        }

        if (listen(s, 5) == -1) {
            perror("listen");
            exit(1);
        }



        printf("Waiting for connections\n");

        t = sizeof(remote);
        if ((s2 = accept(s, (struct sockaddr *)&remote, &t)) == -1) {   // Server waits for client to connect
            perror("accept");
            exit(1);
        }


        pid_t childPID;

        childPID = fork();

        if(childPID >= 0) // Fork was successful
        {
            if(childPID == 0) // Child process
            {
                printf("\I am child\n");
                
                while( (read_size = recv(s2 , client_message , 2000 , 0)) > 0 )
                {
                    //Reply to client
                   if(strcmp(client_message, "bye")==0)
                	{	write(s2, "Server is disconnected", 22);
                		break;
                	}
                    write(s2 , client_message , strlen(client_message));
            	
                }
                
                    if(read_size == 0)
                    {
                        puts("Client disconnected");
                        fflush(stdout);
                    }  
                    else if(read_size == -1)
                    {
                        perror("recv failed");
                    }

            }

        }
        else // Fork failed
        {
            printf("\nFork failed, quitting!\n");
            return 1;
        }
    }
    return 0;
}