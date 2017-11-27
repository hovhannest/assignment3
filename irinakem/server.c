#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define PORT 2345

int main(int argc, char *argv[]){

struct sockaddr_in server_addr, client_addr;
int socket_desc, client_desc;

/* creates socket with socket() */
socket_desc = socket(AF_INET, SOCK_DGRAM, 0);
if(socket_desc == -1) {
	perror ("Failure to create a socket");
	exit(1);
} else {
	printf("Socket is successfully created \n");
}

server_addr.sin_family = AF_INET;
server_addr.sin_addr.s_addr = INADDR_ANY;
server_addr.sin_port = htons(PORT);

/* binds to the port with bind() */
if( bind(socket_desc, (struct sockaddr*)&server_addr, sizeof(struct sockaddr)) < 0 ) {
	perror("Bind error! \n");
	exit(1);
} else {
	printf("Successfully binded! \n");
}

/* listens with listen() for incoming connections */

listen(socket_desc, 3);
printf("Server is ready! \n");

/* when the client connects, and connection is accepted (accept() returns new socket descriptor), 
the server makes fork(), then checks if it's a parent or a child */

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

printf("Connection accepted!");

pid_t pid = fork();
if(pid == 0) {
	setuid(1000);
	printf("Client-server connection is ready! \n");
	int read_size = 0;
	char client_message[2048];
	while( (read_size = recv(client_desc, client_message, 2048, 0)) > 0 ) {
		client_message[read_size] = 0;

/* if the client sends string 'bye' then the server closes the connection and the child exits. */

		if(client_message[0] ==  'b' && 
		client_message[1] == 'y' && 
		client_message[2] == 'e' && (client_message[3] == 0 || 
		client_message[3] == '\n' ||
		client_message[3] == '\r')) {
			write(client_desc, "Server is disconnected\n", 23);
			shutdown(client_desc, SHUT_RDWR);
			close(client_desc);
			break;
			} else {
/* if it's a child, it waits for the client to write something and prints the received message to stdout */
				printf("%s\n", client_message);
				write(client_desc, client_message, read_size);
			}
		}

}

if(read_size == 0) {
	printf("Client is disconnected! \n");
} else if(read_size == -1) {
	perror("recv failure");
}}}

return 0;
}