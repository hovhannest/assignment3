#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>

#define VERBOS

int main(int argc, char *argv[])
{
	int port = 0;
	struct sockaddr_in server, client;
	int socket_desc, client_desc;

	if (argc > 1)
	{
		char *port_str = argv[1];
		port = atoi(port_str);
	}
	else
	{
		char tstr[255];
		fgets(tstr, 255, stdin);
		char *port_str = strtok(tstr, " ");
		port = atoi(port_str);
	}

	if(port == 0)
	{
#ifdef VERBOS
		printf("Port is not specified. Using port 8888\n");
#endif
		port = 8888;
	}
#ifdef VERBOS
	printf("Server is starting on port %d \n", port);
#endif

	socket_desc = socket(AF_INET, SOCK_STREAM, 0);
	if(socket_desc == -1)
	{
		perror ("Failed to create a socket");
		exit(1);
	}
#ifdef VERBOS
	printf("Socket created \n");
#endif

	server.sin_family = AF_INET;
	server.sin_addr.s_addr = INADDR_ANY;
	server.sin_port = htons(port);

	if( bind(socket_desc, (struct sockaddr*)&server, sizeof(server)) < 0 )
	{
		perror("ERROR: bind failed! \n");
		exit(1);
	}
#ifdef VERBOS
	printf("binded! \n");
#endif

	listen(socket_desc, 3);

	printf("Server is ready...\n");

while(1)
{
	int c = sizeof(struct sockaddr_in);
	client_desc = accept(socket_desc, (struct sockaddr*)&client, (socklen_t *)&c);
	if(client_desc < 0)
	{
		perror("cannot accept connection");
		// TODO: should I quit the server? Or try to recreate smt...
		exit(1);
	}
#ifdef VERBOS
	printf("Connection accepted!");
#endif

	pid_t pid = fork();
	if(pid == 0) // child process
	{
		// assume we have a user with is 3322 with access to the network
		setuid(3322);
#ifdef VERBOS
		printf("Client connected\n");
#endif

		//Get a message from client
		int read_size = 0;
		char client_message[2048];
		while( (read_size = recv(client_desc, client_message, 2048, 0)) > 0 )
		{
			client_message[read_size] = 0;
			if(client_message[0] ==  'b' && 
				client_message[1] == 'y' && 
				client_message[2] == 'e' && (client_message[3] == 0 || 
					client_message[3] == '\n' ||
					client_message[3] == '\r'))
			{
				write(client_desc, "Server is disconnected\n", 23);
				shutdown(client_desc, SHUT_RDWR);
				close(client_desc);
				break;
			}
			else
			{
				printf("%s\n", client_message);
				write(client_desc, client_message, read_size);
			}
		}

#ifdef VERBOS
		if(read_size == 0)
		{
			printf("Client disconnected");
		}
		else
#endif
			if(read_size == -1)
		{
			perror("recv failed");
		}
	}
}

	return 0;
}
