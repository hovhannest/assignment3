#include <stdio.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <string.h>

int main(int argc, char* argv[])
{
	struct sockaddr_in address, client;
        struct hostent *he;
	int addrsize = sizeof(address);
        int sockd = 0;
	int msgsize = 0;

	char message[100];
	int newsock = 0;	
	int port = 0;	

	if(argc < 2)
	{
		printf("The port number is not provided.");
		return -1;
	}
	else
	{
		char* portstr = argv[1];
		port = atoi(portstr);
	}	
	
	if(port == 0)
	{
		printf("The provided string is not an integer.");
		return -1;
	}
	
	//Creating a socket
	sockd = socket(AF_INET, SOCK_STREAM, 0);
        if(sockd < 0)
        {
                printf("Couldn't create a socket.");
                return -1;
        }
        
	address.sin_family = AF_INET;
    	address.sin_addr.s_addr = INADDR_ANY;
    	address.sin_port = htons(port);
	
	//Binding
	if(bind(sockd,(struct sockaddr *)&address, addrsize) < 0)
	{
		close(sockd);
		printf("\nThe binding failed.");
		return -1;
	}

	//Listening for incoming connections
	if(listen(sockd, 2) < 0)
	{
		close(sockd);
		printf("\nServer isn't listening.");
		return -1;
	}
	int size = sizeof(client);

	while(1)
	{
		newsock = accept(sockd, (struct sockaddr *)&client, (socklen_t*)&size);
    		if(newsock < 0)
		{	
			printf("\nServer accept error.");
			return -1;
		}

		printf("\nServer: Got connection from %s\n", inet_ntoa(address.sin_addr));

		if(!fork())
		{	
			printf("\nWe are in the child process.");
			while(1)
			{
				msgsize = read(newsock, message, sizeof(message));
				if(msgsize < 0)
				{
					printf("\nError while reading the message.");
					close(newsock);
					return -1;	
				}
				if(msgsize > 0)
				{
					const char* mssg = message;
					if(strncmp(mssg, "bye", 3) == 0)
					{
						close(newsock);
						printf("\nClient has disconnected. Bye.");			       					     return 0;
					}
					else
					{	
						write(newsock, "Got it", sizeof("Got it"));
						printf("\nClient says: %s", message);
					}
				}	
				else {
					close(newsock);
					printf("\nDisconnected.");
					return 0;			
				}
			}
		}
	}
	return 0;
}

