#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <netdb.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>

#define PORT 2345

int main(int argc, char *argv[]) {

	char inputStr[255];
	char *dom;
	int socket_desc;
	struct sockaddr_in client_addr;
	char message[2048];

/* creates socket with socket() */

	socket_desc = socket(AF_INET, SOCK_STREAM, 0);
	if(socket_desc == -1)
	{
		perror("Failure to create socket \n");
		free(dom);
		exit(1);
	}

	printf("Socket is successfully created\n");

	client_addr.sin_addr.s_addr = inet_addr(dom);
	client_addr.sin_family = AF_INET;
	client_addr.sin_port = htons(PORT);

/* makes a connection with connect() */

	if(connect(socket_desc, (struct sockaddr*)&client, sizeof(client)) < 0)
	{
		perror("Connection failure! \n");
		free(dom);
		exit(1);
	}

	printf("Connected\n");

	free(dom);

/* prompts user to input a string to send to the server. 
reads the string, sends to the server with write() */
	
	while(1)
	{
		printf("Enter your message or 'bye' to quite\n");

		fgets(message, 2048, stdin);

		if( send(socket_desc, message, strlen(message), 0) < 0)
		{
			perror("Cannot send the input to the server");
			exit(1);
		}

		memset(message, 0, 2048);
		int recv_retval =  recv(socket_desc, message, 2048, 0 );
		if( recv_retval < 0)
		{
			printf("recv failed\n");
			break;
		}
		else if (recv_retval == 0)
		{
			printf("connection closed\n");
			break;
		}

/* when receives an answer from the server, prints it to standard output */

		printf("%s", message);

/* stays connected as long as server does not close the connection */


		recv_retval = recv(socket_desc, message, 1, 0);
		
		if( recv_retval < 0)
		{

			printf("recv failed\n");

			break;
		}
		else if (recv_retval == 0)
		{

			printf("connection closed\n");

			break;
		}
	}

	return 0;

}