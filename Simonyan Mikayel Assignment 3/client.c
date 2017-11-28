#include<stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <string.h>
 
  
 int main(int argc , char *argv[])
 {
     char input[100];
     char * input_tokens, ip;
     int port = atoi(argv[2]), sock;
     struct sockaddr_in server;
     char msg[1000] , s_reply[2000];
     
     fgets(input, 100, stdin);
     input_tokens = strtok(input," ");
     
     int i = 0;
     while (input_tokens != null)
     {
 	if(i == 0)
 	{
 		ip = input_tokens;
 	}

 	input_tokens = strtok (null, " ,.-");
 	i++;
     }
  
     sock = socket(AF_INET , SOCK_STREAM , 0);
     if (sock == -1)
     {
         printf("Socket creation failed");
     }
     printf("Socket created");
      
     server.sin_addr.s_addr = inet_addr(ip);
     server.sin_family = AF_INET;
     server.sin_port = htons(port);
  
     if (connect(sock , (struct sockaddr *)&server , sizeof(server)) < 0)
     {
         fprintf(stderr,"connect failed. Error");
         return 1;
     }
      
     printf("Connection established");
      
     while(1)
     {
         printf("Enter msg : ");
         scanf("%s" , msg);
          
         if( send(sock , msg , strlen(msg) , 0) < 0)
         {
             printf("Send failed");
             return 1;
         }
          
         if( recv(sock , s_reply , 2000 , 0) < 0)
         {
             printf("recv failed");
             break;
         }
         if(strcmp(s_reply,"Server is disconnected")==0)
 	{
 		break;
 	} 
         printf("Server reply :");
         printf(s_reply);
     }
      
    
     return 0;
 }