#include<stdio.h>
#include<string.h>
#include<stdlib.h>    
#include<sys/socket.h>
#include<arpa/inet.h> 
#include<unistd.h>    
  
int main(int argc , char *argv[])
{
     int s_desc;
     int client_socket; 
     int sock_size; 
     int sock_msg_length;
     struct sockaddr_in server , client;
     char c_msg[2000];
     int port = atoi(argv[1]);
 
     s_desc = socket(AF_INET , SOCK_STREAM , 0);
     if (argc<2)
     {
         printf("Socket was not created");
     }
     printf("Socket created successfully");
      
     
     server.sin_family = AF_INET;
     server.sin_addr.s_addr = INADDR_ANY;
     server.sin_port = htons(port);
      
    
     if( bind(s_desc,(struct sockaddr *)&server , sizeof(server)) < 0)
     {
         printf("Binding failed");
         return 1;
     }
     printf("Binded successfully");
      
    
     listen(s_desc , 3);
      
    
    printf("Connection in progress");
    sock_size= sizeof(struct sockaddr_in);
      
    
     client_socket = accept(s_desc, (struct sockaddr *)&client, (socklen_t*)&c);
     if (client_socket < 0)
     {
         printf("not accepted");
         return 1;
     }
     printf("Connection accepted");
     pid_t pid = fork();
     if (pid == 0){
 
     while( (sock_msg_length = recv(client_socket , c_msg , 2000 , 0)) > 0 )
     {
        if(strcmp(c_msg, "bye")==0)
 	{	write(client_socket, "Server is disconnected", 22);
 		break;
 	}
         write(client_socket , c_msg , strlen(c_msg));
 	
     }
      
     if(sock_msg_length == 0)
     {
         printf("Client disconnected");
         fflush(stdout);
     }  
     else if(sock_msg_length == -1)
     {
         printf("Error");
     }
     }
     else
 	{
 		
 	}
      
     return 0;
}
