#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <stdio.h>
#include <string.h>
 
int main(int argc, char **argv)
{
    if(argc<2)
    {
        fprintf(stderr, "no port\n");
        return 0;
    }

    int port = atoi(argv[1]);
    int listen_fd, comm_fd;
 
    struct sockaddr_in servaddr;
 
    listen_fd = socket(AF_INET, SOCK_STREAM, 0);
 
    bzero( &servaddr, sizeof(servaddr));
 
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htons(INADDR_ANY);
    servaddr.sin_port = htons(port);

    bind(listen_fd, (struct sockaddr *) &servaddr, sizeof(servaddr));

    listen(listen_fd, 10);

    int clientID = 0;

    while(1)
    {
        comm_fd = accept(listen_fd, (struct sockaddr*) NULL, NULL);
        
        pid_t pid = fork();
        if (pid == 0)
        {
            const char end[] = "Bye";
            char buf[100];
            while(1)
            {
                //bzero(buf, 100);
                
                read(comm_fd, buf, 100);
                printf("Server: Client <%d> sent: %s\n", clientID, buf);

                printf("Server: Sending to Client <%d>: %s\n", clientID, buf);
                write(comm_fd, buf, strlen(buf)+1);
                if(strcmp(buf, end) == 0)
                {
                    break;
                }
            }
            printf("Server: Closed connection to Client <%d>\n", clientID);
            close(comm_fd);
            return 0;
        }

        ++clientID;
    }
}
