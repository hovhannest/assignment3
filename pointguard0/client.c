# include <sys/types.h>
# include <sys/socket.h>
# include <netdb.h>
# include <stdio.h>
# include <string.h>
# include <unistd.h>
# include <stdlib.h>


# define MAX_SIZE 1024

int main(int argc, char * argv[]) {

    int sockfd, n;
    char buffer[MAX_SIZE];
    struct sockaddr_in servaddr;
    struct hostent * server;


    if (argc < 3) {
        fprintf(stderr, "usage %s hostname port \n", argv[0]);
        exit(0); }

    int PORT_NUM = atoi(argv[2]);

    sockfd = socket(AF_INET, SOCK_STREAM, 0);

    if (sockfd < 0) {
        perror("cannot open socket.");
        exit(1); }

    server = gethostbyname(argv[1]);

    if (server == NULL) {
        fprintf(stderr, "no such host.\n");
        exit(0); }

    // clear servaddr
    bzero((char * ) & servaddr, sizeof(servaddr));
    // set addressing scheme
    servaddr.sin_family = AF_INET;
    bcopy((char * ) server->h_addr, (char * ) & servaddr.sin_addr.s_addr, server->h_length);
    // listen to port PORT_NUM
    servaddr.sin_port = htons(PORT_NUM);

    if (connect(sockfd, (struct sockaddr * ) & servaddr, sizeof(servaddr)) < 0) {
        perror("cannot connect.");
        exit(1); }

    printf("enter the message: ");
    bzero(buffer, MAX_SIZE);
    fgets(buffer, MAX_SIZE - 1, stdin);

    n = write(sockfd, buffer, strlen(buffer));

    if (n < 0) {
        perror("cannot write to socket.");
        exit(1); }

    bzero(buffer, MAX_SIZE);

    n = read(sockfd, buffer, MAX_SIZE - 1);

    if (n < 0) {
        perror("cannot read from socket.");
        exit(1); }

    printf("%s\n", buffer);

    return 0; }
