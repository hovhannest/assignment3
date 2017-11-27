#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>

#define MAX_LINE_LENGTH 4096
#define SERVER_DEFAULT_PORT 3000

int main(int argc, char** argv)
{
	int socketFD;
	struct sockaddr_in serverAddress;
	char sendingLine[MAX_LINE_LENGTH], receivingLine[MAX_LINE_LENGTH];

	if (argc < 2) {
		perror("No IP or hostname found");
		exit(1);
	}
	
	socketFD = socket(AF_INET, SOCK_STREAM, 0);
	if (socketFD < 0) {
		perror("Problem in socket creation");
		exit(1);
	}

	memset(&serverAddress, 0, sizeof(serverAddress));
	serverAddress.sin_family = AF_INET;
	serverAddress.sin_addr.s_addr = inet_addr(argv[1]);
	int port = SERVER_DEFAULT_PORT;
	if (argc > 2) {
		char* portNumber = argv[2];
		port = atoi(portNumber);
	}
	serverAddress.sin_port = htons(port);

	if (connect(socketFD,
	(struct sockaddr *) &serverAddress, 
	sizeof(serverAddress)) < 0) {
		perror("Problem in connecting to the server");
		exit(1);
	}
	
	while (1) {
		write(1, "Your message : ", 15);
		if (read(0, sendingLine, MAX_LINE_LENGTH) < 0) {
			perror("Problem while reading from standard input");
			exit(1);
		}

		if (send(socketFD, sendingLine, strlen(sendingLine), 0) < 0) {
			perror("Problem while sending message.");
			exit(1);
		}

		if (recv(socketFD, receivingLine, MAX_LINE_LENGTH, 0) < 0) {
			perror("Problem while receiving message.");
			exit(1);
		}

		if (strncmp(receivingLine, "Server is disconnected", 22) == 0) {
			break;
		}

		printf("\nServer reply %s", receivingLine);
		memset(sendingLine, 0, MAX_LINE_LENGTH);
		memset(receivingLine, 0, MAX_LINE_LENGTH);
	}
	close(socketFD);
	return 0;
}
