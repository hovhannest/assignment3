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
    
    char str[100];
    int listen_fd, comm_fd;
 
    struct sockaddr_in servaddr;
 
    listen_fd = socket(AF_INET, SOCK_STREAM, 0);
 
    bzero( &servaddr, sizeof(servaddr));
 
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htons(INADDR_ANY);
    servaddr.sin_port = htons(port);

    bind(listen_fd, (struct sockaddr *) &servaddr, sizeof(servaddr));

    listen(listen_fd, 10);

    while(1)
    {
        comm_fd = accept(listen_fd, (struct sockaddr*) NULL, NULL);
        
        pid_t pid = fork();
        if (pid == 0)
        {
	    const char end[] = "bye";
            while(1)
            {
                bzero(str, 100);
                read(comm_fd,str,100);
                printf("Echoing back - %s \n", str);
                write(comm_fd, str, strlen(str)+1);
		if(strcmp(str, end) == 0)
		{
			return 0;
 	        }
	    }
        }
    }
}
