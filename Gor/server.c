#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>

#define BUFLEN 256

void error(char *msg)
{
    perror(msg);
    exit(1);
}

int main(int argc, char *argv[])
{
     int sockfd, newsockfd, portno, addr_len, pid;
     char buffer[BUFLEN];
     struct sockaddr_in this_addr, that_addr;
     int n;

     if (argc < 2) {
         fprintf(stderr,"ERROR, no port provided\n");
         exit(1);
     }

     sockfd = socket(AF_INET, SOCK_STREAM, 0);
     if (sockfd < 0)
        error("ERROR opening socket");

     bzero((char *) &this_addr, sizeof(this_addr));
     portno = atoi(argv[1]);

     this_addr.sin_family = AF_INET;
     this_addr.sin_addr.s_addr = INADDR_ANY;
     this_addr.sin_port = htons(portno);

     if (bind(sockfd, (struct sockaddr *) &this_addr,
              sizeof(this_addr)) < 0)
              error("ERROR on binding");
     listen(sockfd,5);

     addr_len = sizeof(that_addr);


     while (1) {
        newsockfd = accept(sockfd,
              (struct sockaddr *) &that_addr, &addr_len);
        if (newsockfd < 0)
            error("ERROR on accept");
        pid = fork();
        if (pid == 0)  {
          while (1) {
            close(sockfd);
            n = read(newsockfd, buffer, 255);

            if (n < 0) error("ERROR reading from socket");

            else if(strcmp(buffer, "bye\n") == 0){
              printf("Close connection\n");
              close(newsockfd);
              return 0;
            }

            else printf("Here is the message: %s\n", buffer);
            n = write(newsockfd, "Message received", 18);
          }


          exit(0);
      }
        else close(newsockfd);
    } /* end of while */
     return 0;
}
