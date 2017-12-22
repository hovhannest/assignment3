#include <stdio.h>
#include <string.h>
#include <stdlib.h>    
#include <sys/socket.h>
#include <arpa/inet.h> 
#include <unistd.h>    
 
int main(int argc , char *argv[])
{   
    #define buff_size 1024
    char buffer[buff_size];
    int sct1 , sct2;
    struct sockaddr_in server , client;
    char * port_str;
    int port = 0;
    char inp_str[200];
    char bye[5];

    fgets(inp_str, 200, stdin);
    port_str = strtok(inp_str, " ");    
    port = atoi(port_str);

    sct1 = socket(AF_INET , SOCK_STREAM , 0);
    if (sct1 == -1){
        printf("Failed to create a socket");
        return -1;
    }
    else{
        printf("Socket created");
    }
     
    
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons(port);
     
   
    if(bind(sct1,(struct sockaddr *)&server , sizeof(server)) < 0){
        perror("bind failed. Error");
        return 1;
    }
    else{
        printf("bind done");
    }
     
   
    listen(sct1 , 3);
     
   
    puts("Waiting for connections... Where are you?? :(");
    int s = sizeof(struct sockaddr_in);
     
   
    sct2 = accept(sct1, (struct sockaddr *)&client, (socklen_t*)&s);
    if (sct2 < 0){
        perror("accept failed");
        return 1;
    }
    else{
        printf("Connection accepted");
    }

    int len;
    pid_t cpid;
    switch (cpid = fork()) {
        case 0: len = read(sct1, buffer, buff_size - 1);
            if (len < 0) {
                printf("Error: can't read socket");
                return -1;
            }
            printf("Succesfully received: %s\n", buffer);
            
            len = write(sct1, "message received", 16);
            if (len < 0) {
                printf("Error: writing to socket\n");
                return -1;
            }
            
            strncpy(bye, buffer, 3);
            bye[4] = '\0';
            
            if(strcmp(bye, "bye") == 0){ //if the first 3 letters = bye; sometimes w/ \n
                printf("bye received, child exiting\n");
                close(sct1);//if fails exits anyway
                exit(0);
            }
            break;
        case -1: printf("fork error\n");
            break;
        default:  printf("this is parent\n");
            break;
    }    
    return 0;
}