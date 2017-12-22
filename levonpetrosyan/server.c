#include <unistd.h>
#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>

int main(int argc, char const *argv[])
{
    int server_fd, new_socket, valread;
    struct sockaddr_in address;//, clientaddr;
    int opt = 1;
    int addrlen = sizeof(address);
    char buffer[1024] = {0};
    char *hello = "Your message has been received.";

    if(argc < 2)
    {
        printf("Port is not provided \n");
        return 0;
    }

    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0)
    {
        printf("socket failed");
        exit(EXIT_FAILURE);
    }


    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT,
                   &opt, sizeof(opt)))
    {
        printf("setsockopt");
        exit(EXIT_FAILURE);
    }
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;

    int clientport = atoi(argv[1]);
    address.sin_port = htons(clientport);


    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address))<0)
    {
        printf("bind failed");
        exit(EXIT_FAILURE);
    }
    if (listen(server_fd, 3) < 0)
    {
        printf("listen");
        exit(EXIT_FAILURE);
    }



    if ((new_socket = accept(server_fd, (struct sockaddr *)&address,
                             (socklen_t*)&addrlen))<0)
    {
        printf("accept");
        exit(EXIT_FAILURE);
    }

    pid_t pid = fork();
    if(pid == 0){

        while(1){
            memset(buffer, 0, 255);
            valread = read(new_socket, buffer, 1024);

            if (strncmp(buffer, "bye", 3)==0)// Seda-man is genius
            {
                printf("\nhajox");
                close(new_socket);
                break;
            }

            send(new_socket , hello , strlen(hello) , 0);
            printf("client says: %s", buffer);
        }
    }
    return 0;
}