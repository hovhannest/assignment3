#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>

char* concat(const char *s1, const char *s2)
{
	char *result =  malloc(strlen(s1) + strlen(s2) + 1);
	strcpy(result, s1);
	strcat(result, s2);
	return result;
}


char *fgetstr(char *string, int n, FILE *stream)
{
	char *result;
	result = fgets(string, n, stream);
	if(!result)
		return result;
	if(string[strlen(string) - 1] == '\n')
		string[strlen(string) - 1] = 0;
	
	return(string);
}

int main(int argc, char *argv[])
{
	char message[1000];
//	char bye[] = "bye";
	//char s_message[] = "Affirmative, Dave.";
	
	int port = atoi(argv[1]);
	int sockette = socket(AF_INET, SOCK_STREAM, 0);

	if(sockette == -1)
	{
		printf("Could not create socket\n");
	}
	

	struct sockaddr_in s_addr, c_addr;
	s_addr.sin_family = AF_INET;
	s_addr.sin_port = htons(port);
	s_addr.sin_addr.s_addr = INADDR_ANY;

	int bound = bind(sockette, (struct sockaddr*) &s_addr, sizeof(s_addr));
	
	if(bound == -1)
	{
		printf("Could not bind :(((((\n");
	}

	listen(sockette, 5);
	while(1)
	{
	int clilen = sizeof(c_addr);
	int c_sockette = accept(sockette, (struct sockaddr *) &c_addr, &clilen);
	if(c_sockette == -1)
	{
		printf("Some things just cannot be accepted\n");
	}

	pid_t pid;
	pid = fork();

	if(pid < 0)
		printf("Forking process failed");

	if(pid == 0)
	{
		while(1)
		{
			memset(message, 0, sizeof(message));
			int readd = read(c_sockette, message, sizeof(message));
			if(readd == -1)
				printf("could not receive message");
			else
				printf("%s\n", message);

			int l = strlen(message);
			message[l] = '\0';
                        int k;
                        for (k = 0; k < l ; k++)
			{
			  printf("Message[%d] = %c\n", k, message[k]);
			}
			int y;
			y = strcmp(message, "bye");
			printf("y = %d\n", y);
				if(strcmp(message, "bye") == 0)
			{
				printf("Disconnecting\n");
				write(c_sockette, "Disconnecting", 13);
				close(c_sockette);
				break;
			}
		
			///fgetstr(s_message, 1000, stdin);
		        char s_message[] = "Affirmative, Dave. you have said: ";

			//s_message= "Affirmative, Dave. you have said: ";
			//char * new_string;
			strcat(s_message, message);
			printf("s_message is %s\n", s_message);
			int sent = write(c_sockette, s_message, strlen(s_message));
		
			if(sent == -1)
				printf("Could not send\n");
			


		}
	printf("Child is exiting\n");
	return 0;

	}

	}
	return 0;

}
