#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <string.h>
#include <netinet/in.h>
#include <unistd.h>

int compare_strings(char a[], char b[], int bytesRecv)
{
   int c = 0,count = 0;
 
   while (a[c] == b[c] && ++count< bytesRecv) {
      if (a[c] == '\0' || b[c] == '\0')
         break;
      c++;
   }
   if(count == bytesRecv)
	return 0;
   else if (a[c] == '\0' && b[c] == '\0')
      return 0;
   else
      return -1;
}


int main(int argc, char** argv)
{
int sockfd, new_fd;
struct sockaddr_storage their_addr;
socklen_t addr_size;
struct addrinfo hints, *res;
int addr;
int bytesRecv;
char receivedMessage[777];

if(argc<2)
	{
	printf("No port provided \n");
	return 0;
	}

memset(&hints,0,sizeof hints);
hints.ai_family = AF_UNSPEC; // both IPv4 and IPv6
hints.ai_socktype = SOCK_STREAM;
hints.ai_flags = AI_PASSIVE;   // current IP will be used


if((addr = getaddrinfo(NULL, argv[1], &hints, &res)) != 0)
{
	fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(addr));
	exit(1);
}	// something went wrong and we had faced an error


// Creation of a socket. Binding and listening to it.
sockfd=socket(res->ai_family,res->ai_socktype,res->ai_protocol);
if(sockfd== -1) 
	{ printf(" failed to create socket"); 
		freeaddrinfo(res); return 0;}

if(bind(sockfd, res->ai_addr, res->ai_addrlen)==-1)
	{close(sockfd);printf(" failed to bind"); 
		 freeaddrinfo(res); return 0;}

if(listen(sockfd, 5) == -1)
	{printf(" failed to listen"); close(sockfd);
		freeaddrinfo(res);  return 0;}
addr_size = sizeof their_addr;
new_fd = accept(sockfd, (struct sockaddr *)&their_addr, &addr_size);
if(new_fd == -1)
	{printf("accept failure call 911");close(sockfd); 
	freeaddrinfo(res);	return 0;}
// closing the port. optional. Remove if you accept more than one client
close(sockfd);


pid_t pid = fork();
if(pid ==0)
{
while(1)

 { 
	bytesRecv = recv(new_fd, receivedMessage,777,0);
	if(bytesRecv == -1)
	{		
		printf("Failure. Badumtsss");close(new_fd); 
		freeaddrinfo(res);      return 0;
	}
	else if(bytesRecv== 0)
	{
	 	printf("I can't believe client closed the connection");
		close(new_fd); 
		freeaddrinfo(res);
		return 0;
	}
	else
	{
		
		if(compare_strings(receivedMessage,"bye",bytesRecv)==0)
		{
		write(new_fd, "The End", sizeof("The End"));
		write(1, "The End", sizeof("The End"));
		printf("\n So long and thanks for all the fish");		
		 fflush(stdout);
		 close(new_fd); 
		 freeaddrinfo(res);
		 return 0;
		}
		else
		{
		write(new_fd, receivedMessage,strlen(receivedMessage));
		write(1,receivedMessage,bytesRecv);
		printf("\n");	
		}
	}
 }
}
else
{
}
//printf(" bytes %d\n",bytesRecv);

freeaddrinfo(res); // we're done


return 0;
}
