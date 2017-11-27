#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>

char *fgetstr(char *string, int n, FILE *stream)
{
	char *result;
	result = fgets(string, n, stream);
	if(!result)
		return result;
	if(string[strlen(string) -1] == '\n')
		string[strlen(string) - 1] = 0;
	
	return(string);

}

int main(int argc, char *argv[])
{
	char* ip;
	int port;
	int sockette;
	char msg[1000];
	char s_msg[1000];

	port = atoi(argv[2]);
	ip = argv[1];
	sockette = socket(AF_INET, SOCK_STREAM, 0);

	if(sockette == -1)
	{
		printf("Could not create socket :(\n)");
	}

	struct sockaddr_in addr;
	addr.sin_family = AF_INET;
	addr.sin_port = htons(port);
	addr.sin_addr.s_addr = inet_addr(ip);
	
	int connection = connect(sockette, (struct sockaddr *) &addr, sizeof(addr));

	if(connection == -1)
	{
		printf("Could not connect\n");
	}

	

	while(1)
	{
		strcpy(msg, "");
		fgetstr(msg, 1000, stdin);
		//char msg1[1000];
		//msg1 = msg;
		int l = strlen(msg);
		msg[l] = '\0';
		int k;
                for (k = 0; k < l ; k++)
                {
                  printf("Message[%d] = %c\n", k, msg[k]);
                }

		int b = strcmp(msg, "bye");
		printf("strcmp is %d\n", b);
		
                if(b == 0)
		{
			close(sockette);
			return 0;
		}

		int sent = write(sockette, msg, sizeof(msg));

		if(sent == -1)
		{
			printf("Could not send\n");
		}
                
		//strncpy (s_msg, "", 1);
		bzero(s_msg, sizeof(s_msg));
		int received = read(sockette, s_msg, sizeof(s_msg));

		if(received == -1)
		{
			printf("Could not receive \n");
			return 0;
		}
		else
		{
			printf("%s\n", s_msg);
		}

		
	}
	return 0;

}

