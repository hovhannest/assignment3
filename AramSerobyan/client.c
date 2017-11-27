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

if( argc<3)
{
	printf("no port provided");
	return 0;
}
else if(argc<2)
{
	printf("no ip provided");
	return 0;
}

int sockfd, sr;
struct addrinfo hints, *res ; 
int addr,intRecv;
char our_message[777];
char server_message[777];
memset(&hints, 0 , sizeof hints);
hints.ai_family = AF_UNSPEC; 
hints.ai_socktype = SOCK_STREAM; 

if(( addr = getaddrinfo( argv[1],argv[2],&hints, &res)) != 0)
{
	fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(addr));
	exit(1);
}
sockfd=socket(res->ai_family,res->ai_socktype,res->ai_protocol);
if(sockfd== -1) 
	{ printf(" failed to create socket"); 
		freeaddrinfo(res); return 0;}
if(connect(sockfd, res->ai_addr, res->ai_addrlen)==-1)
	{ printf(" failed to connect"); 
		freeaddrinfo(res); return 0;}

	printf("Messages would be transmited to the server type bye to finish the session");


while(1)
{
	//server_message[0] = '\0';
	memset(&server_message,0, sizeof(server_message));
	scanf("%s", our_message);
//	printf("%s", our_message);
	//write(sockfd,our_message,strlen(our_message));
	sr = send(sockfd, our_message, strlen(our_message),0);
	if(sr == -1)
	{
		printf(" You miserably failed");
		freeaddrinfo(res); return 0;
	}
	//printf("%d",send);
	
	intRecv =recv(sockfd, server_message,777,0);
	printf("\n %d",intRecv);	
	if( intRecv==-1)
	{
		printf(" Server miserably failed");
		freeaddrinfo(res); return 0;
	}
	//else if(intRecv == 0)
	else if(compare_strings(server_message,"The End",7)==0)
	{
		printf(" Connection was terminated \n");
		freeaddrinfo(res); return 0;
	}
	else
	{
		write(1,server_message,strlen(server_message));
		printf("\n");
	}
}


freeaddrinfo(res);

return 0;
}


