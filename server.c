#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>

#define MAX_LINE_LENGTH 4096
#define DEFAULT_SERVER_PORT 3000

int main(int argc, char** argv) 
{
	int listeningSocketFD, connSocketFD, n;
	socklen_t clientLength;
	char readingBuffer[MAX_LINE_LENGTH];
	struct sockaddr_in clientAddress, serverAddress;

	listeningSocketFD = socket(AF_INET, SOCK_STREAM, 0);

	serverAddress.sin_family = AF_INET;
	serverAddress.sin_addr.s_addr = htonl(INADDR_ANY);
	int port = DEFAULT_SERVER_PORT;
	if (argc == 2) {
		char* portNumber = argv[1];
		port = atoi(portNumber);
	}
	serverAddress.sin_port = htons(port);

	bind(listeningSocketFD, 
	(struct sockaddr *) &serverAddress, 
	sizeof(serverAddress));

	listen(listeningSocketFD, 1);

	printf("%s\n","Server is running. Waiting for connection...");
	while(1) {
		clientLength = sizeof(clientAddress);
		connSocketFD = accept(listeningSocketFD,
			(struct sockaddr *) &clientAddress,
			&clientLength);
		
		if (connSocketFD < 0) {
			perror("Receiving failed");
			exit(1);
		}

		pid_t child_pid = fork();
		if (child_pid == 0) {
			while (1){
				n = recv(connSocketFD, 
					readingBuffer, 
					MAX_LINE_LENGTH, 0);
				if (n < 0) {
					perror("Read error");
					exit(1);
				}

				if (strcmp(readingBuffer, "bye")==0){ 
					write(connSocketFD, 
						"Server is disconnected", 
						22);
					break;
				}
				printf("%s\n", "Message received from buffer.");
				printf("%s\n", readingBuffer);
				send(connSocketFD, readingBuffer, n, 0);
				memset(readingBuffer, 0, MAX_LINE_LENGTH);
			}

			close(connSocketFD);
			memset(readingBuffer, 0, MAX_LINE_LENGTH);
		}

		close(listeningSocketFD);
	}
	return 0;
}
