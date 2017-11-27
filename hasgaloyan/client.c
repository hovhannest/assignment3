#include <stdio.h>
#include <sys/socket.h>
#include <string.h>
#include <sys/types.h>
#include <stdlib.h>
#include <unistd.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>

int main(int argc, char *argv[])
{
        if(argc < 2)
        {
			printf("No arguments passed. Please provide IP and port number for the connection.");
			return -1;
        }
		if(argc == 2)
		{
			printf("The port is not passed as an argument");
			return -1;
        }
		if(argc > 3)
        {
			printf("%s", "\nMore than two arguments are passed. Please insert the IP and port values only.");
			return -1;
        }
        
        struct sockaddr_in server;
	struct hostent *he;
        int sockd = 0;
        char* ip = argv[1];;
        char* port = argv[2];
	char message[100];

        int p = atoi(port);
        if(p == 0)
        {
                printf("\nPort is not valid.");
                return -1;
        }

        printf("\nReceived data: IP: %s  PORT: %d", ip, p);

        sockd = socket(AF_INET, SOCK_STREAM, 0);
        if(sockd < 0)
        {
                printf("Couldn't create a socket.");
                return -1;
        }
        printf("\nA socket was created.");

	if((he = gethostbyname(ip)) == NULL)
	{
    		printf("Couldnt resolve host.");
    		return -1;
	}
	printf("The IP Address is: %s\n", inet_ntoa(*((struct in_addr *)he->h_addr)));
	
      	server.sin_family = AF_INET;
      	server.sin_port = htons(p);
	server.sin_addr = *((struct in_addr *)he->h_addr);        

	if (connect(sockd, (struct sockaddr *)&server, sizeof(server)) < 0)
	{
		printf("\n\nCould not establish a connection.\nExiting...");
		return -1;
	}

	printf("\nConnected to server");
	
	char msgfromserver[100];
	while(1)
	{
		printf("\n\nYour message: ");
		fgets(message, 100, stdin);
		send(sockd, message, sizeof(message), 0);
		printf("Sent.");
		int readval = read(sockd, msgfromserver, sizeof(msgfromserver));
		if(readval > 0)
		{
			printf("\nServer says: %s", msgfromserver);
		}		
	}

        return 0;
}

