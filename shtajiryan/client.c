#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <stdio.h>
#include <string.h>

int main(int argc, char **argv)
{
    if(argc<3)
    {
        fprintf(stderr, "no port and/or ip\n");
        return 0;
    }

    int port = atoi(argv[2]);
    
    int sockfd,n;
    struct sockaddr_in servaddr;
 
    sockfd=socket(AF_INET,SOCK_STREAM,0);
    bzero(&servaddr,sizeof servaddr);
 
    servaddr.sin_family=AF_INET;
    servaddr.sin_port=htons(port);
 
    inet_pton(AF_INET,argv[1],&(servaddr.sin_addr));
 
    connect(sockfd,(struct sockaddr *)&servaddr,sizeof(servaddr));
    
    const char end[] = "Bye";
    char sendline[100];
    char recvline[100];
    while(1)
    {
        //bzero( sendline, 100);
        //bzero( recvline, 100);

        fgets( sendline, 100, stdin); /*stdin = 0 , for standard input */
	    sendline[strlen(sendline) - 1] = '\0';
 	    
        printf("Client: Seding to Server: %s\n", sendline);
        write(sockfd, sendline, strlen(sendline) + 1);
        read(sockfd, recvline, 100);

        printf("Client: Server sent: %s\n", recvline);
        if(strcmp(recvline, end) == 0)
        {
            break;
        }
    }

    printf("Client: Server closed connection, exiting\n");
    close(sockfd);
}